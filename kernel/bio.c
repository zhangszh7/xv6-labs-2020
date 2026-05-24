// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.


#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

#define TABLE_SIZE 13 

struct {
  struct spinlock lock;
  struct buf buf[NBUF];

  struct {
      struct spinlock lock;
      struct buf *head;
  } buckets[TABLE_SIZE];
} bcache;

extern uint ticks; // time


void
binit(void)
{
  struct buf *b;

  initlock(&bcache.lock, "bcache");

  for(int i = 0; i < TABLE_SIZE; i++){
      initlock(&bcache.buckets[i].lock, "bcache");
  }


  for(b = bcache.buf; b < bcache.buf+NBUF; b++){
      initsleeplock(&b->lock, "buffer");
      b->next = bcache.buckets[0].head;
      bcache.buckets[0].head = b;
  }

  /* bcache.head.prev = &bcache.head; */
  /* bcache.head.next = &bcache.head; */
  /* for(b = bcache.buf; b < bcache.buf+NBUF; b++){ */
  /*   b->next = bcache.head.next; */
  /*   b->prev = &bcache.head; */
  /*   initsleeplock(&b->lock, "buffer"); */
  /*   bcache.head.next->prev = b; */
  /*   bcache.head.next = b; */
  /* } */
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b;

  int key = blockno % TABLE_SIZE;
  acquire(&bcache.buckets[key].lock);

  // Is the block already cached?
  for(b = bcache.buckets[key].head; b != 0; b = b->next){
      if(b->dev == dev && b->blockno == blockno){
          b->refcnt++;
          release(&bcache.buckets[key].lock);
          acquiresleep(&b->lock);
          return b;
      }
  }

  /* acquire(&bcache.lock); */
  /* for(b = bcache.head.next; b != &bcache.head; b = b->next){ */
  /*   if(b->dev == dev && b->blockno == blockno){ */
  /*     b->refcnt++; */
  /*     release(&bcache.lock); */
  /*     acquiresleep(&b->lock); */
  /*     return b; */
  /*   } */
  /* } */

  release(&bcache.buckets[key].lock);

  // Not cached.
  struct buf *bp = 0;
  int old_key = -1; 

  acquire(&bcache.lock);

  for(int i = 0; i < TABLE_SIZE; i++) {
      acquire(&bcache.buckets[i].lock);

      for(b = bcache.buckets[i].head; b != 0; b = b->next){
          if(b->refcnt == 0){
              if(bp == 0){
                  bp = b;
                  old_key = i;
              } else if(b->tstamp < bp->tstamp) {
                  if(old_key != i){
                      release(&bcache.buckets[old_key].lock);
                      old_key = i;
                  } 
                  bp = b;
              } 
          }
      }
      if(old_key != i)
          release(&bcache.buckets[i].lock);
  }


  if(!bp){
      panic("bget: no buffers"); 
  }

  
  if(old_key != key)
      acquire(&bcache.buckets[key].lock);

  release(&bcache.lock);

  // move the LRU buffer, from the old bucket to the new one. 
  if(bp == bcache.buckets[old_key].head) {
      bcache.buckets[old_key].head = bp->next;
  }
  else {
      for(b = bcache.buckets[old_key].head; b != 0; b = b->next){
          if(b->next == bp){
              b->next = bp->next;
              break;
          }
      }
  }

  if(old_key != key) 
      release(&bcache.buckets[old_key].lock);

  bp->next = bcache.buckets[key].head;
  bcache.buckets[key].head = bp;

  bp->dev = dev;
  bp->blockno = blockno;
  bp->valid = 0;
  bp->refcnt = 1;

  release(&bcache.buckets[key].lock);
  acquiresleep(&bp->lock);
  return bp; 

  /* for(b = bcache.head.prev; b != &bcache.head; b = b->prev){ */
  /*   if(b->refcnt == 0) { */
  /*     b->dev = dev; */
  /*     b->blockno = blockno; */
  /*     b->valid = 0; */
  /*     b->refcnt = 1; */
  /*     release(&bcache.lock); */
  /*     acquiresleep(&b->lock); */
  /*     return b; */
  /*   } */
  /* } */
  /* panic("bget: no buffers"); */
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if(!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");

  int key = b->blockno % TABLE_SIZE;

  acquire(&bcache.buckets[key].lock);
  b->refcnt--;
  if(b->refcnt == 0)
      b->tstamp = ticks;

  release(&bcache.buckets[key].lock);
  releasesleep(&b->lock);

  /* acquire(&bcache.lock); */
  /* b->refcnt--; */
  /* if (b->refcnt == 0) { */
  /*   // no one is waiting for it. */
  /*   b->next->prev = b->prev; */
  /*   b->prev->next = b->next; */
  /*   b->next = bcache.head.next; */
  /*   b->prev = &bcache.head; */
  /*   bcache.head.next->prev = b; */
  /*   bcache.head.next = b; */
  /* } */
  
  /* release(&bcache.lock); */
  /* releasesleep(&b->lock); */
}

void
bpin(struct buf *b) {
  acquire(&bcache.lock);
  b->refcnt++;
  release(&bcache.lock);
}

void
bunpin(struct buf *b) {
  acquire(&bcache.lock);
  b->refcnt--;
  release(&bcache.lock);
}


