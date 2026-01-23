#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


// 筛选函数
void
sieve(int left_fd)
{
    int prime;
    int n;
    int ret;
    
    // 1. 读取第一个数
    ret = read(left_fd, &prime, sizeof(int));
    if(ret < 0){
        fprintf(2, "read failed\n");
        exit(1);
    }
    else if(ret == 0){
        // EOF: 正常结束
        close(left_fd);
        exit(0);
    }
    
    printf("prime %d\n", prime);

    // 2. 创建管道 (必须检查！这是 Primes 实验最容易挂的地方)
    int p[2];
    if(pipe(p) < 0){
        fprintf(2, "pipe failed: possibly ran out of file descriptors\n");
        exit(1);
    }

    // 3. Fork
    int pid = fork();
    if(pid < 0){
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if(pid == 0){
        // === 子进程 ===
        close(p[1]); 
        close(left_fd); 
        sieve(p[0]); // 递归
    }
    else{
        // === 父进程 ===
        close(p[0]); // 关掉右边的读端

        // 4. 循环读取并过滤
        while(1){
            ret = read(left_fd, &n, sizeof(int));
            if(ret < 0){
                fprintf(2, "read loop failed\n");
                exit(1);
            }
            if(ret == 0){
                break; // 读完了
            }

            if(n % prime != 0){
                if(write(p[1], &n, sizeof(int)) < 0){
                    fprintf(2, "write failed\n");
                    exit(1);
                }
            }
        }

        // 5. 资源回收
        close(left_fd);
        close(p[1]); 
        wait(0); 
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    
    // 检查 pipe
    if(pipe(p) < 0){
        fprintf(2, "main pipe failed\n");
        exit(1);
    }

    // 检查 fork
    int pid = fork();
    if(pid < 0){
        fprintf(2, "main fork failed\n");
        exit(1);
    }

    if(pid == 0){
        // === 第一个子进程 ===
        close(p[1]); 
        sieve(p[0]);
    }
    else{
        // === 主生成器 ===
        close(p[0]); 
        
        for(int i = 2; i <= 35; i++){
            // 检查 write
            if(write(p[1], &i, sizeof(int)) < 0){
                fprintf(2, "generator write failed\n");
                exit(1);
            }
        }
        
        close(p[1]); 
        wait(0);
    }
    exit(0);
}
