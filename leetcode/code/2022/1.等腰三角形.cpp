#include <iostream>
#include <vector>
using namespace std;

void triangle(int n)
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            cout<< " ";
        }
       
        for (int j = 0; j < 2 * i - 1; j++)
        {
            cout<< "*";
        }
        cout<< endl;
    }
}


void printTree(int level)
{
    //观察 层次 节点 空格直接的函数关系


}

int main()
{
    triangle(5);
}

/**

请实现一个返回指定高度圣诞树的函数。

例如height = 5应返回:

    *
   ***
  *****
 *******
*********


    *    1- 1  =2-1     4
   ***   2 --3 =4-1     3
  *****  3 --5 =6-1     2
 ******* 4 --7 =8-1     1
********* 5 --9 =10-1   0
高度为 0 到 100 之间的整数。
用空格填充，使每行长度相同。最后一行只有星星，没有空格
特点：
等腰三角形
~~~

 */