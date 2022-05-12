/*写一个函数，
能接收输入一个超长字符串（比如128个字符） 32bit,
这个字符串是由十六进制数组成的一组字符串,
这个函数功能是把接到的这组字符串转换成十进制数字.并将十进制数字返回*/
#include <iostream>
using namespace std;


long long string_to_int(string input)
{
    long  long ret =0;
    //12 --18
    //123 ---291
    for(int i=0;i <input.size() ; i++)
    {
        //case1 ox12a
        char tempIndex = input[i];
        //合法检查

        int bit =tempIndex -'0';
        //a=10 b=11 c=12
        int index = bit <= 9? bit:(9+tempIndex-'a');

        //03 越界怎么处理 
         //a（不越界） +i 越界
        if((ret*16+index) < 0)
        {   
            cout<< "here2 ="<<endl;
            return ret;
            //case5 =-1
            //3.266710722441009e+3
        }
        ret = ret*16+index;
    }
    //9,223,372,036,854,775,807
    //691752902764108185

    return ret;
}

int main()
{  
   // cout << "case1 ="<<string_to_int("12") <<endl;
   // cout << "case2 ="<<string_to_int("123")<<endl; 
    //cout << "case3 ="<<string_to_int("123451234512345")<<endl; //81985162712261440  81985162712261445
    //cout << "case4 ="<<string_to_int("123451234512345a1234512345")<<endl;  //1.4422982353158207e  1311771326193673029
    cout << "case5 ="<<string_to_int("999999999999999")<<endl;  //1.4422982353158207e  3.266710722441009e+3
    
}


