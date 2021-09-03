#include<stdio.h>
void change();
int num = 1024;
//gcc varmin.c var.c
int main(void)
{
    printf("before:num is %d\n", num);
    change();
    printf("after:num is %d\n", num);
    return 0;
}
