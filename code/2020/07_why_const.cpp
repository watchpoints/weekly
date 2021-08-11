#include <stdio.h>  


static int static_a;
int main()  
{  
    const int i = 10;  
    int* pi = (int*)(&i);  
    *pi = 100;  
	
    printf("*pi: %d\n",*pi);  
    printf("i: %d\n",i);  
    printf("pi: %p\n",pi);  
    printf("&i: %p\n", &i);  
    return 0;  
} 
//nm -A * |grep “aaa” | c++filt 
//objdump -S --disassemble a.out
//objdump -T  a.out
//https://godbolt.org/
//disass main
//c++里的常量折叠（或者常量替换)是将const常量放在符号表中，给其分配内存，
//但实际读取时类似宏替换。编译器直接进行替换优化

//https://stackoverflow.com/questions/62904295/understanding-pointers-in-assembler-from-machines-view

/**
C++编译器把Const对象放在了符号表之中，C语言一般是放在只读数据区。【为什么C++编译器这么做？我想一个原因就是减少一些存储操作次数】。
对于volatile变量我们应该怎么考虑？声明的voliate告诉编译器不能优化它呀，
我们只能在符号表中增加标识位来告诉编译器自己不优化volatile变量。
**/