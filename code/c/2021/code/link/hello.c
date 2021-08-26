#include <stdio.h>
struct Point
 {
                int x; // 合法
                int y; // 合法
               /*
                void print()
                {
                        printf("Point print\n"); //编译错误
                };*/
};
int f()
{
 static int fff=1111;
 return 1;
}
int g()
{

 static int fff=1111;
 return 1;
}
int main()
{
   printf("Hello, World!");
   return 0;
}
