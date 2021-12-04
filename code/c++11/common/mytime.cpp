#include <iostream>
#include <memory>
#include <regex.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;



int regex_match(const char *str_, const char *pat_) {
  regex_t reg;
  if (regcomp(&reg, pat_, REG_EXTENDED | REG_NOSUB))
    return -1;

  int ret = regexec(&reg, str_, 0, NULL, 0);
  if(ret == REG_NOMATCH) {
       printf(" err err err No Match\n");
       return 0;
    }
  regfree(&reg);
  return ret;
}

void check_date(const char* ptr)
{   
    char pdate[12];
    memcpy(pdate,ptr,strlen(ptr));

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
       // cout<< "data=" <<data;
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
    char years[11]={0};                             
    sprintf(years,"%04d-%02d-%02d", year,month,day);
    cout<<years<<endl;
}
string fomat_date(string date)
{ 

  time_t rawtime;
  time (&rawtime);
  struct tm * pnow=localtime (&rawtime);
  char s[100]; 
  strftime(s, sizeof(s), "%Y-%m-%d", pnow);  
  printf("Now %ld: %s\n", rawtime, s); 
  //https://www.cnblogs.com/xudong-bupt/p/3550157.html
  printf("%03d\n",4); //格式化输出
  printf("%.2f\n", 100.566);
  return date;
}

bool check_date1(char *pdate)
{
  //std::cout << "check_date" << pdate << endl;
  if (nullptr == pdate)
  {
    return false;
  }
  bool ret = true;
  bool jyear = false;
  bool jmonth = false;
  bool jday = false;
  int year = 0;
  int month = 0;
  int day = 0;
  int count = 0;

  char *p = nullptr;
  const char *split = "-"; //可按多个字符来分割
  p = strtok(pdate, split);
  while (p)
  {
    int data = atoi(p);
    cout << "data=" << data;
    if (count == 0)
    {
      year = data;
    }
    else if (count == 1)
    {
      month = data;
    }
    else if (count == 2)
    {
      day = data;
    }
    count++;
    p = strtok(NULL, split);
  }

  cout << "please input year:" << year << endl;
  cout << "please input month:" << month << endl;
  cout << "please input day:" << day << endl;

  if (year >= 1970 && year <= 2038)
  {
    if (year == 2038 && month > 2)
    {
      return false;
    }
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
    ret = false;
  }

  return ret;
}
//https://www.jlqwer.com/posts/7098.html
int main()
{
    //const char *string = "2021-2-31";
    //const char *pattern = "[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}";
    //cout<<regex_match(string,pattern)<<endl;
    
   // cout<< fomat_date("2021-2-1") <<endl;

    check_date1("2017-11-31");
    return 0;
}
