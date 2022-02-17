#include <iostream>
#include <memory>
#include <functional>

struct Foo
{
    void print_sum(int n1, int n2)
    {
        std::cout << n1 << n2 << '\n';
    }
    int data = 10;
};


void test_bind()
{
    std::cout << "5) bind to a pointer to member function: ";
    Foo foo;
    auto f3 = std::bind(&Foo::print_sum, &foo, 95, std::placeholders::_1);
    f3(5);
}   
int main()
{
    test_bind();
}