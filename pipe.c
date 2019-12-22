#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    int pipefd[2] = {0};//创建管道
    int ret = pipe(pipefd);
    if(ret < 0)
    {
        perror("pipe error");
        return -1;
    }

    int pid1 = fork();//为ps创建子进程
    if(pid1 == 0)
    {
        dup2(pipefd[1], 1);//重定向
        execl("/usr/bin/ps", "ps", "-ef", NULL);
        exit(0);
    }

    int pid2 = fork();//为grep创建子进程
    if(pid2 == 0)
    {
        close(pipefd[1]);//关闭写端则读端退出返回0
        dup2(pipefd[0], 0);//重定向
        execl("/usr/bin/grep", "grep", "sshd", NULL);
        exit(0);

    }
    close(pipefd[0]);//关闭读端
    close(pipefd[1]);//关闭写端

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
