#include <iostream>
#include <typeinfo>
#include <type_traits>
using namespace std;

//https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/81886
template <typename T>
void f(T& param) {
    //T t;
    cout<<typeid(T).name()<<endl;
    //std::cout << "decltype(i) is " << type_name<decltype(i)>() << '\n';
} // param is a reference

//情景一：ParamType是一个指针或引用，但不是通用引用

void test1()
{
        int x=27;                       //x是int
        cout << typeid(x).name() <<endl;
        //T是int，param的类型是int&
        f(x);
        const int cx=x;                 //cx是const int
        cout << typeid(cx).name() <<endl;
        //T是const int，param的类型是const int&
        f(cx);
        const int& rx=x;                //rx是指向作为const int的x的引用
         cout << typeid(rx).name() <<endl;
        //T是const int，param的类型是const int&
        f(rx);
}

void test02()
{

    cout<<"test02"<<endl;
    auto x =5;
    const auto*v =&x;
    const auto u =6;
    cout<<typeid(v).name()<<endl;
    cout<<typeid(u).name()<<endl;


    int x1=10;
    auto *a =&x1;
    cout<<typeid(a).name()<<endl;
    auto b =&x1;
    cout<<typeid(b).name()<<endl;
    auto &c =x;
     cout<<typeid(c).name()<<endl;
    
    auto d =c;
     cout<<typeid(d).name()<<endl;



}

//g++ -std=c++11 4_type_point.cpp
int main() 
{
      test1();
      test02();
}
