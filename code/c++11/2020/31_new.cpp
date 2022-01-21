#include <cstring>
#include <iostream>

int main()
{
       #define PAUSE(msg) std::cout << msg << std::endl; std::cin >> p

        char p;

        size_t size = 1024 * 1024 *100;
        char *l = new char[size]('0');

        PAUSE("new");

        memset(l, 1, size / 2);
        PAUSE("using half large"); //分配50M

        memset(l, 1, size);
        PAUSE("using whole large"); //分配100M

        delete []l;
        PAUSE("del");

        return 0;
}

/**

  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND
30941 root      20   0  114952 103496    928 S  0.0  5.5   0:00.09 a.out



30941 root      20   0  114952 103496    928 S  0.7  5.5   0:00.11 a.out


  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND
31961 root      20   0  114952   1068    900 S  0.0  0.1   0:00.00 a.out


  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND
31961 root      20   0  114952  52284   1000 S  0.0  2.8   0:00.03 a.out


  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND
31961 root      20   0  114952 103500   1000 S  0.0  5.5   0:00.07 a.out


  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND
31961 root      20   0   12548   1176   1000 S  0.0  0.1   0:00.08 a.out

The malloc() function allocates size bytes and returns a pointer
       to the allocated memory.  The memory is not initialized.

By default, Linux follows an optimistic memory allocation
strategy.  This means that when malloc() returns non-NULL there
is no guarantee that the memory really is available.  In case it
turns out that the system is out of memory, one or more processes
will be killed by the OOM killer.  For more information, see the
description of /proc/sys/vm/overcommit_memory and
/proc/sys/vm/oom_adj in proc(5), and the Linux kernel source file
Documentation/vm/overcommit-accounting.rst.
1. 申请地址vm， 物理内存 0
2. 初岁化写入 vm -物理内存，如果 物理内存慢了，置换页。
3. 读取：如果物理内心上，不在磁盘上，置换页

https://www.cnblogs.com/zhaoyl/p/3820852.html
malloc原理和内存碎片
当一个进程发生缺页中断的时候，进程会陷入内核态，执行以下操作： 
1、检查要访问的虚拟地址是否合法 
2、查找/分配一个物理页 
3、填充物理页内容（读取磁盘，或者直接置0，或者啥也不干） 
4、建立映射关系（虚拟地址到物理地址） 
重新执行发生缺页中断的那条指令 
如果第3步，需要读取磁盘，那么这次缺页中断就是majflt，否则就是minflt。 

cat /proc/sys/vm/overcommit_memory
内核参数overcommit_memory 

它是 内存分配策略

可选值：0、1、2。
0， 表示内核将检查是否有足够的可用内存供应用进程使用；如果有足够的可用内存，内存申请允许；否则，内存申请失败，并把错误返回给应用进程。
1， 表示内核允许分配所有的物理内存，而不管当前的内存状态如何。
2， 表示内核允许分配超过所有物理内存和交换空间总和的内存

https://serverfault.com/questions/606185/how-does-vm-overcommit-memory-work

hen you set vm.overcommit_memory to 2, the vm.overcommit_ratio value becomes relevant. By default, this value is set to 50, which means the system would only allocate up to 50% of your RAM (plus swap). This explains why you are unable to start programs that was fine when vm.overcommit_memory = 0 - because there's less than 500MB of allocatable memory (assuming no swap).

When you set it to 300, you are allowing the system to allocate up to 300% of your RAM (plus swap, if any), which is why the CommitLimit value in /proc/meminfo is so high.

Although vm.overcommit_memory = 2 is usually used to prevent overcommitment, here, you're using it to cap the amount that can be overcommitted. Setting it to 300 is dangerous as your system don't have 5171884 kB of memory, and so, depending on how much swap space you have, the system will use swap (which is slow), or will run out of memory altogethe
Unix/Linux的内存分配策略是lazy的，申请的时候不会分配物理内存，只有在使用的时候才分配，

strace -t -T -f -p 23345 -e trace=network,close