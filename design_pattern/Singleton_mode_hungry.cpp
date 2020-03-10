/*
 * 单例模式：资源只有一份，不管多少个对象公用一份资源
 * 饿汉模式：一开始就加载出来
 */
#include<iostream>


using namespace std;

class Hungry
{
    static int *m_data;

public:

    int* GetData()
    {
        return m_data;
    }

    Hungry()
    {}

};

int *Hungry::m_data = new int;

int main()
{
    Hungry h1;
    Hungry h2;

    std::cout << "h1: " << h1.GetData() << std::endl;
    std::cout << "h2: " << h2.GetData() << std::endl;

    return 0;
}
