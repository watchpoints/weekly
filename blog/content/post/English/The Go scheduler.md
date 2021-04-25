# [The Go scheduler](http://morsmachine.dk/go-scheduler)

# Introduction

One of the big features for Go 1.1 is the new scheduler, contributed by Dmitry Vyukov. The new scheduler has given a dramatic increase in performance for parallel Go programs and with nothing better to do, I figured I'd write something about it.

Most of what's written in this blog post is already described in the [original design doc](https://docs.google.com/document/d/1TTj4T2JO42uD5ID9e89oa0sLKhJYD0Y_kqxDv3I3XMw). It's a fairly comprehensive document, but pretty technical.

All you need to know about the new scheduler is in that design document but this post has pictures, so it's clearly superior.

# What does the Go runtime need with a scheduler?

But before we look at the new scheduler, we need to understand why it's needed. Why create a userspace scheduler when the operating system can schedule threads for you?

-----



The POSIX thread API is very much a logical extension to the existing Unix process model and as such, threads get a lot of the same controls as processes. Threads have their own signal mask, can be assigned CPU affinity, can be put into cgroups and can be queried for which resources they use. All these controls add overhead for features that are simply not needed for how Go programs use goroutines and they quickly add up when you have 100,000 threads in your program.

POSIX线程API在很大程度上是对现有Unix进程模型的逻辑扩展，因此，线程获得了许多与进程相同的控件。 线程具有自己的信号掩码，可以分配给CPU亲和力，可以放入cgroup中，并可以查询它们使用的资源。 所有这些控件都增加了Go程序如何使用goroutines根本不需要的功能的开销，并且当您的程序中有100,000个线程时，它们会迅速增加。



-----

Another problem is that the OS can't make informed scheduling decisions, based on the Go model. For example, the Go garbage collector requires that all threads are stopped when running a collection and that memory must be in a consistent state. This involves waiting for running threads to reach a point where we know that the memory is consistent.

另一个问题是，操作系统无法基于Go模型做出明智的调度决策。 

例如，Go垃圾收集器要求在运行收集时停止所有线程，并且内存必须处于一致状态。 

这涉及等待运行的线程到达我们知道内存一致的地步。

-----



When you have many threads scheduled out at random points, chances are that you're going to have to wait for a lot of them to reach a consistent state. The Go scheduler can make the decision of only scheduling at points where it knows that memory is consistent. This means that when we stop for garbage collection, we only have to wait for the threads that are being actively run on a CPU core.

当您在随机点安排了许多线程时，很可能将不得不等待很多线程达到一致的状态。 Go调度程序可以决定仅在知道内存一致的点上进行调度。 这意味着当我们停止垃圾回收时，我们只需要等待正在CPU内核上活跃运行的线程即可。

------



# Our Cast of Characters

There are 3 usual models for threading. One is N:1 where several userspace threads are run on one OS thread. This has the advantage of being very quick to context switch but cannot take advantage of multi-core systems. Another is 1:1 where one thread of execution matches one OS thread. It takes advantage of all of the cores on the machine, but context switching is slow because it has to trap through the OS.



通常有3种线程模型。 一种是N：1，其中多个用户空间线程在一个OS线程上运行。

 这样的优点是可以非常快速地进行上下文切换，但是不能利用多核系统。 

另一个是1：1，其中一个执行线程与一个OS线程匹配。 

它利用了计算机上的所有内核，但是上下文切换很慢，因为它必须捕获整个OS



------

Go tries to get the best of both worlds by using a M:N scheduler. It schedules an arbitrary number of goroutines onto an arbitrary number of OS threads. You get quick context switches and you take advantage of all the cores in your system. The main disadvantage of this approach is the complexity it adds to the scheduler.

To acomplish the task of scheduling, the Go Scheduler uses 3 main entities:

![img](../images/our-cast.jpg)

The triangle represents an OS thread. It's the thread of execution managed by the OS and works pretty much like your standard POSIX thread. In the runtime code, it's called **M** for machine.


590/5000

Go尝试通过使用M：N调度程序来兼顾两全其美。

 它将任意数量的goroutine调度到任意数量的OS线程上。

 您可以快速进行上下文切换，并且可以利用系统中的所有核心。

 这种方法的主要缺点是它增加了调度程序的复杂性。

为了完成调度任务，Go Scheduler使用3个主要实体：

三角形代表OS线程。 它是由操作系统管理的执行线程，其工作原理与您的标准POSIX线程非常相似。 

在运行时代码中，对于机器，它称为M。

------------------

The circle represents a goroutine. It includes the stack, the instruction pointer and other information important for scheduling goroutines, like any channel it might be blocked on. In the runtime code, it's called a **G**.

圆圈代表goroutine。 

它包括堆栈，指令指针和其他对调度goroutine重要的信息，

例如可能会阻塞其的任何通道。 在运行时代码中，它称为G。

----------

The rectangle represents a context for scheduling. You can look at it as a localized version of the scheduler which runs Go code on a single thread. It's the important part that lets us go from a N:1 scheduler to a M:N scheduler. In the runtime code, it's called **P** for processor. More on this part in a bit.



矩形代表计划的上下文。

 您可以将其视为调度程序的本地化版本，该调度程序在单个线程上运行Go代码。 

**这是使我们从N：1调度程序转到M：N调度程序的重要部分。**

 在运行时代码中，对于处理器，它称为P。 这部分的更多内容。

![img](../images/in-motion.jpg)

-----------------------

Here we see 2 threads (**M**), each holding a context (**P**), each running a goroutine (**G**). In order to run goroutines, a thread must hold a context.





The number of contexts is set on startup to the value of the `GOMAXPROCS` environment variable or through the runtime function `GOMAXPROCS()`. Normally this doesn't change during execution of your program. The fact that the number of contexts is fixed means that only `GOMAXPROCS` are running Go code at any point. We can use that to tune the invocation of the Go process to the individual computer, such at a 4 core PC is running Go code on 4 threads.

上下文数量在启动时设置为环境变量“ GOMAXPROCS”的值，或通过运行时函数“ GOMAXPROCS（）”设置。 通常，在执行程序期间这不会改变。 上下文数量是固定的，这意味着在任何时候只有`GOMAXPROCS`在运行Go代码。 

我们可以使用它来调整Go进程对单个计算机的调用，例如在4核PC上的4个线程上运行Go代码。



--------------

The greyed out goroutines are not running, but ready to be scheduled. They're arranged in lists called runqueues. Goroutines are added to the end of a runqueue whenever a goroutine executes a `go` statement. Once a context has run a goroutine until a scheduling point, it pops a goroutine off its runqueue, sets stack and instruction pointer and begins running the goroutine.

To bring down mutex contention, each context has its own local runqueue. A previous version of the Go scheduler only had a global runqueue with a mutex protecting it. Threads were often blocked waiting for the mutex to unlocked. This got really bad when you had 32 core machines that you wanted to squeeze as much performance out of as possible.

The scheduler keeps on scheduling in this steady state as long as all contexts have goroutines to run. However, there are a couple of scenarios that can change that.

灰色的goroutine尚未运行，但已准备好进行调度。 它们被排列在称为运行队列的列表中。 每当goroutine执行`go`语句时，就会将goroutine添加到运行队列的末尾。 

上下文在一个调度程序中运行了一个goroutine直到调度点后，便将其从其运行队列中弹出，设置堆栈和指令指针，然后开始运行该goroutine。

为了减少互斥锁争用，每个上下文都有其自己的本地运行队列。

 Go调度程序的早期版本仅具有全局运行队列，并带有互斥量来保护它。 线程经常被阻塞，等待互斥锁解锁。 当您有32台核心计算机想要最大限度地发挥性能时，这变得非常糟糕。

只要所有上下文都有要运行的goroutine，调度程序便会在此稳态下继续调度。 但是，有两种情况可以改变这种情况。

-----

# Who you gonna (sys)call?

You might wonder now, why have contexts at all? Can't we just put the runqueues on the threads and get rid of contexts? Not really. The reason we have contexts is so that we can hand them off to other threads if the running thread needs to block for some reason.

An example of when we need to block, is when we call into a syscall. Since a thread cannot both be executing code and be blocked on a syscall, we need to hand off the context so it can keep scheduling.

![img](../images/syscall.jpg)

Here we see a thread giving up its context so that another thread can run it. The scheduler makes sure there are enough threads to run all contexts. **M1** in the illustration above might be created just for the purpose of handling this syscall or it could come from a thread cache. The syscalling thread will hold on to the goroutine that made the syscall since it's technically still executing, albeit blocked in the OS.

When the syscall returns, the thread must try and get a context in order to run the returning goroutine. The normal mode of operation is to steal a context from one of the other threads. If it can't steal one, it will put the goroutine on a global runqueue, put itself on the thread cache and go to sleep.

The global runqueue is a runqueue that contexts pull from when they run out of their local runqueue. Contexts also periodically check the global runqueue for goroutines. Otherwise the goroutines on global runqueue could end up never running because of starvation.

This handling of syscalls is why Go programs run with multiple threads, even when `GOMAXPROCS` is 1. The runtime uses goroutines that call syscalls, leaving threads behind.

您可能现在想知道，为什么要有上下文？我们不能只是将运行队列放在线程上并摆脱上下文吗？并不是的。

我们拥有上下文的原因是，如果正在运行的线程出于某种原因需要阻塞，我们可以将它们移交给其他线程。

当我们需要调用系统调用时，就需要阻塞的一个例子。由于线程既不能执行代码，也不能在系统调用中被阻塞，所以我们需要移交上下文，以便它可以保持调度。

在这里，我们看到一个线程放弃了它的上下文，以便另一个线程可以运行它。调度程序确保有足够的线程来运行所有上下文。上图中的 M1 可能只是为了处理此系统调用而创建的，或者它可能来自线程缓存。

 syscalling线程将保留进行syscall的goroutine，因为它在技术上仍在执行，尽管已在OS中被阻塞。

当系统调用返回时，线程必须尝试获取上下文才能运行返回的goroutine。

正常的操作模式是从其他线程之一窃取上下文。

如果无法窃取，它将把goroutine放在全局运行队列中，将自身放在线程缓存中并进入睡眠状态。

全局运行队列是上下文从本地运行队列用尽时开始提取的运行队列。

上下文还定期检查全局运行队列中的goroutine。否则，由于饥饿，全局运行队列上的goroutine可能永远无法运行。

这种处理系统调用的原因是即使GoGOPROCS为1的Go程序也可以运行多个线程的原因。运行时使用goroutines调用系统调用，从而使线程落后。

-------------------

# Stealing work

Another way that the steady state of the system can change is when a context runs out of goroutines to schedule to. This can happen if the amount of work on the contexts' runqueues is unbalanced. This can cause a context to end up exhausting it's runqueue while there is still work to be done in the system. To keep running Go code, a context can take goroutines out of the global runqueue but if there are no goroutines in it, it'll have to get them from somewhere else.

![img](../images/steal.jpg)

That somewhere is the other contexts. When a context runs out, it will try to steal about half of the runqueue from another context. This makes sure there is always work to do on each of the contexts, which in turn makes sure that all threads are working at their maximum capacity.

# Where to go?

There are many more details to the scheduler, like cgo threads, the `LockOSThread()` function and integration with the network poller. These are outside the scope of this post, but still merit study. I might write about these later. There are certainly plenty of interesting constructions to be found in the Go runtime library.

By Daniel Morsing