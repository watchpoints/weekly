#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
     printf("hello=%d\n",200*300*400*500);
     // warning: integer overflow in expression 
     unsigned long a =200*300*400*500;
     cout<< a << endl;
     cout<<(unsigned)(-1);

     
    return 0;
}