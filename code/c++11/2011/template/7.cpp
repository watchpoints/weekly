//【编程实验】不完美转发和完美转发
#include <iostream>
using namespace std;

void print(const int& t)  //左值版本
{
    cout <<"void print(const int& t)" << endl;
}

void print(int&& t)     //右值版本
{
    cout << "void print(int&& t)" << endl;
}

template<typename T>
void testForward(T&& param)
{
    //不完美转发
    print(param);            //param为形参，是左值。调用void print(const int& t)
    print(std::move(param)); //转为右值。调用void print(int&& t)

    //完美转发

    //函数怎么还有<T呀？>
    print(std::forward<T>(param)); //只有这里才会根据传入param的实参类型的左右值进转发
    //https://www.cplusplus.com/reference/utility/forward/
}

int main()
{
    cout <<"-------------testForward(1)-------------" <<endl;
    testForward(1);    //传入右值

    cout <<"-------------testForward(x)-------------" << endl;
    int x = 0;
    testForward(x);    //传入左值

    return 0;
}
/*输出结果
-------------testForward(1)-------------
void print(const int& t)
void print(int&& t)
void print(int&& t)       //完美转发，这里转入的1为右值，调用右值版本的print
-------------testForward(x)-------------
void print(const int& t)
void print(int&& t)
void print(const int& t) //完美转发，这里转入的x为左值，调用左值版本的print
*/


