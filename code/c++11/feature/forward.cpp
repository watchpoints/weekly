#include <iostream>
using namespace std;

void fun(int& x) { cout << "call lvalue ref" << endl; }
void fun(int&& x) { cout << "call rvalue ref" << endl; }
void fun(const int& x) { cout << "call const lvalue ref" << endl; }
void fun(const int&& x) { cout << "call const rvalue ref" << endl; }

template<typename T>
void PerfectForward(T&& t)
{
    std::cout << "T is a ref type?: " << std::is_reference<T>::value << std::endl;
    std::cout << "T is a lvalue ref type?: " << std::is_lvalue_reference<T>::value << std::endl;
    std::cout << "T is a rvalue ref type?: " << std::is_rvalue_reference<T>::value << std::endl;

    fun(forward<T>(t));
}

int main()
{
    PerfectForward(10);           // call rvalue ref

    int a = 5;
    PerfectForward(a);            // call lvalue ref
    PerfectForward(move(a));      // call rvalue ref

    const int b = 8;
    PerfectForward(b);           // call const lvalue ref
    PerfectForward(move(b));     // call const rvalue ref

    system("pause");
    return 0;
}
