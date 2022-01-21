#include <iostream>
#include <typeinfo>
using namespace std;
 
class Base
{
public:
	Base(int a = 1)
	{
		ma = a;
	}
	virtual void show()
	{
		cout<<"base::show"<<endl;
	}
	void show(int b)
	{
		cout<<"base show 2"<<endl;
	}
private:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int b = 2):Base(b)
	{
		mb = b;
	}
	void show()
	{
		cout<<"derive::show "<<endl;
	}
	
private:
	int mb;
};
int main()
{
	Base b;
	Derive d;
	Base* p = &d;
	Derive* pd = &d;
	cout<<"base size:"<<" "<<sizeof(b)<<endl;
	cout<<"derive size:"<<" "<<sizeof(d)<<endl;
    

	cout<<"p type:"<<" "<<typeid(p).name()<<endl;
	cout<<"*p type:"<<" "<<typeid(*p).name()<<endl;
    
	

	cout<<"pd->show()"<<endl;
	pd->show();

	b =d;
	cout<<"test1:call function"<<endl;
	b.show();
	
	return 0;
}
