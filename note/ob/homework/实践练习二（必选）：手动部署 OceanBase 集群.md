
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



### 创建 admin用户

OceanBase建议使用admin进行维护，创建admin用户。

[root ~] # useradd admin
[root ~] # passwd admin # 修改admin用户密码

给予admin用户sudo root权限：

[root ~] # cd /etc/sudoers.d/
# 创建admin(文件可随意),并写入如下内容：
[root ~] # more admin
%admin ALL=(ALL) NOPASSWD: AL




## 1. 目录结构说明

- 命令 
~~~
mkdir -p ~/oceanbase/store/obdemo /data/obdemo/{sstable,etc3}     /redo/obdemo/{clog,ilog,slog,etc2}
for f in {clog,ilog,slog,etc2}; do ln -s /redo/obdemo/$f ~/oceanbase/store/obdemo/$f ; done
for f in {sstable,etc3}; do ln -s /data/obdemo/$f ~/oceanbase/store/obdemo/$f; done 

tree ~/oceanbase/store/ /data/ /redo/



~~~

- 含义说明：

~~~
[root@localhost ~]# tree ~/oceanbase/store/ /data/ /redo/
/root/oceanbase/store/
└── obdemo 总数据目录
    ├── clog -> /redo/obdemo/clog 事务日志 （软连接）
    ├── etc2 -> /redo/obdemo/etc2
    ├── etc3 -> /data/obdemo/etc3
    ├── ilog -> /redo/obdemo/ilog 事务日志 （软连接）
    ├── slog -> /redo/obdemo/slog 事务日志 （软连接）
    └── sstable -> /data/obdemo/sstable  数据文件目录（软连接）
    
/data/
└── obdemo
    ├── etc3
    └── sstable
/redo/
└── obdemo
    ├── clog
    ├── etc2
    ├── ilog
    └── slog

~~~

![](https://files.mdnice.com/user/5197/383382a9-07d3-4155-8a00-734b1c1aa4b3.png)

![](https://files.mdnice.com/user/5197/d057fad9-fdb6-4702-a193-12cea27239b1.png)


> 小贴士

Sorted Strings Table (SSTable)：

1  [什么是SSTable 是内存里的数据吗？](https://niceaz.com/2018/11/27/sstable/#sstable-%E8%B5%B7%E6%BA%90)

2.  https://www.scylladb.com/glossary/sstable/ 

3. 5分钟让你明白“软链接”和“硬链接”的区别 硬链接原文件/链接文件公用一个inode号

## 2. 启动OB进程

- 2.1 准备

![](https://files.mdnice.com/user/5197/17bb5299-acfe-490d-ae2f-ec6acbf40168.png)

~~~

从https://www.oceanbase.com/softwareCenter/community下载3个软件，

分别是

obproxy-3.2.0-1.el7.x86_64.rpm、

oceanbase-ce-3.1.1-4.el7.x86_64.rpm、

oceanbase-ce-libs-3.1.1-4.el7.x86_64.rpm。


[root ~] # rpm -ivh oceanbase-ce-3.1.1-4.el7.x86_64.rpm
[root ~] # rpm -ivh oceanbase-ce-libs-3.1.1-4.el7.x86_64.rpm 
[root ~] # rpm -ivh obproxy-3.2.0-1.el7.x86_64.rpm


软件包默认安装目录是 ： /home/admin/oceanbase 
目录结构如下：


~~~


> 小贴士

- OBD是社区版OceanBase数据库部署工具 OceanBase Deployer（简称 OBD）
- OceanBase运行时所依赖的部分三方动态库
- OceanBase 社区版是一款开源分布式 HTAP数据库管理系统



~~~

echo ‘export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/oceanbase/lib’ >> ~/.bash_profile

/etc/sysctl.conf 
sysctl -p
/etc/security/limits.conf 

~~~


- 启动

~~~
cd /data/observer01/ 

/home/admin/oceanbase/bin/observer 

-r "127.0.0.1:2882"
-o __min_full_resource_pool_memory=268435456,memory_limit=8G,system_memory=4G,stack_size=512K,cpu_count=16,cache_wash_threshold=1G,workers_per_cpu_quota=10,schema_history_expire_time=1d,net_thread_count=4,sys_bkgd_migration_retry_num=3,minor_freeze_times=10,enable_separate_sys_clog=0,enable_merge_by_turn=False,datafile_size=50G,enable_syslog_recycle=True,max_syslog_file_count=10 -z zone1 -p 2881 -P 2882 -c 1 

-d /data/observer01/store -i eth0 -l INFO

#以上命令中指定的参数作用可参考obd自动部署过程yaml文件的说明，请移步 https://gitee.com/oceanbase/obdeploy/tree/master/example 
~~~


#以上命令中指定的参数作用可参考obd自动部署过程yaml文件的说明，请移步 https://gitee.com/oceanbase/obdeploy/tree/master/example 

[admin ~]$ ps -ef |grep observer # 查看进程信息

~~~

## 3 bootstrap集群



![](https://files.mdnice.com/user/5197/f4da41f5-9923-4811-a7fc-c0cee74e71aa.png)

![](https://files.mdnice.com/user/5197/07f08ac3-1f4c-4b3e-a0bd-3c9d1c5b5b79.png)


## 参考

- https://www.modb.pro/db/182982
- https://open.oceanbase.com/answer/detail?id=13700373
- 2-4-如何手动部署OceanBase三副本集群
- https://open.oceanbase.com/docs/tutorials/quickstart/V1.0.0/2-11-advanced-how-to-manually-deploy-an-oceanbase-cluster

【3】 https://open.oceanbase.com/answer/detail?id=13700373



