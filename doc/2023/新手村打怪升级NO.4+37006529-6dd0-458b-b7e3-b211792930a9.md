

每周工作记录，周六发布。

# 一. 开发者手册：日拱一卒 从青铜到白银经



### 周一

1. 如何测试？不管你配置问题 还是环境问题，测试一次，然后剩余60分钟分析问题，



无效尝试：今天感觉不正确，重新搭建环境 120分钟，重启机器30分钟解决最终无效。

一定 分析问题，不是经历测试上，多少次了，

测试很简单，工作需要但是你投入上面 错误的。

1. 晚上走路不那手机一次 +1.

2. 

## 周四



1. c++11 数组遍历   vs const i &



https://en.cppreference.com/w/cpp/container/array



for (const auto& s: a3)



1. map 遍历 



- https://stackoverflow.com/questions/14555751/how-to-iterate-over-a-c-stl-map-data-structure-using-the-auto-keyword

-  map 不负责释放 存储指针的内容map<int,char*>

- map析构的时候，自动清理 里面内容吗？



1. 可以反复 delete ptr 和 free ptr吗？



1. 提交pr时候，不是代码修改范围，不要修改。你修改了还修改错误了。

 这个应该还原回去。

1. 这个需要改，不是每次都要议案，只有进行fail mds了才需要议案和插入回调

2. inux下c++filter查看函数修饰名

c++filt _ZNK4Json5ValueixEPKc

### 周三

https://zhuanlan.zhihu.com/p/348580911

**2.1 创建部署目录**

初始化mon

```Plain Text
 ceph-deploy mon create-initial
```

[https://github.com/ceph/ceph-deploy](https://github.com/ceph/ceph-deploy)



- icfs-deploy new $host 这里传入两个mon节点就可以

- [https://www.zybuluo.com/dyj2017/note/920621](https://www.zybuluo.com/dyj2017/note/920621)

1.  你沉浸测试中，测试时候不停敲击鼠标，出现问题生气，着急，根本从源码，架构，业务角度考虑问题。



### 周三

1. shell 技巧

```PowerShell
tee
命令的作用是将输出的一个副本输送到标准输出，另一个副本拷贝到相应的文件中。它的一般形式为：
tee [options] filename
其中，-a表示追加到文件末尾。
例如，将命令输出重定向到文件hello.txt的同时也输出到屏幕上：
ls -l | tee hello.txt
这将把ls -l命令的输出重定向到hello.txt文件中，并将其同时输出到屏幕上。

shell $@
```





### 周四



- 服务端执行过程中 core怎么，执行命令该怎么处理，内部重试逻辑无效了？--客户端重试

- mon 三节点 ，crash一个，然后命令发送到 从解决该怎么处理？

- disk lvm

- 删除LVM步骤 无效

- [解决无法删除lvm分区的问题](https://www.cnblogs.com/deny/p/14213003.html)

dmsetup 是***low level logical volume management*** 。
- https://ceph.com/community/new-luminous-bluestore/



Roughly speaking, BlueStore is about twice as fast as FileStore, and performance is more consistent with a lower tail latency.粗略地说，BlueStore 的速度大约是 FileStore 的两倍，并且性能与较低的尾部延迟更一致。

### **How does BlueStore work?** [¶](https://ceph.com/community/new-luminous-bluestore/#how-does-bluestore-work%3F) BlueStore 如何运作？¶



![https://ceph.com/community/new-luminous-bluestore/images/filestore-vs-bluestore-2.png](https://ceph.com/community/new-luminous-bluestore/images/filestore-vs-bluestore-2.png)

https://ceph.com/community/new-luminous-bluestore/images/filestore-vs-bluestore-2.png



ceph-volume lvm create --bluestore --data $DEVICE --o



### 周五

1.  mon1 mon2 都执行相同命令 一个leader执行成功，一个非leader 不停重试。是是有问题？



### 周六

- 远离微信视频，视频号。如果消磨时间，不知道怎么办，去运动。去看书，自己制作视频。不要在上面浪费你时间。

## 周天



1. shell dirname $0



# 二. 开源社区 

- 无记录

# 三 .ARTS打卡

- 无记录



# 四. 每周新闻

- 无记录

- 

