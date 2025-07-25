---
title: 企业级智能体dify：不需要编程
date: 2025-04-12
description: c++
draft: false
tags:
  - 小而美
categories:
  - do book
---



# 一、**用户需求**

# 二、系统设计



![dify流程](https://s2.loli.net/2025/07/24/BRSQoKbjgAFJZx1.png)




# 三、Dify环境搭建
## 3.1  方式1：window环境准备
安装 Dify 之前, 请确保你的机器已满足最低安装要求：
- CPU >= 2 Core
- RAM >= 4 GiB
### window11配置
- 16G内存

![](https://cdn.nlark.com/yuque/0/2025/png/215144/1753269020880-16d1d6de-3304-40a1-8c2a-64a4562187c5.png)

### 安装  Docker Desktop

#### window下运行Linux环境2个方式 WSL2 和Docker

- WSL2（Windows Subsystem for Linux 2）是微软推出的一个兼容层，它允许用户在 Windows 系统上运行 Linux 二进制可执行文件。
- 与 WSL1 相比，WSL2 提供了完整的 Linux 内核支持，能够更好地模拟 Linux 环境，从而为 Docker 提供了理想的运行环境
- Docker Desktop 是 Docker 官方为 Windows 系统提供的桌面应用，它封装了 Docker 的运行环境，使得用户能够在 Windows 上轻松使用 Docker。
- Docker Desktop 提供了两种运行模式：基于 WSL2 的模式和基于 Hyper-V 的模式。虽然 Docker Desktop 可以在没有 WSL2 的情况下运行，但为了更好的性能和兼容性，推荐使用 WSL2。
- Docker Desktop for Windows 需依赖 WSL 2 运行 Linux 容器。若未启用 WSL 2，Docker 仅能运行 Windows 容器，无法支持主流 Linux 应用环境
- Docker 容器直接共享 WSL 2 的 Linux 内核，无需额外虚拟化层，减少资源占用并加速启动


![我直接选择docker方式](https://pic1.zhimg.com/v2-39cffde11c544e2cf9d8ed17a05ce0ca_r.jpg?source=1def8aca)

 ####  启用WSL所需Windows功能

- 选择 控制面板--程序--启动window功能
勾选**Windows虚拟机监控程序平台**和**适用于Linux的Windows子系统**
![开启功能，window11默认已开启](https://s2.loli.net/2025/07/23/gQaei3KoIThVGsS.png)

- 安装wsl
```C++
//安装命令
  wsl --instal
//检查 WSL 版本
wsl --list --verbose
  NAME              STATE           VERSION
* docker-desktop    Running         2 --我已经按照过
  Ubuntu-22.04      Stopped         2 --我已经按照过
```

#### 安装Docker Desktop
- 从官网下载按照
- 进入设置->Resources->WSL integration,勾选Ubuntu，点击Apply & restart

![配置](https://s2.loli.net/2025/07/23/NXS9HvFi1CGsEKe.png)




## 3.2 安装dify

Dify 是一款开源的大语言模型（LLM）应用开发平台。它融合了后端即服务（Backend as Service）和 LLMOps 的理念，使开发者可以快速搭建生产级的生成式 AI 应用。即使你是非技术人员，也能参与到 AI 应用的定义和数据运营过程中

推荐阅读官方文档：
- https://docs.dify.ai/zh-hans/introduction

Docker Compose 部署
```shell
git clone git@github.com:watchpoints/dify.git
//主要开发语言 python +TypeScript


docker compose version
Docker Compose version v2.32.4-desktop.1

Compose 项目是 Docker 官方的开源项目，负责实现对 Docker 容器集群的快速编排。
快速编排：站在项目角度将一组相关联容器整合在一起，对这组容器按照指定顺序进行启动

//确保 Docker Desktop 已启动
docker compose up -d
```

最后检查是否所有容器都正常运行：

``` shell
docker compose ps
```

在这个输出中，
你应该可以看到包括 3 个业务服务 `api / worker / web`，
以及 6 个基础组件 `weaviate / db / redis / nginx / ssrf_proxy / sandbox`

```shell
docker compose ps
NAME                     IMAGE                                       COMMAND                   SERVICE         CREATED         STATUS                   PORTS
docker-api-1             langgenius/dify-api:1.7.0                   "/bin/bash /entrypoi…"   api             5 minutes ago   Up 4 minutes             5001/tcp
docker-db-1              postgres:15-alpine                          "docker-entrypoint.s…"   db              5 minutes ago   Up 5 minutes (healthy)   5432/tcp
docker-nginx-1           nginx:latest                                "sh -c 'cp /docker-e…"   nginx           5 minutes ago   Up 4 minutes             0.0.0.0:80->80/tcp, 0.0.0.0:443->443/tcp
docker-plugin_daemon-1   langgenius/dify-plugin-daemon:0.2.0-local   "/bin/bash -c /app/e…"   plugin_daemon   5 minutes ago   Up 4 minutes             0.0.0.0:5003->5003/tcp
docker-redis-1           redis:6-alpine                              "docker-entrypoint.s…"   redis           5 minutes ago   Up 5 minutes (healthy)   6379/tcp
docker-sandbox-1         langgenius/dify-sandbox:0.2.12              "/main"                   sandbox         5 minutes ago   Up 5 minutes (healthy)
docker-ssrf_proxy-1      ubuntu/squid:latest                         "sh -c 'cp /docker-e…"   ssrf_proxy      5 minutes ago   Up 5 minutes             3128/tcp
docker-weaviate-1        semitechnologies/weaviate:1.19.0            "/bin/weaviate --hos…"   weaviate        5 minutes ago   Up 5 minutes
docker-web-1             langgenius/dify-web:1.7.0                   "/bin/sh ./entrypoin…"   web             5 minutes ago   Up 5 minutes             3000/tcp
docker-worker-1          langgenius/dify-api:1.7.0                   "/bin/bash /entrypoi…"   worker          5 minutes ago   Up 4 minutes             5001/tcp
docker-worker_beat-1     langgenius/dify-api:1.7.0                   "/bin/bash /entrypoi…"   worker_beat     5 minutes ago   Up 4 minutes             5001/tcp
```

通过这些步骤，你可以在本地成功安装 Dify。

访问 Dify

你可以先前往管理员初始化页面设置设置管理员账户

http://localhost/install

## 3.3  OceanBase 数据库 Vector 与 Dify 集成

OceanBase 数据库从 V4.3.3 开始支持向量类型存储、向量索引、embedding 向量检索的能力。可以将向量化后的数据存储在 OceanBase 数据库，供下一步的检索使用。
注意：官方文档：https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000002691348


 
 #### 3.3.1 OBCloud 

- 打开 https://console-cn.oceanbase.com/
- 获取数据库实例连接串
![](https://cnlog-img-xybdiy.oss-cn-shanghai.aliyuncs.com/img/202412081033728.png)

- 添加 IP 地址白名单组 运行客户端访问
- 测试本地是否链接成功


###  3.3.2 window 安装mysql客户端 判断本地链接是否成功



```
SELECT version();
5.7.25-OceanBase-v4.2.1.8

表示 OceanBase 兼容 ​​MySQL 5.7.25 协议及语法​
v4.2.1.8`：OceanBase 核心版本号​   【疑问2】

//下载压缩包
D:\tool\mysql-8.0.31-winx64\bin

 ./mysql -h obmt6605sioq4v28.huawei-cn-north-4.oceanbase.cloud -P 3306 -u test@obmt6605sioq4v28 -D mytest -p


//忘记密码可以重置 U_gMn22| C?3_t1iA
```

>obcloud云版本不正确。

### 3.3.3 修改配置


```c++
# The type of vector store to use.

# Supported values are `weaviate`, `qdrant`, `milvus`, `myscale`, `relyt`, `pgvector`, `pgvecto-rs`, `chroma`, `opensearch`, `oracle`, `tencent`, `elasticsearch`, `elasticsearch-ja`, `analyticdb`, `couchbase`, `vikingdb`, `oceanbase`, `opengauss`, `tablestore`,`vastbase`,`tidb`,`tidb_on_qdrant`,`baidu`,`lindorm`,`huawei_cloud`,`upstash`, `matrixone`.
//将 .env 中的VECTOR_STORE变量的值改为oceanbase，选用 oceanbase 作为 Dify 的向量数据库。
VECTOR_STORE=oceanbase

# OceanBase Vector configuration, only available when VECTOR_STORE is `oceanbase`

OCEANBASE_VECTOR_HOST=obmt6605sioq4v28.huawei-cn-north-4.oceanbase.cloud

OCEANBASE_VECTOR_PORT=2881

OCEANBASE_VECTOR_USER=test@obmt6605sioq4v28

OCEANBASE_VECTOR_PASSWORD=U_gMn22|

OCEANBASE_VECTOR_DATABASE=test

OCEANBASE_CLUSTER_NAME=difyai

OCEANBASE_MEMORY_LIMIT=1G

OCEANBASE_ENABLE_HYBRID_SEARCH=false

重启 Docker 容器，通常方法是先执行：  
docker compose stop  //down 删除意思，不能执行这个
再执行：   

docker compose up -d

```
## 我的疑问：
### 疑问1：

- 疑问：oceanbase 能直接在链接oceanbase吗？我看oceanbase自己单独搞一个自己分支，有什么区别呀？https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000002691348
- 寻找：https://github.com/langgenius/dify/pull/10010

### 疑问2：ob云支持吗？官方公布4.3才支持

- ​**​自建 OceanBase 集群​**​（非 OBCloud）中运行 Dify，则必须升级至 ​**​V4.3.3+​**​ 才能原生支持向量索引。

## 参考

- 手把手教你部署 Dify 到本地环境(Windows系统) https://zhuanlan.zhihu.com/p/1918207520041837100
- # 基于 OceanBase + 低代码平台 Dify 快速构建你的专属智能体
