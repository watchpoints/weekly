---
title: 源码解析：文件系统扩展属性操作(setfattr, getfattr)
date: 2024-03-28
description: you can
draft: false
tags:
  - CEPH
categories:
  - CEPH
---
小王提问：

ceph getfattr -nicfs.dir.rfiles  这个命令 如何获取一个目录下文件数量，给出参考来源 和原理分析，不清楚不要回答?

如果创建一个文件，如果引起目录下文件数量发生变化的
getfattr -n ceph.dir.rfiles /some/dir ceph 源码实现分析  如何lib调用mds 实现过程？

分类：
- CephFS文件存储、块存储、对象存储
-  文件系统扩展属性操作(setfattr, getfattr)

基本用法：

当你使用 `getfattr` 命令来获取 `icfs.dir.rfiles` 属性值时，CephFS 会返回目录下文件的数量。这个值可以用来快速获取目录下文件数量，而无需扫描整个目录。
https://docs.ceph.com/en/mimic/cephfs/file-layouts/
https://www.jianshu.com/p/9f7e7a372a5e
https://www.kernel.org/doc/html/v6.12/filesystems/ceph.html


getfattr -n ceph.dir.rfiles /some/dir ceph 源码实现分析 主要 如何lib调用mds。如果创建一个文件变成过程
https://metaso.cn/s/sReyxZY



提问：ceph.dir.rfiles 怎么统计文件个数的
https://metaso.cn/s/sReyxZY



提问：dir->num_local++; 没找到对应代码？
https://metaso.cn/s/sReyxZY



提问：
ceph.dir.rfiles如果统计不准确，涉及mds 快照，分片，快节点有关系吗？
https://metaso.cn/s/sReyxZY

### 总结

- **快照**：确保快照操作未干扰当前目录的统计逻辑。
- **分片**：检查分片状态和跨 MDS 同步机制。
- **缓存**：强制刷新元数据缓存或调整客户端超时设置。
- **临时条目**：通过 MDS 日志排查未完成的操作。
- **最终手段**：升级 Ceph 版本或提交 Issue（统计不准确可能是已知 Bug







-------------

方式：开日志，看这段逻辑，变化情况

目的：
getfattr -n icfs.dir.rfiles xxx 命令查询icfs.dir.rfiles 这个值是怎么计算的，存储在哪里？

- 如何增加的？
- 如何减少的？
- 如果是异步更新，随着时间推移慢慢回复正确

 **这个值是怎么计算的，存储在哪里？**

过程：
https://metaso.cn/s/sReyxZY

--------------------
什么场景下CDir::steal_dentry被调用？

`Dir::steal_dentry` 函数通常在以下场景中被调用：

1. **目录重平衡**：当目录树中某个目录变得过大或过小时，需要重平衡目录树以保持平衡。`steal_dentry` 函数被调用以将某个目录项从一个目录移动到另一个目录。
2. **目录碎片整理**：当目录树中出现碎片（即目录项不连续存储）时，需要整理目录碎片以提高存储效率。`steal_dentry` 函数被调用以将目录项从一个目录移动到另一个目录，以整理碎片。
3. **快照创建**：当创建快照时，需要将快照中的目录项从原来的目录移动到快照目录中。`steal_dentry` 函数被调用以实现这一步骤。
4. **目录迁移**：当需要将目录从一个存储设备迁移到另一个存储设备时，`steal_dentry` 函数被调用以将目录项从原来的目录移动到新的目录。
5. **文件系统检查和修复**：在文件系统检查和修复过程中，可能需要将目录项从一个目录移动到另一个目录以修复文件系统的不一致性。`steal_dentry` 函数被调用以实现这一步骤。

这些场景通常发生在分布式文件系统（如Ceph）中，需要高效地管理目录树和存储空间。`CDir::steal_dentry` 函数是实现这些场景的关键组成部分。


**catter WriteBehind**

Scatter WriteBehind是一种技术，用于提高分布式文件系统的性能，特别是在写操作频繁的场景下。

**什么是Scatter WriteBehind？**

Scatter WriteBehind是一种写缓存机制，允许多个客户端同时写入同一个文件或目录，而不需要等待之前的写操作完成。这种机制通过将写操作分散到多个节点上，减少了写操作的延迟和冲突。

**工作原理**

1. **写操作**: 当客户端发出写操作请求时，文件系统会将写操作分成多个小块（称为"scatter"），并将这些小块发送到多个节点（称为"scatter nodes"）。
2. **写缓存**: 每个scatter node都会缓存接收到的写操作小块，这些小块被称为"writebehind"缓存。
3. **异步写入**: scatter node会异步地将写操作小块写入到存储设备中，这样可以减少写操作的延迟。
4. **确认**: 当所有scatter node都确认写操作完成后，文件系统会返回写操作成功的响应给客户端。

**优点**

1. **提高写性能**: Scatter WriteBehind可以显著提高写性能，因为多个客户端可以同时写入同一个文件或目录。
2. **减少写延迟**: 通过异步写入和写缓存，Scatter WriteBehind可以减少写操作的延迟。
3. **提高并发性**: Scatter WriteBehind可以提高系统的并发性，因为多个客户端可以同时写入同一个文件或目录。

**应用场景**

4. **分布式文件系统**: Scatter WriteBehind特别适用于分布式文件系统，例如Ceph、HDFS等。
5. **云存储**: Scatter WriteBehind也可以应用于云存储系统，例如Amazon S3、Google Cloud Storage等。
6. **大数据处理**: Scatter WriteBehind可以用于大数据处理场景，例如Hadoop、Spark等

这个函数是 `MDCache::project_rstat_frag_to_inode`，它的作用是将一个文件或目录的碎片（frag）的状态信息（rstat）投影到其对应的inode上



这个函数是 Ceph 文件系统中的一个函数，叫做 `kick_off_scrubs`，它是 `ScrubStack` 类的一部分。这个函数的作用是启动或继续 scrubbing 过程。

Scrubbing 是一种文件系统检查和修复机制，用于检查文件系统中的数据完整性和一致性。这个函数会检查 scrubbing 栈中的任务，并根据 scrubbing 状态和配置参数启动或继续 scrubbing 过程。



# Ceph 集群 scrub 设置策略(数据一致性校验)
https://www.jianshu.com/p/a46f6d28167f

什么是 Scrub  
Scrub是 Ceph 集群副本进行数据扫描的操作，用于检测副本间数据的一致性，包括 scrub 和 deep-scrub。其中scrub 只对元数据信息进行扫描，相对比较快；而deep-scrub 不仅对元数据进行扫描，还会对存储的数据进行扫描，相对比较慢。

  
  
作者：Joncc  
链接：https://www.jianshu.com/p/a46f6d28167f  
来源：简书  
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

https://docs.ceph.com/en/latest/cephfs/scrub/