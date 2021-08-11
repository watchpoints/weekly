#include <stdio.h>

int func(void)
{
    printf("func in C...\n");
}
//gcc -S 04_c_ovrider.c 
int main()
{

    func();

}
