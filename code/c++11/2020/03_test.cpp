#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

//执行报错，a是数组类型，地址固定不变，已经绑定了，无法改变自己a++
void test1()
{
	char a[] = "abcd!";
	
	//"abcd!字符串常量 
	//a[] stack上数据。 
	// = copy 赋值，
	//“abcd”存在2份，一个是stack上，一个静态区域
	//a++ == a=a+1，报错const pointer 无法是被修改的。
	
	cout<< *a<<endl;//
	//cout<< a++<<endl;//error: lvalue required as increment operand a+1
	//cout<< *a++<<endl; //error: lvalue required as increment operand
     cout<< *(a+1)<<endl;	
	//*a++ = 'A'; //error: lvalue required as increment operand
	//i++ // i=i+1
	//https://stackoverflow.com/questions/3364445/lvalue-required-as-increment-operand
	a[1]='@'; //这是2个不同空间，stack。
	puts(a);	
	//https://stackoverflow.com/questions/21476869/constant-pointer-vs-pointer-to-constant
	int aa = 10;
	int *const ptr = &aa;  
	*ptr = 5; // right
	//ptr++;    // wrong //error: increment of read-only variable ‘ptr’
	//https://www.geeksforgeeks.org/difference-const-char-p-char-const-p-const-char-const-p/
}  //Const pointers vs Pointing to const variables //https://www.learncpp.com/cpp-tutorial/610-pointers-and-const/
////Const pointers vs Pointing to const variables //https://www.learncpp.com/cpp-tutorial/610-pointers-and-const/

// 编译告警：string constant to ‘char*’
//执行Segmentation fault
void test2()
{   
    char *s = "abcd!";
	s[3] = '#'; 
	puts(s); //
	
	////Segmentation fault
    //	const char *s = "abcd!"; //read-only	
	//Which means this is illegal and results in a segfault
	//https://stackoverflow.com/questions/2346806/what-is-a-segmentation-fault
	//https://segmentfault.com/a/1190000015372857
	//What is a segmentation fault?
	//01 null pointer  
	// 02 write to a portion of memory that was marked as read-only:
	//https://qastack.cn/programming/164194/why-do-i-get-a-segmentation-fault-when-writing-to-a-string-initialized-with-cha

	//预期输出
	//conversion from string constant to ‘char*’
	///https://stackoverflow.com/questions/1524356/c-deprecated-conversion-from-string-constant-to-char
   /***
   //https://hit-alibaba.github.io/interview/basic/arch/Memory-Management.html
   只读部分包括程序代码（.text）和程序中的常量（.rodata）。
    可读写部分（也就是变量）大致可以分成下面几个部分：
   .data： 初始化了的全局变量和静态变量
   .bss： 即 Block Started by Symbol， 未初始化的全局变量和静态变量（这个我感觉上课真的没讲过啊我去。。。）
   heap： 堆，使用 malloc, realloc, 和 free 函数控制的变量，堆在所有的线程，共享库，和动态加载的模块中被共享使用
   stack： 栈，函数调用时使用栈来保存函数现场，自动变量（即生命周期限制在某个 scope 的变量）也存放在栈中。
   **/
   
   //https://stackoverflow.com/questions/6958222/why-do-i-get-a-segmentation-fault-when-i-try-to-modify-a-string-constant
 
 
   //Why a segmentation fault for changing a non-const char*?
 //https://stackoverflow.com/questions/6949150/why-a-segmentation-fault-for-changing-a-non-const-char
 
 
 //const char * cp = "Hello"; //OK
char* p = "Hello"; //OK in C and C++03 (unfortunately), Illegal in C++11
//cp[0] = 'Y'; //Compile-time error, good
//p[0] = 'Y'; //no compiler error, undefined behavior

}

struct XX {
 char a; //11
 int b; //4
 long c;//8
};
struct structA
{
	int a; //4
	char b;//1
};

struct structB
{
	char a; //1
	short b; //2 
};

struct structC
{
	int a; //4
	char b;//4
	float c;//8
};

//本机机器字长为64位
void test3()
{
 char str[] = "Hello";
 char *p  = str;
 int i = 10;
 long l = 1;
 struct XX xx;
 struct structA aa;
 struct structB bb;
 struct structC cc;
 printf("sizeof(str) = %d\n", sizeof(str));
 printf("sizeof(p) = %d\n", sizeof(p));
 printf("sizeof(i) = %d\n", sizeof(i));
 printf("sizeof(l) = %d\n", sizeof(l));

	printf("sizeof(aa) = %d\n", sizeof(aa)); //8
	printf("sizeof(bb) = %d\n", sizeof(bb));//4
	printf("sizeof(cc) = %d\n", sizeof(cc));//16


printf("sizeof(xx struct) = %d\n", sizeof(xx));


 
struct XX &ref=xx;
printf("sizeof(ref) = %d\n", sizeof(ref)); //sizeof(ref) = 16

//sizeof(str) = 6
//sizeof(p) = 8
//sizeof(i) = 4
//sizeof(l) = 8
//sizeof(xx struct) = 16
}

void test4()
{
  int *ptr =0;
  const int &ref =0; //0  rvalue 
  
  //int &ref =0; //0  rvalue 
  // error: invalid initialization of non-const reference of type ‘int&’ from an rvalue of type ‘int’
  
  //01 sizeof
  char * ptr1= "abc";
  const char &ref1='a'; // "abc" const char* 
  //nvalid initialization of non-const reference of type ‘char&’ from an rvalue of type ‘const char*’
}
// return ref
int & test5()
{
	int a=10;
	return a;
	
} 
//https://www.codeproject.com/Articles/453022/The-new-Cplusplus-11-rvalue-reference-and-why-you


class Base 
{
    virtual void method() {std::cout << "from Base" << std::endl;}
public:
    virtual ~Base() {method();}
    void baseMethod() {method();}
};

class A : public Base 
{
    void method() {
		std::cout << "from A" << std::endl;}
public:
    ~A() 
	{
		method(); //构造函数 析构函数内 函数 内的 虚函数 不表现多态
	} 
};

void test6() 
{
    Base* base = new A;
    base->baseMethod();
    delete base;
	
	//from A
	//from A //析构自己  this -> A
	//from Base  /析构自己  this -> A
    
}


//g++ -std=c++11 -g  -w 03_test.cpp 
int main(int argc, const char *argv[])
{  
   test6();
  //test3();
 // test2();
 // test1();
 return 0;
}