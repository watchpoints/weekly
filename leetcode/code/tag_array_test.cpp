#include "array_all.h"
#include <vector>
#include <iostream>
#include "mystring.h"
using namespace std;
 //162 寻找峰值
void test1()
{
    vector<int> test={1,1,1,1,4,1,1,1,1,1,1,1}; //死循环
    Solution162 s;
    cout<< s.findPeakElement(test) <<endl;

}

void test_string()
{
    Solution1 s1;
    string input1="  Hello my    word !    ";
    //string input1="Hello my   word!    ";
    cout<<s1.delSpace(input1)<<endl;
    string input2="We are happy.";
    cout<<"right:We%20are%20happy. mycode ="<<s1.replaceSpace(input2)<<endl;

    string input3="hello";
    s1.myreverse(input3,0,input3.size());
    cout<<"right:olleh mycode ="<<input3<<endl;

}
//g++ -std=c++11 tag_array_test.cpp
int main()
{

    //test1();
    test_string();
    return 0;
}