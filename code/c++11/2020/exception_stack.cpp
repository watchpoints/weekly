#include <iostream>
#include <exception>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define DATA 1024 * 1024 * 30 //30M

int main()
{
    double stack[DATA];
    cout << " alloc success";
    return 0;
}