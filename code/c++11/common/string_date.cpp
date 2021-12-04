#include <iostream>
#include <ctime>
#include <string>
#include <cstdint>
#include <cstring>
using namespace std;
time_t StringToDatetime(string str)
{
    char *cha = (char*)str.data();             // 将string转换成char*。
    tm tm_;                                    // 定义tm结构体。
    int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
    sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// 将string存储的日期时间，转换为int临时变量。
    tm_.tm_year = year - 1900;                 // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
    tm_.tm_mon = month - 1;                    // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
    tm_.tm_mday = day;                         // 日。
    tm_.tm_hour = hour;                        // 时。
    tm_.tm_min = minute;                       // 分。
    tm_.tm_sec = second;                       // 秒。
    tm_.tm_isdst = 1;                          // 非夏令时。
    time_t t_ = mktime(&tm_);                  // 将tm结构体转换成time_t格式。
    return t_;                                 // 返回值。
}
//https://bewaremypower.github.io/2019/07/22/Linux-C-mktime%E8%BF%9B%E8%A1%8C%E6%97%B6%E9%97%B4%E8%BD%AC%E6%8D%A2%E7%9A%84%E9%99%B7%E9%98%B1/
string DatetimeToString(time_t time)
{
	tm *tm_ = localtime(&time);                // 将time_t格式转换为tm结构体
    int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
    year = tm_->tm_year + 1900;                // 临时变量，年，由于tm结构体存储的是从1900年开始的时间，所以临时变量int为tm_year加上1900。
    month = tm_->tm_mon + 1;                   // 临时变量，月，由于tm结构体的月份存储范围为0-11，所以临时变量int为tm_mon加上1。
    day = tm_->tm_mday;                        // 临时变量，日。
  
    char rightdate[100];
    sprintf(rightdate, "%04d-%02d-%02d", year, month, day);
    return string(rightdate);
}

int date_string_int(int year,int month,int day)
{
    return year*10000+month*100+day;
}

string date_int_string( int data)
{

  int years =data/10000;
  data =data-years*10000;  

  int month=data/100;
  data =data-month*100;
  int day =data;

  cout<< " year:" <<years <<  " month:" <<month << " day="<< day <<endl;

  return "";
}

void test1()
{
    int year=2017;
    int month =11;
    int day =31;

    //2017*10000+11*100+31
    int data= date_string_int(year,month,day);
    cout<< "data =" <<data<<endl;
    date_int_string(data);



}
int main()
{   
    test1();
    /**
    //string timeStr = "2021-2-28 00:00:00";
    string date ="2017-11-31";
    cout << date << endl;
    string timeStr =date;
    timeStr.append(" 11:11:11 ");
    time_t timet = StringToDatetime(timeStr);
    cout << timet << endl;
    string timeStr2 = DatetimeToString(timet);
    cout << timeStr2 << endl;**/
    return 0;
}