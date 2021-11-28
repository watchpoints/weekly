// 将字符串复制到数组 dest 中
#include <stdio.h>
#include <string.h>
 
int main ()
{
   //const char src[50] = "http://www.runoob.com";
   char *ptr =NULL;
   char dest[50];
 
   memcpy(dest, ptr,4);
   printf("dest = %s\n", dest);
   
   return(0);
}
