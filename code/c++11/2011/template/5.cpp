#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
using namespace std;
//为什么一个函数可以接受不同类型的参数
template <typename T>
void foo(T &&param)
{
  // std::cout<< param << std::endl;
}
void tet_foo()
{
  int a = 27;
  foo(a);  // x is lvalue, so T is int&  param's type is also int&
  foo(27); // 27 is rvalue, so T is int  param's type is therefore int&&
}

void overloaded_function(std::string &param)
{
  std::cout << "std::string& version" << std::endl;
}
//static_cast < new_type > ( expression )

void overloaded_function(std::string &&param)
{
  std::cout << "std::string&& version" << std::endl;
}

//为什么一个函数可以接受不同类型的参数
template <typename T>
void pass_through(T &&param)
{
  overloaded_function(std::forward<T>(param));
}
void test_forward()
{
  std::string pes;
  pass_through(pes);
  pass_through(std::move(pes));
}

class Widget
{
private:
 std::string name;
public:
  template <typename T>
  void setName(T &&newName) // newName is
  {

    name = std::forward<T>(newName);
  } // universal reference
};

void test_unique()
{ 

  //使用up作为参数
  unique_ptr<int> a(new int(10));
  unique_ptr<int> b=std::move(a);
  cout << "b="<<*b<<endl;
   //unique_ptr<int> c=std::forward(b);
   //why couldn’t deduce template parameter ‘_Tp’
  // unique_ptr<int> c=std::forward(std::move(b)); 
  //why couldn’t deduce template parameter ‘_Tp’
  unique_ptr<int> c=make_unique<int>(11);
   cout << "c="<<*c<<endl;
}
int main()
{  

  test_unique();
  //tet_foo();
  //test_forward();
  return 0;
}
//http://thbecker.net/articles/rvalue_references/section_08.html
//https://en.cppreference.com/w/cpp/utility/forward
//https://stackoverflow.com/questions/3582001/what-are-the-main-purposes-of-using-stdforward-and-which-problems-it-solves