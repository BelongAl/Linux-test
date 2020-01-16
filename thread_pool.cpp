#include<iostream>
#include<thread>
#include<queue>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

typedef void(*task_Handler)(int data); 

class thread_Task
{
    int m_data;
    task_Handler m_handler;

public:

    thread_Task()//构造函数 
    {} 
    thread_Task(int data, task_Handler handler):
        m_data(data),
        m_handler(handler)
    {}

    void Set_handler(int data, task_Handler handler)//设置函数
    {
        m_data = data;
        m_handler = handler; 
    }

    void Run()//运行
    {
        m_handler(m_data);
    }

};

#define MAX_THR 5

class thread_Pool
{
    int m_thread_number;//线程池最大数量
    std::queue<thread_Task> m_task_queue;//任务队列
    pthread_mutex_t m_mutex;//锁
    pthread_cond_t m_cond_product;
    pthread_cond_t m_cond_consumer;

    static void *thr_start(void *arg)
    {
        thread_Pool *pool = (thread_Pool*)arg;
        while(1)//循环使用线程
        {
            pool->thread_Lock();
            while(pool->task_Empty())
            {
                pool->thread_Wait();//包括解锁-》等待-》加锁
            }
            thread_Task tt;
            pool->task_Pop(tt);
            pool->thread_Unlock();
            tt.Run();
        }
        return nullptr;
    }

public:

    void thread_Wait()
    {
        pthread_cond_wait(&m_cond_consumer, &m_mutex);
    }

    void thread_WakeUp()
    {
        pthread_cond_signal(&m_cond_consumer);
    }

    bool task_Empty()
    {
        return m_task_queue.empty();
    }

    void thread_Lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void thread_Unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

    bool task_Pop(thread_Task &tt)
    {
        std::cout << m_task_queue.size() << std::endl;
        tt = m_task_queue.front();
        m_task_queue.pop();
        return true;
    }

    thread_Pool(int thread_number =  MAX_THR):
        m_thread_number(thread_number)
    {}

    bool pool_Init()
    {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_cond_product, NULL);
        pthread_cond_init(&m_cond_consumer, NULL);
        int i = 0;
        for(i = 0; i < m_thread_number; i++)
        {
            pthread_t tid;
            int ret = pthread_create(&tid, NULL, thr_start, (void*)this);
            if(ret != 0)
            {
                std::cerr << "cread thread error:" << std::endl;
                return false;
            }
            pthread_detach(tid);
        }
        return true;
    }

    bool task_Push(thread_Task tt)
    {
        thread_Lock();
        m_task_queue.push(tt);
        thread_Unlock();
        thread_WakeUp();
        return true;
    }

};

void test(int data)
{
    srand(data);
    int sec = rand()%5;
    printf("thread:%p -get data:%d, sleep %d sec\n",pthread_self(),data,sec);
    sleep(sec);
}

int main()
{
    thread_Pool pool;
    pool.pool_Init();
    int i;
    for(i = 0; i < 10; i++)
    {
        thread_Task task(i,test);
        pool.task_Push(task);
    }

    while(1)
    {
        sleep(1);
    }

    return 0;
}


















