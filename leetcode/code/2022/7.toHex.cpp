#include <iostream>
#include <map>
using namespace std;

//C++ LeetCode 405 数字转换为十六进制（位运算）
//https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal/
class Solution {
public:
    string toHex(int num) 
    {
        if(num==0)
            return "0";
        string hex={"0123456789abcdef"};
        string res="";
        int p=-1;
        if(num<0)
            p=8; //负数右移8次
        while(num)
        {
            if(p==0)
                break;
            int c=num & 0x0000000f;//每次取低4位
            cout << "num & 0x0000000f>>:" << c << "hex[c]="<<hex[c]<< endl;
            res=hex[c]+res;
            num>>=4;
            cout << "num>>:" << num << endl;
            p--; 
        }
        return res;
    }
};
//g++ -std=c++11 7.toHex.cpp
int main()
{
  //bin:0001 0010
  //hex:12 
  //dec:18  
  //18(十进制) = 10010(二进制)
  Solution test;
  cout<< test.toHex(18)<<endl;
}
