#include <stdio.h>
#include <time.h>

int main(void) 
{ 
	time_t tick;  //这是一个适合存储日历时间类型。
	struct tm tm; //这是一个用来保存时间和日期的结构。
	
	char s[100]; 
	tick = time(NULL); 
	tm = *localtime(&tick); 
		//struct tm *localtime(const time_t *timer)
    	//timer 的值被分解为 tm 结构，并用本地时区表示。
	strftime(s, sizeof(s), "%Y-%m-%d", &tm); 
		//size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr)
		//str -- 这是指向目标数组的指针，用来复制产生的 C 字符串
		//maxsize -- 这是被复制到 str 的最大字符数。
		//format -- 这是 C 字符串，包含了普通字符和特殊格式说明符的任何组合。这些格式说明符由函数替换为表示 tm 中所指定时间的相对应值。格式说明符是：
	printf("%d:\n", (int)tick); 
	printf("%s\n",  s); 
	return 0; 
}