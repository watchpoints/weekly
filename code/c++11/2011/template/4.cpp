#include <iostream>

//为什么一个函数可以接受不同类型的参数
template<typename T>
void foo(T&& param) //
{
   // std::cout<< param << std::endl;
}

void tet_foo()
{  
    int a=27;
    foo(a) ; // x is lvalue, so T is int&  param's type is also int&
    foo(27); // 27 is rvalue, so T is int  param's type is therefore int&&
}

void overloaded_function(std::string& param) {
  std::cout << "std::string& version" << std::endl;
}
//static_cast < new_type > ( expression )		

void overloaded_function(std::string&& param) {
  std::cout << "std::string&& version" << std::endl;
}

//为什么一个函数可以接受不同类型的参数
template<typename T>
void pass_through(T&& param) {
  overloaded_function(std::forward<T>(param));
}
void test_forward()
{
    std::string pes;
    pass_through(pes);
    pass_through(std::move(pes));
}

class Test
{
public:
  int m_data;
};

int main()
{
    tet_foo();
    test_forward();
    return 0;
}
