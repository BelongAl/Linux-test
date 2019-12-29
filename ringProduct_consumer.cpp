#include<iostream>
#include<semaphore.h>
#include<vector>

#define MAX_QUEUE 5

class ringQueue
{
    std::vector<int> m_array;
    int m_capacity;
    int m_pos_write;//写指针
    int m_pos_read;//读指针
    sem_t m_consumer_sem;//数据资源计数器，消费者计数器
    sem_t m_product_sem;//空闲空间计数器，生产者计数器
    sem_t m_sem_lock;//锁

public:
    ringQueue(int capacity = MAX_QUEUE):
        m_capacity(capacity),
        m_pos_read(0),
        m_pos_write(0),
        m_array(MAX_QUEUE)
    {
        sem_init(&m_consumer_sem, 0, 0);//数据资源初始化
        sem_init(&m_product_sem, 0, MAX_QUEUE);//空闲时间初始化
        sem_init(&m_sem_lock, 0, 1);//锁的初始化
    }

    ~ringQueue()
    {
        sem_destroy(&m_product_sem);
        sem_destroy(&m_consumer_sem);
        sem_destroy(&m_sem_lock);
    }

    bool push(int &data)
    {
        //没有空间空间则直接阻塞，并且空闲空间计数-1；
        sem_wait(&m_product_sem);

        sem_wait(&m_sem_lock);//加锁,保护入队操作
        m_array[m_pos_write++] = data;
        if(m_pos_write == m_capacity)
        {
            m_pos_write = 0;
        }
        sem_post(&m_sem_lock);//解锁，

        sem_post(&m_consumer_sem);//资源计数器+1，唤醒消费者
        return true;
    }

    bool pop(int *data)
    {
        //通过资源计数器判断是否否能获取资源，资源计数器-1
        sem_wait(&m_consumer_sem);

        sem_wait(&m_sem_lock);//加锁
        *data = m_array[m_pos_read++];
        if(m_pos_read == m_capacity)
        {
            m_pos_read = 0;
        }
        sem_post(&m_sem_lock);//解锁

        sem_post(&m_product_sem);//空闲计数器+1，唤醒生产者
        return true; 
    }

};

void *product(void* arg)
{
    ringQueue *rq = (ringQueue*)arg;
    int i = 0;
    while(1)
    {
        rq->push(i);
        std::cout << "productor: " << pthread_self() << "put data: " << i++ << std::endl; 
    }
    return NULL;
}

void *consumer(void *arg)
{
    int data;
    ringQueue *rq = (ringQueue*)arg;
    while(1)
    {
        rq->pop(&data);
        std::cout << "consumer: " << pthread_self() << "get data: " << data << std::endl; 
    }
    return NULL;
}

#define MAX_THR 4

int main()
{
    int ret, i;
    pthread_t ptid[MAX_THR], ctid[MAX_THR];

    ringQueue rq;

    for(i = 0; i < MAX_THR; i++)
    {
        ret = pthread_create(&ptid[i], NULL, product, (void*)&rq);
        if(ret != 0)
        {
            std::cout << "thread create error" << std::endl; 
            return -1;
        }
    }

    for(i = 0; i < MAX_THR; i++)
    {
        ret = pthread_create(&ctid[i], NULL, consumer, (void*)&rq);
        if(ret != 0)
        {
            std::cout << "thread create error" << std::endl; 
            return -1;
        }
    }

    for(i = 0; i < MAX_THR; i++)
    {
        pthread_join(ptid[i], NULL);
        pthread_join(ctid[i], NULL);
    }

    return 0;
}
