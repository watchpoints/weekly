#include <iostream>
#include <exception>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

class G
{
public:
 virtual void func(int) {cout <<"calls G::f int"<<endl;};
 
 // final error: overriding final function ‘virtual void G::func(int)’
 // 该函数不允许被重写，及时是虚函数 一般不这样用
};
class H: G
{
public:
 //virtual void func(double) override {cout<< "calls H::f double "<<endl;}
 //void H::func(double)’ marked override, but does not override
 //01 子类不能重载父类方法
 //02 override表示显示覆盖父类方法
 
 
 virtual void func(int) override {cout<< "calls H::f double "<<endl;} //代码安全
 
 
};

class B
{
public:
    virtual  void m(int x)
    {
       cout<< "calls B::m(int x) "<<endl;
    }
};
class  D : public B
{
public:
    virtual void m ( )//由于函数签名不同，所以二者不构成重置。
    {
        cout<< "calls D::m ( ) "<<endl;
    }
	
	 virtual  void m(int x)
    {
       cout<< "calls D::m(int x) "<<endl;
    }
};


void test01(){
	D  *d1 =new D() ;
   // d1->m(10); // ERROR error: no matching function for call to ‘D::m(int)’
    
	d1->B::m(10);  // OK calls B::m(int x
	B *p=d1;
	p->m(10); //calls D::m(int x) 
	//D d1,
}
//g++ -std=c++11 04_class_override.cpp
int main()
{    
     test01();
	//H *p=new H; //创建派生类对象 
	//p->func(5); //calls H::f 本地覆盖派生类现象 int -->自动转换double
	//p->func(5.0); // calls H::f
    return 0;
}



// 01 为避免用户在派生类中重载一个虚函数
//02 https://www.oschina.net/translate/use-c-11-inheritance-control-keywords-to-prevent-inconsistencies-in-class-hierarchies?cmp
