# 故事



你去面试 一个小公司，他们问你使用raft吗？我

回答没有 他们pass了你，我心里想 根本不可能其他公司用呀？

你被一个女的，问成这样

[raft 。布隆过滤器]

> 因此你从停留在看论文，概念 到开始使用。去使用，跨越这个一步









# 存储引擎

## 一、 第一关: https://github.com/tikv/tikv/issues/6231

1. 【High Performance TiDB】Lesson 10：存储引擎优化

- https://docs.qq.com/pdf/DUlVjVGtFdFdOd2pO?fromtype=pdf&confirmed=1 [ppt]

- https://www.bilibili.com/video/BV1of4y1B7Kz[视频]

  
  
- Data Block Hash Index https://github.com/facebook/rocksdb/wiki/Data-Block-Hash-Index

- ### Recommended Skills

   ~~~shell
   三十分钟成为 Contributor | 为 TiKV 添加 built-in 函数
   https://pingcap.com/zh/blog/30mins-become-contributor-of-tikv
   
   step1 Rust 语言环境
   brew install rust 【不对】
   
   brew uninstall --force rust
   
   rustup self uninstall
   curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
   
   https://www.rust-lang.org/zh-CN/tools/install
   
   rustup update 
   
   
   rustc --version
   rustc 1.17.0-nightly (ba7cf7cc5 2017-02-11)
   
   cargo --version
   cargo 1.55.0-nightly (d21c22870 2021-07-26)
   
   brew install cmake
   cmake -version
   cmake version 3.21.2
   Apple clang version 12.0.5 
   
   step2 tikv
   git clone git@github.com:watchpoints/tikv.git
   
   make build
   make dev
   make release
   
   
   ubunt
   
   
   
   ~~~

  

  小结：

  1. rust 版本安装不用brew install rust命令，采用默认的nightly版本 



- https://github.com/facebook/rocksdb/wiki

  

  

  https://github.com/facebook/rocksdb/wiki/RocksDB-Overview

  

  ![](https://user-images.githubusercontent.com/62277872/119747261-310fb300-be47-11eb-92c3-c11719fa8a0c.png)







### 为了补充基础 我阅读过的文档

1. 布隆过滤器(Bloom Filter)详解 https://www.cnblogs.com/xuwc/p/14017796.html
2. 1

## issue

Score：300

Description：Lock cf use hash index to speedup point lookup. For prewrite(first step of 2PC) stage, the key is not exist in lock cf usually, so we can use bloom filter to speedup this step, for commit(second step of 2PC) stage, the key is exist in lock cf usually, hash index will can improve the lookup speed.

Link: https://github.com/tikv/tikv/issues/6231





Score：3000

Description：Currently we query blob cache in BlobFileReader. It is done after getting file metadata (BlobStorage::FindFile) and getting the file reader from BlobFileCache, both of which require mutex lock. If we move blob cache query to before those two calls, we saves the two mutex lock and other overheads for a cache hit query.

Link: https://github.com/tikv/titan/issues/140





Score：8400

Description：Merge Titan blob cache with RocksDB block cache, and implement PrioritizedCache which caches block content in high pri pool and caches blob content in low pri pool.

Link: https://github.com/tikv/tikv/issues/5742









