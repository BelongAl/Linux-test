/*
 * 单利模式：所有对象只用一份资源
 * 懒汉模式：在使时候再初始化资源
*/
#include<iostream>
#include<mutex>

std::mutex mutex;

class slacker
{
    volatile static int *m_data;//初始化只是初始化指针，没加载资源

public:
    volatile int *Getdata()
    {
        if(m_data == nullptr)
        {
            mutex.lock();
            if(m_data == nullptr)
            {
                m_data = new int;//非原子操作，加锁
            }
            mutex.unlock();
        }
        return m_data;
    }

};

volatile int* slacker::m_data = nullptr;

int main()
{
    slacker s1;
    slacker s2;
    std::cout << "s1" <<  s1.Getdata() << std::endl; 
    std::cout << "s1" <<  s2.Getdata() << std::endl;
}

