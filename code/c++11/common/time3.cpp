#define _CRT_SECURE_NO_WARNINGS 1
#include "iostream"
#include "time.h"
#include "string.h"
using namespace std;

void time_int_sting(int stampTime)
{
	time_t tick = (time_t)stampTime;
	struct tm tm; 
	char s[100];
	
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	printf("%d: %s\n", (int)tick, s); 
	int Year = atoi(s);
	int Mon = atoi(s+5);
	int Day = atoi(s+8);
    cout<< Year << "-"<<Mon << "-"<<Day<<endl;

	
}

int main()
{
    stamp_to_standard(1496299670);
	return 0;
}