---
title: "从青铜到王者系列1：emacs入门教程"
date: 2022-10-29
description: "emacs"
draft: false
tags: ["从青铜到王者"]
---





## 目录

- 如何安装 5分钟完成
- 无插件情况下，直接看官方入门指导 30分钟完成。
- 

## 提问

- 快捷键 `C-h` 怎么输入

## 过程



推荐阅读文章：

https://github.com/redguardtoo/mastering-emacs-in-one-year-guide/blob/master/guide-zh.org



### centos8.6 Installation

Method #01: Installing the Emacs editor using the EPEL repository

~~~
 sudo dnf update
 sudo dnf install epel-release
 sudo dnf install emacs
~~~



Method #02: Install the Emacs editor from the official repository

~~~
https://www.yundongfang.com/Yun45697.html
wget https://ftp.gnu.org/pub/gnu/emacs/emacs-27.2.tar.gz
tar -zxvf emacs-27.2.tar.gz
cd emacs-27.2
./configure
make
make install
~~~



Please remove `~/.emacs` first. `~` means [Home directory](https://en.wikipedia.org/wiki/Home_directory).

Second way is to use [stable setup](https://github.com/redguardtoo/emacs.d/archive/stable.zip),

~~~
cd ~;
git clone https://github.com/redguardtoo/emacs.d.git .emacs.d; 
cd .emacs.d; 
git reset --hard stable
~~~

By default, packages are installed automatically during Emacs startup.

#### 读官方教程（一定要看）

> 完成该教程仅需半小时. 关于 Emacs 多难学的谬论可以休矣.

- emacs -nw -Q

- M-x help-with-tutorial

  按以下步骤阅读教程:

  - 不安装任何插件打开 Emacs, 比如在 Shell 中运行命令 `emacs -nw -Q`
  - `M-x help-with-tutorial` 打开教程

  > 完成该教程仅需半小时. 关于 Emacs 多难学的谬论可以休矣.

  即使你不打算使用 Emacs 默认快捷键, 这步也是必须的, 不要跳过!

  最起码要知道以下命令,

  - `M-x describe-variable`, 快捷键 `C-h v`, 查看变量的文档
  - `M-x describe-function`, 快捷键 `C-h f`, 查看命令的文档
  - `M-x describe-key`, 快捷键 `C-h k`, 查看快捷键的文档











## 资料篇

1. [Emacs 入门指南：Why & How](https://liujiacai.net/blog/2020/11/25/why-emacs/) 【直接看别人文章回让你从入门到放弃感觉，看他推荐的文章2】
2. [一年成为Emacs高手 (像神一样使用编辑器)](https://github.com/redguardtoo/mastering-emacs-in-one-year-guide/blob/master/guide-zh.org)
3. http://localhost:1313/post/2022/10_2022_10_13_how_to-use_emacs/
4. 













