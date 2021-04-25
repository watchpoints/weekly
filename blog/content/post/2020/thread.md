---
title: "多进程多线程篇"
date: 2019-11-29
description: ""
draft: false
tags: ["book"]
categories: ["操作系统原理"]
---

> 这里主要记录学习和探索过程。来验证正确方法才能有正确的结果。

# 走神一会

**你所寻找的东西正在寻找你。-鲁米**

What you seek is seeking you.



**你生来就有一对翅膀。为什么更喜欢一边爬行一边生活？– 鲁米**

You were born with wings. Why prefer to crawl through life?



**你不是一滴水在海洋中。你是整个海洋在一滴水中。 – 鲁米**

You are not a drop in the ocean. You are the entire ocean in a drop.



**在你的提问中寻找答案。-鲁米**

Look for the answer inside your question.






# book

![image.png](https://i.loli.net/2019/11/30/7jliopUsPdcItS8.png)

# 知识图谱



## 线程



### 1.  线程的创建过程



pthread_atfork 函数解决多线程多进程死锁问题

>  With pthread_atfork, we can install up to three functions to help clean up the
> locks.  
>
>  pthread_atfork()在`fork()之前`调用。
>
> 当调用fork时，内部`创建子进程前`在父进程中会调用prepare，内部`创建子进程成功后`，父进程会调用parent ，子进程会调用child。 



### 2. 线程的销毁过程

> 问题 1 什么场景下使用 join ，什么场景下使用 detach ?

 Detaching is not a no-no. It’s legal, a lot of high-quality production code have it, 

 it just might be **dangerous**. (一方面提倡使用，一方面说危险，哪里危险了，给出三个例子，因为工作线程无论是简单业务逻辑还是异步处理 执行时间在多线程环境下是不可预估的。一切无法控制的，这多线程危险)

So, think carefully when choosing between join and detach. 



 Whether an `std::thread` object **is** or **is not** *joinable* determines whether you **may** or **may not** *join* or *detach* it. 

it up to you

it up to you

（我哪里知道？？）



> 小王：第一次尝试回答 

- If at any point the object is *joined* (by calling `*join()*`), the parent thread will wait there for the child to complete its job.

- If it’s *detached* (by calling `*detach()*`), it will keep running in the background and the parent thread won’t wait for it to finish.



> 老王：不懂 不懂 不懂，我问什么时候该设置 join /detach，什么时候不该什么设置join /detach，
>
> 你回答用法.不符合题目要求，你考虑下main函数是如何离开的？

   This answer is aimed at answering question in the title（ when should `std::thread::detach` should be used? ）, rather than explaining the difference between `join` and `detach`. 





> 小王一头雾水，表示 还能怎么分析，没有思路,



##### man pthread_detach  

 http://man7.org/linux/man-pages/man3/pthread_detach.3.html 

![man](https://i.loli.net/2019/11/30/R38sZjhMDom4ULn.png)



 The detached attribute merely determines the behavior of the system when the thread terminates; it does not prevent the thread from being terminated  

if  the process terminates **using exit(3) (or equivalently, if the main thread returns).**



一个线程异常，整个进程退出（正确）

一个线程退出，整个进程结束（错误）

 主进程退出，整个进程结束（正确）



main函数都退出了，整个进程空间都消失了，不管什么样方式进程 都会别被销毁的。

为工作线程执行时间长短不确定（即使非阻塞，或者sleep 来控制，还有cpu调度因素考虑）

这就是我理解必须通过join方式阻塞main线程。



##### man pthread_exit 



在notes 最重要的一句话

```c
To allow other threads to continue execution, 
the main thread should terminate by calling pthread_exit() rather than exit(3).
```

解释：

- pthread_exit 作用就是只终止调用线程，就是自己(这个我知道了，exit就是推出)

- pthread_exit 只清楚自己的数据(这个我知道了，exit就是推出)

- pthread_exit 不清理全局共享的数据，通过atexit()方式注册的销毁数据方式，不被执行（atexit是进程退出执行）

- 如果终止是最后一个线程。真个进程调用exit退出，销毁整个进程。

  

```c
 http://man7.org/linux/man-pages/man3/pthread_exit.3.html 
NAME  
       void pthread_exit(void *retval);
DESCRIPTION        
    
       The pthread_exit() function terminates the calling thread and returns
       a value via retval that (if the thread is joinable) is available to
       another thread in the same process that calls pthread_join(3).

       Any clean-up handlers established by pthread_cleanup_push(3) that
       have not yet been popped, are popped (in the reverse of the order in
       which they were pushed) and executed.  If the thread has any thread-
       specific data, then, after the clean-up handlers have been executed,
       the corresponding destructor functions are called, in an unspecified
       order.

       When a thread terminates, process-shared resources (e.g., mutexes,
       condition variables, semaphores, and file descriptors) are not
       released, and functions registered using atexit(3) are not called.

       After the last thread in a process terminates, the process terminates
       as by calling exit(3) with an exit status of zero; thus, process-
       shared resources are releas
  NOTES       
           
       Performing a return from the start function of any thread other than
       the main thread results in an implicit call to pthread_exit(), using
       the function's return value as the thread's exit status.

       To allow other threads to continue execution, the main thread should
       terminate by calling pthread_exit() rather than exit(3).

       The value pointed to by retval should not be located on the calling
       thread's stack, since the contents of that stack are undefined after
       the thread terminates.
           
```

##### man exit 



```
exit - cause normal process termination

 The exit() function causes normal process termination and the value
       of status & 0377 is returned to the parent (see wait(2)).
  看进程的销毁图片         
  All functions registered with atexit(3) and on_exit(3) are called, in
       the reverse order of their registration.

  
  NOTE
 进程销毁 上下都要打点
 
   After exit(), the exit status must be transmitted to the parent
       process.  There are three cases:

       ·  If the parent has set SA_NOCLDWAIT, or has set the SIGCHLD handler
          to SIG_IGN, the status is discarded and the child dies
          immediately.

       ·  If the parent was waiting on the child, it is notified of the exit
          status and the child dies immediately.

       ·  Otherwise, the child becomes a "zombie" process: most of the
          process resources are recycled, but a slot containing minimal
          information about the child process (termination status, resource
          usage statistics) is retained in process table.  This allows the
          parent to subsequently use waitpid(2) (or similar) to learn the
          termination status of the child; at that point the zombie process
          slot is released.

       If the implementation supports the SIGCHLD signal, this signal is
       sent to the parent.  If the parent has set SA_NOCLDWAIT, it is
       undefined whether a SIGCHLD signal is sent.

   Signals sent to other processes
       If the exiting process is a session leader and its controlling
       terminal is the controlling terminal of the session, then each
       process in the foreground process group of this controlling terminal
       is sent a SIGHUP signal, and the terminal is disassociated from this
       session, allowing it to be acquired by a new controlling process.

       If the exit of the process causes a process group to become orphaned,
       and if any member of the newly orphaned process group is stopped,
       then a SIGHUP signal followed by a SIGCONT signal will be sent to
       each process in this process group.  See setpgid(2) for an
       explanation of orphaned process groups.
```











 http://man7.org/linux/man-pages/man3/pthread_detach.3.html 

 https://www.youtube.com/watch?v=q3-5sDe6lzg 

 https://stackoverflow.com/questions/6042970/pthread-detach-question 













 https://medium.com/@vgasparyan1995/let-me-detach-those-threads-for-you-2de014b26394 



### 3.线程安全



> 陈咬金的三板斧 来了
>
> 第一斧：技术本质是什么？
>
> 第二斧：考虑哪些问题？
>
> 第三斧：如何使用？

#### 第一斧：技术本质是什么？



#### 第二斧：考虑哪些问题？

- 线程可重入
- 线程与信号
- 线程与fork



#### 第三斧：如何使用？





## 进程

### 一. 进程创建方式

- Linux进程创建： 通过fork()系统调用创建进程
- Linux用户级线程创建：通过pthread库中的pthread_create()创建线程
- Linux内核线程创建： 通过kthread_create()



fork, vfork, clone根据不同参数调用do_fork

- pthread_create: flags参数为 CLONE_VM, CLONE_FS, CLONE_FILES, CLONE_SIGHAND
- fork: flags参数为 SIGCHLD
- vfork: flags参数为 CLONE_VFORK, CLONE_VM, SIGCHLD

 

  

#### 运行应用程序( 由fork / exec对完成 )



 启动程序一般都是在命令行中，其实是在与`shell`打交道，然后`shell`帮我们启动程序 

> shell 每次执行指令， 需要 fork 出一个子进程来执行，然后将子进程的镜像替换成目标指令，
>
> 这又会用到 exec 函数。比如下面这条简单的指令 
>
> putty工具登录 

![image.png](https://i.loli.net/2019/11/30/fvt5JX4mZ3AUOFB.png)









参考

[一个小小的 Shell 管道符，内部实现可真不简单！](https://juejin.im/post/5bc98b36f265da0af93b34c6)

[Linux进程启动过程分析do_execve(可执行程序的加载和运行)](https://github.com/gatieme/LDD-LinuxDeviceDrivers/tree/master/study/kernel/01-process/03-execute/01-do_execve)

~~~c++
strace ./a.out
execve("./a.out", ["./a.out"], [/* 30 vars */]) = 0
brk(NULL)                               = 0xd9e000
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f52dcaaa000
    
The  exec()  family of functions replaces the current process image with a new process image
    
    
strace ls
execve("/bin/ls", ["ls"], [/* 29 vars */]) = 0
brk(NULL)                               = 0x19aa000
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fc7e7ad6000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
open("tls/x86_64/libselinux.so.1", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("tls/libselinux.so.1", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("x86_64/libselinux.so.1", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)
open("libselinux.so.1", O_RDONLY|O_CLOEXEC) = -1 ENOENT (No such file or directory)    
~~~



#### close_on_exec 机制

> 陈咬金的三板斧 来了

- 背后原理是什么

![image.png](https://i.loli.net/2019/11/30/YjhLt2KDZiylzvW.png)

 [**文件描述符**](https://en.wikipedia.org/wiki/File_descriptor#Operations_on_the_file_descriptor_table)（File descriptor）是计算机科学中的一个术语，是一个用于表述指向[文件](https://zh.wikipedia.org/wiki/文件)的引用的抽象化概念。 

[Linux 下文件描述符的统计方法](https://vvl.me/2019/06/file-descriptor-statistical-method-under-linux/)



 fork 后，子进程会获得父进程的文件描述符（更具体地说，Linux 下是未设置 `CLONE_FILES` flag 的系统调用 `clone()`）。

子进程执行 exec 的后，内核不再保留原有的 fdtable，无法再根据文件描述符操作文件 

exec 流程

1. fork
2. 销毁
3. 替换

- 能解决什么问题

  

​       进程/线程 因为存在 Too many open files（端口也是文件） ，造成创建失败

    sysctl -a | grep file-max
    fs.file-max = 655360



- 如何解决的

 [文件描述符标志](https://blog.csdn.net/cywosp/article/details/38965239)（即，close-on-exec）为进程和文件描述符所私有。

对这一标志的修改将不会影响同一进程或不同进程中的其他文件描述符 



 如果对描述符设置了FD_CLOEXEC，在使用execl调用执行的程序里，此描述符将在子进程中会被自动关闭，不能使用了。但是在父进程中仍然可以使用 

Linux系统的open函数，其中flags参数可以传入O_CLOEXEC标记，即可自动设置上FD_CLOEXEC标记，但Linux内核版本2.6.23才开始支持此标记。

来源：[Nginx文件描述符泄露？浅析FD_CLOEXEC文件描述符标志](http://blogs.360.cn/post/nginx%E6%96%87%E4%BB%B6%E6%8F%8F%E8%BF%B0%E7%AC%A6%E6%B3%84%E9%9C%B2%EF%BC%9F%E6%B5%85%E6%9E%90fd_cloexec%E6%96%87%E4%BB%B6%E6%8F%8F%E8%BF%B0%E7%AC%A6%E6%A0%87%E5%BF%97.html)

  

~~~c++
TCP:
#ifdef WIN32
	SOCKET ss = ::socket(PF_INET, SOCK_STREAM, 0);
#else
	SOCKET ss = ::socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
#endif

文件：
int fd = open(“foo.txt”,O_RDONLY);
int flags = fcntl(fd, F_GETFD);
flags |= FD_CLOEXEC;
fcntl(fd, F_SETFD, flags);

int fd = open(“foo.txt”,O_RDONLY | O_CLOEXEC);


 O_CLOEXEC (since Linux 2.6.23)
              Enable the close-on-exec flag for the new file descriptor.
              Specifying this flag permits a program to avoid additional
              fcntl(2) F_SETFD operations to set the FD_CLOEXEC flag.
~~~

- ### [为什么close_on_exec不是默认配置？](https://www.thinbug.com/q/9583845)

 因为在UNIX上，最常用的功能之一是进程之间的管道流 - 如果设置了CLOEXEC标志，则不能这样做标准I/O

| 整数值 |                          名称                           | [unistd.h](https://zh.wikipedia.org/wiki/Unistd.h) | 文件流 |
| :----: | :-----------------------------------------------------: | :------------------------------------------------: | :----: |
|   0    |  [Standard input](https://zh.wikipedia.org/wiki/Stdin)  |                    STDIN_FILENO                    | stdin  |
|   1    | [Standard output](https://zh.wikipedia.org/wiki/Stdout) |                   STDOUT_FILENO                    | stdout |
|   2    | [Standard error](https://zh.wikipedia.org/wiki/Stderr)  |                   STDERR_FILENO                    | stderr |

### 二、 进程的销毁



> 陈咬金的三板斧 来了
>
> 第一斧：技术本质是什么？
>
> 第二斧：解决了什么问题？
>
> 第三斧：如何使用？







![image.png](https://i.loli.net/2019/11/30/JVsLgbNcoDPwFIy.png)

![image.png](https://i.loli.net/2019/11/30/CQ9bs74IjiomLTO.png)

 ![QQ截图20130712154916](https://images0.cnblogs.com/blog/529981/201307/12164358-cc25532dd77e43e59caab3472e9f319e.png) 

what-would-you-use-an-exit-handler-

不同语言类比(c/golang/python/java)

> python  The [`atexit`](https://docs.python.org/3/library/atexit.html#module-atexit) module defines functions to register and unregister cleanup functions 
>
> golang  [*Defer* i](https://gobyexample.com/defer)s used to ensure that a function call is performed later in a program’s execution, usually for purposes of cleanup
>
> java  hooks  







![image.png](https://i.loli.net/2019/11/30/9rzlZ34KDHLfeY2.png)



# 相关问题

- Linux中fork子进程后再exec新程序时文件描述符的问题？
- 关于linux进程间的[close-on-exec](https://cloud.tencent.com/developer/article/1177094)机制
- [LinuxThreads的不足](https://xnerv.wang/linux-process-kernel-and-file-system-summary/)
- TCP listening sockets created without FD_CLOEXEC flag 
- 为什么使用execve创建远程shell不会覆盖文件描述符和套接字？
-   C 和 Golang 真的有千丝万缕的联系，比如 `atexit()` 与 `defer func(){}` 

