---
title: "python入门教程"
date: 2021-06-06
description: "python入门教程"
draft: false
categories: ["code_reading"]
---







# 第一天



1. window python 环境安装

- 官网地址：  https://www.python.org/downloads/release/python-396/

python环境变量：path  D:\local\python3

pip环境变量 path   D:\local\python3\Scripts

window可以默认按照自动设置。其他手工设置

- 用于检测当前Windows环境中是否已经安装pip。

  python -m pip --version

 pip 21.1.3 from D:\local\python3\lib\site-packages\pip (python 3.9)

pip --version
pip 21.1.3 from d:\local\python3\lib\site-packages\pip (python 3.9)



where python
D:\local\python3\python.exe



d:\local\python3\python.exe -m pip install --upgrade pip



- pip show 包名来查看包的相关信息

    pip show selenium
Name: selenium
Version: 3.141.0
License: Apache 2.0
Location: d:\local\python3\lib\site-packages 【第三方包的按照路径】


2. centos7升级python3

wget https://www.python.org/ftp/python/3.9.7/Python-3.9.7.tgz
tar xzvf Python-3.9.7.tgz
mkdir /usr/local/python3/
./configure --prefix=/usr/local/python3
make install
mv /usr/bin/python /usr/bin/python.bak
mv /usr/bin/pip /usr/bin/pip.bak
ln -s /usr/local/python3/bin/python3.8 /usr/bin/python
ln -s /usr/local/python3/bin/pip3 /usr/bin/pip
python -V
vim /usr/bin/yum
把最顶部的#!/usr/bin/python改为：#!/usr/bin/python2.7


centos 下 yum安装python3
yum install -y python36
python3 -V
pip3 install selenium
python3 ./toutiao1.0.py
# 项目



## 参考第三方项目

- https://github.com/chg-hou/EnMicroMsg.db-Password-Cracker 【评审中】
- https://github.com/ppwwyyxx/wechat-dump 【评审中】



## day2



- python -m site