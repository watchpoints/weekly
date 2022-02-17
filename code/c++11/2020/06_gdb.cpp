#include <stdio.h>

int func(int a, int b)
{
	int c = a * b;
	
	printf("c is %d\n", c);
	
	return c;
}

int main(int argc,char *argv[])
{
	func(1,2);
	
	return 0;
}