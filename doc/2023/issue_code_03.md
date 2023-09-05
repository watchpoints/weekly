 

新手村打怪升级NO.3

# 一 、站在巨人肩膀上


# 二. 从石器时代到黄金时代

## 一、公开课[资料]

- 数据库/存储学习路径推荐

     [https://mp.weixin.qq.com/s/fl-R_ZMQwy0aNQbsXZeQ3Q](https://mp.weixin.qq.com/s/fl-R_ZMQwy0aNQbsXZeQ3Q)

进展：0
### - 阅读书籍：leveldb 精通 
[https://github.com/watchpoints/leveldb](https://github.com/watchpoints/leveldb)
[https://leveldb-handbook.readthedocs.io/zh/latest/basic.html](https://leveldb-handbook.readthedocs.io/zh/latest/basic.html)
进展：1

- Go 开放课程：MIT 经典课程 《分布式系统》

[https://learnku.com/go/wikis/41239](https://learnku.com/go/wikis/41239)

- MIT 6.84: Distributed Systems

官方地址：[https://pdos.csail.mit.edu/6.824/](https://pdos.csail.mit.edu/6.824/)
进展：0

## 二. TIdb

:::warning
为什么要看tidb，自己一个担心，前面无数人在 做，自己先做晚来了？
然后等待3年 5年什么都不事情？然后看看手机电影，
为纠这个态度，从消息态度变成积极态度，
一个社区无数文档，无数参与 着说明有价值的。
你认为正确行业，你认为正确社区，你认为正确的，
为什么行动呢？着就是最大问题？
有正确，有价值的不去做，反而去做没有用 事情。破冰
:::



## 如何成为 TiFlash Contributor？
[https://asktug.com/t/topic/693824](https://asktug.com/t/topic/693824)

| 资料 | 作者 | 地址 |
| --- | --- | --- |
| **High Performance TiDB 课程学习计划** | 官方 | [https://docs.qq.com/sheet/DY3RueHVEd1JQV1Vi?tab=BB08J2](https://docs.qq.com/sheet/DY3RueHVEd1JQV1Vi?tab=BB08J2) |
| **TiFlash 往期源码解读** | 官方 | [https://asktug.com/t/topic/903372](https://asktug.com/t/topic/903372) |
| **Talent Plan 2021 KV** | **社区** | [https://learn.pingcap.com/learner/course/510001](https://learn.pingcap.com/learner/course/510001)
[https://github.com/tikv/tikv](https://github.com/tikv/tikv) |
| ### TiDB 社区技术月刊
 | 七大模板 | [https://tidb.net/book/tidb-monthly/](https://tidb.net/book/tidb-monthly/) |
| T**iKV 源码阅读三部曲** | 社区 | [https://mp.weixin.qq.com/s/HgflwnZZMHXaIsFV3PdGAg](https://mp.weixin.qq.com/s/HgflwnZZMHXaIsFV3PdGAg) |
| ### issue
 | 
 | [https://github.com/pingcap/tidb/issues/7623](https://github.com/pingcap/tidb/issues/7623)
[https://github.com/tikv/tikv/issues/8650](https://github.com/tikv/tikv/issues/8650) |
| [【源码类学习资源大放送】TiDB、TiKV、TiFlash、Chaos Mesh 等源码解读补给站](https://asktug.com/t/topic/693645)

 | 
 | [https://asktug.com/t/topic/693645/1](https://asktug.com/t/topic/693645/1) |
| [最近学习数据库源码 遇到的疑惑，希望过来人帮忙解答](https://asktug.com/t/topic/1000469) |  | [https://asktug.com/t/topic/1000469?_gl=1*fkkx3*_ga*MjI2MTMzMTkyLjE2NzM5MjYyNDI.*_ga_5FQSB5GH7F*MTY3NzE1NzkzOS4yMi4wLjE2NzcxNTc5MzkuMC4wLjA.](https://asktug.com/t/topic/1000469?_gl=1*fkkx3*_ga*MjI2MTMzMTkyLjE2NzM5MjYyNDI.*_ga_5FQSB5GH7F*MTY3NzE1NzkzOS4yMi4wLjE2NzcxNTc5MzkuMC4wLjA.) |



### 
### 挑战：[TiKV raft engine日志丢失Tikv恢复失败](https://asktug.com/t/topic/1006099)
[https://asktug.com/t/topic/1006099](https://asktug.com/t/topic/1006099)

第一步： 盯着问题根本无法解决，凭空想想不出来的问题变为tikv如何加载recover日志的。

### 挑战2:
[Incorrect data conversion · Issue #44219 · pingcap/tidb](https://github.com/pingcap/tidb/issues/44219)
```basic
CREATE TABLE t1 (c1 INT UNSIGNED NOT NULL);
INSERT INTO t1 VALUES (0);
SELECT c1 > - '7' FROM t1;
```
开发进展：


提问3： [tidb 如何禁止开启启动](https://asktug.com/t/topic/1007498) 
分类：运维部署
回答：

提问4:我的SQL走tiflash
[TIFLASH sql优化问题](https://asktug.com/t/topic/1007129/15)

分类：架构设计
回答：
## 三 ob社区



提问1：
函数ST_ExteriorRing已经新增了，ST_ExteriorRing函数都是boost geometry 不需要自己实现把？
[https://dev.mysql.com/doc/refman/5.7/en/gis-polygon-property-functions.html#function_st-exteriorring](https://dev.mysql.com/doc/refman/5.7/en/gis-polygon-property-functions.html#function_st-exteriorring)
分类：sql引引擎
答： 所有的函数的都是调用boost库的，我看MySQL也是这么做的，当时MySQL还写了一个文章为什么要用

#### 提问2： _ 关于__all_virtual_server_stat表的各字段的含义描述,是从什么渠道了解到的呢,找了官方文档没找到在哪  
分类：多租户
回答：无

#### 提问3： 我这边三台服务器，有一台数据被清除了，集群起不来，这种有办法处理吗，每台服务器上只有一个节点  
分类：高可用 ！！！！
回答：

 gdb 设置断点很容易造成选举失败，推荐单元测试

可以用debug sync  
## 四  TDengine

| 序号 |  模块 | 提问 | 回答 |
| --- | --- | --- | --- |
| 1 | 技能 | [通过招聘看要求](https://mp.weixin.qq.com/s/p0mhBpdHDk2kxQMDD75Z3A) | 必须掌握英语。 |
| 2  | 理念 |  |  |


---


---

### 五 。操作系统



# 三. 开发者手册：日拱一卒 从青铜到白银经  



## 跑吧 Linux内核（入门篇）

### 要点1：Linux系统调用：truncate

函数：

int truncate(const char *path,off_t length);

https://man7.org/linux/man-pages/man2/truncate.2.html



```
 If the file previously was larger than this size, the extra data
       is lost.  If the file previously was shorter, it is extended, and
       the extended part reads as null bytes ('\0')
```



![](https://img-blog.csdnimg.cn/c4df106b62f84df587555257f937e741.png)



要点2：cat 

-A, --show-all
              equivalent to -vET

第一步：使用下述命令看看该文件的格式，dos格式的文件行尾为^M$，unix格式的文件行尾为$

第二步：使用下述命令直接替换结尾符为unix格式

命令行：sed -i "s/\r//" [xxx.sh]

**bash: /home/xxx.sh: /bin/bash^M: bad interpreter: No such file or directory**

### 3.1 C++最重要就是三座大山：面向对象技术，内存管理，模板（泛型编程）

1.  std::unique_ptr 默认不允许拷贝构造，拷贝赋值，为什么函数可以返回unique_ptr ？[而不是&unique_ptr](https://blog.csdn.net/xiaoxiaoguailou/article/details/123316470)
```cpp
 https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique 

template<class T, class... Args>
std::enable_if_t<!std::is_array<T>::value, std::unique_ptr<T>>
make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

```

2  template<typename... _Args> std::move 使用场景是什么？ std：forwrd区别？

```cpp
template <typename ...Args>
  static old_inode_map_ptr allocate_old_inode_map(Args && ...args) {
    static mempool::mds_co::pool_allocator<mempool_old_inode_map> allocator;
    return std::allocate_shared<mempool_old_inode_map>(allocator, std::forward<Args>(args)...);
  }

void InodeStoreBase::decode_old_inodes(bufferlist::const_iterator &p) {
  using ceph::decode;
  mempool_old_inode_map tmp;
  decode(tmp, p);
  if (tmp.empty()) {
    reset_old_inodes(old_inode_map_ptr());
  } else {
    reset_old_inodes(allocate_old_inode_map(std::move(tmp)));
  }
    
```
## 3.2 猴子都能懂的GIT入门〜掌握版本管理〜
### 合并代码用cherry-pick 
![image.png](https://cdn.nlark.com/yuque/0/2023/png/215144/1686045322780-4f2601d4-9f64-4f8f-9f89-50802a6055ce.png#averageHue=%236c7b7e&clientId=u07e5eee0-e1d9-4&from=paste&id=u3ca51b8a&originHeight=418&originWidth=800&originalType=url&ratio=1.340000033378601&rotation=0&showTitle=false&size=108437&status=done&style=none&taskId=u3df83e70-15ad-498a-a3a3-3d9e6a4154a&title=)
:::success
使用BeyondCompare合并代码，
结果发现提交上去代码 根本编译不通过，耗时 2天。都是语法错误
这是处理方式不对。合并代码，

- [https://ruanyifeng.com/blog/2020/04/git-cherry-pick.html](https://ruanyifeng.com/blog/2020/04/git-cherry-pick.html)

这时分两种情况。一种情况是，你需要另一个分支的所有代码变动，那么就采用合并（git merge）。另一种情况是，你只需要部分代码变动（某几个提交），这时可以采用 Cherry pick

如果操作过程中发生代码冲突，Cherry pick 会停下来，让用户决定如何继续操作。
git cherry-pick --continue
:::
![image.png](https://cdn.nlark.com/yuque/0/2023/png/215144/1686045605745-d775ed77-7e9d-4847-a57e-43714470db28.png#averageHue=%23f3f1e1&clientId=u07e5eee0-e1d9-4&from=paste&height=543&id=u39dc8251&originHeight=727&originWidth=1284&originalType=binary&ratio=1.340000033378601&rotation=0&showTitle=false&size=68310&status=done&style=none&taskId=uada570b4-e1b8-4147-86d2-ed80771fe80&title=&width=958.2089313554674)

git rebase --continue

- cherry-pick ：合并一次提交。

解决冲突CONFLICT
在rebase的过程中，也许会出现冲突(conflict)。在这种情况，Git会停止rebase并会让你去解决冲突；在解决完冲突后，用git add命令去更新这些内容的索引(index)，然后，你无需执行 git commit，只要执行：
```
bash
git rebase --continue
https://juejin.cn/post/6844903600976576519
```


作者：麦田里的糖醋鱼
链接：https://juejin.cn/post/6969101234338791432
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

- Linux中执行bash脚本报错/bin/bash^M: bad interpreter: No such file or directory

  https://zhuanlan.zhihu.com/p/597066517

  https://blog.51cto.com/u_15588078/6532252



然后修改文件格式 
#sh>vi filename 

利用如下命令查看文件格式 
:set ff 或 :set fileformat 

可以看到如下信息 
fileformat=dos 或 fileformat=unix 

利用如下命令修改文件格式 
:set ff=unix 或 :set fileformat=unix 

:wq (存盘退出) 

最后再执行文件 


### 周三

- fsconfig 是 Linux 内核中的一个功能，用于在运行时对文件系统进行配置。它允许用户在不重新挂载文件系统的情况下修改一些文件系统的参数。fsconfig 的引入旨在提供一种更灵活、更方便的方式来管理文件系统的配置，而无需中断文件系统的正常运行
https://github.com/golang/go/issues/59537


- `inotify` 是 Linux 内核提供的一种文件系统事件监控机制，用于监视文件和目录的变化。通过使用 inotify，应用程序可以在文件或目录发生更改时获得通知，从而能够实时响应文件系统的变化。

inotify 可以监视以下类型的文件系统事件：

1. **文件创建（CREATE）：** 当一个新文件或目录被创建时触发。

2. **文件删除（DELETE）：** 当一个文件或目录被删除时触发。

3. **文件修改（MODIFY）：** 当一个文件被修改时触发，如写入文件内容。

4. **文件移动或重命名（MOVED_FROM 和 MOVED_TO）：** 当一个文件或目录被移动或重命名时触发。

5. **文件属性变化（ATTRIB）：** 当一个文件的元数据（如权限、所有者等）发生变化时触发。

6. **文件关闭（CLOSE_WRITE 和 CLOSE_NOWRITE）：** 当一个文件被关闭时触发，分为写入关闭和只读关闭。

使用 inotify，应用程序可以实时监控文件系统事件，从而可以在文件或目录发生变化时立即采取适当的操作。这对于实时监控日志文件、自动更新文件内容、实时同步文件变化等应用场景非常有用。

在 Linux 中，你可以使用 C 编程语言中的 inotify API 来编写监控文件系统事件的程序。此外，一些编程语言和工具库也提供了对 inotify 的封装，使其更容易在不同的应用中使用，比如 Python 的 `pyinotify` 库。

总而言之，inotify 是 Linux 提供的一种强大的机制，用于监控文件和目录的变化，使应用程序能够实时响应文件系统的动态。



tmpfs 是 Linux/Unix 系统上的一种基于内存的文件系统
mount -t tmpfs tmpfs /mnt/tmp

所以/dev/shm/这个目录不在硬盘上，而是在内存里。因为是在内存里，所以读写非常快，可以提供较高的访问速度。linux下，tmpfs默认最大为内存的一半大小，

- 聊聊Linux上“too many links”报错出现的原因和处理方法

#define EMLINK 31 /* Too many links */链接太多


将 /tmp/image.iso 这个光碟的 image 档使用 loop 模式挂在 /mnt/cdrom之下。用这种方法可以将一般网络上可以找到的 Linux 光 碟 ISO 档在不烧录成光碟的情况下检视其内容。

#mount -o loop /tmp/image.iso /mnt/cdrom


### 周四 

1. psutil
python3 setup.py install

2. linux 内核模块 && 强制卸载

内核模块是Linux内核向外部提供的一个插口，其全称为动态可加载内核模块（Loadable Kernel Module，LKM）
https://zhuanlan.zhihu.com/p/559057211

# 查看modules文件内容
cat /proc/modules
# 列出Linux内核中modules 的状态
lsmod

3、insmod：将指定模块加载到内核，建议使用modeprobe命令

4、rmmod：将已加载模块从内核中移除，建议使用modeprobe命令

https://www.runoob.com/linux/linux-comm-modprobe.html


实例
安装软驱模块：
 modprobe -v floppy 
卸载软驱模块：

modprobe -v -r floppy 



https://blog.csdn.net/QTM_Gitee/article/details/97013657


5. please move or remove them before you merge


删除工程重新下载，网上reset等方法无效


6. https://docs.ceph.com/en/quincy/ceph-volume/lvm/zap/

7 sgdisk命令 – Linux和Unix的命令行GPT操纵器
原文链接：https://www.linuxcool.com/sgdisk

sgdisk -p /dev/sda
原文链接：https://www.linuxcool.com/sgdisk


### 周五


1. 配置文件问题导致 日志没有输出，因此 全部拷贝别人配置文件，不进行思考导致的

2. 如何离线安装python3库 

python3 -m pip install psutil-5.9.5.tar.gz --upgrade

提示：
- 一定源码编译，其他的不一定兼容
- 用setpy方式安装不行 估计
-  --upgrade 一定填写。
一定看官网文档：
https://packaging.python.org/en/latest/tutorials/installing-packages/#source-distributions-vs-wheels

>Upgrading packages¶
python3 -m pip install --upgrade SomeProject


3.  如何解决core问题

 发现不上自己模块，然后更新整体代码解决一次。
 但是根本原因是什么接口不对，其他不清楚。


4. 如果发现主干代码有问题，耽误 3天时间没有解决。 这个你怪谁 还是不删除解决冲突，根源代码冲突。。

> 惨痛教训：我知道回退代码，但是没有亲自执行，这次 根本原因冲突解决

 

git cherry-pick –continue, –abort，–quit

当使用git cherry-pick发生冲突后,将会出现如下信息：

$ git cherry-pick 23d9422

这时如果要继续cherry-pick，
1. 则首先需要解决冲突，
2. 通过git add .将文件标记为已解决，然后可以使
3. 用git cherry-pick --continue命令，继续进行cherry-pick操作。
https://zhuanlan.zhihu.com/p/90816644

https://www.kimsereylam.com/git/2021/01/01/git-cherry-pick.html#cherry-pick-a-specific-commit

https://waynerv.com/posts/git-cherry-pick-intro/



>日拱一卒，只管去做。

>灵感来源：刻意练习：如何从新手到大师
