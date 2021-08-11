#include<iostream>
using namespace std;
class Base
{
public:
	Base()
	{
		b=10;
	}
	virtual void fun0()
	{
		cout<<"Base::fun0()"<<endl;
	}
	virtual void fun1()
	{
		cout<<"Base::fun1()"<<endl;
	}
	virtual void fun2()
	{
		cout<<"Base::fun2()"<<endl;
	}
	int b;
};
class Derived:public Base
{
public:
	Derived()
	{}
	virtual void fun3()
	{
		cout<<"Derived::fun3()"<<endl;
	}
	virtual void fun4()
	{
		cout<<"Derived::fun4()"<<endl;
	}
	virtual void fun5()
	{
		cout<<"Derived::fun5()"<<endl;
	}
	int d;
};
typedef void (*vpf)();
void Printvpf()
{
	Base b;
	Derived d;
	cout<<"Base::vpf"<<endl;
	vpf* n = (vpf*)*(long*)(&b);
	while(*n)
	{
		(*n)();
		n++;
	}
	cout<<"Derived::vpf"<<endl;
	vpf* pn = (vpf*)*(long*)(&d);
	while(*pn)
	{
		(*pn)();
		pn++;
	}
	cout<<sizeof(Base)<<endl;
	cout<<sizeof(Derived)<<endl;
}
int main()
{
	Printvpf();
	return 0;
}
