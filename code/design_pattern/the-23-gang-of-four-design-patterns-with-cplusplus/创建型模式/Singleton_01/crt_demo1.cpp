#include <iostream>

using namespace std;

template <typename T>
struct Base
{
    void foo()
    {
        (static_cast<T *>(this))->foo();
    }
};

struct Derived : public Base<Derived>
{
    void foo()
    {
        cout << "derived foo" << endl;
    }
};

struct AnotherDerived : public Base<AnotherDerived>
{
    void foo()
    {
        cout << "AnotherDerived foo" << endl;
    }
};

template <typename T>
void ProcessFoo(Base<T> *b)
{
    b->foo();
}
//CRTP is a technique to implement compile-time polymorphism
int main()
{
    Derived d1;
    AnotherDerived d2;
    ProcessFoo(&d1);
    ProcessFoo(&d2);
    return 0;
}