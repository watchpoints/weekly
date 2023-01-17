---
title: "gperftools"
date: 2022-02-09
description: "gperftools"
draft: false
tags: ["系统设计"]
---







### 青铜提问1/100：如何用emacs搭建c++开发环境？



#### 学习别人的思路：

- https://www.zhihu.com/question/51099641
- https://emacs-lsp.github.io/lsp-mode/tutorials/CPP-guide/
- https://www.cnblogs.com/flyingdirt/p/3399624.html
- https://github.com/purcell/emacs.d{web}
- https://www.cnblogs.com/GarfieldEr007/p/5588979.html
- https://www.zybuluo.com/qqiseeu/note/17692



#### 小马过河：



- todo1 

  1. 我应一开始就照抄 世界级大师 Steve Purcell 的 Emacs 配置.

  2. 安心用 [Steve Purcell](http://www.sanityinc.com/) 的 [Emacs 配置](https://github.com/purcell/emacs.d) , 从此走上正轨.

  3. 初学者开始阶段应以模仿为主. 这点是本文核心思想.

~~~
 
 sudo dnf install emacs
 
 emacs --version
 GNU Emacs 26.1 //版本不是最新版本

依赖：
yum install gnutls  gnutls-devel
yum install ncurses-devel


wget https://ftp.gnu.org/gnu/emacs/emacs-28.2.tar.gz

 https://github.com/watchpoints/emacs.d/tree/master
 git clone https://github.com/purcell/emacs.d.git ~/.emacs.d
 
//sed -i 's/https/http/g' ~/.emacs.d/lisp/init-elpa.el
//linux命令行代理神器-proxychains
emacs --debug-init

~~~







###

- https://www.cnblogs.com/eat-and-die/p/10309681.html
- https://zhuanlan.zhihu.com/p/114329781
- https://github.com/redguardtoo/emacs.d





## 基本用法



~~~
perf record -a -e kmem:kmalloc --filter 'bytes_alloc == 192' -e kmem:kfree --filter ' ptr != 0' sleep 200


~~~





- Perftools-Specific Install Notes

~~~shell
https://github.com/gperftools/gperftools/blob/master/INSTALL

yum install libunwind(否者heap profiler产生的数据文件都会是空的)

if you use a 64-bit system, we strongly recommend you
install libunwind before trying to configure or install gperftools.

yum install autoconf automake libtool

git clone https://github.com/gperftools/gperftools
sh autogen.sh
./configure 


~~~





## 问题 cpu 高怎么解决



~~~
perf record  -F 99  -g -a  -p 3830098 -- sleep 20
perf report --stdio
perf record  -F 99  -g -e "kmem:*"  -- sleep 20
~~~



### 青铜提问1：

### 用perf report --stdio 跟踪看不到是地址，不是函数，如何根据 地址确定符号？ python3调用了mpirun库

![](https://img-blog.csdnimg.cn/0487773d284f4bfa822014456a658454.png)



https://maimai.cn/web/gossip_detail?gid=31502447&egid=0d093d48911649ddae82276eadffbb9e&encode_id=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlZ2lkIjoiMGQwOTNkNDg5MTE2NDlkZGFlODIyNzZlYWRmZmJiOWUiLCJpZCI6MzE1MDI0NDcsInUiOjQ2NjUxfQ.SvIZlHlCL89en81UUmhqWSvL9ffhOsAODNqjz9TMb38&from=list



王者回答：

1. https://github.com/freelancer-leon/notes/blob/master/kernel/profiling/perf.md
2. https://www.mpich.org/downloads/

mpirun --version
HYDRA build details:
    Version:                                 3.3.2
    Release Date:                            Tue Nov 12 21:23:16 CST 2019





