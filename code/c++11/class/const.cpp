#include <cstdio>
#include <iostream>

using namespace std;

class CTest
{
public:
void show()
{
    cout<<m_a;
}
CTest(int a)
{
 m_a =a;
}
public:
int m_a;
};

int main(){

 const CTest ct(10);
 //ct.m_a=10;//error: cannot assign to variable 'ct' with const-qualified type
 ct.show();//error: 'this' argument to member function 'show' has type 'const CTest', but function is not marked const
}
