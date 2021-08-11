#include <signal.h>
#include <stdio.h>
#include <unistd.h>
 
struct two_int { int a, b; } data;
 
//从内核到用户空间执行
void signal_handler(int signum)
{
   printf ("%d, %d\n", data.a, data.b);
   alarm (1); //1秒后发送信号 执行这里代码，奇怪从哪里调用呢
} //异步处理
 
int main (void)
{
 
 static struct two_int zeros = { 0, 0 }, ones = { 1, 1 };
 
 signal (SIGALRM, signal_handler);
 
 data = zeros;
 
 alarm (1); //1秒后发送信号
 
while (1)
{
  data = zeros;
  data = ones; //发信号能中断代码的执行
 }
}