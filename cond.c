#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

//定义两个条件变量是防止在唤醒时，唤醒错误
pthread_cond_t gourment_cond;//定义一个美食家变量
pthread_cond_t chief_cond;//定义一个厨师变量

int have_delicacy = 0;//刚开始并没有美食
pthread_mutex_t mutex;//定义一个互斥锁

void *gourment(void *arg)//美食家函数
{
    while(1)
    {
        pthread_mutex_lock(&mutex);//加锁
        while(have_delicacy == 0)//如果没有美食则需要等待,但是在等待前需要先解锁//因为pthread_cond_signal函数是唤醒至少一个，为了防止争抢混乱所采用的。
        {
            pthread_cond_wait(&gourment_cond, &mutex);//等待函数包括：解锁-》休眠-》加锁
        }
        printf("really delicious\n");
        have_delicacy--;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&chief_cond);
    }
    return NULL;
}

void *chief(void *arg)//厨师长函数
{
    while(1)
    {
        pthread_mutex_lock(&mutex);//加锁
        while(have_delicacy == 1)//如过做好了没有人吃则陷入等待
        {
            pthread_cond_wait(&chief_cond, &mutex);
        }
        printf("I made a bowl of Buddha jumping over the wall\n");
        have_delicacy++;
        //做出没事之后唤醒等待的美食家
        pthread_mutex_unlock(&mutex);//解锁
        pthread_cond_signal(&gourment_cond);//唤醒美食家
    }
    return NULL;
}

int main()
{
    pthread_t gourment_tid, chief_tid;
    int ret;

    pthread_cond_init(&chief_cond, NULL);//初始化条件变量
    pthread_cond_init(&gourment_cond, NULL);//初始化条件变量
    pthread_mutex_init(&mutex,NULL);//初始化互斥锁

    int i = 0;
    for(i = 0; i < 4; i++)
    {
        ret = pthread_create(&gourment_tid, NULL, gourment, NULL);//创建美食家线程
        if(ret != 0)
        {
            printf("pthread create error\n");
            return -1;
        }
    }
    for(i = 0; i < 4; i++)
    {
        ret = pthread_create(&chief_tid, NULL, chief, NULL);//创建厨师线程
        if(ret != 0)
        {
            printf("pthread create error\n");
            return -1;
        }
    }

    pthread_join(gourment_tid, NULL);
    pthread_join(chief_tid, NULL);

    pthread_cond_destroy(&chief_cond);//销毁条件变量
    pthread_cond_destroy(&gourment_cond);//销毁条件变量
    pthread_mutex_destroy(&mutex);//销毁互斥锁

    return 0;
}
