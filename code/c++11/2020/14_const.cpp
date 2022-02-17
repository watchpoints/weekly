#include<stdio.h>
#include<stdlib.h>

int main(void) {
    const int a = 1; //a is constant
    fprintf(stdout,"%d\n",a);//prints 1
    int* a_ptr = &a;
    *a_ptr = 4;//memory leak in c(value of a changed)
    fprintf(stdout,"%d",a);//prints 4
return 0;
}
