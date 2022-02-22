#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
     printf("hello=%d\n",200*300*400*500);
     // warning: integer overflow in expression 
     cout<<(unsigned)(-1);

     
    return 0;
}