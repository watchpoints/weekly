#include <iostream>
using namespace std;


template<typename T>
void foo(T&&)
{

}

template<typename T, typename Arg> 
shared_ptr<T> factory(Arg&& arg)
{ 
  return shared_ptr<T>(new T(std::forward<Arg>(arg)));
} 

X x;
factory<A>(x);

void test2()
{
    void foo(X&& x)
    {
        X anotherX = x; // calls X(X const & rhs)
    }

    X&& goo();
    X x = goo(); // calls X(X&& rhs) because the thing on
            // the right hand side has no name
}
void test1()
{
    //https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers
    Widget&& var1 = someWidget;      // here, “&&” means rvalue reference

    auto&& var2 = var1;              // here, “&&” does not mean rvalue reference

    template<typename T>
    void f(std::vector<T>&& param);  // here, “&&” means rvalue reference

    template<typename T>
    void f(T&& param);               // here, “&&”does not mean rvalue reference

    //the parameter w is an lvalue
    the parameter w is an lvalue

}
//c++11的移动语义和完美转发
int main()
{
    print(1, 2, 3, 4);
    return 0;
}


template<class T>
void fun(T a)
{
	cout << "fun a = " << a << endl;
}

template<>
void fun(int a)
{
	cout << "fun1 a = " << a << endl;
}

int main()
{
	fun(3.3);
	fun(3);

	exit(0);
}

//Secondly, this kind of forwarding is less than perfect because it blocks out move semantics
template<typename T, typename Arg> 
shared_ptr<T> factory(Arg const & arg)
{ 
  return shared_ptr<T>(new T(arg));
} 