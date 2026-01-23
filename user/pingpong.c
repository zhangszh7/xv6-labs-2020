#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main()
{
    char signal = 'a';
    int p1[2];  // parent -> child
    int p2[2];  // child -> parent

    // create a pair of pipes.
    pipe(p1);
    pipe(p2);

    int pid = fork();

    if(pid == 0){
        // child
        close(p1[1]);
        close(p2[0]);

        char buf;

        if(read(p1[0], &buf, 1) < 0){
            fprintf(2, "child read failed.\n");
            exit(1);
        }
        else{
            printf("%d: received ping\n", getpid());
        }

        close(p1[0]);

        if(write(p2[1], &signal, 1) < 0){
            fprintf(2, "child write failed.\n");
            exit(1);
        }

        close(p2[1]);

        exit(0);
    }
    else{
        // parent
        close(p1[0]);
        close(p2[1]);

        char buf;

        if(write(p1[1], &signal, 1) < 0){
            fprintf(2, "parent write failed.\n");
            exit(1);
        }

        close(p1[1]);

        if(read(p2[0], &buf, 1) < 0){
            fprintf(2, "parent read failed.\n");
            exit(1);
        }
        else{
            printf("%d: received pong\n", getpid());
        }

        close(p2[0]);
        wait(0);

        exit(0);
    }
}
