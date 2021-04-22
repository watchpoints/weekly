
/**
 寻找异常数字。
 给 N 个整数，其中：
* N-1 个数字是 1， 2， 3， ... N-1；
* 另外一个数字是任意一个其它数字：可以在 1 到 N-1之间，也可以之外。

举例：

case1 1 3 2 3 里面 3是个重复数字， 所以是3 
      1 2 3 ?
----->  N==4 特点  重复

case2 1 3 2 4 里面 4 超过 N-1，所以是 -----4 -
      1 2 3 ?
----> N=4 ,特点 不包括4

1 3 2 -1 里面是 -1  
1 2 3  -1?
--- N=4,不是负数 

请实现一个函数，找出这个其它数字。并完成相关单元测试

函数声明：int findStrangeNum(int* numbers, int numNumber)；

**/

#include <iostream>
using namespace std;

int findStrangeNum(int* numbers, int numNumber)
{

    int total = numNumber * (numNumber + 1) / 2 - numNumber;
    int sum = 0;
    for (int i = 0; i < numNumber; i++) {
        sum += numbers[i];
    }
    return sum - total;
}

int main()
{
    int case1[] = { 1, 3, 2, -1 };

    if (findStrangeNum(case1, sizeof(case1) / sizeof(int)) != -1) {

        cout << "case 1 failed" << endl;
    }

    int case2[] = { 1, 3, 2, 4 };
    if (findStrangeNum(case2, sizeof(case1) / sizeof(int)) != 4) {
        cout << "case 2 failed" << endl;
    }

    int case3[] = { 1, 3, 2, 3 };
    if (findStrangeNum(case3, sizeof(case1) / sizeof(int)) != 3) {
        cout << "case 3 failed" << endl;
    }
    return 0;
}