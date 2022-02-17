#include <iostream>
#include <vector>
using namespace std;

/**

请实现一个返回指定高度圣诞树的函数。

例如height = 5应返回:

    *
   ***
  *****
 *******
*********


    *    1- 1  =2-1     4    0  2*0+1   4 -0 =3
   ***   2 --3 =4-1     3    1  2*1+1   4 -1 =3
  *****  3 --5 =6-1     2    2  2*2 +1  
 ******* 4 --7 =8-1     1    3  2*3 +1 
********* 5 --9 =10-1    0    4  2*4 +1   
高度为 0 到 100 之间的整数。
用空格填充，使每行长度相同。最后一行只有星星，没有空格
特点：
等腰三角形
~~~
 */


void triangle1(int n)
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



void triangle2(int n)
{
    for (int i = 0; i < n; i++)
    {  
        //n-i
        for (int space = 0; space < n - i; space++)
        {
            cout<< " ";
        }
        //2i+1
        for (int j = 0; j < 2 * i + 1; j++)
        {
            cout<< "*";
        }
        cout<< endl;
    }
}

int main()
{
    triangle1(5);
    triangle2(5);
}

