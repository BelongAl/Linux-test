#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

typedef void (*FUNC)();


void Fun(void *str)
{
    printf("%s\n",(char*)str);
}

void process_create(pid_t *pid, void *func, void *arg)
{
    *pid = fork();
    if(*pid < 0)
    {
        perror("fork error\n");
    }
    else if(*pid == 0)
    {
        FUNC pHfun = (FUNC)func;
        int i = 5;
        while(i--)
        {
            pHfun(arg);
            sleep(1);
        }
        exit(0);
    }
    else 
    {
        sleep(5);
        waitpid(-1,NULL,0);
        printf("wait process sucess\n");
    }
}


int main()
{
    pid_t pid;
    char *arg = "Belong Al";
    process_create(&pid, Fun, arg);
    return 0;
}
