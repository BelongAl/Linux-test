#include<time.h>
#include<stdio.h>
#include<iostream>

using namespace std;

/*struct A
{
  int a[10000];
};

void TestFunc1(A a)
{}

void TestFunc2(A& a)
{}

void TestRefAndvalue()
{
  A a;
  //以值传递
  size_t begin1 = clock();
  for(size_t i = 0; i< 10000; ++i)
  {
    TestFunc1(a);
  }
  size_t end1 = clock();

  //以引用为函数参数
  size_t begin2 = clock();
  for(size_t i = 0; i < 10000 ; i++) 
  {
    TestFunc2(a);
  }
   size_t end2 = clock();

  //分别计算两个函数运行结束的时间
  cout << "TestFunc1(int*)-time:" << end1 - begin1 <<endl;
  cout << "TestFunc2(int*)-time:" << end2 - begin2 <<endl;
}

int main()
{
  for(int i = 0; i < 10;++i)
  {
    TestRefAndvalue();
  }
  return 0;
}*/

/*struct A
{
  int a[10000];
};

A a;
A TestFunc1()
{
  return a;
}

A& TestFunc2()
{
  return a;
}

void TestReturnBY()
{
  //以值为函数返回值类型
  size_t begin1 = clock();
  for(size_t i = 0;i<100000; ++i)
  {
    TestFunc1();
  }
  size_t end1 = clock();

  //以引用为函数返回值
  size_t begin2 = clock();
  for(size_t i = 0;i<100000; ++i)
  {
    TestFunc2();
  }
  size_t end2 = clock();

  //计算两个函数运算完成的时间
  cout << "TestFunc1 time:" << end1 - begin1 << endl;
  cout << "TestFunc2 time:" << end2 - begin2 << endl;
}

int main()
{
  for(int i = 0; i<10;i++)
  {
    TestReturnBY();
  }
  return 0;
}*/

int main()
{
  int a = 10;

  int& ra = a;
  ra = 20;

  int *pa = &a;
  *pa = 20;
  return 0;
}

















