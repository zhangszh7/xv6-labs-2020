#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// find 
void find(char *path, char *file);

int main(int argc, char *argv[])
{
    // check the arguments
    if(argc !=  3){
        fprintf(2, "the amount of arguments is wrong.\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}

void find(char *path, char *file)
{
    int fd;
    int flag;  // for reading 
    char *p, buf[512];
    struct dirent de;   
    struct stat st;

    // open the top directory (0: O_RDONLY)
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // stat the directory
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // make sure open a directory
    if(st.type != T_DIR){
        fprintf(2, "find: %s is not a directory\n", path);
        close(fd);
        return;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
        fprintf(2, "find: path too long\n");
        close(fd);
        return;
    }

    // store the parent directory
    strcpy(buf, path);    
    p = buf + strlen(buf);
    *p++ = '/';

    // read the parent directory
    while((flag = read(fd, &de, sizeof(de))) == sizeof(de)){
        if(de.inum == 0 || !strcmp(de.name, "." ) || !strcmp(de.name, ".."))
            continue;

        memmove(p, de.name, DIRSIZ); // get the sub-file name
        p[DIRSIZ] = '\0';
        
        // stat the sub-file
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        switch(st.type){
            case T_DEVICE:
            case T_FILE:
                if(!strcmp(de.name, file)){
                    printf("%s\n", buf);
                }
                break;
            case T_DIR:
                find(buf, file);
                break;
        }
    }
    close(fd);
}

