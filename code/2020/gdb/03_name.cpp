#include<stdio.h>
//math 
void foo(int a)
{
	printf("foo(int)\n");
}

void bar()
{
	foo('a');
}
//not math
void foo(char a)
{
	printf("foo(char)\n");
}

int main()
{
	bar();
	return 0;
}
