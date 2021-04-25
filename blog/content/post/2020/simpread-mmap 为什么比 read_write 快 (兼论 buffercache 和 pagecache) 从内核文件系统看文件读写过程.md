---
title: "mmap 为什么比 read/write 快"
date: 2019-11-26
description: "从内核文件系统看文件读写过程"
draft: false
tags: ["kernel","FQA"]
categories: ["操作系统"]
---

参考文献：

《[从内核文件系统看文件读写过程](http://www.cnblogs.com/huxiao-tee/p/4657851.html)》http://www.cnblogs.com/huxiao-tee/p/4660352.html?utm_source=tuicool&utm_medium=referral

《mmap 是什么》http://www.cnblogs.com/huxiao-tee/p/4660352.html?utm_source=tuicool&utm_medium=referral



# 补充概念

- 用户缓冲缓冲区 是无论内存还是虚拟内存，是内核还是用户态？如何查看

   **用户缓冲区的目的是为了减少系统调用次数，从而降低操作系统在用户态与核心态切换所耗费的时间**

  

   

# 正文：



      首先说一下文件系统，[Linux](http://lib.csdn.net/base/linux "Linux知识库") 文件系统的三层结构想必大家多少都了解一些，每个进程中都有一个用户文件描述符表，表项指向一个全局的文件表中的某个表项，文件表表项有一个指向内存 inode 的指针，每个 inode 唯一标识一个文件。如果同时有多个进程打开同一文件，他们的用户文件描述符表项指向不同的文件表项，但是这些文件表项会指向同一个 inode。

     此时又会引出另外一个东东：page cache。内核会为每个文件单独维护一个 page cache，用户进程对于文件的大多数读写操作会直接作用到 page cache 上，内核会选择在适当的时候将 page cache 中的内容写到磁盘上（当然我们可以手工 fsync 控制回写），这样可以大大减少磁盘的访问次数，从而提高性能。Page cache 是 linux 内核文件访问过程中很重要的[数据结构](http://lib.csdn.net/base/datastructure "算法与数据结构知识库")，page cache 中会保存用户进程访问过得该文件的内容，这些内容以页为单位保存在内存中，当用户需要访问文件中的某个偏移量上的数据时，内核会以偏移量为索引，找到相应的内存页，如果该页没有读入内存，则需要访问磁盘读取数据。为了提高页得查询速度同时节省 page cache 数据结构占用的内存，linux 内核使用树来保存 page cache 中的页。

     在了解了以上的基础之后，我们就来比较一下 mmap 和 read/write 的区别，先说一下 read/write 系统调用，read/write 系统调用会有以下的操作：

1.  访问文件，这涉及到用户态到内核态的转换
2.  读取硬盘文件中的对应数据，内核会采用预读的方式，比如我们需要访问 100 字节，内核实际会将按照 4KB(内存页的大小) 存储在 page cache 中
3.  将 read 中需要的数据，从 page cache 中拷贝到用户缓冲区中

     整个过程还是比较艰辛的，基本上涉及到用户内核态的切换，还有就是数据拷贝接下来继续说 mmap 吧，mmap 系统调用是将硬盘文件映射到用内存中，说的底层一些是将 page cache 中的页直接映射到用户进程地址空间中，从而进程可以直接访问自身地址空间的虚拟地址来访问 page cache 中的页，这样会并涉及 page cache 到用户缓冲区之间的拷贝，mmap 系统调用与 read/write 调用的区别在于：

1.  mmap 只需要一次系统调用，后续操作不需要系统调用
2.  访问的数据不需要在 page cache 和用户缓冲区之间拷贝

     从上所述，当频繁对一个文件进行读取操作时，mmap 会比 read 高效一些。

     最后再说一下 page cache 的话题，从上面所说我们从磁盘文件中读取的内容都会存在 page cache 中，但当我们关闭这个文件时，page cache 中内容会立马释放掉吗？答案是否，磁盘的读取速度比内存慢太多，如果能命中 page cache 可以显著提升性能，万一后续又有对这个文件的操作，系统就可以很快速的响应。当然，这些文件内容也不是一直存在 page cache 中的，一般只要系统有空闲物理内存，内核都会拿来当缓存使用，但当物理内存不够用，内存会清理出部分 page cache 应急，这也就是告诉我们程序对于物理内存的使用能省则省，交给内核使用，作用很大。

     还有就是普通的 write 调用只是将数据写到 page cache 中，并将其标记为 dirty 就返回了，磁盘 I/O 通常不会立即执行，这样做的好处是减少磁盘的回写次数，提供吞吐率，不足就是机器一旦意外挂掉，page cache 中的数据就会丢失。一般安全性比较高的程序会在每次 write 之后，调用 fsync 立即将 page cache 中的内容回写到磁盘中。

# 参考

-  https://www.ibm.com/developerworks/cn/linux/l-ipc/part5/index1.html 【不明白 怎么转换的】
-  https://www.cnblogs.com/huxiao-tee/p/4660352.html?utm_source=tuicool&utm_medium=referral 【不明白 磁盘直接到用户，还是磁盘到内核没会说清楚】
- [Fsync](https://www.cnblogs.com/hustcat/p/3283955.html)
-  https://www.cnblogs.com/JohnABC/p/5821660.html 【3  **Unix环境高级编程** 】
-   http://chinaunix.net/uid-22906954-id-4161625.html 【2】
- https://www.cnblogs.com/alantu2018/p/8472718.html 【1】

