---
title: "源码解读第一天：raft 与Paxos"
date: 2022-01-12
description: "Tidb"
draft: false
tags: ["Tidb"]
---


# Raft 学习记录

# todo

task01：搭建tikv环境

-------------------------------------------------------------

### task01：搭建tikv环境（容器版本，和源码版本）



~~~
在CentOS上进行TiDB/PD/TIKV编译分享

路径：
umount /dev/nvme1n1
mkdir -p /data/kv
mkfs.ext4 /dev/nvme1n1
mount /dev/nvme1n1  /data/kv


cd /data/kv

cd tikv && make && mkdir -p ./bin && cp ./target/release/tikv-server bin

brew install rust
https://senzhangai.github.io/programming/build-tidb-by-source
https://tidb.net/blog/7e4c77a0


~~~

- 容器





# Paxos学习记录



# todo

# 参考资料

- [Paxos与Raft比较](https://blog.csdn.net/zhengchao1991/article/details/124648950)
- [High Performance TiDB 课程](https://zhuanlan.zhihu.com/p/179245036)
- http://localhost:1313/post/tidb/2022/tidb_01/
- https://github.com/watchpoints/mylabs
- Raft/Paxos类协议与分布式事务



## 环境搭建









## 参考

- Raft/Paxos类协议与分布式事务





