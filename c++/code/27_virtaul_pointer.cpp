#include <iostream>
#include <string.h>

using namespace std;

class A
{
public:
    A()
	{
		
	}
    /** 
	//bug1  基类只有声明，没有定义 
	//undefined reference to `vtable for A'

    virtual void foo();
    //{
    //   cout<< "calls A::foo() "<<endl;
    //}
	**/
	
	 virtual void foo()
	 {
		  cout<< "calls A::foo() "<<endl;
	 }
	 
	 int aaaa;
};
class B : public A
{
public:
    //bug2：error: class ‘B’ does not have any field named ‘foo’
	//类初始化列表
    B(string name):m_name(name)
	{
		
	}
	B()
	{
		
	}
	virtual void foo()
    {
       cout<< "calls B::foo"<<endl;
    }
private:

	const string m_name;
};

void test_a()
{
	A* a = new A();
	//构造函数执行顺序 
	//01基础知识 new 和malloc区别
	//02 operator new  https://www.bilibili.com/video/BV1Er4y1A7Xy?p=4	
}
int main()
{    
   
    return 0;
}
