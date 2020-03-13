#include"cp_model.h"
#include<unistd.h>

pthread_mutex_t m_print;

void *product(void *arg)
{
    pthread_detach(pthread_self());
    cpmodel* cp = (cpmodel*)arg;
    for(int i = 1; i  < 88; i++)
    {
        if(cp->ProPush(i))
        {   
            pthread_mutex_lock(&m_print);
            std::cout << "producter: " << pthread_self() << " val: " << i << std::endl;
            pthread_mutex_unlock(&m_print);
        }
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg)
{
    pthread_detach(pthread_self());
    cpmodel* cp = (cpmodel*)arg;
    int val = 0;
    while(true)
    {
        cp->ConPop(val);
        pthread_mutex_lock(&m_print);
        std::cout << "consumer: " << pthread_self() << " val: " << val << std::endl;
        pthread_mutex_unlock(&m_print);
        sleep(1);
    }

}

int main()
{
    pthread_mutex_init(&m_print, NULL);
    cpmodel cp;
    //创建四个生产者和消费者
    pthread_t proarray[CAPACITY];
    for(pthread_t i = 0; i < CAPACITY; i++)
    {
        int ret = pthread_create(&proarray[i], NULL, product, &cp);
        if (ret != 0)
        {
            std::cout << "create error"<< std::endl;
            continue;
        }

    }

    pthread_t conarray[CAPACITY];
    for(pthread_t i = 0; i < CAPACITY; i++)
    {
        int ret = pthread_create(&conarray[i], NULL, consumer, &cp);
        if (ret != 0)
        {
            std::cout << "create error"<< std::endl;
            continue;
        }
    }

    sleep(30);
    return 0;
}

