#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip> // setprecision
#include <sstream> // stringstream
using namespace std;
void test_float();
void removeLastZero(char *numstr);
int main()
{
   /**
    float pi = 1.102;


    char str[1024];
    double num = 9999999999.102;
    sprintf(str, "%.2f", num);
    removeLastZero(str);
    std::cout << str << std::endl;**/

    test_float();

    return 0;
}

bool removeLastZero(char *numstr)
{
    if (NULL == strchr(numstr, '.'))
        return false;

    int length = strlen(numstr);
    for (int i = length - 1; i > 0; --i)
    {
        if ('\0' == numstr[i])
        {
            continue;
        }
        else if ('0' == numstr[i])
        {
            numstr[i] = '\0';
        }
        else if ('.' == numstr[i]) // 小数点之后全为零
        {
            numstr[i] = '\0';
            break;
        }
        else // 小数点后有非零数字
        {
            break;
        }
    }

    return true;
}


void test_float()
{   
    //float a=1.9999;
    float a=1.1;
    char str[1024];
    sprintf(str, "%.2f", a);
    cout<< "a: "<<a <<"str:"<<str<<endl;

    removeLastZero(str);
    std::cout <<"str:"<< str << std::endl;

    // 将字符串转换为浮点数
    float num_float = std::stof(str);

    cout<< "num_float: "<<num_float<<endl;
}