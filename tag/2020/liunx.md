

> 阻塞系统调，例如屏幕输入字幕，屏幕永远不输入，程序永远不结束，如何解决？

- 早期的 UNIX 系统特性是，当某个进程执行一个长时间系统调用时发生阻塞， 如果此时捕获到一个信号，则系统调用便被中断且不再执行，并已失败返回。 因此系统调用也会被分为两类：低速系统调用和高速系统调用。低速系统调用可能使进程永远阻塞， 例如 IO。

早期UNIX系统的一个特性是：如果进程在执行一个低速系统调用而阻塞期间捕捉到一个信号，则该系统调用就被中断不再继续执行。该系统调用返回出错，其errno设置为EINTR。

这样处理是因为一个信号发生了，进程捕捉到它，这意味着已经发生了某种事情，所以是个好机会应当唤醒阻塞的系统调用。

在这里，我们必须区分系统调用和函数。当捕捉到某个信号时，被中断的是内核中执行的系统调用。



可以用中断系统调用这种方法来处理的一个例子是：

一个进程启动了读终端操作，而使用该终端设备的用户却离开该终端很长时间。

在这种情况下，进程可能处于阻塞状态几个小时甚至数天，除非系统停机，否则一直如此。

```c
again :
    if((n = read(fd, buf, BUFFSIZE)) < 0){
        if(errno == EINTR){
            goto again; // just an interrupted system call
            // handle other errors
        }
    }
32

进程阻塞系统调，例如屏幕输入字幕，屏幕永远不输入，程序永远不结束，如何解决？
    
 
The call was interrupted by a signal before any data was read.
表示：由于信号中断，没读到任何数据。
    
Many system calls will report the EINTR error code 
if a signal occurred while the system call was in progress
虽然是异步，感觉上马上返回结果。

简单做法 kill 发送信号
```



- 软中断实际上是以内核线程的方式运行的，每个 CPU 都对应一个软中断内核线程，这个软中断内核线程就叫做  ksoftirqd/CPU 编号。那要怎么查看这些线程的运行状况呢？

watch -d cat /proc/softirqs

TIMER（定时中断）、NET_RX（网络接收）、SCHED（内核调度）、RCU（RCU 锁）

~~~
# -n DEV 表示显示网络收发的报告，间隔1秒输出一组数据$ sar -n DEV 1
~~~

- ```
  tcpdump -i eth0 tcp port 9821 -w 9821.dump
  
  
  ```







- ss -ltnp

