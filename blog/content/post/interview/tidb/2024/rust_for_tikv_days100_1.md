---
title: "源码解读第一天：raft 与Paxos"
date: 2024-05-29
description: "Tidb"
draft: false
tags: ["Tidb"]
---


# 

# todo01：搭建tikv环境

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





## tod02:学习资料



- 阅读文章：https://asktug.com/t/topic/693645

- 【源码合集】TiKV 源码阅读三部曲

  https://mp.weixin.qq.com/s/HgflwnZZMHXaIsFV3PdGAg

- 阅读  Rust 参考手册 中文版
  https://rustwiki.org/zh-CN/reference/expressions/if-expr.html

- TiKV 源码阅读三部曲（一）重要模块

本小节将简单介绍 KVService 及其启动流程，并顺带介绍 TiKV 若干重要结构的初始化流程

- TiKV 源码阅读三部曲（二）读流程
