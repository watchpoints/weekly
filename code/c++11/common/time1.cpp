#include <iostream>
#include <ctime>
#include <string.h>
using namespace std;
 
time_t strTime2unix(char timeStamp[])
{
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    
    sscanf(timeStamp, "%d-%d-%d", 
           &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
     
    tm.tm_hour =0;
    tm.tm_min =0;
    tm.tm_sec =0;
    
    tm.tm_year -= 1900;
    tm.tm_mon--;
 
    return mktime(&tm);
}
 
int main()
{
	char timeStamp[100] = "2015-10-09";
    time_t t = strTime2unix(timeStamp);
    cout << t << endl;
    int a =t;
    cout << a << endl;
    
	// additional
	cout << ctime(&t) << endl;
 
    return 0;
} 