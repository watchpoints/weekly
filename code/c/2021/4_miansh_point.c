#include <stdio.h>
#define MIN__TEST(a,b) ((a)<=(b)?(a):(b))
int main()
{
    int arry[5] = {10,20,30,40,50};
    int *p = &arry[0];
    //printf("1=%d\n",*p);
   //printf("2=%d\n",*p++);
   
    //printf("MIN = %d, *p = %d\n", MIN__TEST(*p++, 10), *p);
    return 0;
    
}
