<<<<<<< HEAD
#include <stdlib.h>
#include <stdio.h>
void func()
{
 //只申请内存而不释放
    void *p=malloc(sizeof(int));
}
int main()
{
    func();
    getchar();
    return 0;
=======
#include <stdlib.h>
#include <stdio.h>
void func()
{
 //只申请内存而不释放
    void *p=malloc(sizeof(int));
}
int main()
{
    func();
    getchar();
    return 0;
>>>>>>> 55d65b891bace52a834abb8e74e3ab2ec75d63f6
}