#include <iostream>
using namespace std;
 
class A{
 
	public:
 
		A(int data=0)
		{
			m_data=data;
		}
 
		int GetData()
		{
			return doGetData();
		}
 
		virtual int doGetData()
		{
			return m_data;
		}
 
	protected:
		int m_data;
 
};
 
class B:public A{
 
	public:
 
		B(int data=1)
		{
			m_data=data;
		}
        //虚函数
		int doGetData()
		{
			return m_data;
		}
 
	protected:
		int m_data;
 
};
 
class C:public B{
 
	public:
 
		C(int data=2)
		{
			m_data=data;
		}
 
	protected:
		int m_data;
 
};
 
 
int main()
{
	C c(10);
	cout<<c.GetData()<<endl; 
	// 1 c.GetData(是普通函数调用，GetData是继承过来的，里面this->doGetData() 虚函数调用，并且this base 类.实际 子类
	// 意思是说doGetData（）是通过this指针调用的，这个是base类指针，并且指向派生类

	cout<<c.A::GetData()<<endl; //1 c.A::GetData()是普通函数调用，GetData是继承过来的，里面this->doGetData() 虚函数调用，并且this base 类
	// 意思是说doGetData（）是通过this指针调用的，这个是base类指针，并且指向派生类

	cout<<c.B::GetData()<<endl;// 1 c.B::GetData()是普通函数调用， GetData是继承过来的，里面this->doGetData() 虚函数调用，并且this base 类。
	cout<<c.C::GetData()<<endl;//同理
	//doGetData（a*） doGetData(c)
	cout<<c.doGetData()<<endl;// c不是指针，doGetData是虚函数 这个不明白
	
	cout<<c.A::doGetData()<<endl; //0 doGetData是虚函数，但是不是通过指针调用的，不触发虚函数，普通函函数调用
	cout<<c.B::doGetData()<<endl; //1 普通函数调用
	cout<<c.C::doGetData()<<endl; //1普通函数调用

	//总结：01派生类 不是指针，调用base类普通函数，但是普通函数里面，this->虚函数调用 
	//this->派生类 因此触发虚函数调用。


	//02 代码71和73行， 虚函数有了，但是不是指针调用，不触虚函数调用。

	//03 作用域  函数调用 。-> :: 在访问自己类范围内的
	return 0;
}
