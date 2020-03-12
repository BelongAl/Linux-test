/*一：这是一个读者写者模型
 *二：使用读写锁实现
 *三：他们存在以下关系
 *  1:读者与读者之间可以并发访问某个资源
 *  2：读者与写者之间，存在互斥
 *  3：写着和写着之间存在互斥
*/ 
#include<pthread.h>
#include<iostream>
#include<unistd.h>

class wrmode
{
    pthread_rwlock_t m_rwlock;//读写锁
    int m_count;//表示某种资源

public:

    wrmode():
        m_count(0)
    {
        pthread_rwlock_init(&m_rwlock, NULL);//对读写锁的初始化
    }

    ~wrmode()
    {
        pthread_rwlock_destroy(&m_rwlock);//销毁读写锁
    }

    static void* writer(void *arg)
    {
        wrmode *wr = (wrmode*)arg; 
        while(1)
        {
            pthread_detach(pthread_self());
            sleep(1);
            int ret = pthread_rwlock_wrlock(&wr->m_rwlock);//加上写锁
            if(ret != 0)
            {
                std::cout << "the lock is use by reader" << std::endl; 
            }
            wr->m_count++;//写入资源
            std::cout << "writer is: " <<  pthread_self() << " m_count: " << wr->m_count << std::endl;
            pthread_rwlock_unlock(&wr->m_rwlock);
        }
    }

    static void* reader(void*arg)
    { 
        pthread_detach(pthread_self());
        wrmode *wr = (wrmode*)arg; 
        while(1)
        {
            sleep(1);
            int ret = pthread_rwlock_rdlock(&wr->m_rwlock);//加上读锁
            if(ret != 0)
            {
                std::cout << "the lock is use by reader" << std::endl;
            }
            std::cout << "reader is: " << pthread_self() << " m_count: " << wr->m_count << std::endl;//读取信息
            pthread_rwlock_unlock(&wr->m_rwlock);//解锁
        }
    }

};



