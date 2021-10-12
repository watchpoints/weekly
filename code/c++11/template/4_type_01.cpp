#include <iostream>
#include <typeinfo>
using namespace std;

 
struct Base {}; // non-polymorphic
struct Derived : Base {};
 
struct Base2 { virtual void foo() {} }; // polymorphic
struct Derived2 : Base2 {};


template <typename T>
void  myname1(T* x)
{
    cout<<"myname1 x is "<<x<<endl;
    T t; 
    cout<<typeid(t).name()<<endl;
}
template <typename T>
void  myname2(T x)
{
    cout<<" myname2 x is "<<x<<endl;
    T t;      // This is char* now
    cout<<typeid(t).name()<<endl;
    //https://en.cppreference.com/w/cpp/language/typeid
}
void test_type_id()
{
     int myint = 50;
    std::cout << "myint has type: " << typeid(myint).name() << '\n';

     Derived2 d2;
    Base2& b2 = d2;
    std::cout << "reference to polymorphic base: " << typeid(b2).name() << '\n';
}

//https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/81886
template <typename T>
void f(T& param) {
    cout<<typeid(t).name()<<endl;
} // param is a reference

//g++ -std=c++11 4_type_point.cpp
int main() 
{
   //const char *p="stackOverflow";
   char *p="stackOverflow";
    myname1(p);
    myname2(p); 
    //很神奇，在不同函数不同T类型
    //https://www.cnblogs.com/QG-whz/p/4952980.html

    test_type_id();
}
