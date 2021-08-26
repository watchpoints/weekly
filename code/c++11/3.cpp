#include <cstdio>
#include <iostream>

using namespace std;

class A{
    public:
        A(){
            cout<<"A"<<endl;
        }
       virtual ~A(){
            cout<<"~A()"<<endl;
        }
 private:
  int m_a;
};
class B{
    public:
        B(){
            cout<<"B"<<endl;
        }
       virtual ~B(){
            cout<<"~B()"<<endl;
        }
 private:
  int m_b;
};
//class C:public B,public A{
class C:public A,public B{
    public:
        C(){
            cout<<"C"<<endl;
        }
       virtual ~C(){
            cout<<"~C()"<<endl;
        }
};

class D1:public virtual A
{

};
class D2:public  A
{

};

void test1()
{    
     D1 d;
     cout<< "sizeof(A)="<<sizeof(A)<<endl; //16
     cout<< "sizeof(D1)="<<sizeof(D1)<<endl; //16+8
     cout<< "sizeof(D2)="<<sizeof(D2)<<endl;//16
     //sizeof(D1)=24
     //sizeof(D2)=16
}
int main(){

    //C c;
    //A* pa =new C();
    //delete pa;

     test1();
    return 0;
}
/*
B
A
C
~C()
~A()
~B()
*/
/*
A
B
C
~C()
~B()
~A()
*/