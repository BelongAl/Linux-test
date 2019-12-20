#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int tickets = 100;
pthread_mutex_t mutex;


void *thr_tout(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);//对线程加锁
        if(tickets > 0)
        {
            usleep(1000);
            printf("tout:%p - get a ticket:%d\n",pthread_self(), tickets);
            tickets--;
            pthread_mutex_unlock(&mutex);//对线程解锁
        }
        else
        {
            pthread_mutex_unlock(&mutex);//对线程解锁
            printf("tout:%p exit\n", pthread_self());
            pthread_exit(NULL);//没票了则退出线程
        }
    }
    return NULL;
}

int main()
{
    int i = 0, ret;
    pthread_t tid[4];
    pthread_mutex_init(&mutex,NULL);
    for(i = 0; i < 4; i++)
    {
        ret = pthread_create(&tid[i], NULL,thr_tout, NULL);//线程创建
        if(ret != 0)
        {
            printf("thread create error\n");
            return -1;
        }
    }
    for(i = 0; i < 4; i++)
    {
        pthread_join(tid[i], NULL);//线程等待
    }
    pthread_mutex_destroy(&mutex);//互斥锁销毁
    return 0;
}
