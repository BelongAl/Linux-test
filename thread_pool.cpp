#include<iostream>
#include<queue>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>


typedef void(*task_handler_t)(void *); 

using namespace std;

class ThreadTask
{
    int m_data;//操作数据
    task_handler_t m_handler;//操作方式
public:
    void Run()//用传入的方法处理传入的数据
    {
        return m_handler(&m_data);
    }

    ThreadTask(int data, task_handler_t handler):
        m_data(data),
        m_handler(handler)
    {}

    ~ThreadTask()
    {}

};

#define MAX_THREAD 5

class ThreadPool
{
    int m_thr_count;//定义线程的数量
    std::queue<ThreadTask> m_task_queue;//任务队列
    pthread_mutex_t m_mutex;//定义互斥锁,保护
    pthread_cond_t m_cond;//定义条件变量，用于实现线程池中的阻塞等待

    static void *thr_start(void *arg)//线程入口函数,获取一个任务，然后调用任务对象的Run接口
    {
        //线程池中的线程并不会在处理完一个任务之后退出，因此是一个死循环
        ThreadPool *pool = (ThreadPool*)arg;
        while(1)
        {
            //判断队列是否为空
            pthread_mutex_lock(&pool->m_mutex);
            while(pool->m_task_queue.empty())
            {
                pthread_cond_wait(&pool->m_cond, &pool->m_mutex);
            }
            ThreadTask task = pool->m_task_queue.front();//获取队首节点
            pool->m_task_queue.pop();
            pthread_mutex_unlock(&pool->m_mutex);

            //解锁完之后再进行函数处理，否则会造成当前线程获取任务进行处理期间其他线程无法获取锁，导致无法处理任务--演变为任务的串行化处理
            //并且加锁是为了保护task_queue
            task.Run();//使用任务中用户传入的函数处理任务
        }
        return nullptr;
    }

public:

    ThreadPool(int count = MAX_THREAD):
        m_thr_count(count)
    {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_cond, NULL);
        for(int i = 0; i < m_thr_count; i++)
        {
            pthread_t tid;
            int ret = pthread_create(&tid, NULL, thr_start, (void*)this);
            if(ret != 0)
            {
                std::cout << "thread create error" << std::endl;
                exit(0);
            }
            pthread_detach(tid);//直接分离(对线程的返回值不关心)            
        }

    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }

    bool PushTask(const ThreadTask &task)
    {
        //能够入队的都是生产者，无最大限制，因此不需要阻塞
        pthread_mutex_lock(&m_mutex);
        m_task_queue.push(task);
        pthread_mutex_unlock(&m_mutex);
        pthread_cond_signal(&m_cond);//唤醒线程池中线程的线程处理任务
        return true;
    }

};

void test(void* data)
{
    srand(time(NULL));
    int sec = rand()%5;
    std::cout << " thread: " << pthread_self() << " get data: " << *(int*)data << " sleep :" << sec <<std::endl;
    sleep(sec);
    return;
}

int main()
{
    ThreadPool pool;

    for(int i = 0; i < 10; i++) 
    {
        ThreadTask task(i, test);
        pool.PushTask(task);
        sleep(1);
    }
    return 0;
}
