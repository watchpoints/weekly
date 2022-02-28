---
title: "python学习记录"
date: 2022-02-08
description: ""
draft: false
tags: ["系统设计"]
---

# day1

环境搭建：

window：

1. 删除 Microsoft Store 安装python(无法删除 30分钟) 

   - 直接删除环境变量：%USERPROFILE%\AppData\Local\Microsoft\WindowsApps 路径

   - 下载 https://www.python.org/

   - D:\local\Python\Python310

     

pip 是首选的安装程序。
从Python 3.4开始，它默认包含在Python二进制安装程序中。
D:\local\Python\Python310\Scripts



pip 安装第三方库路径：
D:\local\Python\Python310\Lib\site-packages



pip下载镜像

C:\Users\wangchuanyi\AppData\Roaming\pip\pip.ini
[global]
index-url = http://mirrors.aliyun.com/pypi/simple/
[install]
trusted-host=mirrors.aliyun.com





Python IDE界的倚天和屠龙：Pycharm 和 Jupyter notebook

习惯在学习，数据探索的阶段使用*Anaconda/Jupyter notebook* (交互式，可视性强，方便阅读），实际做项目时用*PyCharm*





CentOS 7 升级Python2.7.5到Python3.10

编译安装Python3.10
~~~
# 下载 Python-3.10.2.tgz

# 解压
tar -zxf Python-3.10.2.tgz
# 安装依赖包
yum install zlib-devel bzip2-devel openssl-devel ncurses-devel sqlite-devel readline-devel tk-devel gcc  libffi-devel
# 进入python目录
cd Python-3.10.2 
# 编译
./configure --prefix=/usr/local/python3.10(目录)
make && make install
~~~



将系统默认的python2备份
mv /usr/bin/python /usr/bin/python.bak

创建新的软连接
ln -s /usr/local/python3.10/bin/python3.10 /usr/bin/python
ln -s /usr/local/python3.10/bin/pip3 /usr/bin/pip

更改yum配置修改yum不能用的问题
python -V
Python 2.7.5
vi /usr/bin/yum
#!/usr/bin/python --> #!/usr/bin/python2.7
vi /usr/libexec/urlgrabber-ext-down
#!/usr/bin/python --> #!/usr/bin/python2.7

pip位置
ln -s /usr/local/python3.10/bin/pip3 /usr/bin/pip

















