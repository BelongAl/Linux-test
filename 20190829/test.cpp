#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std; 

/*int main()
{
  int a = 10;
  int& ra = a;

  cout << "&a = "<<&a<<endl;
  cout << "&ra = "<<&ra<<endl;
  
  return 0;
}*/

//inline 修饰的内敛函数
/*inline int Add(int left, int right)
{
  return left +right;
}
int main()
{
  int ret = 0;
  ret = Add(1,2);
  return 0;
}*/ 

//auto定义变量时必须对其初始化
/*int TestAuto()
{
  return 10;
}
int main()
{
  int a = 10;
  auto b = a;
  auto c = 'a';
  auto d = TestAuto();

  cout << typeid(b).name() <<endl;
  cout << typeid(b).name() <<endl;
  cout << typeid(b).name() <<endl;
  return 0;
}*/

/*int main()
{
  int x = 10;
  auto a = &x;
  auto* b = &x;
  auto& c = x;

  cout << typeid(a).name() <<endl;
  cout << typeid(b).name() <<endl;
  cout << typeid(c).name() <<endl;

  *a = 20;
  *b = 30;
  c = 40;

  return 0;
}*/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std; 
int main()
{
  int arr[] = {1,2,3,4,5,6,7,8,9,10};
  for(auto val : arr)
  {
    cout << val <<" ";
  }
  return 0;
}












