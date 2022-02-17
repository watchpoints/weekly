#include <iostream>
#include <map>
using namespace std;
 
int hexTodec( const char* data )
{//将data表示的16进制字符串转为10进制整数
	if( data==NULL || *data=='\0' )
		throw std::exception("Invalid input!");
 
	unsigned int result = 0;//中间结果用无符号整数保存
	bool isPositive = true;
	if( *data == '-' || *data=='+' )
	{
		isPositive = *data=='-' ? false : true;//设置正负标记
		data++;
	}
	
	if( data[0]=='0' && data[1]=='x' )//过滤0x开头
		data += 2;
 
	static bool isMapInit = false;//标识map是否已初始化
	static map<char,int> kv;
	if( isMapInit == false )
	{
		kv['0'] = 0;
		kv['1'] = 1;
		kv['2'] = 2;
		kv['3'] = 3;
		kv['4'] = 4;
		kv['5'] = 5;
		kv['6'] = 6;
		kv['7'] = 7;
		kv['8'] = 8;
		kv['9'] = 9;
		kv['a'] = 10;
		kv['b'] = 11;
		kv['c'] = 12;
		kv['d'] = 13;
		kv['e'] = 14;
		kv['f'] = 15;
		kv['A'] = 10;
		kv['B'] = 11;
		kv['C'] = 12;
		kv['D'] = 13;
		kv['E'] = 14;
		kv['F'] = 15;
		isMapInit = true;//只初始化一次map
	}
 
	while( *data != '\0' )
	{
		if( kv.count(*data) > 0 )
			result = result*16 + kv[*data];//计算加权值
		else
			break;
 
		/*******不使用map的方案***********/
		//if( *data >= '0' && *data <= '9' )
		//{
		//	result = result*16 + *data - '0';
		//}
		//else if( *data >='a' && *data <= 'f' )
		//{
		//	result = result*16 + *data -'a' + 10;
		//}
		//else if( *data >='A' && *data <= 'F' )
		//{
		//	result = result*16 + *data -'A' + 10;
		//}
		//else
		//	break;
 
		if( isPositive&&result>0x7FFFFFFF || !isPositive&&result>0x80000000 )//判断是否溢出
			throw std::exception("int overflow");
		else
			data++;
	}
 
	return isPositive==true ? result : -(int)result;//判断正负并返回对应值
}
 

//C++ LeetCode 405 数字转换为十六进制（位运算）
//405. 数字转换为十六进制数
//https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal/
//https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal/solution/bian-cheng-xiong-jing-dian-jin-zhi-zhuan-b0ku/
//https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal/solution/python3-si-lu-qing-xi-jian-dan-yi-dong-j-563n/
//https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal/solution/python3-si-lu-qing-xi-jian-dan-yi-dong-j-563n/
//print(int(hex_val, 16))

//C++ LeetCode 405 数字转换为十六进制（位运算）
class Solution {
public:
    string toHex(int num) {
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
			cout<< "num= " <<num<<endl;
            int c=num & 0x0000000f;//每次取低4位
			cout<< "c= " <<c<<endl;
            res=hex[c]+res;
			cout<< "num>>=4 " <<num<<endl;
            num>>=4;
            p--; 
        }
        return res;
    }
};

int main()
{
	try
	{
		int result = hexTodec("-0x80000000");
		cout << result << endl;
	}
	catch(std::exception e)
	{
		cout << e.what() << endl;
	}
	system("pause");
}