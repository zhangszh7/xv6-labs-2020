#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


// version 3.0: Guard Clauses ...

// version 2.0: recursion

// sieve 
void sieve(int rp);

int
main()
{
    // the 1st pipe: main -> the 1st sieve.
    int p[2];
    if(pipe(p) < 0){
        fprintf(2, "main: pipe failed.\n");
        exit(1);
    }


    // fork the 1st sieve.
    int pid = fork();
    if(pid < 0){
        fprintf(2, "main: fork failed.\n");
        exit(1);
    }
    else if(pid == 0){ // sieve
        close(p[1]);
        sieve(p[0]);
    }
    else{ // main: write 2~35

        close(p[0]);

        for(int i = 2; i <= 35; i++){
            if(write(p[1], &i, sizeof(i)) < 0){
                fprintf(2, "main: write failed.\n");
                exit(1);
            }
        }

        close(p[1]);
        wait(0);
    }
    exit(0);

}

void sieve(int rp)  // rp: read-side of the left pipe
{
    int buf;
    int prime;

    // read the 1st number.
    int flag = read(rp, &buf, sizeof(buf)); // error(-1), over(0)

    if(flag < 0){
        fprintf(2, "sieve: read from %d failed.\n", rp);
        exit(1);
    }
    else if(flag == 0){ // the last sieve.
        close(rp);
        exit(0);
    }

    prime = buf;
    printf("prime %d\n", prime);


    // the right pipe
    int p[2];
    if(pipe(p) < 0){
        fprintf(2, "sieve: pipe failed.\n");
        exit(1);
    }


    // fork the right sieve.
    int pid = fork();
    if(pid < 0){
        fprintf(2, "sieve: fork failed.\n");
        exit(1);
    }
    else if(pid == 0){ // the right sieve
        close(rp);
        close(p[1]);
        sieve(p[0]);
    }
    else{ // the left sieve: write
        close(p[0]);

        while(1){
            flag = read(rp, &buf, sizeof(buf));

            if(flag < 0){
                fprintf(2, "sieve: read from %d failed.\n", rp);
                exit(1);
            }
            else if(flag == 0){ // read over, write over, close pipe.
                close(rp);
                close(p[1]);
                wait(0);
                exit(0);
            }
            else if(buf % prime != 0){ // sieve, write.
                if(write(p[1], &buf, sizeof(buf)) < 0){
                        fprintf(2, "left sieve: write to %d failed.\n", p[1]);
                        exit(1);
                }
            }
        }
    }
}
                    

// version 1.0: iteration

/* int */
/* main() */
/* { */
/*     int buf; */

/*     // left pipe */
/*     int p[2]; */

/*     pipe(p); */

/*     // start up */
/*     int pid = fork(); */
/*     if(pid < 0){ */
/*         fprintf(2, "fork failed.\n"); */
/*         exit(1); */
/*     } */

/*     if(pid == 0){ */
/*         int prime; */
/*         int isparent = 0; */
/*         int p1[2]; */
/*         int flag; */
/*         int pid1; */
        

/*         // only need read-side of the left pipe */
/*         close(p[1]); */


/*         // the first number */
/*         if(read(p[0], &buf, sizeof(buf)) < 0){ */
/*             fprintf(2, "(first)read from %d failed.\n", p[0]); */
/*             exit(1); */
/*         } */
/*         prime = buf; */
/*         printf("prime %d\n", prime); */ 


/*         while(1){ */
/*             // read from the left pipe */
/*             flag =  read(p[0], &buf, sizeof(buf)); */

/*             if(flag < 0){ */
/*                 fprintf(2, "read failed.\n"); */
/*                 exit(1); */
/*             } */
/*             else if(flag == 0){ */
/*                 // read over, which also means write over. */
/*                 // close all the pipe. */
/*                 close(p[0]); */

/*                 if(isparent){ */
/*                     close(p1[1]); */
/*                     wait(0); */
/*                 } */

/*                 exit(0); */
/*             } */
/*             else{ */
/*                 //the former sieve */
/*                 if(buf % prime != 0 ){ */
/*                     // right pipe */
/*                     if(!isparent){ */
/*                         if(pipe(p1) < 0){ */
/*                             fprintf(2, "pipe failed: too many open files\n"); */
/*                             exit(1); */
/*                         } */

/*                         // the latter sieve */
/*                         pid1 = fork(); */
/*                         if(pid1 < 0){ */
/*                             fprintf(2, "fork failed.\n"); */
/*                             exit(1); */
/*                         } */

/*                         if(pid1 == 0){ */
/*                             close(p[0]); */
/*                             p[0] = p1[0]; */
/*                             close(p1[1]); */

/*                             // the first number */
/*                             if(read(p[0], &buf, sizeof(buf)) < 0){ */
/*                                 fprintf(2, "read from %d failed.\n", p[0]); */
/*                                 exit(1); */
/*                             } */
/*                             prime = buf; */
/*                             printf("prime %d\n", prime); */
/*                             continue; */
/*                         } */
/*                         else{ */
/*                             close(p1[0]); */
/*                             isparent = 1; */
/*                         } */

/*                     } */

/*                    if( write(p1[1], &buf, sizeof(buf)) < 0){ */
/*                        fprintf(2, "write failed."); */
/*                        exit(1); */
/*                    } */

/*                 } */
/*             } */
/*         } */
/*     } */
/*     else{ */
/*         close(p[0]); */
/*         for(int i = 2; i <= 35; i++){ */
/*             buf = i; */
/*             if(write(p[1], &buf, sizeof(buf)) < 0 ){ */
/*                 fprintf(2, "write failed.\n"); */
/*                 exit(1); */
/*             } */
/*         } */

/*         close(p[1]); */
/*         wait(0); */
/*         exit(0); */
/*     } */
/* } */





