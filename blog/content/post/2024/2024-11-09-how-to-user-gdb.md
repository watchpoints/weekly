---
title: GDB调试指南（1）
date: 2024-11-09
draft: false
tags:
  - GitHub
---
目前仅有管软节点的backend参与元数据同步，元数据同步可仿gpair独立创建落盘的对象，并且实现多节点backend同时承接不同的任务，由mds统一管理同时支持任务的自动分配、负载均衡。并且任意节点的backend都可以提供daemon命令支持对集群所有复制任务的管理操作
GDB（GNU Debugger）是UNIX及UNIX-like下的强大调试工具，可以调试ada, c, c++, asm, minimal, d, fortran, objective-c, go, java,pascal等语言。本文以C程序为例，介绍GDB启动调试的多种方式。


# gdb 常见问题100问


### Q3:为什么CXXLD执行需要很长时间？


### Q2： 如何判断一个可执行程序是否可用debug信息

- readelf -S helloWorld|grep debug

   readelf查看段信息
   
- file helloWorld

如果最后是stripped，则说明该文件的符号表信息和调试信息已被去除，

### Q1:GDB调试报错 Source file is more recent than executable | 符号表与源代码文件对应错误


定位过程：

搜索引擎优化 (SEO)

gdb Source file is more recent than executable site:https://stackoverflow.com/




## 联系我 ：

洞悉技术的本质，享受科技的乐趣

先完成10%目标，迈出100%之一行动


---

📚 **专长**：
- 🛠️ 擅长C++并发编程
- 💾 专注分布式存储，分布式数据库
- 🕒 时间的践行者，立志成为影响力讲师

---
🌱 **我能提供**：

1. 后端程序员的**简历优化+就业辅导+职业规划**
2. **全栈工程师**(C++, Rust, Go, Python)项目开发
3. 一年**12本书的践行记录**，分享知识与成长。


- **微信/ Github:** watchpoints
- **公众号:** 后端开发成长指南

#后端工程师 #分布式系统 #人生旅程 #职业成长

#学无止境 #技术分享 #职业规划 #开源贡献