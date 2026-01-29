#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUF_SIZE 512

// version 2.0
void runcmd(char *argv[]);

int main(int argc, char *argv[])
{
    char *new_argv[MAXARG];
    char buf[BUF_SIZE]; 
    char ch;

    int idx;    // index of the new_argv
    int n = 0;  // index of the buf

    if(argc < 2){
        fprintf(2, "xargs: at least one command\n");
        exit(1);
    }

    for(int i = 1; argv[i] != 0; i++)
        new_argv[i-1] = argv[i];

    idx = argc - 2;
    new_argv[++idx] = buf;
    // read from the standard input
    while(read(0, &ch, 1) > 0){

        if(n == BUF_SIZE || idx == MAXARG - 1){
            fprintf(2, "xargs: too many arguments\n");
            exit(1);
        }

        switch(ch){
            case ' ':  
                if(n == 0 || buf[n-1] == 0) // skip spaces
                    continue;
                buf[n++] = 0;
                new_argv[++idx] = &buf[n];
                break;

            case '\n': // end of a line
                buf[n++] = 0;
                new_argv[++idx] = 0;
                runcmd(new_argv);

                n = 0;
                idx = argc - 2;  // reset to receive another line
                new_argv[++idx] = &buf[n];
                break;

            default:
                buf[n++] = ch;
        }
    }

    // the last line
    if(n > 0){
        buf[n] = 0;
        new_argv[++idx] = 0;
        runcmd(new_argv);
    }
    exit(0);
}


void runcmd(char *argv[])
{
    int pid = fork();
    if(pid < 0){
        fprintf(2, "xargs: cannot fork\n");
        exit(1);
    }

    if(pid == 0){
        exec(argv[0], argv); 
        fprintf(2, "xargs: cannot exec %s\n", argv[0]);
        exit(1);
    }
    wait(0);
    return;
}




// version 1.0: it works but also exists some problems.
/* void forkexec(char *argv[]); */

/* int main(int argc, char *argv[]) */
/* { */
/*     char *_argv[MAXARG];    // store all the argvs. */
/*     char *p; */
/*     char *q; */
/*     int nargv = argc - 1;   // the nth argument */

/*     char ch; */
/*     char buf[512]; */

/*     // the command line */
/*     p = buf; */
/*     q = buf; */
/*     int flag; */

/*     for(int i = 1; i < argc; i++){  // arguments from the command line */
/*         _argv[i-1] = argv[i]; */
/*     } */

/*     // read from the standard input */
/*     while((flag = read(0, &ch, sizeof(ch))) > 0){ */
/*         if(ch == ' '){  // read one argument */
/*             *q++ = 0; */
/*             _argv[nargv++] = p; */
/*             p = q; */
/*             continue; */
/*         } */

/*         if(ch == '\n'){ */
/*             *q++ = 0; */
/*             _argv[nargv++] = p; */
/*             nargv = argc - 1; */
/*             p = q; */

/*             forkexec(_argv); */
/*             continue; */
/*         } */

/*         if(ch == '\\'){ */
/*             if(read(0, &ch, sizeof(ch)) < 0){ */
/*                 fprintf(2, "xargs: cannot read.\n"); */
/*                 exit(1); */
/*             } */

/*             if(ch == 'n'){ */
/*                 *q++ = 0; */
/*                 _argv[nargv++] = p; */
/*                 nargv = argc - 1; */
/*                 p = q; */

/*                 forkexec(_argv); */
/*                 continue; */
/*             } */
/*             *q++ = '\\'; */
/*         } */

/*         *q++ = ch; */
/*     } */
/*     if(flag < 0){ */
/*         fprintf(2, "xargs: cannot read\n"); */
/*         exit(1); */
/*     } */


/*     exit(0); */
/* } */



/* void forkexec(char *argv[]) */
/* { */
/*     int pid; */
/*     // exectue the argument line */
/*     pid = fork(); */
/*     if(pid < 0){ */
/*         fprintf(2, "xargs: cannot fork.\n"); */
/*         exit(1); */
/*     } */

/*     if(pid == 0){ */
/*         if(exec(argv[0], argv) < 0){ */
/*             fprintf(2, "xargs: cannot exec %s\n", argv[0]); */
/*             exit(1); */
/*         } */
/*         exit(0); */
/*     } */

/*     wait(0); */
/*     return; */
/* } */

