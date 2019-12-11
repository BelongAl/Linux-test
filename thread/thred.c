#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

void *thread_start(void *arg)
{
    while(1)
    {
        printf("a parametr form main thread: %s\n",(char*)arg);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t pid;
    char *buff = "BelongAl";
    int ret = pthread_create(&pid, NULL, thread_start, (void*)buff);
    if(ret != 0)
    {
        printf("thread create error:%d\n", ret);
        return -1;
    }
    while(1)
    {
        printf("I'm main thread,child tid is %p\n",pid);
        sleep(1);
    }
    return 0;
}

