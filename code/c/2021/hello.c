#include <stdio.h>
struct Point
 {
                int x; // 合法
                int y; // 合法
                void print()
                {
                        printf("Point print\n"); //编译错误
                };
};
int main()
{
   printf("Hello, World!");
   return 0;
}
