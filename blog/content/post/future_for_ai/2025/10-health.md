---
title: 将个人跑步数据可视化
date: 2025-05-09
description: c++
draft: false
tags:
  - 小而美
  - AIGC从青铜到王者
categories:
---

#  用户需求

[一个想法，分享你的跑步页面](https://github.com/yihong0618/running_page/issues/12)

在数字化时代，拥有一个展示自己技能、成就和个人品牌的在线平台变得越来越重要。
今天，我们将深度解析一款名为 `running_page` 的开源项目
它是一个简单易用的个人主页生成器
让你无需深入了解前端开发即可创建个性化主页

`running_page` 是由开发者 [@yihong0618](https://github.com/yihong0618) 开发的开源项目，
旨在帮助用户将个人跑步数据可视化，
并生成一个美观的静态网页，以展示自己的运动历程
Create a personal running home page

演示地址：
https://github.com/yihong0618/running_page
https://yihong.run/
https://w.ben29.xyz/
https://grun.vercel.app/

![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250522173849.png)


特点：
 **多平台数据支持**：支持从多个运动平台导入数据，便于整合管理。
- **自动化部署**：利用 GitHub Actions 实现数据的自动同步和网页的自动生成。
- **地图展示**：通过 Mapbox 展示跑步轨迹地图，直观展示跑步路线
- **个性化定制**：用户可以根据个人喜好定制网页的样式和内容。
- **隐私保护**：支持模糊处理跑步轨迹，保护用户隐私


# 项目架构分析：

![](https://money-1256465252.cos.ap-beijing.myqcloud.com/mac/20250522160405.png)


### ✅ 最新版本（v2.x）使用的技术框架：

### 🔧 后端（数据同步 + 数据库）：

- **Python 3.9+**
- 数据存储：**SQLite3**
- 同步逻辑：`keep_sync.py`, `strava_sync.py`, `garmin_sync.py`,
- 数据更新方式：手动或 GitHub Actions

### 💻 前端（网页展示）：

- ✅ **Vite 5 + React 18** Vite 是一个超快的前端构建工具  
赋能下一代 Web 应用的发展

- ✅ **TypeScript**
- ✅ 状态管理：`zustand`
- ✅ 地图：`Leaflet.js`
- ✅ 图表：`echarts`
- ✅ 样式：CSS Modules + Tailwind CSS

> ✅ 2024年更新版本（v2.0 起）已经不再使用 Gatsby，改为 **Vite** 构建。
> 
# 部署方式

| 部署方式            | 后端框架           | 前端框架         | 同步方式           | 备注               |
| --------------- | -------------- | ------------ | -------------- | ---------------- |
| 本地部署            | Python 3.9+    | React + Vite | 手动运行 Python 脚本 | 适合开发或调试          |
| Vercel + GitHub | GitHub Actions | React + Vite | 自动同步（CI/CD）    | 推荐方式，易于托管 + 自动更新 |

## 本地部署：window云主机2c2g源码安装
#### ✅  步骤一：克隆项目

```shell
git clone https://github.com/yihong0618/running_page.git
cd running_page

```
####  ✅ 步骤二：安装依赖

安装 Node.js 和 Python 依赖：

Node.js® 是一个免费、开源、跨平台的 JavaScript 运行时环境, 
它让开发人员能够创建服务器 Web 应用、命令行工具和脚本


```shell
https://github.com/wangcy6/running_page/blob/master/Dockerfile
#确保你的电脑已经安装了Node.js

npm config set prefix "E:\package\nodejs\node_global"
npm config set cache "E:\package\nodejs\node_cache"
npm config ls

- 变量名：`NODE_PATH`
- 变量值：`E:\package\nodejs\node_global\node_modules`
# 安装前端依赖
npm install

# 安装 Python 依赖
Miniconda 是一个轻量级的Python环境管理工具，
仅包括conda、Python及其所需的基本依赖库。因此，它的安装包大小较小，
只有几十兆，相比于Anaconda更加灵活

E:\miniconda3
添加miniconda下的三个路径:\mini_conda; \mini_conda\Library\bin; \mini_conda\Scripts

[注意顺序]

pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple/ 


pip install requests -i https://pypi.tuna.tsinghua.edu.cn/simple

pip国内的一些镜像： 
中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/ 
豆瓣(douban) http://pypi.douban.com/simple/ 
清华大学 https://pypi.tuna.tsinghua.edu.cn/simple/ 


```


python：最大问题 依赖报错

```
ERROR: Could not find a version that satisfies the requirement sqlalchemy (from versions: none)
ERROR: No matching distribution found for sqlalchemy

# 1. 清除 pip 缓存
pip cache purge

# 2. 升级 pip 和构建工具
pip install --upgrade pip setuptools wheel -i  https://pypi.tuna.tsinghua.edu.cn/simple/ 

# 3. 重新安装 timezonefinder
pip install timezonefinder


eviltransform 是一个 Python 模块，用于在中国使用的坐标系（如 GCJ-02）与国际 GPS 坐标（WGS-84）之间相互转换。  

很多国产运动平台（如 Keep、咕咚）上传的跑步轨迹使用 GCJ-02，需要转换成 GPS 坐标来正确显示在地图上。

pip install eviltransform gpxpy polyline Crypto  -i  https://pypi.tuna.tsinghua.edu.cn/simple/ 


Traceback (most recent call last):
  File "D:\other\running_page\run_page\keep_sync.py", line 15, in <module>
    from Crypto.Cipher import AES
ModuleNotFoundError: No module named 'Crypto'

pip install pycryptodome  -i  https://pypi.tuna.tsinghua.edu.cn/simple/ 

```

说明：window 环境下 不cmd 命令 不用powershell

### ✅ 步骤三：准备你的跑步数据

- 官方提供数据：https://show.gotokeep.com/sportpc-page/home


```c
https://github.com/yihong0618/running_page

 python3 run_page/keep_sync.py ${keep_phone_number} ${keep_password};\

--with-gpx

```


 ### ✅ 步骤四：启动本地开发

```shell
npm config set prefix "E:\package\nodejs\node_global"
npm config set cache "E:\package\nodejs\node_cache"
npm install 
npm run dev
http://localhost:5173

```
---

### ✅ 第 6 步（可选）：构建产物并部署到 Vercel / 静态服务器

npm run build

构建结果会在 dist/ 目录里，
适合部署到 Vercel、Netlify、Cloudflare Pages 或任何静态网站服务


## 定制开发

相关资料：https://walkerdu.com/2025/01/15/running-page/

## 链接我

正在挑战365天认知马拉松：  
✍️ 晨间写作（6:30-7:30）   
🏃 夜间健行（20:00-21:00）


![如果有疑问添加个人微信](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)


如果您觉得阅读本文对您有帮助， 
请点一下“**点赞，转发**” 按钮
 您的“**点赞，转发**” 将是我最大的写作动力！






