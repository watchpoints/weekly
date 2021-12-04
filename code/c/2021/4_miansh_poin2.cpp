#include <stdio.h>
#include<iostream>
using namespace std;
int main()
{	

float a = 1.0f;
std::cout << (int)a << std::endl; //1
std::cout << (int&)a << std::endl;//????/
std::cout << std::boolalpha << ( (int)a == (int&)a ) << std::endl; 

float b = 0.0f;
std::cout << (int)b << std::endl;
std::cout << (int&)b << std::endl;
std::cout << std::boolalpha << ( (int)b == (int& )b ) << std::endl; // 结果输出
 
  return 0;
}
