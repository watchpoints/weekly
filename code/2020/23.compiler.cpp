#include<iostream>
using namespace std;
class Base {
    public:  
    virtual ~Base() {} 
};  
class Derived : public Base {}; 


int main()
{    
     
    // Derived d;  
    // Base& b = d;  
    // cout << typeid(b).name() << endl; //

     double data =3.14;
     int n = static_cast<int>(data); 

     //int& pd = dynamic_cast<int&>(data); // 基本类型不行
     //https://blog.csdn.net/sdausxc/article/details/51885890

    //  Derived* pb = dynamic_cast<Derived*>(&b);
    return 0;
}

/**
 * 
 *  int n = static_cast<int>(3.14); 
    std::cout << "n = " << n << '\n';
    g++ -S 23.compiler.cpp
 */