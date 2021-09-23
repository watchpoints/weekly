#include <iostream>
using namespace std;
//最终递归函数
void print() { cout << "empty" << endl; }

template <class T>
void print(T t){ cout << t<< endl;}

template <class T>
void print1(T t, T t1){ cout << t<<"****" << t1<< endl;}

// 展开函数
template <typename T, typename... Args>
void print(T head, Args... args)
{
    cout << head << ",";
    print(args...);
}

int main()
{
    print(1, 2, 3, 4);
    return 0;
}

