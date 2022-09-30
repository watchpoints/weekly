---
title: "mimob代码阅读"
date: 2022-02-10
draft: false
categories: ["code_reading"]
---


> 慢慢来，多看、多问、多总结







## 预备知识：

- 源码：https://github.com/watchpoints/miniob
- 2019CMU数据库导论(intro to database systems fall 2019) https://www.bilibili.com/video/av91919329?amp%3Bseid=15982257803189242166
- 2020 MIT 6.824 分布式系统 https://www.bilibili.com/video/BV1R7411t71W/
- https://github.com/CDDSCLab/training-plan
- https://github.com/tidb-incubator/tinykv



### day1

- [ ]  https://github.com/watchpoints/miniob_2022/blob/main/docs/how_to_build.md

- centos7.8系统【建议用8】

```shell

#命令安装版本过低
yum install  cmake
cmake --version 
2.8.12.2 //cmake 版本 >= 3.10
rpm -e cmake-2.8.12.2-2.el7.x86_64
# 下载二进制安装包
wget https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-linux-x86_64.sh
bash cmake-3.24.0-linux-x86_64.sh

export PATH=$PATH:$GOROOT/bin:/root/github/cmake-3.24.0-linux-x86_64/bin
/root/github/cmake-3.24.0-linux-x86_64/bin
cmake --version
cmake version 3.24.0

# gcc过低 需要源码安装
gcc --version
gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-44
tar xzvf gcc-11.3.0.tar.gz
cd gcc-11.3.0

# gcc不同版本切换 

Centos7 使用 yum 安装的 GCC 版本是 4.8.5，蛮老的版本了，现在稍微新一点的库都要求 GCC5 以上，但是重新编译安装最新的GCC又特别麻烦，而且不能在各个版本的 GCC 之间来回切换，不够灵活，刚好有一个叫做 SCL 的软件库可以提供不同版本 GCC 的灵活切换，可以完美解决我们的需求，安装十分简单：
————————————————
版权声明：本文为CSDN博主「thewangcj」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/wcjlyj/article/details/122844189

yum install centos-release-scl scl-utils-build
yum search devtoolset # 搜索 GCC
devtoolset-9-gcc.x86_64 : GCC version 9

yum install devtoolset-9-gcc.x86_64 
scl enable devtoolset-9 # 启用 GC9
```







