---
title: 2023秋冬OS训练营第二阶段总结-watchpoints
date: 2023-11-04 14:15:53
categories:
	- report
tags:
  - author: watchpoints
  - repo: https://github.com/LearningOS/2023a-rcore-watchpoints
---

## 写在前面

作为上班族，每天下班很晚了，本来没有多余时间，

看到 从零开始用 **Rust** 语言写一个基于 **RISC-V** 架构的 类 Unix 内核这样标题 打退堂鼓。

> 转你一想 这就是自己本来期望形式，参与开源方向，并且数据库，操作系统都 作为软件工程师 基本功，不管结果如何就参与了.



## 一阶段总结

- 选择看什么资料，根据看c++经验，一定选择英文原版 代替看中文翻译

  

## 二阶段总结

二阶段对我而言几乎是全新的知识。以学习为目的，尽量在理解清楚的基础上做题。题目的完成也以通过测试为目的，未考虑错误处理等等细节。

### ch1 & ch2

这两章从应用执行环境说起，引入了程序执行环境和特权级切换这两个基础概念。第二章的批处理系统和后续的操作系统，本身的角色就是在硬件和应用直接充当桥梁的角色。用 M 态抽象硬件的能力，用 S 态提供接口，供 U 态的应用调用。

开始阶段研究了一下实验环境在我的 Gentoo Linux 下执行的可能性，做过一些改动来适配。事实证明，直到目前为止（ ch5 ）都能正常运行。具体过程记录在 [我的准备工作](https://blog.windeye.tk/blog/hubofoskernel2023a2/) 和 [【腾讯文档】Qemu常见问题Q&A](https://docs.qq.com/doc/DRGVsdFZZek9lVFhT) 的第 2 ~ 4 个问题。

### ch3

题目要求：实现 sys_task_info 系统调用，统计任务信息：包括状态、当前任务所用到的系统调用ID及对应的调用次数，任务开始到目前为止的时长（只考虑开始到现在的时长）

#### 思路

1. task只会以两种方式开始执行，run_first_task 或run_next_task，只在一个地方退出：mark_current_exited。所以在任务开始时保存当时的时间，退出的时候再取一次时间，二者的差就是当前任务的运行时长。
2. 系统调用最终都是通过 sys_call 进行调用，它的入参就有该次调用的 syscall_id 只需要在这里统计当前任务的系统调用 ID和次数即可。

#### 小结

具体实现时为了不影响正在执行的任务，直接从当前任务的 TaskInfo 中克隆了它的副本来计算目前为止的运行时间，应该可以有更优的方式。

### ch4

个人觉得是前三个里面最难的，内存管理的部分非常抽象并有一定复杂度。主要是实现 map 和 unmap 功能，能让应用正常申请释放内存。

#### 思路

1. map 时需要遍历当前 task 的 MemorySet.areas，从而判定需要分配的虚拟地址范围 start ~ start + len 有没有跟 areas 里面的 某个 area 有重叠，如果没有，才能创建新的 MapArea，并将它推入 MemorySet.areas。 最后这步不能忘记！

2. 因为 map 的机制，areas 中不会出现部分重叠的区域，所以 unmap 的时候，找到的 area 的 范围和 start ~ start + len 一定是重合的。还有就是记得要将它从 areas 中移除。
3. map 和 unmap ，start ~ start + len 覆盖的虚拟和物理地址页面都可能不止一个

#### 小结

理解映射关系花了很多时间和心思，最后总结的结果是：map 时，经过一系列检查，先创建了一个 area，此时它的 VPNRange 是 start ~ start + len， 但 data_frames 还是空的，直到我们将它 push 到 MemorySet.areas 时，在 push 函数的实现里，才为每个vpn 分配了一个 ppn ，并将二者的映射插入到 data_frames 中，完成映射。unmap 时我采取的方法是直接遍历 area.vpn_range, 将 vpn 和 ppn 的映射关系断开，然后从 MemorySet.areas 中去掉当前的area，被分配的物理节点会在映射断开时自动释放，area 在从 areas 中移除后，也会自动被释放，从而完成 unmap 操作。

### ch5

相对ch4， 这个让人稍微能缓缓。

#### 思路

spawn 的实现，在提示里面已经给出了，可以参考 fork + exec。区别就是建立 MemorySet 时，改成从 path 读取，最后按照 exec 的方式来设置 trap_cx。

stride 调度算法仅仅只是过关。实现了当 priority 变化时，重新 设置当前任务的 stride。 在 task 管理中，添加任务时根据 stride 添加到适当的位置。当 pop_front 取任务时，自然就取到最小的。没有考虑溢出或其他情况。

#### 小结

Stride 考虑溢出的话会更公平，调度队列考虑使用 BtreeMap，利用它的天然有序性，以 stride 为 key，应该表现会更好。

### 未完待续
