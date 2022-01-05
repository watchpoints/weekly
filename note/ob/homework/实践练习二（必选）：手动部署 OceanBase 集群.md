
## 题目理解

https://open.oceanbase.com/answer/detail?id=13700696

- 练习目的

本次练习目的掌握 OceanBase 集群的手动部署技能 [思考 如何手工部署？]

理解进程的相关目录、集群初始化、OBProxy和 OB 集群关系等。



- 练习条件

 有笔记本或服务器，内存至少12G 。



- 练习内容

请记录并分享下列内容：

- （必选）手动部署一个 OB 单副本集群，包括一个 OBProxy 节点。
- （必选）创建一个业务租户、一个业务数据库，以及一些表等。

- （可选）如果单台服务器内存有32G，或者有三台服务器，改为部署一个 OB 三副本集群，包括一个 OBProxy 节点。


- （可选）如果有三台服务器并且服务器内存有 32 G，可以单服务器内启动 2 个节点，实现 1-1-1 扩容到 2-2-2 

题目理解

-  OBD 自动化部署 这个方式不是手工的。
-  机器配置低，用单节点模拟
-  扩容操作虽然不能操作，但是可以学习理论

## 环境配置

说明：

1. 这是一次失败尝试 ，我只能在2c 2G内存 机器上尝试 手工部署，但是依然可以学习理论知识

![image.png](https://s2.loli.net/2021/12/30/Tp9eCfdyKYA1gG2.png)





## 第一步：初始目录结构

~~~
mkdir -p ~/oceanbase/store/obdemo /data/obdemo/{sstable,etc3}     /redo/obdemo/{clog,ilog,slog,etc2}
for f in {clog,ilog,slog,etc2}; do ln -s /redo/obdemo/$f ~/oceanbase/store/obdemo/$f ; done
for f in {sstable,etc3}; do ln -s /data/obdemo/$f ~/oceanbase/store/obdemo/$f; done 

tree ~/oceanbase/store/ /data/ /redo/

~~~

## 参考

- https://www.modb.pro/db/182982
- https://open.oceanbase.com/answer/detail?id=13700373
- 2-4-如何手动部署OceanBase三副本集群


