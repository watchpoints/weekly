/* zombie.c */
#include <sys/types.h>
#include <unistd.h> 
//01 Zombie进程不可以用kill命令清楚，因为进程已退出，
//如果需要清除这样的进程，那么需要清除其父进程，或是等很长的时间后被内核清除。
//因为Zombie的进程还占着个进程ID号呢，这样的进程如果很多的话，不利于系统的进程调度。 
//https://coolshell.cn/articles/656.html
main()
{
    pid_t pid; 
    pid=fork();
    if(pid<0) { /* 如果出错 */ 
        printf("error occurred!\n");
    }else if(pid==0){ /* 如果是子进程 */ 
        exit(0);
    }else{  /* 如果是父进程 */ 
        sleep(60);  /* 休眠60秒 */ 
        wait(NULL); /* 收集僵尸进程 waitpid */ 
    }
}//       pid_t waitpid(pid_t pid, int *wstatus, int options);

 
 