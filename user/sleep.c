#include "kernel/types.h"  // typedef to simplify the type names
#include "kernel/stat.h"   // metadata about a file 
#include "user/user.h"     // the signatures of system calls 

int
main(int argc, char *argv[])
{
    int n;
    // inspect the argument: n >= 0 and argc == 2
    switch (argc){
        case 1:
            printf("sleep needs an argument!\n");
            exit(1);
        case 2:
            n = atoi(argv[1]);  
            sleep(n); 
            exit(0);
        default:
            printf("too much arguments, just one allowed!\n");
            exit(1);
    }
}


