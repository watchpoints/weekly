#include <iostream>
using namespace std;
void test() {} //当没有参数时，执行此函数

template <typename T>
void test(T value) 
{
    cout << "value = " << value << endl;
}

template <typename T, typename... Argv>
void test(T value, Argv... argvs)
{
    cout << "value = " << value << ", " ;
    test(argvs...);
}

int main()
{
    test(1, 2, 3.0, "aa");
    return 0;
}
