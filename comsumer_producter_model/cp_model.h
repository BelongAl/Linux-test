#include<pthread.h>
#include<queue>
#include<iostream>

const int CAPACITY = 15; 

class cpmodel
{
    std::queue<int> m_depository;
    int m_capacity;//仓库容量
    pthread_cond_t m_con_cond;//消费者队列
    pthread_cond_t m_pro_cond;//生产者队列
    pthread_mutex_t m_mutex;//锁

public:

    cpmodel(int capacity = CAPACITY):
        m_capacity(capacity)
    {
        pthread_cond_init(&m_con_cond, NULL);
        pthread_cond_init(&m_pro_cond, NULL);
        pthread_mutex_init(&m_mutex, NULL);
    }

    ~cpmodel()
    {
        m_capacity = 0;
        pthread_cond_destroy(&m_con_cond);
        pthread_cond_destroy(&m_pro_cond);
        pthread_mutex_destroy(&m_mutex);
    }

    bool ProPush(int val)
    {
        pthread_mutex_lock(&m_mutex);
        while(m_depository.size() == m_capacity)
        {
            pthread_cond_wait(&m_pro_cond, &m_mutex);
        }
        m_depository.push(val);
        pthread_mutex_unlock(&m_mutex);//解锁
        pthread_cond_signal(&m_con_cond);//唤醒消费者队列
        return true;
    }

    bool ConPop(int &val)
    {
        pthread_mutex_lock(&m_mutex);
        while(m_depository.empty())
        {
            pthread_cond_wait(&m_con_cond, &m_mutex);
        }
        val = m_depository.front();
        m_depository.pop();
        pthread_mutex_unlock(&m_mutex);
        pthread_cond_signal(&m_pro_cond);

        return true;
    }

};
