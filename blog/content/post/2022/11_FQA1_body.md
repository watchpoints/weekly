---
title: "阅读与写作第一期：王者回答"
date: 2022-10-18
draft: false
tags: ["60秒问答"]
---

### 学习方法

~~~


### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

### 二、这个技术的优势和劣势分别是什么 

### 三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

### 四、技术的组成部分和关键点。

### 五、技术的底层原理和关键实现

### 六、已有的实现和它之间的对比
~~~



- 步骤

~~~
1. 阅读(输入：三天,万句   输出 笔记摘要就哪里，但是自己看不懂，别人看不懂 ) 

2. 发现问题（输入：技术文档 30分钟 百句，输出 ：一大堆东西不无法语言描述，反正就是他，自己回答不清楚，别人听不清楚）

3. 整理问题（输入：5分钟 10句 技术文档  输出：核心根本无法理解，至少说到重点，不管别人听不懂，至少自己理解极限了）

4. 回答问题（输入：60秒,2句 技术语言  输出：一个视频）
~~~



- 输出结果

1. http://localhost:1313/post/2022/11_FQA1_body/ 【回答】
2. http://localhost:1313/post/2022/06_ask_1 【提问】



# FQA



### 提问：4/100，虚拟内存和物理内存怎么映射的？和slap 伙伴系统 vmare啥关系？

![dr](assets/image-20221102101732096.png)

提问2/100：[slabtop 查看 task_struct越来越大，这个怎么回收？](https://maimai.cn/web/gossip_detail?encode_id=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlZ2lkIjoiZTE1OTg3NTIzZTQ4NGZhN2IyYjViYWFmZDczY2YwNWQiLCJpZCI6MzEyMTczNTAsInUiOjQ2NjUxfQ.gH0qE4ziR-zcar_qTHMSnWyph2PriADQkzdmplm2Djs&from=list)





### 提问3/100 [valgrind 跟踪可执行程序程序   Permission denied](https://maimai.cn/web/gossip_detail?encode_id=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlZ2lkIjoiZDQ1NTdhMWYzMGIwNDA2NzhjMTQ1MjliNDUwOGExZjIiLCJpZCI6MzEyMjIzMjQsInUiOjQ2NjUxfQ.FPWKLn4BETXrO8-Heh_103rrnwpVThKewc7sWVU-rrA&from=list)

valgrind --tool=memcheck  /usr/bin/beegfs-ctl
valgrind: /usr/bin/beegfs-ctl: Permission denied
root用户怎么处理





# 答案



# tool

### 提问3/100 [valgrind 跟踪可执行程序程序   Permission denied](https://maimai.cn/web/gossip_detail?encode_id=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlZ2lkIjoiZDQ1NTdhMWYzMGIwNDA2NzhjMTQ1MjliNDUwOGExZjIiLCJpZCI6MzEyMjIzMjQsInUiOjQ2NjUxfQ.FPWKLn4BETXrO8-Heh_103rrnwpVThKewc7sWVU-rrA&from=list)

valgrind --tool=memcheck  /usr/bin/beegfs-ctl
valgrind: /usr/bin/beegfs-ctl: Permission denied
root用户怎么处理



~~~
sudo chown 0:0 ./beegfs-ctl
sudo chown 0:0 /usr/bin/beegfs-ctl
sudo chown 0:0  /bin/sh

valgrind --tool=memcheck  --log-file="dump" --trace-children=yes --dsymutil=yes --leak-check=full --show-leak-kinds=all ./beegfs-ctl --migrate --storagepoolid=2 --destinationpoolid=1 /mnt/beegfs/big/case7/vdb.1_2.dir/vdb.2_4.dir/vdb_f272664.file】

/var/log/local/tool/Python-3.6.8
valgrind --tool=memcheck --log-file="dump1" --trace-children=yes --dsymutil=yes --leak-check=full --show-leak-kinds=all --suppressions=/var/log/local/tool/Python-3.6.8/Misc/valgrind-python.supp /usr/local/python/bin/python3.6 ./test.py


valgrind --tool=memcheck  --log-file="dump" --trace-children=yes --dsymutil=yes --leak-check=full --show-leak-kinds=all --suppressions=/var/log/local/tool/Python-3.6.8/Misc/valgrind-python.supp /usr/local/python/bin/python3.6 /opt/beegfs/move/src/beegfs_test.py

~~~



# c++ 



### 提问1/100 c++ 全局静态对象 什么时候初始化

- 对于 *C* 语言的全局和静态变量，不管是否被初始化，其内存空间都是全局的；如果初始化，那么初始化发生在任何代码执行之前，属于编译期初始化。

- 标准规定：全局或静态对象当且仅当对象首次用到时才进行构造，并通过 *atexit()* 来管理对象的生命期，在程序结束之后（如调用 *exit* ， *main* ），按 *FILO* 顺序调用相应的析构操作

- dynamic initialization
        这种初始化针对的是需要调用构造函数才能完成的初始化。这种初始化会在main函数执行前由运行时库调用对应的代码进行初始化。

- 全局变量无疑要在main函数开始前执行完成，但可细分为在编译时和在运行时初始化

- 但是在C++语言中，由于构造函数不是常量表达式，因此无法用构造函数给全局对象赋初值。

  因此才有了编译器自动生成函数用来对全局对象赋初值，注意，这个动作是在程序运行期执行的，而不是类对象的全局变量（如int，char*等类型）的初始化由于可以使用常量表达式，因此在编译器就可以完成赋值动作，编译完后该变量的值已经被赋值好了。

  最后，用来给全局对象赋初值和析构的函数在main函数内的一开始以及退出前被调用，如

# Lliunx



### 提问 5/100：[daemon(守护进程)](https://blog.csdn.net/xuleilx/article/details/8258798)

- ，每一个从此终端开始运行的进程都会依附于这个终端，这个终端就称为这些进程的控制终端，当控制终端被关闭时，相应的进程都会自动关闭。但是守护进程却能够突破这种限制，它从被执行开始运转，直到整个系统关闭时才退出。
- https://blog.csdn.net/woxiaohahaa/article/details/53487602
- [**pstree**](https://blog.csdn.net/Kobe__Bryant__/article/details/120220117#:~:text=linux%E6%9F%A5%E7%9C%8B%E8%BF%9B%E7%A8%8B%20%E5%92%8C%E8%BF%9B%E7%A8%8B%E4%B9%8B%E9%97%B4%E7%9A%84%E5%85%B3%E7%B3%BB%20%5Bps%20-exj%20pstree%5D%201%20PID%20%28Process,S%20sleeping%20%E7%9D%A1%E7%9C%A0%E7%8A%B6%E6%80%81%20%28%E5%8F%AF%E4%BB%A5%E8%A2%AB%E7%B3%BB%E7%BB%9F%E5%94%A4%E9%86%92%29%20T%20stop%20%E5%81%9C%E6%AD%A2%E7%8A%B6%E6%80%81%20%5D) 
- pd -exj
- 

提问4/200：判断系统有没有重启 ？

- uptime -s

-s, --since
              system up since, in yyyy-mm-dd HH:MM:SS format

-  date -s "2019-06-25 20:13:00"



### 题目3/100 内存很大，找不到那里是



free查看内存占用很多（cache除外），但是top查看进程占用内存很小
业务程序是：python调用调用另外一个程序执行，初步这个地方出问题了。大量请求
该怎么查看呢slabtop 一个task占用很多内存



###



：perf record  -F 99  -g -e kmem:kmem_cache_alloc_node  -- sleep 30

查资料

- https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/deployment_guide/s2-proc-meminfo

- /proc/meminfo 内存参数内核分配/释放过程介绍

- # [原创]systemtap脚本分析系统中dentry SLAB占用过高问题

- https://cn.pingcap.com/blog/flame-graph 工欲性能调优，必先利其器（2）- 火焰图

- 动态追踪技术漫谈 https://blog.openresty.com.cn/cn/dynamic-tracing/

- 发生即看见，一切可回溯 | TiDB 故障诊断与性能排查探讨

​    

- https://blog.csdn.net/Darlingqiang/article/details/118914428
- https://pythonextensionpatterns.readthedocs.io/en/latest/debugging/valgrind.html
- https://www.semicolonworld.com/question/56034/how-to-use-valgrind-with-python

输出：





第三次

- https://blog.csdn.net/sy4331/article/details/120241025

- https://blog.csdn.net/sy4331/article/details/126010513

- https://blog.csdn.net/sy4331/article/details/126010513

- https://hammertux.github.io/slab-allocator

- [Linux系统性能测试工具（二）——内存压力测试工具memtester](https://www.cnblogs.com/sunshine-blog/p/11905491.html)

- https://blog.csdn.net/s1421578048/article/details/104262751/

- # 详解slab机制

https://www.cnblogs.com/zafu/p/10042099.html

SLAB_RECLAIM_ACCOUNT

https://github.com/brendangregg/perf-tools

https://chegva.com/3464.html





如何排查Linux slab_unreclaimable内存占用高的原因？

- https://blog.csdn.net/liuxiao723846/article/details/72625394

- https://help.aliyun.com/document_detail/316787.html
- https://access.redhat.com/solutions/6123251
- https://serverfault.com/questions/877257/why-cant-my-linux-kernel-reclaim-its-slab-memory
- https://www.jianshu.com/p/a7af7c29c9e2



~~~
ps aux | awk '{mem += $6} END {print mem/1024/1024}'
less /proc/meminfo  |grep ^S
cat /proc/slabinfo |awk '{printf "%7i MiB : %s\n",$6*$(NF-1)/256,$1}'|sort -nk 1 -r|head -5
cat /proc/slabinfo | grep -i cred_jar
~~~



- 见buffer_head描述的是磁盘block和内存[buffer](https://so.csdn.net/so/search?q=buffer&spm=1001.2101.3001.7020)之间的映射关系。

  https://blog.csdn.net/u010039418/article/details/116034421

~~~
perf record -e probe_tikv:malloc -F 99 -p tikv_pid -g -- sleep 10
perf script > out.perf
/opt/FlameGraph/stackcollapse-perf.pl out.perf > out.folded
/opt/FlameGraph/flamegraph.pl  --colors=mem out.folded > mem.svg

sudo perf record -e cpu-clock -g -p 1648249


perf record -a -e kmem:kmalloc --filter 'bytes_alloc == 192' -e kmem:kfree --filter ' ptr != 0' sleep 200

perf record -a -e kmem:kmalloc --filter 'bytes_alloc == 192' -e kmem:kfree --filter ' ptr != 0' sleep 200


perf script -i perf.data > perf.unfold


./FlameGraph/stackcollapse-perf.pl perf.unfold > perf.folded

./FlameGraph/flamegraph.pl perf.folded > perf_job_salve_2.svg

https://cn.pingcap.com/blog/flame-graph


~~~



1. 火焰图（Flame Graph）是由 Linux 性能优化大师 Brendan Gregg 发明的





~~~
perf record -F 99 -p tikv_pid -g -- sleep 60
perf script > out.perf
~~~



2. Valgrind

~~~
https://pythonextensionpatterns.readthedocs.io/en/latest/debugging/valgrind.html

https://pythonextensionpatterns.readthedocs.io/en/latest/debugging/leak_newrefs_vg.html#leaked-new-references-valgrind-label

https://github.com/vpython/visual/blob/master/valgrind-python.supp
As per recommendation from Readme.valgrind I did the following.

1) Downloaded the python source with

sudo apt-get build-dep python2.7
apt-get source python2.7
2) Applied the code patch, i.e. "Uncomment Py_USING_MEMORY_DEBUGGER in Objects/obmalloc.c".

3) Applied the suppression patch, i.e. "Uncomment the lines in Misc/valgrind-python.supp that suppress the warnings for PyObject_Free and PyObject_Realloc"

4) Compiled python with

--enable-framework

Installs it in /Library/Frameworks/Python.framework/Versions/

--with-pydebug

Debug build of Python. See Misc/SpecialBuilds.txt

--without-pymalloc

With Valgrind support Misc/README.valgrind



./configure --prefix=/home/dejan/workspace/python --without-pymalloc
make -j4 install


If you don't want to read about the details of using Valgrind, there
are still two things you must do to suppress the warnings.  First,
you must use a suppressions file.  One is supplied in
Misc/valgrind-python.supp.  Second, you must do one of the following:

  * Uncomment Py_USING_MEMORY_DEBUGGER in Objects/obmalloc.c,
    then rebuild Python
  * Uncomment the lines in Misc/valgrind-python.supp that
    suppress the warnings for PyObject_Free and PyObject_Realloc

/usr/local/pytho

Valgrind包括如下一些工具：

Memcheck。这是valgrind应用最广泛的工具，一个重量级的内存检查器，能够发现开发中绝大多数内存错误使用情况，比如：使用未初始化的内存，使用已经释放了的内存，内存访问越界等。这也是本文将重点介绍的部分。
Callgrind。它主要用来检查程序中函数调用过程中出现的问题。
Cachegrind。它主要用来检查程序中缓存使用出现的问题。
Helgrind。它主要用来检查多线程程序中出现的竞争问题。
Massif。它主要用来检查程序中堆栈使用中出现的问题。
Extension。可以利用core提供的功能，自己编写特定的内存调试工具
————————————————
版权声明：本文为CSDN博主「sishuihuahua」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/sishuihuahua/article/details/85930419

https://www.gnu.org/software/automake/
yum install autoconf
yum install automake
yum install libtool
./autogen.sh

git clone https://sourceware.org/git/valgrind.git
cd valgrind
cd valgrind
./autogen.sh
./configure
make
make install

./configure --with-pydebug --without-pymalloc --with-valgrind

/usr/share/doc/platform-python-devel

valgrind --tool=memcheck --trace-children=yes --dsymutil=yes --leak-check=full --show-leak-kinds=all --suppressions=/var/log/local/tool/Python-3.6.8/Misc/valgrind-python.supp /usr/local/python/bin/python3.6 /opt/beegfs/move/src/beegfs_test.py


Linux Perf 性能分析工具及火焰图浅析
Perf 是内置于 Linux 内核源码树中的性能剖析（profiling）工具、
https://zhuanlan.zhihu.com/p/54276509
https://phoenixnap.com/kb/linux-perf

perf stat对全局性能进行统计；
perf top可以实时查看当前系统进程函数占用率情况；
pref record记录信息到perf.data；
perf report生成报告；
1，首先使用 perf record 命令记录进程的 CPU 使用情况
命令：

sudo perf record -e cpu-clock -g -p 1648249


2. 使用 perf script 工具对 perf.data 进行解析
命令：sudo 

perf script -i perf.data > perf.unfold

3. 使用 Flame Graph 工具将 perf.unfold 中的符号折叠 //生成脚本文件
yum install perl


./FlameGraph/stackcollapse-perf.pl perf.unfold > perf.folded


4. 使用 Flame Graph 工具将 perf.folded 生成 svg 火焰图
命令：

./FlameGraph/flamegraph.pl perf.folded > perf_job_salve_1.svg 

//执行完成后生成perf.svg图片，可以下载到本地，用浏览器打开 perf.svg


命令：perf script | ./FlameGraph/stackcollapse-perf.pl | ./FlameGraph/flamegraph.pl > out.svg

火焰图插件下载地址：
https://github.com/brendangregg/FlameGraph
快捷命令：
git clone https://github.com/brendangregg/FlameGraph.git
火焰图原文翻译：
https://zhuanlan.zhihu.com/p/73385693

软件的性能分析，往往需要查看 CPU 耗时，了解瓶颈在哪里
http://www.ruanyifeng.com/blog/2017/09/flame-graph.html

致命报错：想生成火焰图，但是老是抛出错误：ERROR: No stack counts found，could not generate flame graph: exit status 2。
弄了很久，以为是 perf 没装，但是装了也不行，然后发现必须在访问程序时，火焰图才能生成


# -d 表示高亮变化的字段
# -A 表示仅显示Normal行以及之后的15行输出
$ watch -d grep -A 15 'Normal' /proc/zoneinfo
Node 0, zone   Normal
  pages free     21328
        min      14896
        low      18620
        high     22344
        spanned  1835008
        present  1835008
        managed  1796710
        protection: (0, 0, 0, 0, 0)
      nr_free_pages 21328
      nr_zone_inactive_anon 79776
      nr_zone_active_anon 206854
      nr_zone_inactive_file 918561
      nr_zone_active_file 496695
      nr_zone_unevictable 2251
      nr_zone_write_pending 0
      
https://help.aliyun.com/document_detail/316787.html
~~~



### 提问2/100 rpm管理



~~~
https://www.tecmint.com/20-practical-examples-of-rpm-commands-in-linux/

rpm -qpR beegfs-move-utils-1.0-1.el8.noarch.rpm
-q : Query a package
-p : List capabilities this package provides.
-R: List capabilities on which this package depends..

How to List all files of an installed RPM package
rpm -ql beegfs-move-utils-1.0-1.el8.noarch.rpm

~~~



### 提问1 keeplive





> 分析：



sysctl -a|grep tcp_keepalive

net.ipv4.tcp_keepalive_intvl = 75

当探测没有确认时，重新发送探测的频度。缺省是75秒。



net.ipv4.tcp_keepalive_probes = 9

在认定连接失效之前，发送多少个TCP的keepalive探测包。缺省值是9。

这个值乘以tcp_keepalive_intvl之后决定了，一个连接发送了keepalive之后可以有多少时间没有回应



net.ipv4.tcp_keepalive_time = 1200
当keepalive起用的时候，TCP发送keepalive消息的频度。缺省是2小时



~~~

# # The default value:
# cat /proc/sys/net/ipv4/tcp_congestion_control
cubic
# cat /proc/sys/net/ipv4/tcp_keepalive_time
7200
# cat /proc/sys/net/ipv4/tcp_keepalive_intvl
75
# cat /proc/sys/net/ipv4/tcp_keepalive_probes
9
 
## To update the value:

echo 30 > /proc/sys/net/ipv4/tcp_keepalive_time
echo 10 > /proc/sys/net/ipv4/tcp_keepalive_intvl
echo 6 > /proc/sys/net/ipv4/tcp_keepalive_probes


在Linux中我们可以通过修改 /etc/sysctl.conf 的全局配置：

net.ipv4.tcp_keepalive_time=30
net.ipv4.tcp_keepalive_intvl=10
net.ipv4.tcp_keepalive_probes=6

~~~

添加上面的配置后输入 sysctl -p 使其生效

你可以使用  sysctl -a | grep keepalive 命令来查看当前的默认配置

~~~

fs.aio-max-nr=1048576
net.ipv4.tcp_keepalive_time=30
net.ipv4.tcp_keepalive_intvl=10
net.ipv4.tcp_keepalive_probes=6


# Controls the default maxmimum size of a mesage queue
kernel.msgmnb = 65536

# Controls the maximum size of a message, in bytes
kernel.msgmax = 65536

# Controls the maximum shared segment size, in bytes
kernel.shmmax = 68719476736
# Controls the maximum number of shared memory segments, in pages
kernel.shmall = 4294967296
#Lashou WEB
net.ipv4.tcp_tw_reuse = 1
net.ipv4.tcp_max_syn_backlog = 8192
net.ipv4.tcp_max_tw_buckets = 10000
net.ipv4.tcp_max_syn_backlog = 65535
net.core.netdev_max_backlog = 32768
net.core.somaxconn = 32768
net.ipv4.tcp_synack_retries = 2
net.ipv4.tcp_syn_retries = 2
net.ipv4.tcp_timestamps = 0
net.ipv4.tcp_fin_timeout = 10
net.ipv4.ipfrag_time = 15
net.ipv4.ip_local_port_range = 1024  65535
vm.overcommit_memory=1
# add config
kernel.shmall = 2097152
kernel.shmmax = 2147483648
kernel.shmmni = 4096
kernel.sem = 250        32000   100      128
net.ipv4.tcp_max_syn_backlog = 65536
net.core.netdev_max_backlog = 32768
net.core.somaxconn = 32768


net.core.wmem_default = 8388608
net.core.rmem_default = 8388608
net.core.rmem_max = 16777216
net.core.wmem_max = 16777216
net.ipv4.tcp_timestamps = 0
net.ipv4.tcp_synack_retries = 2
net.ipv4.tcp_syn_retries = 2


net.ipv4.tcp_mem = 94500000 915000000 927000000
net.ipv4.tcp_max_orphans = 3276800
net.ipv4.ip_local_port_range = 1024 65535
fs.file-max = 655360

net.ipv4.tcp_keepalive_time=30
net.ipv4.tcp_keepalive_intvl=10
net.ipv4.tcp_keepalive_probes=6

~~~

- 打开最大文件个数

~~~
echo 'fs.aio-max-nr=1048576' | sudo tee /etc/sysctl.conf

sysctl -p

echo -e "root soft nofile 655350\nroot hard nofile 655350\n* soft nofile 655350\n* hard nofile 655350">> /etc/security/limits.conf
~~~



- tcp backlog

  阅读linux tcp socket 请求队列大小参数 backlog 简介

  [详解socket中的backlog 参数](https://zhuanlan.zhihu.com/p/104874605)

![](https://upload-images.jianshu.io/upload_images/2792101-e436bf34b4dbe79a.png?imageMogr2/auto-orient/strip|imageView2/2/w/657/format/webp)
