#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;
typedef unsigned char BYTE;
//小白疑问：大小端读取和字段类型有关系吗？
//int num =0x12345678
//按照大小端概念理解 int c=num & 0x0000000f 应该是1,结果怎么是读取的8那
//不一样呢 我混淆什么概念吗？


//回答：大小端对移位运算有没有影响？
// 大小端是内存存储数据高位和数据低位的不同顺序；
//而移位移的是寄存器中的内存，与内存是无关的。


// C++ LeetCode 405 数字转换为十六进制（位运算）
// https://leetcode-cn.com/problems/convert-a-number-to-hexadecimal/
class Solution
{
public:
    string toHex(int num)
    {
        if (num == 0)
            return "0";
        string hex = {"0123456789abcdef"};
        string res = "";
        int p = -1;
        if (num < 0)
            p = 8; //负数右移8次
        while (num)
        {
            if (p == 0)
                break;

            int c = num & 0x0000000f; //每次取低4位
            res = hex[c] + res;
            num >>= 4;
             // 题目：C++ LeetCode 405 数字转换为十六进制（位运算）
            //小白疑问：大小端读取和字段类型有关系吗？
            //int num =0x12345678
            //按照大小端概念理解 int c=num & 0x0000000f 应该是1,结果怎么是读取的8那
            //不一样呢 我混淆什么概念吗？
            

            //问题：移位 不考虑大小端吗？
            printf("num>>4  0x%x \n", num);
            //移位命令属于上层业务， 大小端存储属于下层业务 这里理解吗
            p--;
        }
        return res;
    }
};

void test_cpu()
{
    //编写一段程序判断系统中的CPU 是Little endian 还是Big endian 模式
    //知识串烧：
    // sizeof(char)= 1字节,1 Byte（B）= 8 bit； 4Bit为16进制一位.一个字节对应16进制的2个数字。
    // hex: 0x12  bin:0001 0010    dec: 12

    //实践经验：
    // x86 系列CPU 都是little-endian 的字节序
    //假设从内存地址0x0000 开始有以下数据 0x12 0x34 0xab 0xcd
    //如果我们去读取一个地址为0x0000 的四个字节变量(32bit)
    // big-endian 0x12 0x34 0xab 0xcd
    // little-endian 0xcd 0xab 0x34 0x12 第一个字节是最低位字节
    
    //a是一个int类型，占用4个字节，如果是小端，那么1应该先放到低地址；
    //如果是大端，那么 1应该放到高地址。(char*)&a是把a的指针强转成一个char的指针，
    //那么默认指的位置也就是低地址，低地址的第一个字节如果保存的是1，那么就是小端，如果不是1，就是大端
    int num = 1;
    if (*((char*)&num) == 1)
    {
        cout << "little";
    }

    unsigned int num1 = 255; // 0xff
    unsigned char *p1 = (unsigned char *)&num1;
    printf("dex ==%d,hex num1= 0x%x,Address =%p,*p=%d \n", num1, num1, p1, *p1);
    // int a =255
    // bin:1111 1111
    // hex: 0xff
    //  32 big-endian    0x ff  00  00  00
    //  32 little-endian 0x 00  00  00  ff
    //指针判断方法:写入int，读取字节
    if (*p1 == 0xff)
    {
        printf("The endian of cpu is little\n");
        //疑问：是读取前面8bit 还是后面8bit 还是整个32bit
        // char* ptr ="abcd"
    }
    else // num == 0xff000000
    {
        printf("The endian of cpu is big\n");
    }
    //指针判断方法:写入字节，读取整数
    unsigned int num2 = 0;
    unsigned int *p2 = &num2;
    *(unsigned char *)p2 = 0xff;
    printf("dex ==%d,hex num1= 0x%x,Address =%p,*p=%d \n", num2, num2, p2, *p2);
    if (num2 == 0xff)
    {
        printf("The endian of cpu is little\n");
    }
    else // num == 0xff000000
    {
        printf("The endian of cpu is big\n");
    }

    //使用联合体判断方法
    union u
    {
        int a;
        char b;
    } c;
    c.a = 1;
    printf("write int  1, read char =%d \n", c.b);

    //字符串表示16进制 这个和上面没有关系呢
    const char a[] = "0x120x340x560x780xff";
    char tmpHex[5];
    int num;
    printf("the size of pattern:%d\n", sizeof(a)); // 4*4+最后有一个'\0'
    for (int i = 0; i < sizeof(a) - 1; i += 4)
    {
        strncpy(tmpHex, a + i, 4);
        tmpHex[4] = '\0';
        //16--10
        sscanf(tmpHex, "%x", &num);
        printf("%s %d\n", tmpHex, num);
    }
    //unsigned int value = 0x12345678为
    //unsigned char buf[4]

    unsigned int num3 = 0x12345678;
    unsigned char *p3 = (unsigned char *)&num3;
    //int lowbit =num3 && 0x000000ff;//
   // printf(" lowbit dex ==%d,hex num1= 0x%x,Address =%p,*p=%d \n", num3, num3, p3, *(p3+4));

   //问题：大小端对移位运算有没有影响？
   // 大小端是内存存储数据高位和数据低位的不同顺序；
   //而移位移的是寄存器中的内存，与内存是无关的。

}
// g++ -std=c++11 7.toHex.cpp -g
int main()
{

   // test_cpu();
     Solution case1;
     int a= 305419896; //0x12345678
     //int a= 255; //0xff
     cout<< case1.toHex(a)<<endl;
}

/**
https://blog.csdn.net/lqglqglqg/article/details/48025329
https://www.cnblogs.com/Hijack-you/p/11960858.html
**/

//https://maimai.cn/web/gossip_detail?encode_id=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlZ2lkIjoiNmZlMGZiNDE1ODczNGRlMmEzOTQ5ZmJlNDA1Njg2YWEiLCJpZCI6Mjk5NjQ5MjMsInUiOjQ2NjUxfQ.kPMIqIb60uebCKDk6I5ERZI7lspV9VQbW0WDQqizrR4&from=list