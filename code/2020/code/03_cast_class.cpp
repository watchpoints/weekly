<<<<<<< HEAD
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector> 
#include <climits>
using namespace std;


struct Base {};
struct Derived : public virtual Base {};

//static_cast 不能用于虚继承
void f()
{
    Base* b = new Derived;
    Derived* d = static_cast<Derived*>(b); //
}

int main()
{
    
       return 0;
}


/***
 * https://stackoverflow.com/questions/7484913/why-cant-static-cast-be-used-to-down-cast-when-virtual-inheritance-is-involved
=======
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector> 
#include <climits>
using namespace std;


struct Base {};
struct Derived : public virtual Base {};

//static_cast 不能用于虚继承
void f()
{
    Base* b = new Derived;
    Derived* d = static_cast<Derived*>(b); //
}

int main()
{
    
       return 0;
}


/***
 * https://stackoverflow.com/questions/7484913/why-cant-static-cast-be-used-to-down-cast-when-virtual-inheritance-is-involved
>>>>>>> 55d65b891bace52a834abb8e74e3ab2ec75d63f6
 **/ 