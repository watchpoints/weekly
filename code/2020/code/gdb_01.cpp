<<<<<<< HEAD
#include<stdio.h>
int fibonacci(int n);
int main(void)
{
    printf("Fibonacci(3) is %d\n",fibonacci(3));

    return 0;
}

int fibonacci(int n)
{
    if(n<=0||n==1)
        return 1;
    else
        return fibonacci(n-1)+fibonacci(n-2);
=======
#include<stdio.h>
int fibonacci(int n);
int main(void)
{
    printf("Fibonacci(3) is %d\n",fibonacci(3));

    return 0;
}

int fibonacci(int n)
{
    if(n<=0||n==1)
        return 1;
    else
        return fibonacci(n-1)+fibonacci(n-2);
>>>>>>> 55d65b891bace52a834abb8e74e3ab2ec75d63f6
}