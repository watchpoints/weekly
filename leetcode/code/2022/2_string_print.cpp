#include <iostream>
using namespace std;

//打印等腰三角形
/*
 *
 ***
 *****
 *******
 *********
 */
void printTree(int level)
{
    for (int i = 0; i <= level; i++)
    {
        // 01打印空格
        // 4(5-1) 3 2 1(5-4) 0(5-5)
        for (int j = 0; j < level - i; j++)
        {
            cout << " ";
        }
        // 02打印*
        // 1 3  5 7  9
        for (int j = 0; j < 2 * i - 1; j++)
        {
            cout << "*";
        }
        // 03 打印换行
        cout << endl;
    }
}
/*        level 空格
 *     1     4     1
 ***    2     3     3
 *****   3     2     5
 *******  4     1     7
 ********* 5     0     9
 */
void show(int level)
{
    for (int n = 1; n <= level; n++)
    {
        // level-n
        for (int j = 0; j < level - n; j++)
        {
            cout << " ";
        }
        // 2n-1
        for (int j = 0; j < 2 * n - 1; j++)
        {
            cout << "*";
        }
        cout << endl;
    }
}
/*
    *
   ***
  *****
 *******
*********
*/
void show1(int n)
{
    for (int level = 1; level <= n; level++)
    {
        for (int i = 0; i < n - level; i++)
        {
            cout << " ";
        }
        for (int i = 0; i < 2 * level - 1; i++)
        {
            cout << "*";
        }
        cout << endl;
    }
}
int main()
{
    printTree(5);
    show(5);
    return 0;
}