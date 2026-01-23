#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main()
{
    int buf;

    // left pipe
    int p[2];

    pipe(p);

    // start up
    int pid = fork();
    if(pid < 0){
        fprintf(2, "fork failed.\n");
        exit(1);
    }

    if(pid == 0){
        int prime;
        int isparent = 0;
        int p1[2];
        int flag;
        int pid1;
        

        // only need read-side of the left pipe
        close(p[1]);


        // the first number
        if(read(p[0], &buf, sizeof(buf)) < 0){
            fprintf(2, "(first)read from %d failed.\n", p[0]);
            exit(1);
        }
        prime = buf;
        printf("prime %d\n", prime); 


        while(1){
            // read from the left pipe
            flag =  read(p[0], &buf, sizeof(buf));

            if(flag < 0){
                fprintf(2, "read failed.\n");
                exit(1);
            }
            else if(flag == 0){
                // read over, which also means write over.
                // close all the pipe.
                close(p[0]);

                if(isparent){
                    close(p1[1]);
                    wait(0);
                }

                exit(0);
            }
            else{
                //the former sieve
                if(buf % prime != 0 ){
                    // right pipe
                    if(!isparent){
                        if(pipe(p1) < 0){
                            fprintf(2, "pipe failed: too many open files\n");
                            exit(1);
                        }

                        // the latter sieve
                        pid1 = fork();
                        if(pid1 < 0){
                            fprintf(2, "fork failed.\n");
                            exit(1);
                        }

                        if(pid1 == 0){
                            close(p[0]);
                            p[0] = p1[0];
                            close(p1[1]);

                            // the first number
                            if(read(p[0], &buf, sizeof(buf)) < 0){
                                fprintf(2, "read from %d failed.\n", p[0]);
                                exit(1);
                            }
                            prime = buf;
                            printf("prime %d\n", prime);
                            continue;
                        }
                        else{
                            close(p1[0]);
                            isparent = 1;
                        }

                    }

                   if( write(p1[1], &buf, sizeof(buf)) < 0){
                       fprintf(2, "write failed.");
                       exit(1);
                   }

                }
            }
        }
    }
    else{
        close(p[0]);
        for(int i = 2; i <= 35; i++){
            buf = i;
            if(write(p[1], &buf, sizeof(buf)) < 0 ){
                fprintf(2, "write failed.\n");
                exit(1);
            }
        }

        close(p[1]);
        wait(0);
        exit(0);
    }
}





