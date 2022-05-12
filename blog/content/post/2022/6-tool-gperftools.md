---
title: "gperftools"
date: 2022-02-09
description: "gperftools"
draft: false
tags: ["系统设计"]
---











## 基本用法

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













