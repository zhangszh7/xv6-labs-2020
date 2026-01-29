#include "kernel/types.h"  // typedef to simplify the type names
#include "kernel/stat.h"   // metadata about a file 
#include "user/user.h"     // the signatures of system calls 

//version 2.0
int main(int argc, char *argv[])
{
    // check the amount of arguments
    if(argc != 2){
        fprintf(2, "usage: sleep <ticks>\n");
        exit(1);
    }

    // check the content of arguments
    int n = atoi(argv[1]);
    if(n <= 0)
        printf("usage: n must be an integer which >= 0\n");

    sleep(n);
    exit(0);
}
    




// version 1.0
/* int */
/* main(int argc, char *argv[]) */
/* { */
/*     int n; */
/*     // inspect the argument: n >= 0 and argc == 2 */
/*     switch (argc){ */
/*         case 1: */
/*             printf("sleep needs an argument!\n"); */
/*             exit(1); */
/*         case 2: */
/*             n = atoi(argv[1]); */  
/*             sleep(n); */ 
/*             exit(0); */
/*         default: */
/*             printf("too much arguments, just one allowed!\n"); */
/*             exit(1); */
/*     } */
/* } */


