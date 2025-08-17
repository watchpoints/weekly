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

开始时间：2025-7-28

结束时间：



# 一、**用户需求**

# 二、系统设计



![dify流程](https://s2.loli.net/2025/07/24/BRSQoKbjgAFJZx1.png)


## 2.1 从零开始学 Dify（一）：Dify架构是什么

### 1.  为什么要了解Dify架构，直接使用不好吗？
- 定位：实现Dify上下游生态系统闭环，围绕工作流和知识库，高效进行AIGC内容创作。
- 服务：Dify源码剖析及答疑
- 输出：系统性输出《Dify应用操作和源码剖析》教程；通过Dify扩展系统平台构建AIGC内容生态
- 具体问题：部署Dify启动至少启动12个容器，不容易维护，那些必须的启动，那些不需要启动的了解清楚。
- 具体问题: Dify提供多个向量数据库，如何选择
### 2. Dify全局视图


Dify 是一款开源的大语言模型（LLM）应用开发平台。它融合了后端即服务（Backend as Service）和 LLMOps 的理念，使开发者可以快速搭建生产级的生成式 AI 应用。即使你是非技术人员，也能参与到 AI 应用的定义和数据运营过程中

![](https://s2.loli.net/2025/07/31/AthEqWFXonHlZgC.png)


启动服务后：docker-compose -f docker/docker-compose.yaml up -d

>一看就傻眼了，这么多，一看就是不好的设计。 如下下面，服务依赖太严重了。

``` shell
PS D:\code\python\dify> docker ps
CONTAINER ID   IMAGE                                       COMMAND                   CREATED             STATUS
          PORTS                                      NAMES
d29d1942021d   nginx:latest                                "sh -c 'cp /docker-e…"   About an hour ago   Up 4 minutes             0.0.0.0:80->80/tcp, 0.0.0.0:443->443/tcp   docker-nginx-1
5c7a0d286643   langgenius/dify-api:1.7.1                   "/bin/bash /entrypoi…"   About an hour ago   Up 4 minutes             5001/tcp                                   docker-worker_beat-1
7821b5ad58ed   langgenius/dify-api:1.7.1                   "/bin/bash /entrypoi…"   About an hour ago   Up 4 minutes             5001/tcp                                   docker-worker-1
0cff35ca8f19   langgenius/dify-api:1.7.1                   "/bin/bash /entrypoi…"   About an hour ago   Up 4 minutes             5001/tcp                                   docker-api-1
a7a976a450a4   langgenius/dify-plugin-daemon:0.2.0-local   "/bin/bash -c /app/e…"   About an hour ago   Up 4 minutes             0.0.0.0:5003->5003/tcp                     docker-plugin_daemon-1
ce80c374d103   postgres:15-alpine                          "docker-entrypoint.s…"   About an hour ago   Up 4 minutes (healthy)   5432/tcp                                   docker-db-1
127720bac742   ubuntu/squid:latest                         "sh -c 'cp /docker-e…"   About an hour ago   Up 4 minutes             3128/tcp                                   docker-ssrf_proxy-1
af6af9b1d5d1   langgenius/dify-sandbox:0.2.12              "/main"                   About an hour ago   Up 4 minutes (healthy)                                              docker-sandbox-1
8608eefde902   langgenius/dify-web:1.7.1                   "/bin/sh ./entrypoin…"   About an hour ago   Up 4 minutes             3000/tcp                                   docker-web-1
72e78ae3761b   redis:6-alpine                              "docker-entrypoint.s…"   About an hour ago   Up 4 minutes (healthy)   6379/tcp                                   docker-redis-1
00febcb8efbe   semitechnologies/weaviate:1.19.0            "/bin/weaviate --hos…"   6 days ago          Up 4 minutes                                                        docker-weaviate-1
```

整理如下：
1. **API服务** (`docker-api-1`): Dify的核心API服务
2. **Worker服务** (`docker-worker-1`): 处理后台任务
3. **Redis** (`docker-redis-1`): 用于缓存和消息队列
4. **Web前端** (`docker-web-1`): Dify的Web界面
5. **Nginx** (`docker-nginx-1`): 作为反向代理服务器

6. **OceanBase** (`oceanbase`): 你确认使用的主要数据库和向量数据库
7. **Sandbox** (`sandbox`): 代码执行环境
8. **SSRF代理** (`ssrf_proxy`): 安全代理，防止服务器端请求伪造攻击
	​**​核心作用​**​：为 SSRF（服务器端请求伪造）代理提供专用网络通道，确保所有外部请求（如插件调用、API 访问）必须经过安全代理审查，防止恶意攻

9. **Plugin Daemon** (`plugin_daemon`): 插件系统

**停止非核心容器​**​：

docker stop docker-ssrf_proxy-1 docker-sandbox-1  docker-worker_beat-1 

docker stop docker-ssrf_proxy-1 docker-sandbox-1  docker-worker_beat-1 

docker stop docker-ssrf_proxy-1 docker-sandbox-1  docker-worker_beat-1 

- **🚫 不可行​**​：
    - 删除 PostgreSQL → ​**​系统崩溃​**​（核心配置和用户数据丢失）
    - 删除 Weaviate → ​**​AI 功能失效​**​（知识库检索、语义搜索不可用）
- ​**​⚠️ 替代方案​**​：
    - PostgreSQL 可替换为其他 SQL 数据库（如 MySQL），但需修改 Dify 数据层代码
    - **Weaviate 可替换为 Milvus/Qdrant 等向量数据库，但需重新配置向量服务连接**

测试访问： http://localhost/install 
输入 用户名：wang_cyi   密码：qwer1234

访问：http://localhost/apps

### 3.  组件使用说明

##### 1 **web服务：前端应用**
```c++
services:
  # API service
  api:
    image: langgenius/dify-api:1.7.1
    restart: always //确保容器在任何情况下（例如崩溃、系统重启等）都自动重启。
   depends_on:
      db:
        condition: service_healthy
      redis:
        condition: service_started
```

redis` 服务依赖
- `condition: service_started` 表示当前服务只需要等待 `redis` 服务启动完成，不需要等待它通过健康检查。
- 这意味着一旦 Redis 容器启动，当前服务就可以开始启动，不管 Redis 是否真正准备好接受连接。
db` 服务依赖
- `condition: service_healthy` 表示当前服务会等待 `db` 服务不仅启动完成，而且必须通过健康检查（health check）才会开始启动。
- 这确保了当前服务只有在数据库完全准备好并且可以接受连接时才会启动。


##### 2.db服务：Postgres数据库

```c
 db:
    image: postgres:15-alpine 
    restart: always //启动 PostgreSQL 数据库
    environment:
      POSTGRES_USER: ${POSTGRES_USER:-postgres}

      POSTGRES_PASSWORD: ${POSTGRES_PASSWORD:-difyai123456}

      POSTGRES_DB: ${POSTGRES_DB:-dify}

      PGDATA: ${PGDATA:-/var/lib/postgresql/data/pgdata}

    healthcheck:
      test: [ 'CMD', 'pg_isready', '-h', 'db', '-U', '${PGUSER:-postgres}', '-d', '${POSTGRES_DB:-dify}' ]
```


##### 3.redis服务：Redis缓存

``` python
 # The redis cache.
  redis:
    image: redis:6-alpine
     //使用名为 `redis` 的 Docker 镜像，
     //版本为 `6-alpine`，启动 Redis 缓存服务
    restart: always

    environment:
      REDISCLI_AUTH: ${REDIS_PASSWORD:-difyai123456}

    volumes:
      # Mount the redis data directory to the container.
      - ./volumes/redis/data:/data
       #配置，将本地目录 `./volumes/redis/data` 挂载到容器中的 `/data` 目录，以持久化存储 Redis 数据。
    # Set the redis password when startup redis server.
    command: redis-server --requirepass ${REDIS_PASSWORD:-difyai123456}
    #启动 Redis 服务器时设置密码 `difyai123456`

    healthcheck:
      test: [ 'CMD', 'redis-cli', 'ping' ]
```

##### 4.weaviate服务：向量数据库（默认启动，配置其他的也启动）

``` python
# The Weaviate vector store.
  weaviate:
    image: semitechnologies/weaviate:1.19.0
    profiles:

      - ''
      - weaviate
    restart: always
    volumes:
      # Mount the Weaviate data directory to the con tainer.
      - ./volumes/weaviate:/var/lib/weaviate

    environment:
      # The Weaviate configurations
      # You can refer to the [Weaviate](https://weaviate.io/developers/weaviate/config-refs/env-vars) documentation for more information.

      PERSISTENCE_DATA_PATH: ${WEAVIATE_PERSISTENCE_DATA_PATH:-/var/lib/weaviate}

      QUERY_DEFAULTS_LIMIT: ${WEAVIATE_QUERY_DEFAULTS_LIMIT:-25}

      AUTHENTICATION_ANONYMOUS_ACCESS_ENABLED: ${WEAVIATE_AUTHENTICATION_ANONYMOUS_ACCESS_ENABLED:-false}

      DEFAULT_VECTORIZER_MODULE: ${WEAVIATE_DEFAULT_VECTORIZER_MODULE:-none}

      CLUSTER_HOSTNAME: ${WEAVIATE_CLUSTER_HOSTNAME:-node1}

      AUTHENTICATION_APIKEY_ENABLED: ${WEAVIATE_AUTHENTICATION_APIKEY_ENABLED:-true}

      AUTHENTICATION_APIKEY_ALLOWED_KEYS: ${WEAVIATE_AUTHENTICATION_APIKEY_ALLOWED_KEYS:-WVF5YThaHlkYwhGUSmCRgsX3tD5ngdN8pkih}

      AUTHENTICATION_APIKEY_USERS: ${WEAVIATE_AUTHENTICATION_APIKEY_USERS:-hello@dify.ai}

      AUTHORIZATION_ADMINLIST_ENABLED: ${WEAVIATE_AUTHORIZATION_ADMINLIST_ENABLED:-true}

      AUTHORIZATION_ADMINLIST_USERS: ${WEAVIATE_AUTHORIZATION_ADMINLIST_USERS:-hello@dify.ai}
```

当你使用 Docker Compose 命令时，可以通过 `--profile` 参数指定要启动的配置文件


对于 Weaviate 服务，它被分配了两个 profiles：
- 空字符串 `''`：表示这个服务在没有指定任何 profile 时也会启动（默认启动）
- `weaviate`：表示这个服务属于 "weaviate" profile
##### 5.sandbox服务：沙盒

- ​**​功能​**​：安全代码执行环境，支持 Python 工具链、数据预处理等
- ​**​删除建议​**​：⚠️ 若未使用“代码解释器”或自定义工具可删，否则代码类功能失效

##### 6.ssrf_proxy服务：SSRF代理服务
##### 7.ssrf_proxy_network：SSRF 安全隔离网络​**​

- ​**​核心作用​**​：为 SSRF（服务器端请求伪造）代理提供专用网络通道，确保所有外部请求（如插件调用、API 访问）必须经过安全代理审查，防止恶意攻击。
- ​**​功能细节​**​：
    - ​**​流量过滤​**​：所有从容器发起的出站请求（如访问外部 API、下载插件）会被强制路由到 `ssrf_proxy` 容器（即 Squid 代理），由代理验证目标地址合法性后再转发
    - ​**​风险隔离​**​：将高风险操作（如联网请求）与核心服务（API/DB）隔离，避免攻击者通过容器漏洞直接访问内网资源
    - ​**​插件安全​**​：若插件需调用外部服务（如 Google 搜索），必须通过此代理，否则请求会被拒绝
##### 8 Dify环境搭建​**​`docker-worker_beat-1`​**​

- ​**​功能​**​：定时任务调度器（Celery Beat），用于周期任务（如数据同步）
- ​**​删除建议​**​：⚠️ 若无需定时任务（如每日数据备份）可删

​##### 9 **​`docker-plugin_daemon-1`​**​

- ​**​功能​**​：插件守护进程，管理第三方工具的生命周期（如搜索引擎、绘图工具）
    
- ​**​删除建议​**​：⚠️ 若未启用任何插件（如搜索、多模态）可删，否则插件无法运行

##### 9. plugin_daemon

```C
 # plugin daemon

  plugin_daemon:

    image: langgenius/dify-plugin-daemon:0.2.0-local

    restart: always
    
   depends_on:
      db:
        condition: service_healthy
```
### 4. 向量数据库配置


默认启动weaviate

 Weaviate:面向AI时代的开源向量数据库
```c
# ------------------------------
# Vector Database Configuration
# ------------------------------
# The type of vector store to use.
# Supported values are `weaviate`, `qdrant`, `milvus`, `myscale`, `relyt`, `pgvector`, `pgvecto-rs`, `chroma`, `opensearch`, `oracle`, `tencent`, `elasticsearch`, `elasticsearch-ja`, `analyticdb`, `couchbase`, `vikingdb`, `oceanbase`, `opengauss`, `tablestore`,`vastbase`,`tidb`,`tidb_on_qdrant`,`baidu`,`lindorm`,`huawei_cloud`,`upstash`, `matrixone`.

VECTOR_STORE=weaviate

# Prefix used to create collection name in vector database

VECTOR_INDEX_NAME_PREFIX=Vector_index


# ------------------------------

# docker env var for specifying vector db type at startup

# (based on the vector db type, the corresponding docker

# compose profile will be used)

# if you want to use unstructured, add ',unstructured' to the end

# ------------------------------

COMPOSE_PROFILES=${VECTOR_STORE:-weaviate}
```



## 2.1 从零开始学 Dify（一）：基于向量数据库混合检索 + Dify 构建知识库

### 学习目标
- [ ]  什么是向量数据库
- [ ] 为什么个人window笔记本不选择 ob数据库

### 1. 什么是向量数据库


传统数据库的局限性 
- 无法理解非结构化数据的意义
- 无法有效搜索或分类复杂的文档、音频和图像数据。

向量数据库也叫矢量数据库，是一种以数学向量的形式存储数据集合的数据库

向量数据库的兴起主要源于大模型 embedding 的应用
 
**Weaviate** 和 **ChromaDB** 

| 向量数据库    | URL                                                                                                             | GitHub Star | Language |
| -------- | --------------------------------------------------------------------------------------------------------------- | ----------- | -------- |
| chroma   | [https://github.com/chroma-core/chroma](https://link.zhihu.com/?target=https%3A//github.com/chroma-core/chroma) | 7.4K        | Python   |
| weaviate | [https://github.com/weaviate/weaviate](https://link.zhihu.com/?target=https%3A//github.com/weaviate/weaviate)   | 6.9K        | Go       |

Weaviate 更适合：
1. 复杂语义搜索：需要结合关键词、向量、结构化过滤（如电商多条件商品搜索）。
2. 知识图谱应用：存储和查询实体关系（如人物-公司-地点的关联）。
3. 推荐系统：基于用户行为向量和物品属性的混合推荐。
4. 企业级应用：需要分布式扩展、高可用性和持久化保障。
ChromaDB 更适合：
- 快速原型开发：AI实验或小规模项目，需快速集成向量检索。
- 嵌入缓存层：作为大模型（如LLM）的短期记忆存储（如对话历史）。
- 轻量级应用：数据量小（百万级以下），无需复杂查询逻辑。
- 本地开发环境：单机运行，无需复杂运维。

### 2. 为什么个人window笔记本不选择 OceanBase数据库（资源限制）

### 2.1 目前的OceanBase云默认是4.2版本，不支持向量数据库

OceanBase 数据库从 V4.3.3 开始支持向量类型存储、向量索引、embedding 向量检索的能力。可以将向量化后的数据存储在 OceanBase 数据库，供下一步的检索使用。
注意：官方文档：https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000002691348

 
 OBCloud 
- 打开 https://console-cn.oceanbase.com/
- 获取数据库实例连接串
![](https://cnlog-img-xybdiy.oss-cn-shanghai.aliyuncs.com/img/202412081033728.png)

- 添加 IP 地址白名单组 运行客户端访问
- 测试本地是否链接成功

 **window 安装mysql客户端 判断本地链接是否成功**

```

//下载压缩包
D:\tool\mysql-8.0.31-winx64\bin

./mysql -h obmt6605sioq4v28.huawei-cn-north-4.oceanbase.cloud -P 3306 -u admin@obmt6605sioq4v28 -D mytest -p

SELECT version();

5.7.25-OceanBase-v4.2.1.8

表示 OceanBase 兼容 ​​MySQL 5.7.25 协议及语法​

v4.2.1.8：OceanBase 核心版本号​ 

//忘记密码可以重置 U_gMn22| C?3_t1iA
```

避坑：


> [!NOTE] obcloud 目前默认4.2版本 不是4.3
> <ClusterVersionException: (code=ErrorCode.NOT_SUPPORTED, message=OceanBase Vector Store is not supported because cluster version is below 4.3.3.0.)

![image.png](https://s2.loli.net/2025/07/26/sPoS5fauCLNBi2k.png)


### 2.2 window笔记本16G内存，打开电脑利用10G，内存不够

####   方式1：window环境准备
安装 Dify 之前, 请确保你的机器已满足最低安装要求：
- CPU >= 2 Core
- RAM >= 4 GiB
 window11配置
- 16G内存

![](https://cdn.nlark.com/yuque/0/2025/png/215144/1753269020880-16d1d6de-3304-40a1-8c2a-64a4562187c5.png)

##### 安装  Docker Desktop

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


安装dify



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


#####  2.3  OceanBase 数据库 Vector 与 Dify 集成


```c++

D:\code\python\dify\docker
.env

# OceanBase Vector configuration, only available when VECTOR_STORE is `oceanbase`

OCEANBASE_VECTOR_HOST=oceanbase

OCEANBASE_VECTOR_PORT=2881

OCEANBASE_VECTOR_USER=root@test

OCEANBASE_VECTOR_PASSWORD=difyai123456

OCEANBASE_VECTOR_DATABASE=test

OCEANBASE_CLUSTER_NAME=difyai

OCEANBASE_MEMORY_LIMIT=6G

OCEANBASE_ENABLE_HYBRID_SEARCH=false

重启 Docker 容器，通常方法是先执行：  
docker compose stop  //down 删除意思，不能执行这个
再执行：   


docker-compose up -d # 创建并后台运行所有容器


http://localhost/apps
```


> [!NOTE] obcloud 目前默认4.2版本 不是4.3
> <ClusterVersionException: (code=ErrorCode.NOT_SUPPORTED, message=OceanBase Vector Store is not supported because cluster version is below 4.3.3.0.)

##### 查缺补漏 

参考资料
1. Docker-Compose 入门到精通 （图解+秒懂+史上最全）
2. docker系列：docker-compose用法详解
- 一般一个系统的部署可能涉及到tomcat、mysql、redis、nginx、mongodb等应用，如果采用容器部署的话可能涉及十多个容器（中小项目），每个容器都单独维护的话，会非常繁琐，可能每个容器需要执行几十个命令，效率非常低。通过docker-compose可以提前编写好脚本，等上线部署的时候几分钟就可以搞定了。比起传统的项目部署方式效率大大提升了。还可以达到一次编写脚本，达到多次部署的效果。
- Compose允许用户通过一个docker-compose.yml模板文件（YAML 格式）来定义一组相关联的应用容器为一个项目（project）。  
- Compose模板文件是一个定义服务、网络和卷的YAML文件。


- Docker-Compose标准模板文件应该包含version、services、networks 三大部分，
最关键的是services和networks两个部分。


### 2.3 ob数据库启动失败 本地无法连接

- 内存不够：

``` shell

 cd D:\tool\mysql-8.0.31-winx64\bin

.\mysql -h 127.0.0.1 -P 2881 -uroot@sys -p
difyai123456
ERROR 2013 (HY000): Lost connection to MySQL server at 'reading initial communication packet', system error: 0 
结果还是连接失败


docker inspect oceanbase | findstr "Status Health"

docker logs oceanbase --tail 20

Open ssh connection ok
[WARN] OBD-1011: (127.0.0.1) The recommended value of fs.aio-max-nr is 1048576 (Current value: 65536)
[WARN] OBD-1007: (127.0.0.1) The recommended number of core file size is unlimited (Current value: 0)
[WARN] OBD-1007: (127.0.0.1) The recommended number of stack size is unlimited (Current value: 8192)
[WARN] OBD-1017: (127.0.0.1) The value of the "vm.overcommit_memory" must be 0 (Current value: 1, Recommended value: 0)

[ERROR] OBD-2000: (127.0.0.1) not enough memory. (Available: 3G, Need: 3G), Please execute `echo 1 > /proc/sys/vm/drop_caches` as root in 127.0.0.1 to release cached.

```


- Windows环境下docker desktop修改最大内存

 1. 增加 Docker 分配给容器的内存（总物理内存不够，不是容器限制也不行）


 WSL 2 Loss of resource control settings compared to old WSL , on Docker Desktop

在 WSL 2 中，资源控制设置不再通过 Docker Desktop 的图形界面直接管理
而是通过 Windows 系统的 ​**​`.wslconfig` 配置文件​**​实现
##### 一、配置 `.wslconfig` 文件

**作用​**​：
在 WSL 2 中，资源控制设置不再通过 Docker Desktop 的图形界面直接管理
而是通过 Windows 系统的 ​**​`.wslconfig` 配置文件​**​实现

​**​文件位置​**​：`C:\Users\<用户名>\.wslconfig`
​
修改步骤​**​：
1. window r+R 打开文件资源管理器，地址栏输入 `%UserProfile%` 进入用户目录。
2. .wslconfig 备份一次

3. 用文本编辑器（如记事本或 VS Code）打开文件并添加以下配置


```shell
[wsl2]
memory=8GB       # 限制内存使用（建议为物理内存的 50%-70%）
processors=2     # 限制 CPU 核心数
swap=1GB         # 交换空间大小
localhostForwarding=true  # 允许本地端口转发
autoMemoryReclaim=true    # 关闭 WSL 后自动释放内存[6,9]
```
#####  二、生效配置的步骤

1. ​**​关闭 WSL 及 Docker​**​：
    - 以管理员身份打开 PowerShell，执行：wsl --shutdown  
        
2. ​**​重启 Docker Desktop​**​：
    - 重新启动 Docker Desktop，WSL 2 会自动加载新配置
3. 启动容器oceanbase

```shell
docker compose restart oceanbase



//直连 OBServer
obclient -h127.0.0.1 -P2881 -uroot@sys -p
difyai123456
./mysql -h127.0.0.1 -P2881 -uroot 


```


4. window 通过msyql客户端链接

```shell
 cd D:\tool\mysql-8.0.31-winx64\bin

.\mysql -h 127.0.0.1 -P 2881 -uroot@sys -p
difyai123456
ERROR 2013 (HY000): Lost connection to MySQL server at 'reading initial communication packet', system error: 0 
结果还是连接失败

```


### 2.4 删除其非必须容器还是不行


1. **API服务** (`docker-api-1`): Dify的核心API服务
2. **Worker服务** (`docker-worker-1`): 处理后台任务
3. **Redis** (`docker-redis-1`): 用于缓存和消息队列
4. **Web前端** (`docker-web-1`): Dify的Web界面
5. **Nginx** (`docker-nginx-1`): 作为反向代理服务器

6. **OceanBase** (`oceanbase`): 你确认使用的主要数据库和向量数据库
7. **Sandbox** (`sandbox`): 代码执行环境
8. **SSRF代理** (`ssrf_proxy`): 安全代理，防止服务器端请求伪造攻击
9. **Plugin Daemon** (`plugin_daemon`): 插件系统

COMPOSE_PROFILES=oceanbase

docker-compose -f docker/docker-compose.yaml down

docker-compose -f docker/docker-compose.yaml up -d api worker worker_beat web redis nginx oceanbase sandbox ssrf_proxy plugin_daemon


继续精简命令:
cd D:\code\python\dify\docker
docker-compose -f docker/docker-compose.yaml up -d api web nginx redis oceanbase


docker-compose -f docker/docker-compose.yaml up restart oceanbase

docker stop  docker-nginx-1  docker-api-1  docker-redis-1 docker-web-1 docker-db-1

> [!NOTE] window笔记本内存不够，改为其他方案
> 这个路不通，换其他数据库


## 3. 谁占用内存小，就使用那个


Chroma 核心优势：

主要在 Python 环境下工作，尤其是使用 LangChain 或 LlamaIndex。

对小规模数据集（百万级以下）性能表现良好，查询延迟适中，适合快速原型开发。

社区：社区较小但活跃，轻量级，API 简单，适合新手和快速开发项目，文档简单明了

提问：dify 向量数据库修改改成 chroma

```

.env 和docker-compose.yaml 需要怎么调整

VECTOR_STORE=chroma
COMPOSE_PROFILES=chroma

注意：这个还是启动 weaviate

 # The Weaviate vector store.

  weaviate:
    image: semitechnologies/weaviate:1.19.0
    profiles:
      - '' //什么不指定 默认启动。手工屏蔽
      - weaviate
## About

the AI-native open-source embedding database
```

# 开发过程


### 3.4 大模型

  开通阿里云百炼模型调用服务并获取 API KEY

注册链接🔗：[https://bailian.console.aliyun.com/](https://bailian.console.aliyun.com/)

-  注册完阿里云百炼账号后，点击开通服务

![](https://cnlog-img-xybdiy.oss-cn-shanghai.aliyuncs.com/img/202412081035917.png)
- >接下来，我们获取API-KEY
  完整过程参考官方教程： https://bailian.console.aliyun.com/?switchAgent=10846379&productCode=p_efm&switchUserType=3&tab=api#/api
**获取API Key**

1. 前往[API-Key](https://bailian.console.aliyun.com/?tab=model#/api-key)页面，在**我的**页签（下图位置①）下单击**创建我的API-KEY**（下图位置②）。

    ![](https://help-static-aliyun-doc.aliyuncs.com/assets/img/zh-CN/6440412571/p982665.png)
    

#### 设置模型供应商和系统模型
    
 参考文档：https://docs.dify.ai/zh-hans/guides/model-configuration/readme
 Dify 在需要模型时，会根据使用场景来选择设置过的默认模型。
 在 `设置 > 模型供应商` 中设置默认模型。
 - 安装插件 通义千问


疑问5: 配置修改重启


##### 验证启动本地oceanbase是否能正确登录

```
docker ps | findstr oceanbase
bda7329f789b   oceanbase/oceanbase-ce:4.3.5-lts 

Docker容器进入的4种方式(推荐最后一种)
docker exec -it bda7329f789b /bin/bash
# 获取集群信息
[root@bda7329f789b ~]# obd cluster list
+------------------------------------------------------------+
|                        Cluster List                        |
+-----------+------------------------------+-----------------+
| Name      | Configuration Path           | Status (Cached) |
+-----------+------------------------------+-----------------+
| obcluster | /root/.obd/cluster/obcluster | deployed        |
+-----------+------------------------------+-----------------+
//obd cluster start obcluster

obd cluster display obcluster


obd cluster list
obd cluster display obcluster
obd cluster start  obcluster
//直连 OBServer
obclient -h127.0.0.1 -P2881 -uroot@sys -p
difyai123456

[root@bda7329f789b ~]#
docker logs oceanbase-ce | tail -1
看官方文档：https://hub.docker.com/r/oceanbase/oceanbase-ce


docker exec oceanbase obclient -h127.1 -uroot@test -P2881 -pdifyai123456 -e "SELECT VERSION();"


docker compose stop oceanbase 
docker compose rm -f oceanbase 
docker compose up -d oceanbase


```


- 官方经验

https://github.com/oceanbase/docker-images/blob/main/oceanbase-ce/README_CN.md

在此阅读官方文档：
- OceanBase 数据库 Vector 与 Dify 集成
- https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000002015585`ob_vector_memory_limit_percentage` 用于配置向量索引模块对内存资源占用的阈值（单位为百分比）。

```

- 修改 `.env` 中的 `VECTOR_STORE` 变量值为 `oceanbase`。
- 使用 `docker compose --profile oceanbase up -d` 启动服务。

docker ps | findstr  oceanbase
docker exec -it 4b3255e1560d /bin/bash

//直连 OBServer
obclient -h127.0.0.1 -P2881 -uroot@sys -p
difyai123456
./mysql -h127.0.0.1 -P2881 -uroot 

.\mysql -h 127.0.0.1 -P 2881 -u "root@sys" -p --ssl-mode=DISABLED


 阅读：desst不支持window我不相信
 https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000002866370

 
```

.\mysql -h127.0.0.1 -P2881 -uroot@test




```



功能：停止并删除ob容器

 原理：
 - docker-compose down​默认会删除所有在 `docker-compose.yml` 中定义的服务容器、网络和卷（若使用 `-v` 参数）​**​，无法直接通过该命令删除单个容器
步骤：

```shell
//判断是否启动
docker ps | findstr oceanbase
//stop 
docker stop oceanbase
docker rm oceanbase
```

重启启动：
 
cd D:\code\python\dify\docker
// docker-compose -f docker/docker-compose.yaml down  删除全部容器，不轻易执行，建议单独对自定容器删除重启。
docker-compose -f docker/docker-compose.yaml up -d chroma
docker-compose -f docker/docker-compose.yaml up



dify默认的向量数据库是weaviate 为什么
## 我的疑问：


### 疑问1：

- 疑问：oceanbase 能直接在链接oceanbase吗？我看oceanbase自己单独搞一个自己分支，有什么区别呀？https://www.oceanbase.com/docs/common-oceanbase-database-cn-1000000002691348
- 寻找：https://github.com/langgenius/dify/pull/10010

### 疑问2：ob云支持吗？官方公布4.3才支持

- ​**​自建 OceanBase 集群​**​（非 OBCloud）中运行 Dify，则必须升级至 ​**​V4.3.3+​**​ 才能原生支持向量索引。
### 疑问3：odocker compose up -d 命令报错 done

```
unable to get image 'nginx:latest': error during connect: Get "http://%2F%2F.%2Fpipe%2FdockerDesktopLinuxEngine/v1.47/images/nginx:latest/json": open //./pipe/dockerDesktopLinuxEngine: The system cannot find the file specified.
Starting the Docker Engine...

启动：docker
http://localhost/apps
```

### 疑问4：什么执行 `docker compose up -d` 会启动 OceanBase 容器而不是 Oracle 容器。done

 为什么启动了 OceanBase 容器


1. **环境变量配置**： 在 `docker/.env` 文件中，`VECTOR_STORE` 变量被设置为 `oceanbase`：
    
    ```javascript
    VECTOR_STORE=oceanbase
    ```
    
    这个环境变量决定了使用哪个向量数据库。
    
2. **Docker Compose 配置文件中的 profiles 机制**： 在 `docker/docker-compose.yaml` 文件中，OceanBase 服务的配置包含以下 profiles 设置：

    
    ```yaml
    oceanbase:
      image: oceanbase/oceanbase-ce:4.3.5-lts
      container_name: oceanbase
    ```

3. **Docker Compose 启动机制**： 在 Docker Compose 文件底部有一个重要的注释：
    ```javascript
    # ------------------------------
    # docker env var for specifying vector db type at startup
    # (based on the vector db type, the corresponding docker
    # compose profile will be used)
    # if you want to use unstructured, add ',unstructured' to the end
    # ------------------------------
    COMPOSE_PROFILES=${VECTOR_STORE:-weaviate}
    ```
    
这表明 Docker Compose 会根据 `VECTOR_STORE` 环境变量的值来选择启动哪些服务。
    
4. **自动激活的 profile**： 当执行 `docker compose up -d` 时，系统会读取 `VECTOR_STORE=oceanbase` 这个环境变量，然后自动激活 `oceanbase` profile，从而启动 OceanBase 容器。
### 疑问5: 配置修改重启

我修改了云服务 但是发现OCEANBASE_VECTOR_HOST 版本不行不支持4.3 版本 因此退回到 OCEANBASE_VECTOR_HOST=oceanbase 该如何处理 ，还原配置后，如何重启呢?


```bash
docker compose restart oceanbase
docker compose restart api worker worker_beat


<ClusterVersionException: (code=ErrorCode.NOT_SUPPORTED, message=OceanBase Vector Store is not supported because cluster version is below 4.3.3.0.)> 
还是报错


docker exec oceanbase obclient -h127.1 -uroot@test -P2881 -pdifyai123456 -e "SELECT VERSION();"


D:\tool\mysql-8.0.31-winx64\bin

./mysql -h 127.0.0.1 -P 3306 -u admin@obmt6605sioq4v28 -D mytest -p


```

###  ​**​首选推荐：ChromaDB​**​

- ​**​核心优势​**​：
    - ​**​嵌入式设计​**​：无需独立服务进程，直接嵌入应用运行，内存占用极低（实测在百万级向量下内存消耗约200-400MB）
        
        。
    - ​**​简化部署​**​：通过`pip install chromadb`即可集成，支持内存（`InMemory`）或本地持久化存储（如DuckDB），适合资源受限环境
        
        。
    - ​**​开发友好​**​：提供Python/JS API，与LangChain深度集成，适合快速构建Dify的RAG应用原型
        
        。
- ​**​适用场景​**​：轻量级语义搜索、小型知识库（百万级向量内）、本地开发或边缘设备部署

#### 【为开始】疑问6 dify架构是什么

- Dify 架构全景：从模块设计到部署实践的完整指南  https://www.cnblogs.com/jishuba/p/18868659


# 基于向量数据库混合检索 + Dify 构建知识库


# 参考
- https://z0yrmerhgi8.feishu.cn/wiki/O4qFwwWfnipjcgkHwLocYIvHnAbDify应用操作和源码剖析

定位：实现Dify上下游生态系统闭环，围绕工作流和知识库，高效进行AIGC内容创作。
服务：Dify源码剖析及答疑
输出：
系统性输出《Dify应用操作和源码剖析》教程；
通过Dify扩展系统平台构建AIGC内容生态

- 手把手教你部署 Dify 到本地环境(Windows系统) https://zhuanlan.zhihu.com/p/1918207520041837100
- # [Dify 架构全景：从模块设计到部署实践的完整指南](https://www.cnblogs.com/jishuba/p/18868659 "发布于 2025-05-09 18:01")
- # 基于 OceanBase + 低代码平台 Dify 快速构建你的专属智能体

