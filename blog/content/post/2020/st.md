---
title: "协程库"
date: 2020-03-14
description: "高性能可扩展服务"
draft: false
categories: ["code_reading"]
---

# 卡片

| 协程库                                                       | 语言                                                         | 开发者                                               |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---------------------------------------------------- |
| [state threads library](https://github.com/ossrs/state-threads) | 3000行[C代码](https://github.com/wangcy6/reading_code_note/tree/master/liunx/st) | [历史渊源](https://coolshell.cn/articles/12012.html) |
|                                                              |                                                              |                                                      |



# 摘要

> 今日理解。



一、一个普通的应用程序，假如采用st库，那么这个应用具备2个特点

- highly scalable applications，在负载伸缩和系统伸缩方面 将很容易的扩展，
- design of fast ，让开发快速简单，可以自由地使用静态变量和不可重入的库函数。

二、scalable：是什么

简单说，一个用户把cpu个数，或者线程个数增加一倍，性能也应该提一倍(system1蓝色原来的配置,system2 提高一倍之后的)

> an application has a good *load scalability* if it can sustain its throughput over a wide range of loads

![image.png](https://i.loli.net/2020/03/15/hcZ2Fp69D3W1tYu.png)



三、ST的多核架构核心是：Multi-Process EDSM，

继承了 *Multi-Process*, *Multi-Threaded*, and *Event-Driven State Machine* architectures全部优点。

The State Threads library combines the advantages of all of the above architectures。

| Architecture                            | load  scalability | system scalability | note           |
| --------------------------------------- | ----------------- | ------------------ | -------------- |
| Multi-Process Architecture              | poor              | good               | 负载差         |
| Multi-Threaded Architecture             | good              | poor               | 有资源竞争     |
| Event-Driven State Machine Architecture | good              | poor               | 系统负载扩展差 |
| Multi-Process EDSM                      | good              | good               | 完美           |



但是，

这是半成品，我只提供一个lib，因为其他方面，操作系统或者第三方技术已经做很好了。完全交给开发者，

> Process management is not in the ST's scope but instead is left up to the application.



具体完成过程需要开发者来实现 

用户来决定fork多少进程，每个进程分配多少资源 。

> The application designer has full control of how many processes to create 

# 项目实践

- task 编译成动态库和静态库 10分钟

~~~c
git clone https://github.com/wangcy6/state-threads.git
cd state-threads
make linux-debug //不同平台
state-threads/obj/
libst.so.1.9
libst.a

 cd examples/
 make linux-debug //不同平台
 EXAMPLES    = $(OBJDIR)/lookupdns $(OBJDIR)/proxy $(OBJDIR)/server  
    
    
USAGE
    server -l <log_directory> [<options>]
  Possible options:

    -p <num_processes>      create specified number of processes
    -t <min_thr>:<max_thr>  specify thread limits per listening socket
                            across all processes
  .
    
~~~

- 阅读 examples代码 50分钟

  > https://github.com/wangcy6/state-threads/blob/master/examples/server.c

------------------------------------看到这里可以 -end------下面是记录 ----------------------------------



# 遗留问题

- st 真的有那么厉害吗？和golang 等其他语言比较是不是弱？
- st根本无法利用多核，怎么宣称是为互联网应用而生的呢？





## 文档FQA

阅读文档1 ，累计盘茄次数 5    耗时 130分钟

理解 **ST本质上仍然是基于[EDSM模型](https://coolshell.cn/articles/12012.html)，但旨在取代传统的异步回调方式** ,**为[互联网应用程](https://github.com/zfengzhen/Blog/blob/master/article/%E4%B8%BA%E4%BA%92%E8%81%94%E7%BD%91%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E8%80%8C%E7%94%9F%E7%9A%84State%20Threads%5B%E5%B8%B8%E8%A7%81%E5%90%8E%E5%8F%B0%E6%9E%B6%E6%9E%84%E6%B5%85%E6%9E%90%5D.md)序而生[的State Threads](https://www.cnblogs.com/NerdWill/p/6166220.html)** 

涉及内容

**State Threads Library Documentation**

- [**Introductory paper**](http://state-threads.sourceforge.net/docs/st.html)
- [**Complete reference**](http://state-threads.sourceforge.net/docs/reference.html)
- [**Programming notes**](http://state-threads.sourceforge.net/docs/notes.html)
- [**Frequently Asked Questions and answers (FAQ)**](http://state-threads.sourceforge.net/docs/faq.html)





-  ST scheduler （不懂）

  > the ST scheduler is hidden inside the library and invisible to an application writer. 

-   traditional EDSM (不懂)

  >  event-driven state machines (EDSM)  

-  virtual processor (不懂)

  >  ST的多核架构 

- ，**ST的threads可以并发地线性地处理I/O事件** （不懂）

-  execute state   event wait queue  

- setjmp/longjmp （不懂）

   the context switch overhead is a cost of doing `_setjmp()/_longjmp()` (no system calls are involved).  

并发不是并行，只要不让cpu闲着就行 这就是好的设计，例如多线程，多进程。



whenever a thread needs to wait for a specific event to occur, it saves its execution state and puts itself on an event wait queue -- this is equivalent to registering a callback in traditional EDSMs. 



Then the scheduler is executed. It pulls the first thread from the run queue and restores its execution state -- this is equivalent to invoking a callback in traditional EDSMs. 



If there are no runnable threads (the run queue is empty), the scheduler waits for I/O or timing events using `select()`, `poll()` or possibly other exotic mechanism available on that OS.



 When events occur, the scheduler "wakes up" threads waiting for those events by putting them on the run queue. Then it restores the execution state of the first runnable thread. The cycle is complete. 

- 看了一下代码和运行一个例子 懂了

![image.png](https://i.loli.net/2019/11/18/wscU78RMAI6ZDmf.png)

- # [The Go scheduler](http://morsmachine.dk/go-scheduler)

- TCP-Finite-State-Machine [TCP 有限状态机](https://github.com/wangcy6/weekly/blob/master/daily-interview/5.%E6%89%8B%E7%BB%98tcp%E7%8A%B6%E6%80%81_home.md)



## 代码FQA

### 重要数据结构

~~~c
typedef struct _st_vp {
  _st_thread_t *idle_thread; /* Idle thread for this vp */
  st_utime_t last_clock;     /* The last time we went into vp_check_clock() */

  _st_clist_t run_q;    /* run queue for this vp */
  _st_clist_t io_q;     /* io queue for this vp */
  _st_clist_t zombie_q; /* zombie queue for this vp */
#ifdef DEBUG
  _st_clist_t thread_q; /* all threads of this vp */
#endif
  int pagesize;

  _st_thread_t *sleep_q; /* sleep queue for this vp */
  int sleepq_size;       /* number of threads on sleep queue */

#ifdef ST_SWITCH_CB
  st_switch_cb_t switch_out_cb; /* called when a thread is switched out */
  st_switch_cb_t switch_in_cb;  /* called when a thread is switched in */
#endif
} _st_vp_t;
~~~



### 重要接口



### 循环处理

~~~c++

void *_st_idle_thread_start(void *arg)
{
    _st_thread_t *me = _ST_CURRENT_THREAD();
 
    while (_st_active_count > 0) {
        /* Idle vp till I/O is ready or the smallest timeout expired */
        _ST_VP_IDLE();
 
        /* Check sleep queue for expired threads */
        _st_vp_check_clock();
 
        me->state = _ST_ST_RUNNABLE;
        _ST_SWITCH_CONTEXT(me);
    }
 
    /* No more threads */
    exit(0);
 
    /* NOTREACHED */
    return NULL;
}

/*
 * Switch away from the current thread context by saving its state 
 * and calling the thread scheduler
 */
#define _ST_SWITCH_CONTEXT(_thread)       \
    ST_BEGIN_MACRO                        \
    if (!MD_SETJMP((_thread)->context)) { \
      _st_vp_schedule();                  \
    }                                     \
    ST_END_MACRO
 
/*
 * Restore a thread context that was saved by _ST_SWITCH_CONTEXT 
 * or initialized by _ST_INIT_CONTEXT
 */
#define _ST_RESTORE_CONTEXT(_thread)   \
    ST_BEGIN_MACRO                     \
    _ST_SET_CURRENT_THREAD(_thread);   \
    MD_LONGJMP((_thread)->context, 1); \
    ST_END_MACRO
 
void _st_vp_schedule(void)
{
    _st_thread_t *thread;
 
    if (_ST_RUNQ.next != &_ST_RUNQ) {
        /* Pull thread off of the run queue */
        thread = _ST_THREAD_PTR(_ST_RUNQ.next);
        _ST_DEL_RUNQ(thread);
    } else {
        /* If there are no threads to run, switch to the idle thread */
        thread = _st_this_vp.idle_thread;
    }
    ST_ASSERT(thread->state == _ST_ST_RUNNABLE);
 
    /* Resume the thread */
    thread->state = _ST_ST_RUNNING;
    _ST_RESTORE_CONTEXT(thread);
}
~~~












