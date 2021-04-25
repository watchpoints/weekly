---
title: "man"
date: 2019-12-02
description: ""
draft: false
tags: ["man"]
categories: ["liunx"]
---





# 帮助文档

## 内存管理

### 1 man malloc

http://man7.org/linux/man-pages/man3/malloc.3.html

- Thread safety （多个线程可以同时执行） arena

```scss
To scalably handle memory allocation in
       multithreaded applications, glibc creates additional memory
       allocation arenas if mutex contention is detected.  Each arena is a
       large region of memory that is internally allocated by the system
       (using brk(2) or mmap(2)), and managed with its own mutexes
```

- runtime （free）

> Usually, all it can do is allow a later call to `malloc` to reuse the space. In the meantime, the space remains in your program as part of a free-list used internally by `malloc`.

：Linux下谷歌有个 tcmalloc 能做到立刻释放到OS，或者malloc_trim(0)



### 2 man mmap

### 定义

mmap, munmap - map or unmap files or devices into memory





mmap分配的内存，等于是直接从物理内存中映射了一块过来。释放这块内存时，可以直接归还给OS

https://www.cnblogs.com/lookof/archive/2013/03/26/2981768.html

1. brk(sbrk)，性能损耗少; mmap相对而言，性能损耗大

2. mmap不存在内存碎片(是物理页对齐的，整页映射和释放); brk(sbrk)可能存在内存碎片(由于new和delete的顺序不同，可能存在空洞，又称为碎片)

   [Glibc内存分配算法](https://cloud.tencent.com/developer/article/1138651)

glibc采用的是dlmalloc。为了避免频繁调用系统调用，它内部维护了一个内存池，方便reuse，又称为free-list或bins

```javascript
DEFAULT_MXFAST             64 (for 32bit), 128 (for 64bit) // free-list(fastbin)最大内存块
DEFAULT_TRIM_THRESHOLD     128 * 1024 // malloc_trim的门槛值 128k
DEFAULT_TOP_PAD            0
DEFAULT_MMAP_THRESHOLD     128 * 1024 // 使用mmap分配内存的门槛值 128k
DEFAULT_MMAP_MAX           65536 // mmap的最大数量
```

glibc中，设置了默认进行malloc_trim的threshhold为128K，也就是说当dlmalloc管理的内存池中最大可用内存>128K时，就会执行malloc_trim操作，归还部分内存给操作系统；

而在可用内存<=128K时，及时程序中delete了这部分内存，这些内存也是不会归还给操作系统的。

表现为：调用delete之后，进程占用的内存并没有减少。





测试：

1. 循环new分配64K * 2048的内存空间，写入脏数据后，循环调用delete释放。top看进程依然使用131M内存，没有释放。 —— 此时用brk
2. 循环new分配128K * 2048的内存空间，写入脏数据后，循环调用delete释放。top看进程使用，2960字节内存，完全释放。 —— 此时用mmap
3. 设置M_MMAP_THRESHOLD 256k，循环new分配128k * 2048 的内存空间，写入脏数据后，循环调用delete释放，而后调用malloc_trim(0)。top看进程使用，2348字节，完全释放。 ——此时用brk

### 3 malloc_trim - release free memory from the top of the heap



当我调用了**malloc_trim(0)**以后，我发现堆中全部的空余内存全部被归还给系统了，包括那些洞。不过，free list bing/fast bin中依然维护着这些内存地址，当再次需要申请小内存块时，总是前面的洞被再次从系统中“要”回来，然后分给调用者。这一点显得**malloc_trim(0)**很高级



##  

https://www.cnblogs.com/lookof/archive/2013/03/26/2981768.html

### 4  man brk



## 锁



### pthread_rwlock_unlock



       int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

DESCRIPTION
       The  pthread_rwlock_unlock() function shall release **a lock** held on the read-write lock object referenced
       by rwlock. 

       If this function is called to release a read lock from the read-write lock object and  there  are  other
       read locks currently held on this read-write lock object, the read-write lock object remains in the read
       locked state. 
       
       If this function releases the last read lock for this read-write lock  object,  the  read-
       write lock object shall be put in the unlocked state with no owners.


​       
​    
​       If  this function is called to release a write lock for this read-write lock object, the read-write lock
​       object shall be put in the unlocked state.
​    
​       If there are threads blocked on the lock when it becomes available, the scheduling policy  shall  deter‐
​       mine  which  thread(s)  shall acquire the lock.  If the Thread Execution Scheduling option is supported,
​       when threads executing with the scheduling policies SCHED_FIFO, SCHED_RR, or SCHED_SPORADIC are  waiting
​       on  the  lock,  they shall acquire the lock in priority order when the lock becomes available. For equal
​       priority threads, write locks shall take precedence over read locks.  If the Thread Execution Scheduling
​       option  is  not  supported,  it  is implementation-defined whether write locks take precedence over read
​       locks.


​     



### pthread_rwlock_wrlock



key::whether  a  read  or  write lock deadlock



```shell
 The  pthread_rwlock_wrlock()  function  shall  apply  a  write lock to the read-write lock referenced by rwlock.(写锁) 
   
The calling thread acquires the write lock if no other thread (reader or writer) holds the read-write  lock  rwlock. (获取成功，没有任何其他锁)
   
    Otherwise, the thread shall block until it can acquire the lock. 
    
 The calling thread may deadlock 
 if at the time the call is made it holds the read-write  lock  (whether  a  read  or  write lock).(占用不释放)
```
ERRORS
       The pthread_rwlock_trywrlock() function shall fail if:

       EBUSY  The read-write lock could not be acquired for writing because it was already locked  for  reading
              or writing.


## 网络

###  send

 send  send a message on a socket

 send(sockfd, buf, len, flags);



- ERRORS



```shell


   EAGAIN or EWOULDBLOCK
          The socket is marked nonblocking and the requested operation would block.   POSIX.1-2001  allows  either
          error  to  be  returned for this case, and does not require these constants to have the same value, so a
          portable application should check for both possibilities.

   EBADF  An invalid descriptor was specified.

   ECONNRESET
          Connection reset by peer.

   EDESTADDRREQ
          The socket is not connection-mode, and no peer address is set.

   EFAULT An invalid user space address was specified for an argument.

   EINTR  A signal occurred before any data was transmitted; see signal(7).

   EINVAL Invalid argument passed.

https://stackoverflow.com/questions/3081952/with-c-tcp-sockets-can-send-return-zero

On success, these calls return the number of bytes sent.  On error,
       -1 is returned, and errno is set appropriately.
       
   >0
   <0(-1)
   =0 在这种情况下，从内存中，远程 TCP 栈缓冲区已经填满，栈通知本地端，它将延迟到一些空间被清空，本地缓冲区也已经填满。
```
### https://linux.die.net/man/2/recv



~~~shell

Return Value

These calls return the number of bytes received, 
or -1 if an error occurred. 

The return value will be 0 when the peer has performed an orderly shutdown.

Errors
These are some standard errors generated by the socket layer. Additional errors may be generated and returned from the underlying protocol modules; see their manual pages.

EAGAIN or EWOULDBLOCK
The socket is marked nonblocking and the receive operation would block, or a receive timeout had been set and the timeout expired before data was received. POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same value, so a portable application should check for both possibilities.
EBADF
The argument sockfd is an invalid descriptor.

ECONNREFUSED
A remote host refused to allow the network connection (typically because it is not running the requested service).
EFAULT
The receive buffer pointer(s) point outside the process's address space.

EINTR

The receive was interrupted by delivery of a signal before any data were available; see signal(7).

EINVAL

Invalid argument passed.

ENOMEM

Could not allocate memory for recvmsg().

ENOTCONN
The socket is associated with a connection-oriented protocol and has not been connected (see connect(2) and accept(2)).
ENOTSOCK
The argument sockfd does not refer to a socket.
~~~

### man 2 read

~~~c
/* myread() 函数的一部分，在 while (1) 大循环里 */
if ((n = read(fd, buf, nbytes)) < 0) {
	if (errno == EINTR) {
		perror("EINTR");
		n = 0;
	} else {
		perror("read error");
		return -1;
	}
} else {
	break;
}

/* main() 函数中 */
signal(SIGALRM, sig_alrm);
alarm(1);
myread(fd, buf, MAX);

/* sig_alrm() 函数 */
void sig_alrm(ing signo)
{
	return;
}
http://yungkcx.github.io/jekyll/update/2016/09/16/read()-and-EINTR.html
https://stackoverflow.com/questions/48959061/tcp-socket-can-read-still-fail-with-eintr-when-select-indicates-there-are-d
~~~



## epoll





Edge Triggered (ET) 边沿触发
.socket的接收缓冲区状态变化时触发读事件，即空的接收缓冲区刚接收到数据时触发读事件
.socket的发送缓冲区状态变化时触发写事件，即满的缓冲区刚空出空间时触发读事件



# 进程

- man pthread_exit



DESCRIPTION
       The pthread_exit() function terminates the calling thread and returns a value via retval that (if the thread is
       joinable) is available to another thread in the same process that calls pthread_join(3).

       Any clean-up handlers established by pthread_cleanup_push(3) that have not yet been popped, are popped (in  the
       reverse  of  the  order  in  which they were pushed) and executed.  If the thread has any thread-specific data,
       then, after the clean-up handlers have been executed, the corresponding destructor functions are called, in  an
       unspecified order.
    
       When  a  thread  terminates, process-shared resources (e.g., mutexes, condition variables, semaphores, and file
       descriptors) are not released, and functions registered using atexit(3) are not called.
    
       After the last thread in a process terminates, the process terminates as by calling exit(3) with an exit status
       of zero; thus, process-shared resources are released and functions registered using atexit(3) are called.



ERRORS
       This function always succeeds.

