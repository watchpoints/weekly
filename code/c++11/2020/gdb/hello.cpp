<<<<<<< HEAD
#include <stdio.h>

#include "hello.h"
int bar(int c, int d);
//‘bar’ was not declared in this scope 没有声明

////////////////////////////////////////////////////////

int foo(int a, int b)
{
    return bar(a, b);
}

//undefined reference to `bar(int, int)'
int bar(int c, int d)
{
    int e = c + d;
    return e;
}

int main(int argc, char **argv)
{
    foo(2, 5);
    return 0;
=======
#include "hello.h"



int main()
{
cout<<foo(10,20)<<endl;
return 0;
>>>>>>> f577c8c170f24658a4ac57545b79660714e1201a
}
