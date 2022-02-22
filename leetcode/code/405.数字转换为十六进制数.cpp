#include <iostream>
#include <stdio.h>
#include <algorithm>
using namespace std;
/*
 * @lc app=leetcode.cn id=405 lang=cpp
 *
 * [405] 数字转换为十六进制数
 *
 * https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal/description/
 *
 * algorithms
 * Easy (55.93%)
 * Likes:    233
 * Dislikes: 0
 * Total Accepted:    49.7K
 * Total Submissions: 88.9K
 * Testcase Example:  '26'
 *
 * 给定一个整数，编写一个算法将这个数转换为十六进制数。对于负整数，我们通常使用 补码运算 方法。
 *
 * 注意:
 *
 *
 * 十六进制中所有字母(a-f)都必须是小写。
 *
 * 十六进制字符串中不能包含多余的前导零。如果要转化的数为0，那么以单个字符'0'来表示；对于其他情况，十六进制字符串中的第一个字符将不会是0字符。 
 * 给定的数确保在32位有符号整数范围内。
 * 不能使用任何由库提供的将数字直接转换或格式化为十六进制的方法。
 *
 *
 * 示例 1：
 *
 *
 * 输入:
 * 26
 *
 * 输出:
 * "1a"
 *
 *
 * 示例 2：
 *
 *
 * 输入:
 * -1
 *
 * 输出:
 * "ffffffff"
 *
 *
 */

// @lc code=start
/************************
  1. num 十进制, 十六进制 包含字母  :A~F表示10~15怎么一一对应起来  string key = "0123456789abcdef";
  2. 怎么转换。需要写函数吗？
    137
    1000 1001
    0000 1111 -> 0F
    进行&运算后，高位全部清0，保留低4位

    3. 0xffffffff 表示-1 还是4294967295
    //在二进制补码运算中，最高位表示符号位，符号位是 0 表示正整数和零，符号位是 1 表示负整数
   那么-1就应该表示为1000 0001了（8bit）
   但是，在C语言里，-1的正确表示应该是1111 1111，即0xFF

    1、先取1的原码： 00000000 00000000 00000000 00000001
    2、得反码：     11111111 11111111 11111111 11111110
    3、得补码：     11111111 11111111 11111111 11111111
    //补码：反码加1称为补码
    可见，－1在计算机里用二进制表达就是全1。16进制为：0xFFFFFF
    几个特殊值，比如：正值的最大表示为0111 1111，即127。负值的最小表示为1000 0000，即-128。
    (gdb) p/x -1
    $1 = 0xffffffff
    (gdb) p 0xffffffff
    $3 = 4294967295


    #define cmp(a,b) (((a)-(b))&(1<<31))==1?-1:1; 0x80000000

    input = -15
    positive version (15) in binary: 0000 | 0000 | 0000 | 0000 | 0000 | 0000 | 0000 | 1111
    2s complement:
    invert the bits: 1111 | 1111 | 1111 | 1111 | 1111 | 1111 | 1111 | 0000
    add 1: 1111 | 1111 | 1111 | 1111 | 1111 | 1111 | 1111 | 0001

long temp=(long)Math.pow(2,32);
if(num<0){
    temp=num+temp;
}else{
    temp=num;
}
如何区别二进制32个1表示-1，还是4294967295？
https://www.zhihu.com/question/50098632
 *************************/
class Solution
{
public:
    string toHex(int num)
    {
        if (num == 0)
            return "0";

        string output;
        string key = "0123456789abcdef";
        unsigned int n = num > 0 ? num : (unsigned int)num;

        //循环8次
        while (n > 0)
        {
            int index = n & 0x000f; // 第四位
            output.push_back(key[index]);
            n = n >> 4;
        } //如果负数怎么处理。
        reverse(output.begin(), output.end());
        return output;
    }
    //假设32系统,十六进制转化为十进制
    int toInteger(string input)
    {
        unsigned int num = 0;
        for (char item : input)
        {
            int bit = item >= 'a' ? item - 'a' + 10 : item - '0'; //将字符对应的ASCII值转为数值
            num = num * 16 + bit;
        }

        return (int)num;
    }
};

int main(int argc, char *argv[])
{

    Solution temp;
    cout << "0xfffffff =" << temp.toInteger("ffffffff"); // 32bit
    cout << "0x1a = " << temp.toInteger("1a");           // 32bit
}

// @lc code=end
