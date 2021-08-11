#include <iostream>
using namespace std;
 
class Base
{
public:
	Base(int a)
	{
		ma = a;
	}
	void show()
	{
		cout<<"base show 1"<<endl;
	}
	void show(int b)
	{
                cout<<b<<endl;
		cout<<"base show 2"<<endl;
	}
private:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int b):Base(b)
	{
		mb = b;
	}
	void show()
	{
		cout<<"derive show 1"<<endl;
	}
	
private:
	int mb;
};
//gcc -shared -fPIC -o 1.so 1.c
int main()
{
	Derive d(1);
	d.show();
	d.Base::show();
	d.Base::show(2);
	 
	return 0;
}
