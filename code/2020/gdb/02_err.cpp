#include<iostream>
void bar()
{
	foo('a');
}

int foo(char a)
{
	printf("foobar\n");
	return 0;
}

int main()
{
	bar();
	return 0;
}
