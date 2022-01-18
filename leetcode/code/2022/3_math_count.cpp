/**
 * @file 3_math_count.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-13
 *
 * @copyright Copyright (c) 2022
 *
 */
/**
 *
有这样一道智力题：“某商店规定：三个空汽水瓶可以换一瓶汽水。

如果小张手上有n个空汽水瓶，最多可以换多少瓶汽水喝？



假如 ：小张手上有十个空汽水瓶，她最多可以换多少瓶汽水喝？”

答案是5瓶，



方法如下：

先用9个空瓶子换3瓶汽水，喝掉3瓶满的，喝完以后4个空瓶子，用3个再换一瓶，喝掉这瓶满的，这时候剩2个空瓶子。

然后你让老板先借给你一瓶汽水，喝掉这瓶满的，喝完以后用3个空瓶子换一瓶满的还给老板。
https://www.nowcoder.com/questionTerminal/fe298c55694f4ed39e256170ff2c205f
*/
#include <iostream>
using namespace std;

int getnumber(int n)
{
    int ret = 0;
    while (n > 0)
    {

        if (n < 2)
        {
            break;
        }
        if (n == 2)
        {
            ret++;
            break;
        }
        ret += n / 3;
        n = n / 3 + n % 3;
    }
    return ret;
}

int mycount(int n)
{
    int total = 0;
    int empty = n;
    while (empty > 0)
    {   
        //结束条件
        if (empty == 0 || empty == 1)
        {
            break;
        }
        if (empty == 2)
        {
            total++;
            break;
        }
        total += empty / 3;
        empty = empty / 3 + empty % 3; //约束条件是什么
    }
    return total;
}
int depth(int n)
{
    if ( n == 0 || n == 1){
        return 0;
    }
    if ( n == 2){
        return 1;
    }
    return n/3 + depth(n/3 + n%3);
}
//bug1 死循环
//用三个空瓶换一瓶汽水，剩一个空瓶无法继续交换
int main()
{
    //小张手上有十个空汽水瓶，她最多可以换多少瓶汽水喝？
    cout << getnumber(10) << endl;
    cout << mycount(10) << endl;
    cout << depth(10) << endl;
    return 0;
}