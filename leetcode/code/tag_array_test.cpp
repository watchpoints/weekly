#include "array_all.h"
#include <vector>
#include <iostream>

using namespace std;
 //162 寻找峰值
void test1()
{
    vector<int> test={1,1,1,1,4,1,1,1,1,1,1,1}; //死循环
    Solution162 s;
    cout<< s.findPeakElement(test) <<endl;

}
int main()
{

    test1();
    return 0;
}