#include <iostream>
#include <string.h>

using namespace std;
class Base
{
public:
	virtual  ~Base(){}
private:
   int data;
};

class Test
{
public:
	Test( Base& base):m_base(base),m_ptr(&base)
	{ 
	  cout<< &m_ptr<<endl;
	  cout<< m_ptr<<endl;
	  cout<< &m_base<<endl;
	  //Test( Base &base):m_base(&base),m_ptr(&base)
	  //invalid initialization of non-const reference of type ‘Base&’ from a temporary of type ‘Base*’
	}
	Base* m_ptr;
	Base &m_base;
};

///////////////////////////////////////
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
	//A a;
	//构造函数执行顺序  
	//01基础知识 new 和malloc区别 隐藏boss：成员初始化在 在构造函数之前
	//02 operator new  https://www.bilibili.com/video/BV1Er4y1A7Xy?p=4	
	//Foo *p = (Foo *)operator new(sizeof(Foo)); // 调用 ::operator new(size_t) 全局函数
	//new (p) Foo(x);
	
	//03 引用和指针 隐藏boss： 空间分配
	 // https://juejin.cn/post/6844904165114642440
	//04:
	
}  

void test_ref()
{
	Base base;
	cout << "&base=" <<&base <<endl; //16
	Test a(base);
	cout << "sizeof(base)=" <<sizeof(base) <<endl; //16
	cout << "sizeof(a)=" <<sizeof(a) <<endl;//8
	cout << "sizeof(a._base)=" <<sizeof(a.m_base) <<endl; //16
}

void test_pointer()
{
	int a = 3;
	int &ra = a;
	ra++;

	int b = 4;
	int *pa = &b;
	pa++;
	
	(*pa)++;
}

int main()
{    
    //test_a();
	test_ref();
    return 0;
}
