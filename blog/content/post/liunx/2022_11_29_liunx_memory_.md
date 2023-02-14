---
title: "从青铜到王者系列：故障定位"
date: 2022-11-20
description: ""
draft: false
tags: ["book"]
---









## 如何学习

> 在本文中，我将试着去回答liunx 内核参数：
> Q1 (Why): 为什么需要协程？
> 我们会一起回顾协程出现的历史背景，当时要解决什么问题；同时，现在是什么场景，需要使用协程来进行处理？为什么进程或者线程不能够很好地满足当下的使用场景？
> Q2 (What): 到底什么是协程？
> 我们一直在谈论协程。由于协程中包含有「程」这个字眼，因此经常被拿来与进程线程进行对比，称协程为「用户态线程」；但又有人从协程实现的角度，说「协程是一种泛化的函数」。这就把我们给绕晕了。我们不禁要问，到底什么是协程？在本次分享中，我会试着进行回答。
> Q3 (How): 怎么实现协程 (库)？
> 在回答了协程是什么之后，第三个问题就紧随而来，我们可以自己去实现一个简单的协程或者协程库吗？如果可以实现，那我们应该怎么实现呢？
> Q4 (Usage): 使用协程时需要注意什么？
> 通过实际编码实现一个简单的协程库后，我们再来看 libco 的实现，就会清晰明了。我们会在第四部分介绍使用协程时需要注意的一些问题。
> 这就是我本次分享想要达成的目标 —— 回答这四个问题。

~~~
 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题 

 二、这个技术的优势和劣势分别是什么 

三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

四、技术的组成部分和关键点。

五、技术的底层原理和关键实现

六、已有的实现和它之间的对比
~~~





#  故障1 



### 现象描述：发现内存被占用，但是查找不到相关进程，



### 1. free ps top 这些工具统计差距这么大，多出内存去哪里？

~~~

[root@localhost ~]# free -g
              total        used        free      shared  buff/cache   available
Mem:           1007          16         990           0           0         987
Swap:            62           0          62


ps aux | awk '{mem += $6} END {print mem/1024/1024}'
2.13824


top free 命令完整来源
cat /proc/meminfo
while true; do cat /proc/meminfo | grep MemFree; sleep 10; done
~~~

 画外音：

- ps top 这个工具使用场景是什么？用户态的内存 没有专门统计另一项缓存: Slab。
- /proc/meminfo是了解Linux系统[内存](https://so.csdn.net/so/search?q=内存&spm=1001.2101.3001.7020)使用状况的主要接口，我们最常用的”free”、”vmstat”等命令就是通过它获取数据的 ，/proc/meminfo所包含的信息比”free”等命令要丰富得多

### 2 陈咬金三板斧：不用什么原有 执行 drop_caches 之后，slab内存还是没有下降，无效操作？

~~~shell
## 没有释放
echo 3 > /proc/sys/vm/drop_caches

## 其中SUnreclaim:,随着测试时间加长SUnreclaim一直在增加，证明存在内存泄露可能.
On linux Slab is subdivided into two: SReclaimable and SUnreclaim .

[root@localhost ~]# cat /proc/meminfo | grep "SUnreclaim"
SUnreclaim:     11777088 kB
[root@localhost ~]# cat /proc/meminfo | grep "SUnreclaim"
SUnreclaim:     11778504 kB
[root@localhost ~]#


### slab

cat /proc/slabinfo  | awk '{if($3*$4/1024/1024 > 100){print $1,$3*$4/1024/1024} }' |sort -nr

task_struct 8784.26
taskstats 390.255
signal_cache 1379.58
kmalloc-192 119.878
kmalloc-128 146.234
cred_jar 218.97
beegfs_inode_cache 226.975



# 尝试在/sys/kernel/slab/*/中提供了alloc_calls和free_calls。 这个中断

cat: /sys/kernel/slab/kmalloc-128/alloc_calls: Function not implemented

cat /sys/kernel/slab/task_struct/alloc_calls
cat: /sys/kernel/slab/task_struct/alloc_calls: Function not implemented

CONFIG_SLUB_DEBUG_ON 不知道如何开启 

slabinfo  -S
slabtop

在内核中打开slub_debug，相关的工具有slabinfo。
slabinfo的数据来源是/sys/kernel/slab。
slabtop -s l

 OBJS ACTIVE  USE OBJ SIZE  SLABS OBJ/SLAB CACHE SIZE NAME
1246208 1246174  99%    7.56K 311552        4   9969664K task_struct
1246362 1246362 100%    1.19K  47937       26   1533984K signal_cache
1254336 1254336 100%    0.12K  39198       32    156792K kmalloc-128
1252356 1252356 100%    0.19K  29818       42    238544K cred_jar

经过确认  cat /boot/config-4.18.0-305.3.1.el8_4.x86_64
cat /boot/config-4.18.0-305.3.1.el8_4.x86_64 |grep CONFIG_SLUB_DEBUG_ON
# CONFIG_SLUB_DEBUG_ON is not set
开启

~~~



> 画外音：

- cat /sys/kernel/slab/task_struct/alloc_calls 使用条件是什么？需要重新编译源码 开启CONFIG_SLUB_DEBUG_ON 这里没做不到 中断了
- 


### 3 对比业务发现tcp链接内存不增长，会马上释放，然后短链接改为长链接 内存也不增长。


~~~
--正常情况
Overhead  Shared Object              Symbol
  34.41%  [kernel]                   [k] clear_page_erms                                                                                                                                                                                                               ◆
  21.52%  libc-2.28.so               [.] __memset_avx2_erms                                                                                                                                                                                                            ▒
   1.13%  [kernel]                   [k] _raw_spin_lock                                                                                                                                                                                                                ▒
   1.11%  [kernel]                   [k] _raw_spin_lock_irqsave                                                                                                                                                                                                        ▒
   1.03%  [kernel]                   [k] unpin_user_pages_dirty_lock                                                                                                                                                                                                   ▒
   0.76%  [kernel]                   [k] get_page_from_freelist                                                                                                                                                                                                        ▒
   0.61%  ld-2.28.so                 [.] do_lookup_x                                                                                                                                                                                                                   ▒
   0.60%  [kernel]                   [k] native_irq_return_iret                                                                                                                                                                                                        ▒
   0.53%  [kernel]                   [k] __list_del_entry_valid                                                                                                                                                                                                        ▒
   0.52%  [kernel]                   [k] update_sd_lb_stats.constprop.118                                                                                                                                                                                              ▒
   0.47%  [kernel]                   [k] update_rq_clock   
   
~~~

~~~
内存增长的perf top
Overhead  Shared Object              Symbol
  11.98%  [kernel]                   [k] copy_user_enhanced_fast_string
  10.42%  [kernel]                   [k] ext4_mpage_readpages
   7.35%  [kernel]                   [k] native_queued_spin_lock_slowpath
   5.89%  libc-2.28.so               [.] __memcmp_avx2_movbe
   5.67%  libc-2.28.so               [.] __memmove_avx_unaligned_erms
   3.61%  [kernel]                   [k] _raw_spin_lock_irqsave
   2.15%  [kernel]                   [k] __list_del_entry_valid
   1.71%  libpthread-2.28.so         [.] pthread_spin_lock
   1.30%  [kernel]                   [k] ext4_es_lookup_extent
   1.09%  [kernel]                   [k] mutex_lock
   0.99%  [kernel]                   [k] mlx5_eq_comp_int
   0.96%  libmpi.so.12.1.8           [.] 0x00000000001c47d5
   0.92%  [kernel]                   [k] get_page_from_freelist
   0.91%  [kernel]                   [k] free_pcppages_bulk
   0.79%  [kernel]                   [k] osq_lock
~~~

perf record  -F 99  -g -a  -- sleep 20
perf report --stdio
perf record  -F 99  -g -e "kmem:*"  -- sleep 20



~~~
- 正常情况
# Children      Self  Command          Shared Object              Symbol                                                                                                                                                                                               >
# ........  ........  ...............  .........................  .....................................................................................................................................................................................................>
#
    32.52%     0.00%  beegfs-ctl/Main  [unknown]                  [k] 0000000000000000
            |
            ---0
               |
                --30.94%--ibv_query_device

    31.47%     6.76%  beegfs-ctl/Main  libc-2.28.so               [.] __memset_avx2_erms
            |
            |--24.70%--__memset_avx2_erms
            |          |
            |           --24.31%--page_fault
            |                     |
            |                      --24.18%--do_page_fault
            |                                |
            |                                 --24.17%--__do_page_fault
            |                                           |
            |                                            --24.14%--handle_mm_fault
            |                                                      |
            |                                                       --24.10%--__handle_mm_fault
            |                                                                 |
            |                                                                 |--22.95%--do_huge_pmd_anonymous_page
            |                                                                 |          |
            |                                                                 |           --22.22%--clear_huge_page
            |                                                                 |                     |
            |                                                                 |                      --21.73%--clear_subpage
            |                                                                 |                                |
            |                                                                 |                                 --21.37%--clear_page_erms
            |                                                                 |
            |                                                                  --1.09%--do_anonymous_page
            |
             --6.75%--0
                       |
                        --6.62%--ibv_query_device

    31.36%     0.05%  beegfs-ctl/Main  [kernel.kallsyms]          [k] page_fault
            |
             --31.31%--page_fault
                       |
                        --24.81%--do_page_fault
                                  __do_page_fault
                                  |
                                   --24.75%--handle_mm_fault
                                             |
                                              --24.70%--__handle_mm_fault
                                                        |
                                                        |--23.24%--do_huge_pmd_anonymous_page
                                                        |          |
                                                        |           --22.49%--clear_huge_page
                                                        |                     |
                                                        |                      --22.00%--clear_subpage
                                                        |                                |
                                                        |                                 --21.64%--clear_page_erms
                                                        |
                                                         --1.27%--do_anonymous_page
                                                                   |
                                                                    --0.52%--mem_cgroup_charge

    30.94%     0.00%  beegfs-ctl/Main  libibverbs.so.1.14.43.0    [.] ibv_query_device
            |
            ---ibv_query_device

    24.82%     0.00%  beegfs-ctl/Main  [kernel.kallsyms]          [k] do_page_fault
            |
            ---do_page_fault
               |
                --24.81%--__do_page_fault
                          |
                           --24.75%--handle_mm_fault
                                     |
                                      --24.70%--__handle_mm_fault
                                                |
                                                |--23.24%--do_huge_pmd_anonymous_page
                                                |          |
                                                |           --22.49%--clear_huge_page
                                                |                     |
                                                |                      --22.00%--clear_subpage
                                                |                                |
                                                |                                 --21.64%--clear_page_erms
                                                |
                                                 --1.27%--do_anonymous_page
                                                           |
                                                            --0.52%--mem_cgroup_charge

    24.81%     0.00%  beegfs-ctl/Main  [kernel.kallsyms]          [k] __do_page_fault
            |
             --24.81%--__do_page_fault
                       |
                        --24.75%--handle_mm_fault
                                  |
                                   --24.70%--__handle_mm_fault
                                             |
                                             |--23.24%--do_huge_pmd_anonymous_page
                                             |          |
                                             |           --22.49%--clear_huge_page
                                             |                     |
                                             |                      --22.00%--clear_subpage
                                             |                                |
                                             |                                 --21.64%--clear_page_erms
                                             |
                                              --1.27%--do_anonymous_page
                                                        |
                                                         --0.52%--mem_cgroup_charge

    24.75%     0.03%  beegfs-ctl/Main  [kernel.kallsyms]          [k] handle_mm_fault
            |
             --24.72%--handle_mm_fault
                       |
                        --24.70%--__handle_mm_fault
                                  |
                                  |--23.24%--do_huge_pmd_anonymous_page
                                  |          |
                                  |           --22.49%--clear_huge_page
                                  |                     |
                                  |                      --22.00%--clear_subpage
                                  |                                |
                                  |                                 --21.64%--clear_page_erms
                                  |
                                   --1.27%--do_anonymous_page
                                             |
                                              --0.52%--mem_cgroup_charge

    24.70%     0.07%  beegfs-ctl/Main  [kernel.kallsyms]          [k] __handle_mm_fault
            |
             --24.63%--__handle_mm_fault
                       |
                       |--23.24%--do_huge_pmd_anonymous_page
                       |          |
                       |           --22.49%--clear_huge_page
                       |                     |
                       |                      --22.00%--clear_subpage
                       |                                |
                       |                                 --21.64%--clear_page_erms
                       |
                        --1.27%--do_anonymous_page
                                  |
								  
								  
								  
								  
								    24.47%     0.00%  ConnAccept       [unknown]                  [.] 0000000000000000
            |
            ---0
               |
                --24.23%--ibv_query_device
                          |
                          |--1.14%--0x7f2f0cfc8830
                          |          __memset_avx2_erms
                          |          |
                          |           --1.11%--page_fault
                          |                     |
                          |                      --0.94%--do_page_fault
                          |                                __do_page_fault
                          |                                handle_mm_fault
                          |                                __handle_mm_fault
                          |                                |
                          |                                 --0.92%--do_huge_pmd_anonymous_page
                          |                                           |
                          |                                            --0.91%--clear_huge_page
                          |                                                      clear_subpage
                          |                                                      |
                          |                                                       --0.87%--clear_page_erms
                          |
                          |--1.02%--0x7f2f0cfc8530
                          |          __memset_avx2_erms
                          |          |
                          |           --0.96%--page_fault
                          |                     |
                          |                      --0.79%--do_page_fault
                          |                                __do_page_fault
                          |                                handle_mm_fault
                          |                                __handle_mm_fault
                          |                                |
                          |                                 --0.73%--do_huge_pmd_anonymous_page
                          |                                           clear_huge_page
                          |                                           clear_subpage
                          |                                           |
                          |                                            --0.71%--clear_page_erms
                          |
                          |--1.00%--0x7f2f0ce7e6f0
                          |          __memset_avx2_erms
                          |          |
                          |           --0.96%--page_fault
                          |                     |
                          |                      --0.78%--do_page_fault
                          |                                __do_page_fault
                          |                                handle_mm_fault
                          |                                __handle_mm_fault
                          |                                |
                          |                                 --0.76%--do_huge_pmd_anonymous_page
                          |                                           |
                          |                                            --0.73%--clear_huge_page
                          |                                                      |
                          |                                                       --0.72%--clear_subpage
                          |                                                                 |
                          |                                                                  --0.70%--clear_page_erms
                          |
                          |--0.98%--0x7f2f0c2a0b80
                          |          __memset_avx2_erms
                          |          page_fault
                          |          |
                          |           --0.67%--do_page_fault
                          |                     __do_page_fault
                          |                     |
                          |                      --0.63%--handle_mm_fault
                          |                                __handle_mm_fault
                          |                                |
                          |                                 --0.62%--do_huge_pmd_anonymous_page
                          |                                           |
                          |                                            --0.60%--clear_huge_page




    24.42%     5.91%  ConnAccept       libc-2.28.so               [.] __memset_avx2_erms
            |
            |--18.52%--__memset_avx2_erms
            |          |
            |           --18.11%--page_fault
            |                     |
            |                      --17.99%--do_page_fault
            |                                __do_page_fault
            |                                |
            |                                 --17.91%--handle_mm_fault
            |                                           |
            |                                            --17.87%--__handle_mm_fault
            |                                                      |
            |                                                      |--16.61%--do_huge_pmd_anonymous_page
            |                                                      |          |
            |                                                      |           --16.23%--clear_huge_page
            |                                                      |                     |
            |                                                      |                      --16.05%--clear_subpage
            |                                                      |                                |
            |                                                      |                                 --15.67%--clear_page_erms
            |                                                      |
            |                                                       --1.21%--do_anonymous_page
            |



~~~

~~~

+   32.52%     0.00%  beegfs-ctl/Main  [unknown]                  [k] 0000000000000000                                                                                                                                                                                 ◆
+   31.47%     6.76%  beegfs-ctl/Main  libc-2.28.so               [.] __memset_avx2_erms                                                                                                                                                                               ▒
+   31.36%     0.05%  beegfs-ctl/Main  [kernel.kallsyms]          [k] page_fault                                                                                                                                                                                       ▒
+   30.94%     0.00%  beegfs-ctl/Main  libibverbs.so.1.14.43.0    [.] ibv_query_device                                                                                                                                                                                 ▒
+   24.82%     0.00%  beegfs-ctl/Main  [kernel.kallsyms]          [k] do_page_fault                                                                                                                                                                                    ▒
+   24.81%     0.00%  beegfs-ctl/Main  [kernel.kallsyms]          [k] __do_page_fault                                                                                                                                                                                  ▒
+   24.75%     0.03%  beegfs-ctl/Main  [kernel.kallsyms]          [k] handle_mm_fault                                                                                                                                                                                  ▒
+   24.70%     0.07%  beegfs-ctl/Main  [kernel.kallsyms]          [k] __handle_mm_fault                                                                                                                                                                                ▒
+   24.47%     0.00%  ConnAccept       [unknown]                  [.] 0000000000000000                                                                                                                                                                                 ▒
+   24.42%     5.91%  ConnAccept       libc-2.28.so               [.] __memset_avx2_erms                                                                                                                                                                               ▒
+   24.23%     0.00%  ConnAccept       libibverbs.so.1.14.43.0    [.] ibv_query_device                                                                                                                                                                                 ▒
+   23.71%     0.01%  ConnAccept       [kernel.kallsyms]          [k] page_fault                                                                                                                                                                                       ▒
+   23.24%     0.02%  beegfs-ctl/Main  [kernel.kallsyms]          [k] do_huge_pmd_anonymous_page                                                                                                                                                                       ▒
+   22.49%     0.09%  beegfs-ctl/Main  [kernel.kallsyms]          [k] clear_huge_page                                                                                                                                                                                  ▒
+   22.00%     0.28%  beegfs-ctl/Main  [kernel.kallsyms]          [k] clear_subpage                                                                                                                                                                                    ▒
+   21.84%    21.51%  beegfs-ctl/Main  [kernel.kallsyms]          [k] clear_page_erms                                                                                                                                                                                  ▒
+   18.01%     0.00%  ConnAccept       [kernel.kallsyms]          [k] do_page_fault                                                                                                                                                                                    ▒
+   18.01%     0.04%  ConnAccept       [kernel.kallsyms]          [k] __do_page_fault                                                                                                                                                                                  ▒
+   17.94%     0.03%  ConnAccept       [kernel.kallsyms]          [k] handle_mm_fault                                                                                                                                                                                  ▒
+   17.88%     0.04%  ConnAccept       [kernel.kallsyms]          [k] __handle_mm_fault                                                                                                                                                                                ▒
+   16.61%     0.01%  ConnAccept       [kernel.kallsyms]          [k] do_huge_pmd_anonymous_page                                                                                                                                                                       ▒
+   16.23%     0.03%  ConnAccept       [kernel.kallsyms]          [k] clear_huge_page                                                                                                                                                                                  ▒
+   16.08%     0.30%  ConnAccept       [kernel.kallsyms]          [k] clear_subpage                                                                                                                                                                                    ▒
+   15.89%    15.70%  ConnAccept       [kernel.kallsyms]          [k] clear_page_erms                                                                                                                                                                                  ▒
+   11.13%     0.28%  swapper          [kernel.kallsyms]          [k] do_idle                         
~~~

perf report --stdio

~~~
# Children      Self  Command          Shared Object              Symbol                                                                                                                                                                                               >
# ........  ........  ...............  .........................  .....................................................................................................................................................................................................>
#
    18.48%     0.00%  beegfs-ctl/Main  [kernel.kallsyms]          [k] entry_SYSCALL_64_after_hwframe
            |
            ---entry_SYSCALL_64_after_hwframe
               do_syscall_64
               |
               |--14.57%--ksys_read
               |          vfs_read
               |          |
               |           --14.54%--new_sync_read
               |                     |
               |                      --14.53%--FhgfsOps_buffered_read_iter
               |                                read_common
               |                                |
               |                                 --14.53%--FhgfsOpsHelper_readCached
               |                                           |
               |                                            --14.53%--__FhgfsOpsHelper_readCacheFlushed
               |                                                      FhgfsOpsRemoting_readfileVec
               |                                                      |
               |                                                       --14.51%--FhgfsOpsCommKit_readfileV2bCommunicate
               |                                                                 |
               |                                                                  --14.51%--FhgfsOpsCommkit_communicate
               |                                                                            |
               |                                                                            |--8.52%--__commkit_readfile_recvdata
               |                                                                            |          |
               |                                                                            |           --8.50%--__commkit_readfile_receive
               |                                                                            |                     |
               |                                                                            |                      --8.39%--__IBVSocket_recvContinueIncomplete
               |                                                                            |                                |
               |                                                                            |                                |--6.63%--_copy_to_iter
               |                                                                            |                                |          |
               |                                                                            |                                |           --6.53%--copyout
               |                                                                            |                                |                     |
               |                                                                            |                                |                      --6.47%--copy_user_enhanced_fast_string
               |                                                                            |                                |
               |                                                                            |                                 --1.15%--__IBVSocket_postRecv
               |                                                                            |                                           |
               |                                                                            |                                            --1.03%--mlx5_ib_post_recv
               |                                                                            |                                                      |
               |                                                                            |                                                       --0.91%--_raw_spin_lock_irqsave
               |                                                                            |
               |                                                                            |--2.83%--NodeConnPool_acquireStreamSocketEx
               |                                                                            |          |
               |                                                                            |           --1.89%--__mutex_lock.isra.6
               |                                                                            |                     |
               |                                                                            |                      --1.35%--osq_lock
               |                                                                            |
               |                                                                            |--1.74%--SocketTk_poll
               |                                                                            |          |
               |                                                                            |           --1.17%--IBVSocket_poll
               |                                                                            |                     |
               |                                                                            |                      --0.93%--__IBVSocket_receiveCheck
               |                                                                            |                                |
               |                                                                            |                                 --0.90%--__IBVSocket_recvWC
               |                                                                            |                                           |
               |                                                                            |                                            --0.86%--__IBVSocket_waitForRecvCompletionEvent
               |                                                                            |                                                      |
               |                                                                            |                                                       --0.82%--mlx5_ib_poll_cq
               |                                                                            |
               |                                                                             --0.83%--__commkit_prepare_io
               |                                                                                       |
               |                                                                                        --0.66%--mutex_lock
               |
                --3.73%--ksys_write
                          vfs_write
                          |
                           --3.73%--new_sync_write
                                     FhgfsOps_buffered_write_iter
                                     write_common
                                     FhgfsOpsHelper_writeCached
                                     |
                                      --3.73%--FhgfsOpsHelper_writefileEx
                                                FhgfsOpsRemoting_writefileVec
                                                |
                                                 --3.72%--FhgfsOpsCommKit_writefileV2bCommunicate
                                                           |
                                                            --3.71%--FhgfsOpsCommkit_communicate
                                                                      |
                                                                      |--2.70%--__commkit_writefile_sendData
                                                                      |          IBVSocket_send
                                                                      |          |
                                                                      |           --2.28%--IBVBuffer_fill
                                                                      |                     |
                                                                      |                      --1.99%--_copy_from_iter
                                                                      |                                |
                                                                      |                                 --1.96%--copyin
                                                                      |                                           |
                                                                      |                                            --1.95%--copy_user_enhanced_fast_string
                                                                      |
                                                                       --0.74%--NodeConnPool_acquireStreamSocketEx

    18.48%     0.00%  beegfs-ctl/Main  [kernel.kallsyms]          [k] do_syscall_64
            |
             --18.48%--do_syscall_64
                       |
                       |--14.57%--ksys_read
                       |          vfs_read
                       |          |
                       |           --14.54%--new_sync_read
                       |                     |
                       |                      --14.53%--FhgfsOps_buffered_read_iter
                       |                                read_common
                       |                                |
                       |                                 --14.53%--FhgfsOpsHelper_readCached
                       |                                           |
                       |                                            --14.53%--__FhgfsOpsHelper_readCacheFlushed
                       |                                                      FhgfsOpsRemoting_readfileVec
                       |                                                      |
                       |                                                       --14.51%--FhgfsOpsCommKit_readfileV2bCommunicate
                       |                                                                 |
                       |                                                                  --14.51%--FhgfsOpsCommkit_communicate
                       |                                                                            |
                       |                                                                            |--8.52%--__commkit_readfile_recvdata
                       |                                                                            |          |
                       |                                                                            |           --8.50%--__commkit_readfile_receive
                       |                                                                            |                     |
                       |                                                                            |                      --8.39%--__IBVSocket_recvContinueIncomplete
                       |                                                                            |                                |
                       |                                                                            |                                |--6.63%--_copy_to_iter
                       |                                                                            |                                |          |
                       |                                                                            |                                |           --6.53%--copyout
                       |                                                                            |                                |                     |
                       |                                                                            |                                |                      --6.47%--copy_user_enhanced_fast_string
                       |                                                                            |                                |
                       |                                                                            |                                 --1.15%--__IBVSocket_postRecv
                       |                                                                            |                                           |
                       |                                                                            |                                            --1.03%--mlx5_ib_post_recv
                       |                                                                            |                                                      |
                       |                                                                            |                                                       --0.91%--_raw_spin_lock_irqsave
                       |                                                                            |
                       |                                                                            |--2.83%--NodeConnPool_acquireStreamSocketEx
                       |                                                                            |          |
                       |                                                                            |           --1.89%--__mutex_lock.isra.6
                       |                                                                            |                     |
                       |                                                                            |                      --1.35%--osq_lock
                       |                                                                            |
                       |                                                                            |--1.74%--SocketTk_poll
                       |                                                                            |          |
                       |                                                                            |           --1.17%--IBVSocket_poll
                       |                                                                            |                     |
                       |                                                                            |                      --0.93%--__IBVSocket_receiveCheck
                       |                                                                            |                                |
                       |                                                                            |                                 --0.90%--__IBVSocket_recvWC
                       |                                                                            |                                           |
                       |                                                                            |                                            --0.86%--__IBVSocket_waitForRecvCompletionEvent
                       |                                                                            |                                                      |
                       |                                                                            |                                                       --0.82%--mlx5_ib_poll_cq
                       |                                                                            |
                       |                                                                             --0.83%--__commkit_prepare_io
                       |                                                                                       |
                       |                                                                                        --0.66%--mutex_lock
                       |
                        --3.73%--ksys_write
                                  vfs_write
                                  |
                                   --3.73%--new_sync_write
                                             FhgfsOps_buffered_write_iter
                                             write_common
                                             FhgfsOpsHelper_writeCached
                                             |
                                              --3.73%--FhgfsOpsHelper_writefileEx
                                                        FhgfsOpsRemoting_writefileVec
                                                        |
                                                         --3.72%--FhgfsOpsCommKit_writefileV2bCommunicate
                                                                   |
                                                                    --3.71%--FhgfsOpsCommkit_communicate
                                                                              |
                                                                              |--2.70%--__commkit_writefile_sendData
                                                                              |          IBVSocket_send
                                                                              |          |
                                                                              |           --2.28%--IBVBuffer_fill
                                                                              |                     |
                                                                              |                      --1.99%--_copy_from_iter
                                                                              |                                |
                                                                              |                                 --1.96%--copyin
                                                                              |                                           |
                                                                              |                                            --1.95%--copy_user_enhanced_fast_string
                                                                              |
                                                                               --0.74%--NodeConnPool_acquireStreamSocketEx

    18.22%     0.08%  beegfs-ctl/Main  [kernel.kallsyms]          [k] FhgfsOpsCommkit_communicate
            |
             --18.15%--FhgfsOpsCommkit_communicate
                       |
                       |--8.52%--__commkit_readfile_recvdata
                       |          |
                       |           --8.50%--__commkit_readfile_receive
                       |                     |
                       |                      --8.39%--__IBVSocket_recvContinueIncomplete
                       |                                |
                       |                                |--6.63%--_copy_to_iter
                       |                                |          |
                       |                                |           --6.53%--copyout
                       |                                |                     |
                       |                                |                      --6.47%--copy_user_enhanced_fast_string
                       |                                |
                       |                                 --1.15%--__IBVSocket_postRecv
                       |                                           |
                       |                                            --1.03%--mlx5_ib_post_recv
                       |                                                      |
                       |                                                       --0.91%--_raw_spin_lock_irqsave
                       |
                       |--3.56%--NodeConnPool_acquireStreamSocketEx
                       |          |
                       |          |--2.38%--__mutex_lock.isra.6
                       |          |          |
                       |          |          |--1.70%--osq_lock
                       |          |          |
                       |          |           --0.52%--mutex_spin_on_owner
                       |          |
                       |           --0.52%--schedule
                       |
                       |--2.70%--__commkit_writefile_sendData
                       |          IBVSocket_send
                       |          |
                       |           --2.28%--IBVBuffer_fill
                       |                     |
                       |                      --1.99%--_copy_from_iter
                       |                                |
                       |                                 --1.96%--copyin
                       |                                           |
                       |                                            --1.95%--copy_user_enhanced_fast_string
                       |
                       |--1.86%--SocketTk_poll
                       |          |
                       |           --1.22%--IBVSocket_poll
                       |                     |
                       |                      --0.94%--__IBVSocket_receiveCheck
                       |                                |
                       |                                 --0.91%--__IBVSocket_recvWC
                       |                                           |
                       |                                            --0.87%--__IBVSocket_waitForRecvCompletionEvent
                       |                                                      |
                       |                                                       --0.84%--mlx5_ib_poll_cq
                       |
                       |--0.83%--__commkit_prepare_io
                       |          |
                       |           --0.67%--mutex_lock
                       |
                        --0.52%--__commkit_cleanup_generic
                                  |
                                   --0.51%--NodeConnPool_releaseStreamSocket

    16.54%     0.25%  swapper          [kernel.kallsyms]          [k] do_idle
            |

~~~
> 画外音
ofed_info  -s
MLNX_OFED_LINUX-5.8-1.0.1.1:

ofed_info  -s
MLNX_OFED_LINUX-5.6-1.0.3.3:

https://docs.nvidia.com/networking/display/MLNXOFEDv581011/Introduction


### 启动

beegfs客户端无法启动，提示管理节点链接不上
ip a
ibstata 
不显示ip 网卡

/etc/init.d/opensmd restart 失败


 1164  lsmod |grep rdma
 1178  rmmod beegfs
 1180  lsmod |grep beegfs


lsmod |grep beegfs
lsmod - Show the status of modules in the Linux Kernel



###  相关文章

- linux内存占用问题调查——slab

  https://dandelioncloud.cn/article/details/1392448199679090689/

  - slab申请释放和追踪 alloc_calls

    Chapter 8 Slab Allocator

  - https://www.kernel.org/doc/gorman/html/understand/understand025.html

  - https://www.kernel.org/doc/gorman/html/understand/understand015.html#htoc88

    SLAB 分配器和 kmalloc



- SLUB DEBUG原理 图解slub

  http://www.wowotech.net/memory_management/426.html

  linux 内核 内存管理 slub算法 （一） 原理

  https://blog.csdn.net/lukuen/article/details/6935068

- Linux kernel configuration

```
$ git clone https://github.com/torvalds/linux.git
```



- 趣谈Linux操作系统学习笔记：物理内存管理（下）（第24讲）
- https://www.cnblogs.com/luoahong/p/10907734.html

kmem_cache_create("task_struct"

kmem_cache_alloc_node

[1]  bpftrace使用sizeof()查看task_struct的大小

[2]BeeGFS源码分析2-客户端概要分析

[3] Linux内存管理slub分配器

 https://blog.csdn.net/weiqifa0/article/details/108591091
