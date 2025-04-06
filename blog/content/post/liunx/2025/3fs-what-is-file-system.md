---
title: 从青铜到王者系列：3FS如何实现一个文件系统
date: 2025-03-23
description: 从青铜到王者
draft: false
tags:
  - code_reading
---
从青铜到王者系列:深入浅出理解DeepSeek 3FS (2)从内核到用户态文件系统的设计之路


大家好，我是小王同学，

本文希望帮你深入理解分布式存储系统3FS更进一步

- **昵称**：20点下班就是一件很幸福事情
- **愿景**：让小孩都可以听得懂我在讲什么。
- **我能提供什么**：多问自己一次为什么这样用
- **不明白的留言**：
1. Wechat/GitHub：watchpoints




![](https://files.mdnice.com/user/5197/619f94e3-143d-4c87-b967-1bcfdb5f7d62.png)

## 作业 实现 hello world 的文件系统 ，并说出IO流程


参考答案 
- https://www.qiyacloud.cn/2021/05/2021-06-07/
- https://github.com/libfuse/libfuse/blob/master/example/hello.c

没有FUSE的IO流程示意图


![](https://files.mdnice.com/user/5197/3313ee72-057f-4768-8e49-a131d64c3138.png)


![](https://files.mdnice.com/user/5197/8bdff1ff-7384-4b4e-a565-6e0fc5a2db39.png)

带FUSE的IO流程示意图



![](https://files.mdnice.com/user/5197/cfa47aa2-5c69-4251-864e-9cf7ebeef126.png)



![](https://files.mdnice.com/user/5197/882d8f2a-5511-4dd0-8609-3aa9de0cefd7.png)


## 一、导读

从青铜到王者系列：深入浅出理解 DeepSeek 3FS（1）

我整理 大模型训练 
- 在checkpoint时候对大的文件读写，
- 在批量查询的时候对小文件读写

目前
- 谷歌的GFS 适合 大文件顺序读写，
- Ceph  mds 元数据 存在瓶颈 8k作用


幻方为什么要搞3FS？
AI训练与推理的业务需求，传统分布式文件系统已经难以满足：
1. 海量数据顺序读（语料输入）
2. 检查点（分步骤重算）
3. 顺序写（模型参数保存）；
4. 随机读



因此重新实现一个新的文件系统



>不考虑分布式，个人笔记或者购买云主机文件系统
如何支持大小文件读写，并且满足高效查询




## 提问：Linux 系统启动过程

划重点：重点是文件系统部分，扩展虚拟机（KVM）与内核部分。



![](https://files.mdnice.com/user/5197/f2b0ec1d-bd25-400c-8efe-93668ddd27b9.png)


电脑通电后，加载主板上的BIOS(basic input output system)程序

BIOS是电脑启动时加载的第一个软件。
它是一组固化到计算机内主板上一个ROM芯片上的程序

##### boot：
虚拟机（KVM）如何设置引导硬盘顺序

什么是 KVM？
基于内核的虚拟机（KVM）是 Linux® 操作系统的一种开源虚拟化技术。借助 KVM，Linux 可作为虚拟机监控程序运行多个独立的虚拟机（VM）

BIOS--》boot
第1引导顺序：hard drive 硬盘

第2引导顺序：cdrom 光驱  ----》安装系统

第3引导顺序：removable device 可移动设备--》u盘，移动硬盘  --》安装系统

第4引导顺序：Network  --》从网络启动--》网络中安装服务器启动  --》安装

#### 文件系统部分 
启动流程：


1. 内核加载完成



2. 执行mount操作

挂载文件系统想要操作文件系统，

第一件事情就是挂载文件系统



3. 读取超级块
4. 定位inode 2
5. 将其挂载为根目录

```
系统启动过程：
+----------------+     +---------------+     +------------------+
| 读取超级块     | --> | 定位inode 2   | --> | 挂载为根目录(/)  |
+----------------+     +---------------+     +------------------+
```

![](https://files.mdnice.com/user/5197/19959a5e-7bf2-44c5-8bb6-7ca9f37d6349.png)


在 Linux 启动过程中，加载 ext4 文件系统主要经历以下几个阶段：

1. **内核加载与初始化**  
   - 当系统通过 BIOS/UEFI 加载引导加载程序（如 GRUB）后，内核被加载到内存中。  
   - 内核初始化时，会建立 VFS（虚拟文件系统）核心数据结构，如 dentry 哈希表和 inode 哈希表，并加载必要的文件系统驱动（ext4 驱动可以是内置的或作为模块加载）。

2. **挂载根文件系统**  
   - 内核根据启动参数（例如 root=/dev/sda1）确定根文件系统所在的设备。  
   - 调用函数如 **vfs_kern_mount()**（进一步调用 ext4_mount()）来挂载 ext4 文件系统。  
   - 在挂载过程中，ext4 驱动会读取设备上的超级块（superblock），验证 ext4 的魔数（magic number）和其它元数据，同时初始化 inode 表、日志（journal）等内部数据结构。  
   - ext4 文件系统的根目录在挂载时根据约定总是分配 inode 号 2。

3. **切换根文件系统（pivot_root/switch_root）**  
   - 如果系统使用了 initrd 或 initramfs 作为临时根文件系统，内核会在加载完真正的 ext4 文件系统后，通过 pivot_root 或 switch_root 将根目录切换到挂载的 ext4 文件系统上。
   - 此时，init（或 systemd）进程启动，用户空间接管并继续后续的初始化工作。

这样，整个过程确保了 ext4 文件系统的元数据和数据块被正确加载，并最终成为系统的根文件系统，使用户和应用程序可以正常访问文件数据。


#### 作业：为什么不同挂载点的inode号码都是2
```
[root@watchpoints ~]# lsblk
NAME   MAJ:MIN RM SIZE RO TYPE MOUNTPOINT
loop0    7:0    0   1G  0 loop /mnt/icfs
sr0     11:0    1  19M  0 rom
vda    253:0    0  50G  0 disk
└─vda1 253:1    0  50G  0 part /
[root@watchpoints ~]# ls -id /
2 /
[root@watchpoints ~]# ls -id /mnt/icfs
2 /mnt/icfs

同理

我们有3个不同的文件系统分别挂载到了3个不同的挂载点(目录)。
EXT4格式的文件系统我们分了3个，
分别挂载了/，
/boot，
/home。

那么这里的了/，/boot，/home就是3个不同的挂载点。

```


https://github.com/torvalds/linux/blob/master/fs/ext4/ext4.h

```c
/*
 * Special inodes numbers
 */
#define	EXT4_BAD_INO		 1	/* Bad blocks inode */
#define EXT4_ROOT_INO		 2	/* Root inode */
#define EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#define EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#define EXT4_BOOT_LOADER_INO	 5	/* Boot loader inode */
#define EXT4_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#define EXT4_RESIZE_INO		 7	/* Reserved group descriptors inode */
#define EXT4_JOURNAL_INO	 8	/* Journal inode */
```

## 二、提问：Linux 文件系统是怎么工作的？


![](https://files.mdnice.com/user/5197/fbb9cc2e-e131-4b9b-96b5-d8b36b4666be.png)


通过这张图，你可以看到，在 VFS 的下方，Linux 支持各种各样的文件系统，如 Ext4、XFS、NFS 等等。按照存储位置的不同，

这些文件系统可以分为三类。

- 类是基于磁盘的文件系统，也就是把数据直接存储在计算机本地挂载的磁盘中。
 见的 Ext4、XFS、OverlayFS 等，都是这类文件系统。

- 类是基于内存的文件系统，也就是我们常说的虚拟文件系统。

 
 - 类是网络文件系统，
 
 也就是用来访问其他计算机数据的文件系统，比如 NFS、SMB、iSCSI 等。
 
 网络文件系统 NFS：首次突破内核态
 
 随着计算需求的增长，单台计算机的性能逐渐无法满足日益增长的计算和存储要求。
 
 人们开始引入多台计算机，以分担负载并提高整体效率。

在这一场景下，一个应用程序往往需要访问分布在多台计算机上的数据。

为了解决这一问题，人们提出了在网络中引入虚拟存储层的概念，将远程计算机的文件系统（如某个目录）通过网络接口挂载到本地计算机的节点上。

这样做的目的是使本地计算机能够无缝地访问远程计算机的数据，就好像这些数据存储在本地一样。


![](https://files.mdnice.com/user/5197/a9c943c9-78b6-441a-ad56-12c5d1d28df4.png)

![](https://files.mdnice.com/user/5197/fa006f13-edfb-4e80-94f1-183aa24b56e9.png)

linux分析利刃之sar命令详解

参考：
https://www.cnblogs.com/zsql/p/11628766.html
 sar -n DEV 1 1#统计网络信息  这个有用带宽统计

查看进行哪些线程



#### 文件系统的格式
来源：
https://zh.wikipedia.org/wiki/Ext4

第四代扩展文件系统（英语：Fourth extended filesystem，缩写为ext4）是Linux系统下的日志文件系统


文件系统(六)：一文看懂linux ext4文件系统工作原理

ext4它突出的特点有：数据分段管理、多块分配、延迟分配、持久预分配、日志校验、支持更大的文件系统和文件大小。

ext4文件系统的具体实现比较复杂，本文尝试用比较简单的方式用一篇文章的篇幅来简单地介绍一下它的工作原理。


命令：dumpe2fs /dev/loop0


![](https://files.mdnice.com/user/5197/c07c56ac-f09c-4c06-aa8b-8cffdc03de4f.png)

![](https://files.mdnice.com/user/5197/7f401eb1-0b91-4867-b6b6-d07f7eb6fe3b.png)


![](https://files.mdnice.com/user/5197/94061f23-39f7-464f-bd01-dce0d673ee66.png)


从上面dumpe2fs的数据上我们可以看出，一个1GB大小的空间，ext4 文件系统将它分隔成了0~7的8个Group。


其中，每个Group中又有superblock、Group descriptors、bitmap、Inode table、usrer data、还有一些保留空间，细分之后的空间布局如下：
ext4 的总体磁盘布局如下：



![](https://files.mdnice.com/user/5197/34cf07ac-24f6-432d-a0ac-92160e5e6067.png)


具体含义内：
ext4文件系统信息表

从上面《1.1 ext4文件系统信息表》中可以知道Primary superblock在第0号block，每个block的大小为4096Byte

文件系统信息、块大小和块组信息、Inode 相关信息、文件系统大小和使用情况、日志相关信息、挂载信息、校验和和备份信息。


这里面我还需要重点说一下，超级块和块组描述符表都是全局信息，而且这些数据很重要。


如果这些数据丢失了，整个文件系统都打不开了，这比一个文件的一个块损坏更严重。所以，这两部分我们都需要备份，但是采取不同的策略。默认情况下，超级块和块组描述符表都有副本保存在每一个块组里面


其实使用dumpe2fs命令查看的ext4文件系统信息就是从superblock上的数据解析而来。

除了Primary superblock，还在不同的group中有备份superblock，其内容与Primary superblock原始数据相同，Primary superblock损坏的时候可以从备份区恢复回来

![](https://files.mdnice.com/user/5197/e0275fef-c23a-4b7d-978d-9c32ca904527.png)



回顾：

在 Linux 内核挂载 ext4 文件系统时，挂载流程大致如下：

1. **系统调用 mount() 启动挂载流程**  
   当用户或内核启动参数（如 root=/dev/sda1）触发挂载时，会调用通用的 mount 系统调用，进而进入 VFS 层的挂载流程。

2. **进入 ext4 挂载入口 ext4_mount()**  
   ext4 文件系统在内核中注册为一种文件系统类型，其挂载入口函数通常为 ext4_mount()（定义在 fs/ext4/super.c 中）。  
   在该函数中，会调用 ext4_fill_super()，用来读取磁盘上的超级块（superblock）并验证 ext4 文件系统的完整性。

3. **解析超级块与加载根 inode**  
   ext4_fill_super() 会从设备上读取超级块数据，然后填充超级块结构体。在超级块中保存了文件系统全局信息，其中就包括 ext4 文件系统的根目录 inode 的固定编号（EXT4_ROOT_INO 定义为 2，见 fs/ext4/ext4.h）。  
   紧接着，内核通过 ext4_iget()（或相关函数）加载 inode 2，这个 inode 就代表了文件系统的根目录。

4. **构造根目录 dentry 并完成挂载**  
   加载根 inode 后，内核会创建相应的 dentry（目录项），并将该根 dentry 作为挂载点的根，最终将整个文件系统挂载为根目录（/）。

相关源码主要分布在内核源代码的 fs/ext4/ 目录下，具体可以查看 ext4_mount()、ext4_fill_super() 以及 ext4_iget() 等函数。  




#### ext4 文件系统特点的

- 带着问题去 阅读：ext4文件系统单个文件存储大小是多少？4TB 还是16TB

![](https://files.mdnice.com/user/5197/6b247a88-b726-45be-9650-a112f9ce3ec9.png)
> Linux的ext4文件系统，为什么inode里只有12个指向数据块的直接指针？

- Ext4只有12个指向数据块的直接指针是沿用的Ext3的设计，为了保证前向兼容。对于Ext4文件系统来说，12个直接块可以解决大部分小文件的场景。如果文件太大，通过直接块的方式元数据将会占用太多空间。

- EXT3 适合中小规模存储，不适合超大文件（>2TB）

```c++


代码位置：
- https://github.com/torvalds/linux/blob/master/fs/ext4/ext4.h#L771


struct ext4_inode {
  __le32  i_block[EXT4_N_BLOCKS];/* Pointers to blocks */ n=15
};

/*
 * Constants relative to the data blocks
 */
#define	EXT4_NDIR_BLOCKS		12
#define	EXT4_IND_BLOCK			EXT4_NDIR_BLOCKS
#define	EXT4_DIND_BLOCK			(EXT4_IND_BLOCK + 1)
#define	EXT4_N_BLOCKS			(EXT4_TIND_BLOCK + 1) //14
```


具体如何保存的呢？

- EXT4_N_BLOCKS 有如下的定义，计算下来一共有 15 项。
- 在 ext2 和 ext3 中，其中前 12 项直接保存了块的位置，也
  就是说，我们可以通过 i_block[0-11]，直接得到保存文件内容的块。

- 如果一个文件比较大，12 块放不下 i_block[12]指向一个块，这个块里面不放数据块，而是放数据块的位置，
 这个块我们称为间接块。

也就是说，我们在 i_block[12]里面放间接块的位置，通过 i_block[12]找到间接块后，间接块里面放数据块的位置，通过间接块可以找到数据块。


- 如果文件再大一些，i_block[13]会指向一个块，我们可以用二次间接块【指针的指针】
- 如果文件再大一些，i_block[14]会指向三次间接块

![](https://files.mdnice.com/user/5197/8859f9aa-96ce-4c66-a651-21027837e477.png)


EXT4 文件系统采用 **索引节点（inode）结构** 来管理文件，其中 **inode 结构中包含 15 个指针**，用于定位文件数据块。  

### **EXT4 Inode 结构中的 15 个指针**
| 指针类型 | 数量 | 说明 |
|---------|------|------|
| **直接块指针（Direct Block）** | 12 | 直接指向数据块，每个指针可寻址 1 个数据块。 |
| **一次间接指针（Singly Indirect）** | 1 | 指向一个块，该块内存储多个直接块指针。 |
| **二次间接指针（Doubly Indirect）** | 1 | 指向一个块，该块内的指针指向多个一次间接块。 |
| **三次间接指针（Triply Indirect）** | 1 | 指向一个块，该块内的指针指向多个二次间接块。 |

---

### **单个文件最大支持大小计算**
假设 **块大小（block size）为 4KB**（EXT4 常用设置），计算单个文件最大大小：

1. **直接块存储数据**：
   - 12 个直接指针 × 4KB = **48KB**

2. **一次间接块存储数据**：
   - 1 个指针块存储 1024 个指针（4KB / 4B = 1024）
   - 1024 × 4KB = **4MB**

3. **二次间接块存储数据**：
   - 1 个指针块存储 1024 个一次间接指针
   - 1024 × 4MB = **4GB**

4. **三次间接块存储数据**：
   - 1 个指针块存储 1024 个二次间接指针
   - 1024 × 4GB = **4TB**

---

### **最终计算（最大单个文件大小）**
- **直接块**：48KB  
- **一次间接块**：4MB  
- **二次间接块**：4GB  
- **三次间接块**：4TB  
- **总计** ≈ **4TB + 4GB + 4MB + 48KB ≈ 4TB**

---

### **结论**
- **EXT4 单个文件最大可达 16TB**（如果启用了 **更大的 block size，比如 64KB**）。
- **在 4KB 块大小下，单个文件最大 ≈ 4TB**（由于索引结构限制）。
- **相比 EXT3（单文件最大 2TB），EXT4 更适合大文件存储**。

如果需要存储超过 4TB 的单个文件，可以：
1. **使用更大的块大小（如 64KB，最大 16TB）**。
2. **考虑使用其他文件系统（如 XFS、Btrfs、ZFS）**。


对比：


![](https://files.mdnice.com/user/5197/ffc8e407-cf9b-4deb-a3d5-ea75bfb10930.png)



>带着问题去 阅读：ext4如何存储大文件的，遇得什么麻烦

>对于大文件来讲，我们要多次读取硬盘才能找到相应的块，这样访问速度就会比较慢。

为了解决这个问题，ext4 做了一定的改变。
它引入了一个新的概念，叫做 Extents。

我们来解释一下 Extents。

比方说，一个文件大小为 128M，如果使用 4k 大小的块进行存储，需要 32k 个块。

如果按照 ext2 或者 ext3 那样散着放，数量太大了。

但是 Extents 可以用于存放连续的块，也就是说，我们可以把 128M 放在一个 Extents 里面。

这样的话，对大文件的读写性能提高了，文件碎片也减少了。

![](https://files.mdnice.com/user/5197/716e46fc-c67c-4067-91fc-119a343cf696.png)

除了根节点，其他的节点都保存在一个块4k里面，4k扣除ext4_extent_header的12个byte，剩下的能够放340项，每个extent最大能表示128MB的数据，340个extent会使你的表示的文件达到42.5GB。


## 三、ext4文件系统为例子 cat /mnt/icfs/fileA 判断文件fileA是否存在


✅前置条件：

在 Linux 中，文件系统（如 ext4）通常只能在块设备（block device）上创建和操作，例如：

- 物理磁盘 (/dev/sda, /dev/nvme0n1)

- 磁盘分区 (/dev/sda1, /dev/nvme0n1p1)

- 虚拟块设备（如 LVM 逻辑卷 /dev/mapper/vg-lv）

如果 没有额外的磁盘，但你仍然需要：
👉 你可以使用 Loop 设备（/dev/loopX）将普通文件当作块设备

📌 例子：用 Loop 设备模拟一个 ext4 文件系统

```shell
# 1. 创建一个 1GB 的普通文件文件
dd if=/dev/zero of=/root/temp/virtual_disk.img bs=1M count=1024
//bs 块大小为bytes个字节
# 2. 绑定到 Loop 设备（让 Linux 认为它是一个磁盘）
losetup /dev/loop0 /root/temp/virtual_disk.img
//loop 设备是一种伪设备(pseudo-device)

ls -l /dev/loop0
brw-rw---- 1 root disk 7, 0 3月  24 11:25 /dev/loop0

要从 ls -l 的结果的第一位标识位看出来。
- 表示普通文件；
d 表示文件夹；
c 表示字符设备文件，
b 表示块设备文件，
s 表示套接字 socket 文件，
l 表示符号链接

# 3. 在 /dev/loop0 上创建 ext4 文件系统
mkfs.ext4 /dev/loop0
//mkfs.ext4命令来自英文词组make filesystem Ext4的缩写，其功能是对磁盘设备进行EXT4格式化操作。

创建含有 262144 个块（每块 4k）和 65536 个inode的文件系统



# 4. 挂载到 /mnt 目录
mkdir /mnt/icfs
mount /dev/loop0 /mnt/icfs
//mount - mount a filesystem
# 5. 检查挂载情况
df -h | grep mydisk
ls /mnt/mydisk

```



# 提问 cat /mnt/icfs/file 查找过程是什么

![](https://files.mdnice.com/user/5197/a6d61712-9f04-467a-8ee4-37e7bac4a0c6.jpg)

准备：

strace 作为一种动态跟踪工具，strace 底层使用内核的 ptrace 特性来实现其功能

，ptrace可以让一个进程监视和控制另一个进程的执行,并且修改被监视进程的内存、寄器等,主要应用于断点调试和系统调用跟踪，strace和gdb工具就是基于ptrace编写的


![](https://files.mdnice.com/user/5197/6a8c4d1e-a595-427a-b875-1b6254029fad.png)



Open 系统调用说起


![](https://files.mdnice.com/user/5197/6ef7cdf2-0aa3-4cf7-942d-044dc5c3fdbc.jpg)

这张图十分重要，一定要掌握。因为我们后面的字符设备、块设备、管道、进程间通信、网络等等，全部都要用到这里面的知识。
希望当你再次遇到它的时候，能够马上说出各个数据结构之间的关系





# 从mount到fuse客户端


## 1. FUSE 是什么？

Linux内核官方文档对 FUSE 的解释如下：

What is FUSE?
FUSE is a userspace filesystem framework. 
It consists of a kernel module (fuse.ko), 
a userspace library (libfuse.*)
and a mount utility (fusermount).

思考问题：内核的 fuse.ko 模块，还有 libfuse 库。这两个角色是的作用？

这两个模块一个位于内核，一个位于用户态，是配套使用的，最核心的功能是协议封装和解析。

举给例子，内核 fuse.ko 用于承接 vfs 下来的 io 请求，然后封装成 FUSE 数据包，转发给用户态。

这个时候，用户态文件系统收到这个 FUSE 数据包，它如果想要看懂这个数据包，就必须实现一套 FUSE 协议的代码，这套代码是公开透明的，属于 FUSE 框架的公共的代码

，这种代码不能够让所有的用户文件系统都重复实现一遍，于是 libfuse 用户库就诞生了。

划重点：FUSE 是一个用来实现用户态文件系统的框架

用户态文件系统是区别于内核文件系统的，在用户态文件系统没有出现之前，
常见的文件系统如Ext2、Ext4等都是在内核中直接实现的。


![](https://files.mdnice.com/user/5197/66b3bd0e-c3aa-4d2c-8355-2845c87e5438.png)
通过这张图，你可以看到，在 VFS 的下方，Linux 支持各种各样的文件系统，如 Ext4、XFS、NFS 等等。按照存储位置的不同，

这些文件系统可以分为三类。

- 类是基于磁盘的文件系统，也就是把数据直接存储在计算机本地挂载的磁盘中。
 见的 Ext4、XFS、OverlayFS 等，都是这类文件系统。


- 类是基于内存的文件系统，也就是我们常说的虚拟文件系统。
 类文件系统，不需要任何磁盘分配存储空间，但会占用内存。我们经常用到的 /proc 文件系统
 
 - 类是网络文件系统，
 
 也就是用来访问其他计算机数据的文件系统，比如 NFS、SMB、iSCSI 等。




## 2. FUSE 原理是什么

代码：
https://github.com/libfuse/libfuse/blob/master/example/hello.c

![](https://files.mdnice.com/user/5197/5f0ee6c7-ccb5-4ac3-ac01-1f503535d0f4.png)

![](https://files.mdnice.com/user/5197/7a5d505b-3b65-475d-b4ef-a8f1ef0a4cb8.png)

![](https://files.mdnice.com/user/5197/4b730f9d-ae88-4267-a03d-dcbf6a2126f2.png)

这个图的意思是：

背景：一个用户态文件系统，挂载点为 /tmp/fuse ，用户二进制程序文件为 ./hello ；

当执行 ls -l /tmp/fuse 命令的时候，

流程如下：
IO 请求先进内核，经 vfs 传递给内核 FUSE 文件系统模块；

内核 FUSE 模块把请求发给到用户态，由 ./hello 程序接收并且处理。

处理完成之后，响应原路返回；


划重点：

实现了 FUSE 的用户态文件系统有非常多的例子，比如，GlusterFS，SSHFS，CephFS，Lustre，GmailFS，EncFS，S3FS


### 应用：JuiceFS ：FUSE 用户态分布式文件系统

收益：

FUSE 通过将用户空间与内核空间解耦，为开发者提供了在用户空间实现文件系统的巨大灵活性和便利性。

特别是在云计算和分布式存储等现代计算环境中，FUSE 使得构建和维护复杂的存储系统变得更加高效、可定制和易于扩展。


![](https://files.mdnice.com/user/5197/8ec2aabf-968e-44a2-8954-d3d1c7d42c07.png)



以用户挂载 JuiceFS 后，open 其中一个文件的流程为例。

请求首先通过内核 VFS，然后传递给内核的 FUSE 模块，经过 /dev/fuse 设备与 JuiceFS 的客户端进程通信。



具体步骤如下：

当 JuiceFS mount 后，JuiceFS 内部的 go-fuse 模块会 open /dev/fuse 获取 mount fd，并启动几个线程读取内核的 FUSE 请求;

具体步骤如下：
1. JuiceFS mount 后，JuiceFS 内部的 go-fuse 模块会 open /dev/fuse 获取 mount fd，并启动几个线程读取内核的 FUSE 请求;
2. 用 open 函数，通过 C 库和系统调用进入 VFS 层，VFS 层再将请求转给内核的 FUSE 模块；
3. FUSE 模块根据协议将 open 请求放入 JuiceFS mount 的 fd 对应的队列中，并唤醒 go-fuse 的读请求线程，等待处理结果；
4. 的 go-fuse 模块读取 FUSE 请求并在解析请求后调用 JuiceFS 的对应实现；
5. fuse 将本次请求的处理结果写入 mount fd，即放入 FUSE 结果队列，然后唤醒业务等待线程；
6. 程被唤醒，得到本次请求的处理结果，然后再返回到上层。

## 应用： 将 Ceph 文件系统挂载为 FUSE 客户端



参考代码：
src\ceph_fuse.cc


基本用法：


![](https://files.mdnice.com/user/5197/d463bcec-c11e-4575-ae58-93fd192e6576.png)


![](https://files.mdnice.com/user/5197/c490b41a-99fd-43c8-8e3f-ec1ea40b9391.png)


- 用户可以用 Ceph 提供的 ceph-fuse 挂载 CephFS 进行访问


ceph-fuse处理IO流程

举个例子 lookup /fuse，从lookup /fuse到ceph-fuse的流程图如下



![](https://files.mdnice.com/user/5197/092aa4df-4505-473d-87e5-f080edc0cd67.png)

MDS返回结果给ceph-fuse进程。从ceph-fuse到lookup /fuse的流程图如下
![](https://files.mdnice.com/user/5197/d0d58e74-e600-4f90-b210-d88da39b24d9.png)


```cpp
const static struct fuse_lowlevel_ops fuse_ll_oper = {
 init: do_init,
 destroy: 0,
 lookup: fuse_ll_lookup,
 forget: fuse_ll_forget,
 getattr: fuse_ll_getattr,
 setattr: fuse_ll_setattr,
 readlink: fuse_ll_readlink,
 mknod: fuse_ll_mknod,
 mkdir: fuse_ll_mkdir,
 unlink: fuse_ll_unlink,
 rmdir: fuse_ll_rmdir,
 symlink: fuse_ll_symlink,
 rename: fuse_ll_rename,
 link: fuse_ll_link,
 open: fuse_ll_open,
 read: fuse_ll_read,
 write: fuse_ll_write,
 flush: fuse_ll_flush,
 release: fuse_ll_release,
 fsync: fuse_ll_fsync,
 opendir: fuse_ll_opendir,
 readdir: fuse_ll_readdir,
 releasedir: fuse_ll_releasedir,
 fsyncdir: fuse_ll_fsyncdir,
 statfs: fuse_ll_statfs,
 setxattr: fuse_ll_setxattr,

};

```


cephfs：用户态客户端write

从fuse到cephfs客户端的函数流程如下

![](https://files.mdnice.com/user/5197/93b6c341-6830-4800-b318-1d5ca4f720e1.png)

- 例子 mkdir /mnt/ceph-fuse/test

mkdir就是创建目录，客户端并不直接创建目录，而是将mkdir的请求（op为CEPH_MDS_OP_MKDIR）发给MDS，然后MDS执行mkdir的操作，并返回创建的目录的元数据。

客户端无非就是发送请求和处理回复。

![](https://files.mdnice.com/user/5197/71ba2ae4-b1da-45a7-9424-d9b968adbbee.png)

cephfs：用户态客户端getattr（1）

从libfuse到cephfs用户态客户端的过程
![](https://files.mdnice.com/user/5197/f30ded85-ee8f-49c6-9df3-2be393da0765.png)

cephfs用户态客户端是从fuse_ll_getattr开始，fuse_ll_getattr的作用是通过cfuse->client->ll_getattr获取inode，填充struct stat信息，并返回该stat信息。

![](https://files.mdnice.com/user/5197/df48c1da-843f-4776-a009-589f2c22df64.png)

cephfs：用户态客户端getattr（2）

客户端获得的inode来自于MDS（元数据服务器），所以当客户端缓存的某个inode需要更新时，会向MDS发送op为CEPH_MDS_OP_GETATTR的请求。MDS回复的消息中带有最新的inode数据。这种情况下，就得了解发请求和处理请求的流程。


![](https://files.mdnice.com/user/5197/d2a2413b-5534-42ea-90bf-01fbd7c55dc1.png)


划重点：
- 将 Ceph 文件系统挂载为内核客户端 这里不讨论 


# 提问：s3fs-fuse


![](https://files.mdnice.com/user/5197/1f8001d8-4526-4b05-a9e2-c3f2647253fb.png)

s3fs-fuse 是一个基于 FUSE（Filesystem in Userspace）的文件系统，允许 Linux、macOS 和 FreeBSD 系统通过 FUSE 挂载 Amazon S3 存储桶。

通过 s3fs-fuse，用户可以像操作本地文件系统一样操作 S3 存储桶中的文件和目录。该项目的主要编程语言是 C++。


为什么使用fuse --小团队开发 简单为主 


![](https://files.mdnice.com/user/5197/b0079726-1c01-40b3-a437-089de50026c0.png)




## 参考【想要电子书可以私信】

- 【1】 Linux 性能优化实战-- IO章节


![](https://files.mdnice.com/user/5197/356a4bf3-9b8f-4308-bb0a-f508f4910c62.png)

- 【2】趣谈 Linux 操作系统 核心原理篇：第五部分 文件系统 (4讲)

![](https://files.mdnice.com/user/5197/325d15f8-994b-47b3-8aa9-7b07cff208f7.png)


- [3] 鸟哥的Linux私房菜 基础学习篇 第四版

![](https://files.mdnice.com/user/5197/ad54163e-1235-49cc-ad3c-432170ef657d.png)

- 【4】

 https://www.kernel.org/doc/html/latest/filesystems/fuse.html


- 【5】公共号 奇伢
https://www.qiyacloud.cn/

https://www.cnblogs.com/liwen01/p/18237062

- [6] JuiceFS 是一款面向云原生设计的高性能分布式文件系统
https://juicefs.com/zh-cn/blog/engineering/fuse-file-system-design

https://juicefs.com/zh-cn/blog/engineering/deepseek-3fs-vs-juicefs

-[7] ceph
- https://docs.ceph.com/en/quincy/radosgw/nfs/

- DeepSeek 3FS 源码解读——客户端篇


- [8] 3FS
https://www.high-flyer.cn/blog/3fs-2/

https://zhuanlan.zhihu.com/p/27742193940


DeepSeek 3FS 源码解读——协程&RDMA篇

DeepSeek 3FS 源码解读——磁盘 IO 篇

## 总结 

-  文件系统的根目录的indoe号是多少？为什么
- 文件系统单个文件最大存储文件数量？为什么
- 大文件如何实现高效查询，采用数据数据结构？

