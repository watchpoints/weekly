---
title: "成为ob贡献者第四天"
date: 2024-08-17
description: "自我驱动学习，专业上只能靠自己"
draft: false
categories: ["oceanbase"]
---





# #一、Overview 



大家好， 成为ob贡献者第四篇文章。
本文主要对 内核实战教程第二期🥰认识数据库存储结构  学习进行记录 

  

#### 很容易可以了解内容部分

![why](https://foruda.gitee.com/images/1723870034742173527/a4638a73_116887.png)

![输入图片说明](https://foruda.gitee.com/images/1723869879662270718/c8da996e_116887.png)



![输入图片说明](https://foruda.gitee.com/images/1723870167660139474/eda86b4e_116887.png)

![输入图片说明](https://foruda.gitee.com/images/1723870284298916902/f3c583a3_116887.png "屏幕截图")

![输入图片说明](https://foruda.gitee.com/images/1723870391285180745/a0b4a9be_116887.png "屏幕截图")



#### 稍微了解背景知识



![存储器层次](https://foruda.gitee.com/images/1723891872045910544/51185f6a_116887.png)

![存储器层次](https://foruda.gitee.com/images/1723891559756070759/d2e14f2d_116887.png)



> 深入理解计算机系统9.3章节 VM as a Tool for Caching



原文：

<div alt="card"> 
 In order to manage memory more efficiently and robustly,modern systems provide an abstraction of mainmemory known asvirtual memory (VM). Virtual memory is an elegant interaction of hardware exceptions,hardware address translation, main memory, disk files, and kernel software that provides each process with a large,uniform,and private address space
  Withone clean mechanism,virtual memory provides three important capabilities.(1)Itusesmain memoryefficientlybytreatingitasacacheforan addressspacestored on disk, keeping only the active areas in main memory, and transferring data back and forth betweendisk and memory as needed. 
(2) It simplifies memory management by providing each process with a uniformaddress space.
(3) It protects the address space of each process from corruption by other processes
</div>




<div alt="timeline">
    <div alt="timenode">
        <div alt="meta">2019.9 ~ 2022.6 <a href="#">家里蹲B</a></div>
        <div alt="body">
            人言落日是天涯，望极天涯不见家。
            已恨碧山相阻隔，碧山还被暮云遮。
        </div>
    </div>
    <div alt="timenode">
        <div alt="meta">2015.9 ~ 2019.6 <a href="#">家里蹲A</a></div>
        <div alt="body">
            看花开花落，谈笑风生...
        </div>
    </div>
</div>





虚拟内存作为谁的缓存工具？



虚拟内存被组织成存储在磁盘上的 N 个连续的字节单元数组, 

磁盘上数据缓存到内存上。





任何时刻，**虚拟页**被分为**三个不相交的子集**：

- **Unallocated**
  还没被 VM 系统创建的页，没有数据，**不占空间**
- **Cached**
  已缓存在**物理内存**中的**被分配**了的**页**
- **Uncached**
  被分配但**没缓存在物理内存**中的页



![fig 9.3](https://i-blog.csdnimg.cn/blog_migrate/e6456e98111f82d68d6a8b1bad1ae199.png)





# 二、 Solutions

![输入图片说明](https://foruda.gitee.com/images/1723878839811144466/2b2c5b80_116887.png)



#### 1 macOS 下使用 Docker 搭建 centos 开发环境
https://github.com/oceanbase/obdeploy


- 创建docker容器支持ssh登录
~~~
https://github.com/watchpoints/mydocker
docker pull oceanbase/centos7:latest

docker run -itd  -p 2881:2881 -p 22:22 --privileged=true  --name oceanbase-ce oceanbase/centos7 ///usr/sbin/init

# –privileged 和 /usr/sbin/init 很重要，必须要带上，否则后面启动ssh服务时会报错
# Failed to get D-Bus connection: Operation not permitted


docker ps             
默认情况下，docker ps 只显示正在运行的容器
CONTAINER ID   IMAGE               COMMAND       CREATED       STATUS          PORTS                                        NAMES
3346da816048   oceanbase/centos7   "/bin/bash"   2 years ago   Up 13 minutes   0.0.0.0:22->22/tcp, 0.0.0.0:2881->2881/tcp   oceanbase-ce

登录：
docker start 3346da816048
docker attach  3346da816048
docker exec -it oceanbase-ce /bin/bash




修改ssh配置，允许root登录

vi /etc/ssh/sshd_config
PermitRootLogin yes 
#允许root用户登录,本人由于是在本地虚拟机测试玩的，为了方便，全开
PasswordAuthentication yes 
# 允许密码登录

#RSAAuthentication 弃用RSAAuthentication
#为什么在 CentOS7.4 的配置文件中没有
#RSAAuthentication 这一行？
# UsePAM yes remove

PubkeyAuthentication yes #启用公钥私钥配对认证方式
AuthorizedKeysFile .ssh/authorized_keys #公钥
# 客户端
cat ~/.ssh/id_rsa.pub
# 服务端
vim ~/.ssh/authorized_keys
# cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

# ssh-copy-id 
可以把本地主机的公钥复制到远程主机的authorized_keys文件上，



清除root账号的密码
passwd -d root

修改root密码
passwd root 


将authorized_keys目录的权限为600
将.ssh目录的权限为700
chmod 600 ~/.ssh/authorized_keys
chmod 700 ~/.ssh

systemctl restart sshd
Failed to get D-Bus connection: Operation not permitte

改为
service sshd restart

~~~

- 设置 SSH 是连接远程主机

~~~
登录：
docker start 3346da816048
docker attach  3346da816048
docker exec -it oceanbase-ce /bin/bash
ssh root@192.168.3.3
方式1:ssh root@127.0.0.1 -p 22  (端口映射主要原理是利用host机的ip地址和端口来访问容器ip和端口）
方式2：ssh root@127.0.0.1 -i ~/.ssh/id_rsa_server
方式3:
SSH 客户端配置文件的位置
~/.ssh/config
配置文件的结构

Host ob
  HostName 127.0.0.1
  Port 22
  User root
  IdentityFile ~/.ssh/id_rsa #私钥 


# Host:		别名,ssh登录的时候就可以用  
# HostName: 	   远程服务器IP
# Port:				 远程服务器ssh端口，22端口可以忽略，如果不是22端口必须指定
# User:				 ssh 远程登录账号
# IdentityFile  对应的本机私钥路径

ssh ob
Last login: Sat Aug 17 08:35:24 2024 from gateway

~~~

#### 02 源码编译
~~~

https://oceanbase.github.io/oceanbase/build-and-run/

bash build.sh debug --init --make

# 进入生成的 debug 构建目录
cd build_debug
# 进行构建
make -j{N} observer (需要很长时间)
# 查看构建产物
stat src/observer/observer
~~~

#### 3  OceanBase 开发环境搭建  

vscode +远程本地docker环境

~~~sql
kex_exchange_identification: Connection closed by remote host
Connection closed by 127.0.0.1 port 22
local-server-2> ssh child died, shutting down

vi /etc/ssh/sshd_config
#RSAAuthentication 弃用RSAAuthentication
#为什么在 CentOS7.4 的配置文件中没有
#RSAAuthentication 这一行？
# UsePAM yes remove
systemctl restart sshd
~~~



####  4 代码风格（多看 几遍）



~~~css
https://oceanbase.github.io/oceanbase/coding_standard/
https://www.oceanbase.com/docs/-code-style-cn-10000000000018872
https://github.com/oceanbase/oceanbase/blob/develop/docs/docs/en/container.md

~~~



目录结构

OceanBase 系统的子目录说明如下：

- src：存放源代码，包含头文件和实现文件
- unittest：存放单元测试代码和开发人员编写的小规模集成测试代码
- tests：存放测试团队的测试框架和用例
- tools：存放外部工具
- doc：存放文档
- rpm：存放 rpm spec 文件
- script：存放 OceanBase 的运维脚



#### 5 调试部署

~~~shell
https://gitee.com/oceanbase/obdeploy/blob/master/example/mini-local-example.yaml
obd cluster deploy obtest -c mini-local-example.yaml
obd cluster edit-config obtest
obd cluster redeploy obtest

obd cluster list
obd cluster display watchpoints
obd cluster start  obtest

obclient -h127.0.0.1 -P2881 -uroot -Doceanbase -A


/root/observer/bin/observer -r 127.0.0.1:2882:2881 -p 2881 -P 2882 -z zone1 -c 1 -d /root/log/data -i lo -o __min_full_resource_pool_memory=2147483648,memory_limit=5G,system_memory=1G,datafile_size=8G,log_disk_size=5G,cpu_count=4,enable_syslog_wf=False,enable_syslog_recycle=True,max_syslog_file_count=4



~~~



#### 小贴士

多看、多问、多总结 肯定是可以攻克的



一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

二、这个技术的优势和劣势分别是什么

三、这个技术适用的场景。任何技术都有其适用的场景

四、技术的组成部分和关键点。

五、技术的底层原理和关键实现

六、已有的实现和它之间的对比



- https://mp.weixin.qq.com/s/fA1-53Hb5j4G4wmnVn4Sgg
- https://tianchi.aliyun.com/forum/post/760604
- https://open.oceanbase.com/course/427

~~~~


~~~~

