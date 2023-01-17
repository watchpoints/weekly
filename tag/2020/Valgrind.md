##  Valgrind  

### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题





### 二、这个技术的优势和劣势分别是什么 







### 三、这个技术适用的场景。









### 四、技术的组成部分和关键点。

- Your program is then run on a synthetic CPU provided by the Valgrind core  
- Valgrind simulates every single instruction your program executes.  
- Valgrind serialises execution so that only one (kernel) thread is running at a time  
- A thread executes code only when it holds the abovementioned lock  

> Valgrind doesn't schedule the threads itself
>
> . It merely ensures that only one thread runs at once, using a simple locking scheme  
>
> , but it does mean that threaded apps never use more than one CPU simultaneously, 
>
> even if you have a multiprocessor or multicore machine.  

-  if you have some kind of concurrency,critical race, locking, or similar, bugs. In that case you might consider using the tools Helgrind  



### 五、技术的底层原理和关键实现

### 六、已有的实现和它之间的对比

![](https://cdn.jsdelivr.net/gh/smaugx/MyblogImgHosting/rebootcat/valgrind_massif_memory_analysing/1.png)

   callgrind ------> 它主要用来检查程序中函数调用过程中出现的问题。

​    cachegrind ------> 它主要用来检查程序中缓存使用出现的问题。

​    helgrind ------> 它主要用来检查多线程程序中出现的竞争问题。

​    massif ------> 它主要用来检查程序中堆栈使用中出现的问题

### Massif工具

- 内存一直上涨
  原文链接：https://blog.csdn.net/21aspnet/article/details/89741965

- https://valgrind.org/docs/manual/ms-manual.html
- [输出]https://www.mdnice.com/writing/ba274042ffa149c2b85f1afb40785647



1. gdb原理 ，如果获取线程信息，如果断点调试

答： [ptrace](https://blog.csdn.net/weixin_42078660/article/details/113730921)   读取子进程的寄存器信息

- https://zhuanlan.zhihu.com/p/469781175

2. [0x00007ffcd2733968 in ?? ()](https://blog.csdn.net/dyingfair/article/details/89188088)

答：



小白回答 平滑感觉的判断 行不通-，然后慌了【网上方法不行。】

-【平时 根本拿出10分钟， 20分停留这里思考】

- 没有-g废话这个你说 不是 没有-g显示问号吗？pass
- 版本符号不对 （废话） pass
- 没有任何问题 就是显示？？？
- 添加调试符号也不行

~~~

Thread 3 (Thread 0x7f1e4ac8d700 (LWP 45221)):
#0  0x00007f20d5743e29 in syscall () from /lib64/libc.so.6
#1  0x00000000095722fb in oceanbase::lib::CoFutex::wait(int, long) ()
#2  0x00000000090b3333 in oceanbase::common::ObPriorityQueue2<1, 2, 3>::do_pop(oceanbase::common::ObLink*&, long, long) ()
#3  0x000000000907d3cd in oceanbase::omt::ObTenant::get_new_request(oceanbase::omt::ObThWorker&, long, oceanbase::rpc::ObRequest*&) ()
#4  0x0000000009087d19 in oceanbase::omt::ObThWorker::worker(long&, long&, int&) ()
#5  0x0000000009088327 in non-virtual thunk to oceanbase::omt::ObThWorker::run(long) ()
#6  0x0000000002bf4284 in std::_Function_handler<void (), oceanbase::lib::CoKThreadTemp<oceanbase::lib::CoUserThreadTemp<oceanbase::lib::CoSetSched> >::start()::{lambda()#1}>::_M_invoke(std::_Any_data const&) ()
#7  0x00000000095718e5 in oceanbase::lib::CoSetSched::Worker::run() ()
#8  0x0000000009570572 in oceanbase::lib::CoRoutine::__start(boost::context::detail::transfer_t) ()
#9  0x000000000956d09f in make_fcontext ()
#10 0x0000000000000000 in ?? ()

Thread 2 (Thread 0x7f1e4aa67700 (LWP 45222)):
#0  0x00007f20d5743e29 in syscall () from /lib64/libc.so.6
#1  0x00000000095722fb in oceanbase::lib::CoFutex::wait(int, long) ()
#2  0x00000000090b3333 in oceanbase::common::ObPriorityQueue2<1, 2, 3>::do_pop(oceanbase::common::ObLink*&, long, long) ()
#3  0x000000000907d3cd in oceanbase::omt::ObTenant::get_new_request(oceanbase::omt::ObThWorker&, long, oceanbase::rpc::ObRequest*&) ()
#4  0x0000000009087d19 in oceanbase::omt::ObThWorker::worker(long&, long&, int&) ()
#5  0x0000000009088327 in non-virtual thunk to oceanbase::omt::ObThWorker::run(long) ()
#6  0x0000000002bf4284 in std::_Function_handler<void (), oceanbase::lib::CoKThreadTemp<oceanbase::lib::CoUserThreadTemp<oceanbase::lib::CoSetSched> >::start()::{lambda()#1}>::_M_invoke(std::_Any_data const&) ()
#7  0x00000000095718e5 in oceanbase::lib::CoSetSched::Worker::run() ()
#8  0x0000000009570572 in oceanbase::lib::CoRoutine::__start(boost::context::detail::transfer_t) ()
#9  0x000000000956d09f in make_fcontext ()
#10 0x0000000000000000 in ?? ()


~~~



https://stackoverflow.com/questions/121326/what-does-the-gdb-backtrace-message-0x0000000000000000-in-mean









3 heap-profiler是内存监控器 原理 gperftools

答：

4. [gdb 问号？](https://maimai.cn/web/gossip_detail?encode_id=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlZ2lkIjoiNmEwZWJhZmE3YWNjNDk5NDkxYWUyMmNjMzZiYTg5NTciLCJpZCI6Mjk5NzgzMDAsInUiOjQ2NjUxfQ.IVeEdD-50IwvxZi0jw3Fkrj3CSzzih6y_rW4g0qe3jE&from=list)

   

http://127.0.0.1:1313/post/2021/06_ask_1/