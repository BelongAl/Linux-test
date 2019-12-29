#include<iostream>
#include<pthread.h>
#include<queue>

#define MAX_QUEUE 5

class blockQueue
{
    std::queue<int> m_blacK_queue;
    int m_capacity; 
    pthread_mutex_t m_mutex;//实现互斥
    pthread_cond_t m_product_cond;//生产者队列
    pthread_cond_t m_consumer_cond;//消费者队列

 public:
    blockQueue(int qmax = MAX_QUEUE)://资源初始化
        m_capacity(qmax)
    {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_product_cond, NULL);
        pthread_cond_init(&m_consumer_cond, NULL);
    }

    ~blockQueue()//释放资源
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_product_cond);
        pthread_cond_destroy(&m_consumer_cond);
    }

    bool push(int &data)//入队
    {
        pthread_mutex_lock(&m_mutex);//加锁
        while(m_blacK_queue.size() == m_capacity)//若队列以满,则让生产者条件队列等待
        {
            pthread_cond_wait(&m_product_cond, &m_mutex);
        }
        m_blacK_queue.push(data);//入队
        pthread_mutex_unlock(&m_mutex);//解锁
        pthread_cond_signal(&m_consumer_cond);//唤醒消费者队列

        return true;
    }

    bool pop(int *data)//出队
    {
        *data = m_blacK_queue.front();
        pthread_mutex_lock(&m_mutex);//加锁
        while(m_blacK_queue.empty())//若队列中没有元素，则让消费者队列等待
        {
            pthread_cond_wait(&m_consumer_cond, &m_mutex);
        }
        m_blacK_queue.pop();//出队
        pthread_mutex_unlock(&m_mutex);//解锁
        pthread_cond_signal(&m_product_cond);//唤醒生产者队列

        return true;
    }
    
};

#define MAX_THR 4

void *Productor(void *arg)//生产者不断创造数据
{
    blockQueue *q = (blockQueue*)arg;
    int i= 0;
    while(1)
    {
        q->push(i);
        std::cout << "Productor: " << pthread_self() << "put data: "<< i++ << std::endl;
    }
    return NULL;
}

void *Consumer(void *arg)//消费者不断获取数据
{
    blockQueue *q = (blockQueue*)arg;
    while(1)
    {
        int data;
        q->pop(&data);
        std::cout << "Consumer: " << pthread_self() << "get data: "<< data << std::endl;
    }
    return NULL;
}

int main()
{
    blockQueue q;

    pthread_t ptid[MAX_THR], ctid[MAX_THR];
    int ret, i;
    for(i = 0; i < MAX_THR; i++)
    {
        ret = pthread_create(&ptid[i], NULL, Productor, (void*)&q);
        if(ret != 0)
        {
            std::cout << "thread create error" << std::endl;
            return -1;
        }
    }

    
    for(i = 0; i < MAX_THR; i++)
    {
        ret = pthread_create(&ctid[i], NULL, Consumer, (void*)&q);
        if(ret != 0)
        {
            std::cout << "thread create error" << std::endl;
            return -1;
        }
    }

    for(i = 0; i < MAX_THR; i++)
    {
        pthread_join(ctid[i], NULL);
        pthread_join(ptid[i], NULL);
    }

    return 0;
}




