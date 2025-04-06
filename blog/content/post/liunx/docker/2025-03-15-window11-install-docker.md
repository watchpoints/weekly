---
title: 从青铜到王者系列(1):手把手教你用WSL 2在Windows 11家庭版上安装Docker，开发必备教程！
date: 2022-11-20
description: ""
draft: false
tags:
  - book
---

## 如何学习

~~~
 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题 

 二、这个技术的优势和劣势分别是什么 

三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

四、技术的组成部分和关键点。

五、技术的底层原理和关键实现

六、已有的实现和它之间的对比
~~~


 【图文硬核】手把手教你用WSL 2在Windows 11上安装Docker，开发必备教程！

  英文：Installing Docker on Windows 11 using WSL 2: A Step-by-Step Guide

### 读者提问： 

- Windows下想使用Linux环境，WSL、Docker、VM应该怎么选择？
-  windows上跑linux用虚拟机好还是WSL好？
- Win11家庭版如何开启Hyper-V?




![](https://picx.zhimg.com/v2-39cffde11c544e2cf9d8ed17a05ce0ca_r.jpg?source=1def8aca)


> 从招聘岗位考虑，看别人招聘什么我就学什么


**技能要求：**

- 熟练掌握CI/CD自动化持续集成流程与工具，具备丰富的实践经验。
    
- 精通Docker技术，包括Dockerfile编写、镜像管理与容器部署。
    
- 熟练使用Docker Compose进行多容器编排与管理。
    
- 有Kubernetes实际搭建与使用经验者优先。

Docker 和 CI/CD关系？

- **持续集成(Continuous integration)** 是一种软件开发实践，每次集成都通过自动化的构建（包括编译，发布，自动化测试）来验证，从而尽早地发现集成错误。

- **持续部署(continuous deployment)** 是通过自动化的构建、测试和部署循环来快速交付高质量的产品

  
- **Docker** 是一种开源的容器化平台，通过将应用程序及其依赖打包成独立的容器，实现跨环境的快速部署和一致性运行。

Docker 为 CI/CD 提供了标准化的环境和高效的部署方式，而 CI/CD 则通过自动化流程实现 Docker 容器的快速交付，二者结合显著提升了开发效率和软件质量。

> 从市场上有流程产品

     GitHub [Actions]是 GitHub 推出的一款 CI/CD 工具
     
	
自然选择Docker



## 一、Docker安装的必备条件

因为Windows上安装Docker实际上是基于Hyper-V或者WSL2这两项虚拟化技术

### 预备知识:推荐优先使用 WSL 2 作为后端。



Windows 11 用户在使用 Docker Desktop 时，可以选择 WSL 2 或 Hyper-V 作为后端运行环境。这两种方案各有特点，你可以根据自身需求进行选择：

- WSL 2 比较适合开发环境。
- Hyper-V 则更适用于生产环境，特别是在需要高性能和稳定性的情况下。

### 1.1 基础要求

不管选择哪种后端方案，你的设备都必须满足以下基本条件：

| 前提条件      | 要求                                         |
| --------- | ------------------------------------------ |
| 系统版本      | Windows 11 家庭版、专业版、企业版或教育版（22H2 或更高版本）     |
| CPU       | 12th Gen Intel(R) Core(TM) i5-12500H（当前机器） |
| 内存        | 至少 4GB 系统内存                                |
| BIOS/UEFI | 开启硬件虚拟化支持                                  |
|           |                                            |

### 1.2 后端要求

| WSL 2 后端 | Hyper-V 后端 | Hyper-V 后端     |
| -------- | ---------- | -------------- |
| 后端要求     | 启用 WSL 2   | 启用 Hyper-V 和容器 |


- Windows 11 (64-bit)

（Win + I）命令查看

Windows 11家庭版中，Hyper-V功能默认不可用，
因为微软仅在专业版、企业版和教育版中预装了Hyper-V。
家庭版用户无法直接通过“启用或关闭Windows功能”启用Hyper-V。


- Hardware virtualization enabled at the BIOS level


使用纯文本和ASCII字符描述的 **WSL、Docker与Hyper-V的关系图**

+-----------------------+
| Windows 11 操作系统   |
+-----------------------+
          |
          v
+-----------------------+
|       Hyper-V         |  <- 提供虚拟化支持
+-----------------------+
          |
          v
+-----------------------+
|        WSL 2          |  <- 运行Linux内核
+-----------------------+
          |
          v
+-----------------------+
|   Docker Desktop      |  <- 管理容器
+-----------------------+
          |
          v
+-----------------------+
| Docker 容器（Linux）  |  <- 运行应用程序
+-----------------------+


- **WSL 2** 是 **Hyper-V** 的上层应用，依赖Hyper-V的虚拟化功能。
    
- **Docker Desktop** 使用 **WSL 2** 作为后端，通过WSL 2运行Linux容器。
    
- **Hyper-V** 是底层虚拟化技术，支持WSL 2和Docker Desktop


说明：

WSL2 简介

WSL2 是适用于 Windows 11 的 Windows 子系统的一个新版本，它提供了更完整的 Linux 内核功能以及更好的系统性能。与传统的 Windows 应用运行环境不同，

WSL2 相比 WSL1 具有以下显著优势：

**完整的 Linux 内核：WSL2 包含了一个真正的 Linux 内核，这使得它能够运行更多类型的 Linux 应用，并且在系统调用和****文件系统****方面具有更好的兼容性和性能表现。例如，一些需要特定 Linux 内核特性的应用或工具，如 Docker 所依赖的一些底层功能，在 WSL2 中能够更好地运行。

**显著提升的文件系统性能：WSL2 采用了新的虚拟硬盘格式（.vhdx）以及 9P 协议来与 Windows 文件系统交互，这大大提高了文件系统的读写速度。在使用 Docker 时，容器内文件的操作速度更快，从而提升了容器的启动速度和整体性能。**


### **第一步： 开启虚拟化**

**开启虚拟化(默认已经开启)：**


否开启虚拟化可以直接在“任务管理器（启动：**CTRL + ALT + Delete**）--性能--CPU“中查看

 ![](https://picx.zhimg.com/80/v2-3e6ca63deb5fcdd134423575fc669926_1440w.webp?source=1def8aca)

如果虚拟化显示”已启用“就说明没问题，如果没启用就需要进主板BIOS中开启


在BIOS模式下开启CPU的虚拟化  开机启动的时候按下F2或者Del即可进入Blos模式。
然后点击高级，点intel虚拟化技术，点击开启，全部操作完成后，按下F10确认保存，即可

### **第二步： 开启Hyper-V**


在Windows 11家庭版中，Hyper-V功能默认不可用，因为微软仅在专业版、企业版和教育版中预装了Hyper-V。家庭版用户无法直接通过“启用或关闭Windows功能”启用Hyper-V。

 解决方法：

如果您确实需要在Windows 11家庭版中启用Hyper-V，可以通过以下步骤尝试强制安装Hyper-V功能：

1. **创建批处理文件**：
    
    - 在桌面上新建一个文本文件，命名为`Hyper-V.cmd`。
        
~~~
pushd "%~dp0"
 
dir /b %SystemRoot%\servicing\Packages\*Hyper-V*.mum >hyper-v.txt
 
for /f %%i in ('findstr /i . hyper-v.txt 2^>nul') do dism /online /norestart /add-package:"%SystemRoot%\servicing\Packages\%%i"
 
del hyper-v.txt
 
Dism /online /enable-feature /featurename:Microsoft-Hyper-V-All /LimitAccess /ALL

~~~
        
2. **以管理员身份运行**：
    
    - 右键点击`Hyper-V.cmd`文件，选择“以管理员身份运行”。（大概5-10分钟）
        
    - 按提示操作，完成后重启计算机。
        
3. **检查是否成功**：
    
    - 重启后，按`Win + R`键，输入`optionalfeatures`，打开“启用或关闭Windows功能”窗口。（控制面板中也打开）
        
    - 检查Hyper-V是否已勾选并启用。





![]()
![](https://picx.zhimg.com/80/v2-e6dfcb630336ddfc24e1c2d3db384537_1440w.webp?source=1def8aca)

 
 
 
 整理: 如何检查是否成功启用了Hyper-V？


**1、检查Hyper-V服务是否启动**

按**Win+R**输入**services.msc**并按**Enter**键打开服务窗口，检查全部以Hyper-V开头的服务是否处于正在运行状态。

**2、检查Hyper-V启用状态**

在PowerShell中支持此命令：**Get-WindowsOptionalFeature -Online -FeatureName Microsoft-Hyper-V**，检查状态是否为已启用。

**3、检查Hyper-V能否开启**

直接在搜索框中输入**Hyper-V**，检查Hyper-V管理器选项是否出现

### 第三步 安装 WSL 2
- **启用 linux 子系**

 WSL 2 allows you to run Linux distributions natively on Windows,

用管理员身份打开 powershell ，然后输入如下命令回车：
```powershell
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
```

- **启动虚拟机功能（Enable the Virtual Machine Platform**）

由于我们后续要安装 docker desktop，需要依赖于 wsl 2，所以需要启动 Windows 自带的虚拟机平台功能，


```powershell
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```


Restart your computer


### 第四步： 安装 Linux 发行版


Windows Subsystem for Linux (WSL) is a feature of Windows that allows you to run a Linux environment on your Windows machine, without the need for a separate virtual machine or dual booting. WSL is designed to provide a seamless and productive experience for developers who want to use both Windows and Linux at the same time.

在 Windows 11 上安装 WSL 2 的过程如下：

1. 启用 WSL ---完成
2. 启用“虚拟机平台”--完成
3. 设置 WSL 2 为默认值
4. 安装 Linux 发行版

```powershell
wsl --update
wsl --set-default-version 2
//命令安装太慢 
//下载离线版本 https://github.com/microsoft/WSL/releases



wsl --list --online
//View the list of available Linux distributions
//看看微软为我们提供了哪些可用 Linux 发行版
//当前默认的 Ubuntu 版本为最新的 LTS 版本，即 22.04.1


wsl --install -d Ubuntu  
//Install your chosen distribution
//耗时比较久 命令方式比较慢，改为离线版本
//  Microsoft Store Installation---# Ubuntu 22.04.5 LTS
//https://apps.microsoft.com/detail/9pn20msr04dw?hl=zh-cn&gl=US
```

![image.png](https://s2.loli.net/2025/03/15/sNePgKTAtdu86bY.png)


Installing, this may take a few minutes...
Please create a default UNIX user account. The username does not need to match your Windows username.
For more information visit: https://aka.ms/wslusers
Enter new UNIX username: money/123



### 第五步：WSL2用法


wsl --status
默认分发: docker-desktop
默认版本: 2


Windows WSL配置ubuntu环境并登录

方法一：使用Ubuntu自带的终端窗口
方法二：使用Windows Terminal登录




wsl 控制台与 win10 之间复制粘贴的实现

## 二、 与Docker Desktop集成


![](https://img.sysgeek.cn/img/logo/docker-1.jpg)

- 在 Linux 系统上，安装 Docker 非常简单——你只需要在「终端」中输入几行命令，系统就能自动完成安装和配置，So Easy🎊！
- 但在 Windows 11 系统上，情况就变了🌚——你需要依赖 WSL2 或者 Hyper-V 虚拟化平台作为后端，才能运行 Docker Desktop。


下载
 - https://www.docker.com/products/docker-desktop/  不要选择 实验班版本，window11 提示无法安装
 -  安装过程
 ![docker](https://s2.loli.net/2025/03/15/Vobqs64hGJcLfmd.png)

Go to Settings > General

	
Ensure Use the WSL2-based engine is checked and go to Settings > Resources > WSL Integration lastly enable integration with your installed Ubuntu distribution

![[Pasted image 20250315160756.png]]

## 三、与VSCODE 集成

## 设置你最喜欢的代码编辑器

建议使用 Visual Studio Code 或 Visual Studio，因为它们直接支持使用 WSL 进行远程开发和调试。 Visual Studio Code 使你能够将 WSL 用作功能完备的开发环境。 Visual Studio 提供了对 C++ 跨平台开发的本机 WSL 支持


GitHub 配置ssh 免密码登录
```text
ssh-keygen -t rsa -C "wang_cyi@163.com"
```
git设置：

git config --global user.email wangjia@163.com
git config --global user.name wangjia

冲突解决：
git reset --hard  6f684a48c4cec1a9b2f3c66f94dde793a84e5a76
//Resets the index and working tree.


WSL+VSCode食用指南

 一、背景
    通过适用于 Linux 的 Windows 子系统使用 Visual Studio Code
二、WSL + VSCode 原理架构

 三、开吃
    https://learn.microsoft.com/zh-cn/windows/wsl/tutorials/wsl-vscode
     https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack

 插件
     Code Spell Checker



VSCode 在新窗口打开文件，不覆盖旧窗口

 一、背景
     vscode 每次看代码时候，打开一个文件，覆盖原有的文件
二、工具

. 在 **用户—>工作台—>编辑管理** 选项卡下拉找到`Enable Preview`（**或者直接搜索Enable Preview，找到编辑管理里的Enable Preview**）**，去掉这个选项的勾**



# FQA 


1. wsl2模式下 ping 不同网络？

wsl -l -v
  NAME              STATE           VERSION
* docker-desktop    Stopped         2
  Ubuntu-22.04      Running         2
networkingMode=Mirrored # 镜像网络模式 配置镜像模式网络：直接从局域网连接到 WSL

wsl --shutdown


### 3   WSL2 Ubuntu系统占用C盘空间过大的问题分析与解决

轻松搬迁！教你如何将WSL从C盘迁移到其他盘区，释放存储空间！

```shell

以ubuntu22.04为例：

查看WSL分发版本
在Windows cmd中输入如下命令
wsl -l -v

Ubuntu-22.04      Running         2
wsl --shutdown
输入 `wsl --shutdown` 使其停止运行

导出分发版为tar文件到d盘
wsl --export Ubuntu-22.04 D:\Ubuntu_WSL\Ubuntu.tar

注销当前分发版
wsl --unregister Ubuntu-22.04

重新导入并安装WSL

wsl --import Ubuntu-22.04 D:\Ubuntu_WSL D:\Ubuntu_WSL\Ubuntu.tar --version 2

设置默认登陆用户为安装时用户名
ubuntu2204 config --default-user money

请注意，这里的发行版名称的版本号是纯数字，比如Ubuntu-22.04就是Ubuntu2204。

删除tar文件(可选)
del d:\wsl-ubuntu.tar


https://learn.microsoft.com/zh-cn/windows/wsl/basic-commands#run-as-a-specific-user
```

## 参考

[1] Hyper-V 技术概述 done
[2] 如何启动 Windows 11 任务管理器 done
[3] https://www.linkedin.com/pulse/installing-docker-windows-11-using-wsl-2-step-by-step-ankit-lodaf
[4] windows 上安装 docker有什么作用？ https://www.zhihu.com/question/3931662

【5】 https://www.cnblogs.com/ittranslator/p/14128570.html
     https://learn.microsoft.com/en-us/windows/wsl/about
【6】 https://zhuanlan.zhihu.com/p/617357186

