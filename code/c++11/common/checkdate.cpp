
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

bool check_date( const char* dates)
{   
    if(nullptr ==dates)
    {
        return false;
    }
    char *ptr = const_cast<char *>(dates);
    int len = strlen(ptr);
    char pdate[len];
    memcpy(pdate, ptr, len);
   

    bool ret =true;
    bool jyear =false;  
    bool jmonth =false;
    bool jday =false;
    int year =0;
    int month =0;
    int day =0;
    int count =0;

    char *p;
    const char *split = "-"; //可按多个字符来分割
    p = strtok(pdate, split);
    while(p)
    {  
        int data =atoi(p);
        cout<< "data=" <<data;
        if(count == 0)
        {
            year =data;
        }else if(count ==1)
        {
            month =data;
        }else if(count ==2)
        {
            day =data;
        }
        count++;
        p = strtok(NULL, split);
    }
    
    cout << "please input year:" <<year <<endl;
    cout << "please input month:" <<month <<endl;
    cout << "please input day:" <<day<<endl;

    if (year >=1970 && year <=2038)
    { 
        jyear = 1;
    }
    else
    {
         jyear = 0;
    }

    if (month >= 1 && month <= 12)
    {
         jmonth = 1;
    }
    else
    {
        jmonth = 0;   
    }
    

    if (month == 4 || month == 6 || month == 9 || month == 11)
    {

        if (day >= 1 && day <= 30)
            jday = 1;
        else

            jday = 0;
    }
    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {

            if (day >= 1 && day <= 29)

                jday = 1;

            else

                jday = 0;
        }
        else
        {

            if (day >= 1 && day <= 28)

                jday = 1;
            else
                jday = 0;
        }
    }
    else
    {

        if (day >= 1 && day <= 31)

            jday = 1;

        else

            jday = 0;
    }

    if (jyear == 1 && jmonth == 1 && jday == 1)
    {
        cout << "输入的年月日合法。" << endl;
    }
    else   
    {       
            cout << "输入的年月日不合法。" << endl;
            ret =false;
           
    }
   

    return ret;
}

bool isValid_date(const char* pdata)//判断日期（年月日）是否合法的函数定义
{   
    if(nullptr ==pdata)
    {
        return false;
    }
    int year, month, day;
    sscanf(pdata, "%d-%d-%d", &year, &month, &day);
        
    cout << "please input year:" <<year <<endl;
    cout << "please input month:" <<month <<endl;
    cout << "please input day:" <<day<<endl;

    int leap=0;  //判断闰年的标记
	
	if(month<1||month>12||day<1||day>31)
	    return 0; //返回不合法
	
	if((month==4||month==6||month==9||month==11)&&(day==31))
	     return 0;//返回不合法

	if((year%4==0&&year%100!=0)||year%400==0)//判断是否是闰年
		leap=1; //是闰年

    if((leap==1&&month==2&&day>29)||(leap==0&&month==2&&day>28))
         return false;//返回不合法

	return true; //返回合法
}
int main()
{   
     //char * ptr ="2017-11-31";
     //char * ptr ="2021-2-29";
     //char * ptr ="2021-2-30";
     //const char * ptr ="2017-6-31";
     //const char * ptr ="2017-2-29";
     const char * ptr ="2017-12-31";
    
    //为什么不能用指向“字符串“常量的指针呢？
    //strtok函数会对你的输入字符串进行修改
    if(check_date(ptr))
    {
       cout << " check_date 输入的年月日合法。" << endl;
    }else
    {
         cout << " check_date 输入的年月日不合法。" << endl;
    }

    if(isValid_date(ptr))
    {
       cout << " isValid_date 输入的年月日合法。" << endl;
    }else
    {
         cout << " isValid_date 输入的年月日不合法。" << endl;
    }

    

    return 0;
}
