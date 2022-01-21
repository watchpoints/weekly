#include <stdio.h>  
int main()  
{   
   const char *ptest ="abc";
    const int i = 10;  
    int* pi = (int*)(&i);  
    *pi = 100;  
    printf("*pi: %d\n",*pi);  
    printf("i: %d\n",i);  
    printf("pi: %p\n",pi);  
    printf("&i: %p\n", &i);  
    return 0;  
} 
