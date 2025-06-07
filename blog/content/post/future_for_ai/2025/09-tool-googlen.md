---
title: 
date: 2025-05-09
description: c++
draft: false
tags:
  - 小而美
  - AIGC从青铜到王者
categories:
---

各位老师好， 我是007

今天是 2025年5月13日，每天定投15分钟  实现百万梦想的第9天


### 谷歌插件

参考：05期破局行动 | AI编程极速开发应用 | 可复制版
### 【案例一】侧边栏AI助手

提示词：
```Markdown
帮我实现一个 Chrome 插件，要求的功能如下：
1、点击插件时，打开右侧边栏，展示网站：https://kimi.moonshot.cn/
# 注意
- 无需使用图标
- 使用V3版本的manifest.json
- 请注意中文的格式编码问题
```

提示词：

```Markdown
我现在使用这个谷歌插件时，点击插件，没有反应。请帮我分析并修改代码，确保可以正确执行。
```

  
### 【案例二】网站二维码

提示词：
```Markdown
帮我实现一个 Chrome 插件，要求的功能如下：

1、打开任意网页时，在右下角显示网页的 logo 图标，点击这个图标时会展开显示网页二维码
2、二维码大小256*256、白底，上边距、左右边距都是25px
3、二维码的下面分两行显示网站名称和网页标题，标题最多显示10个字
4、二维码中间显示网页logo

#注意
- 无需使用图标
- 使用V3版本的manifest.json
- 请注意中文的格式编码问题
```

输入提示词：

```Markdown
请帮我隐藏掉二维码，只展示网页 logo。当点击 logo 时，才展示二维码。
```

### 【案例三】翻译插件

根据我们的想法编写提示词：
  

```Markdown
帮我实现一个 Chrome 插件，要求的功能如下：
1、当我们选中网页上的文字，然后自动将选中的文字翻译成英文。
2、在鼠标停留位置展示翻译的结果
3、结果展示后，点击鼠标左键或者等待 3s 后，自动关闭翻译结果

# 注意
- 无需使用图标
- 使用V3版本的manifest.json
- 请注意中文的格式编码问题
```


### 【案例四】爬虫插件

  

提示词：

```Markdown
帮我实现一个 Chrome 插件，要求的功能如下：
1、分析这个网站链接的内容 https://mp.weixin.qq.com/s/yLqUCTKX4tajf4HF7-AbqQ
2、基于分析的结果，找到正文部分，并爬取保存到文件中。文件名为 weixin_{拼接现在的时间戳}.txt
# 注意
- 无需使用图标
- 使用V3版本的manifest.json
- 请注意中文的格式编码问题
```

  
```
## 【案例五】爬取微博热榜

请帮我编写一个 Python 程序，去爬取微博的热榜。
# 要求：
1、使用 requests 和 python3
2、目标网站： https://tophub.today/n/KqndgxeLl9 
3、爬取表格中热榜的序号、名字、热度、链接，并保存到 csv 文件里。

请一步步思考，你可以先打开目标网站分析其网页结构，并编写代码。
```


### 学习1 https://github.com/Ocyss/boos-helper


本项目仅供学习交流，禁止用于商业用途

使用该脚本有一定风险(如黑号,封号,权重降低等)，本项目不承担任何责任

>注册一个自己的小号 30 分钟完成


项目介绍
![image.png](https://s2.loli.net/2025/05/28/uzQwR84nBhTkAK1.png)

直聘助手, 皆在减少投递简历的麻烦, 和提高投递简历的效率,技术栈使用
VIte + Vue3 + element-plus


#### macos 源码安装
```shell
//下载代码
git clone git@github.com:AiZeroToOne/boos-helper.git

cd boos-helper

//安装 https://nodejs.org/en
brew install node
node -v   
v22.16.0

//安装依赖
pnpm install

//pnpm-lock.yaml 由 pnpm 生成 项目的依赖项版本始终保持一致
brew install pnpm
npm install -g pnpm
export https_proxy=http://127.0.0.1:7890 http_proxy=http://127.0.0.1:7890 all_proxy=socks5://127.0.0.1:7890

配置PNPM镜像源（可选）
npm config set registry https://registry.npmjs.org
# 检查
pnpm config get registry

pnpm -v
9.12.3

pnpm install 

//pnpm-lock.yaml 这个比 python 好多了，python 对的版本依赖太不友好了。
//一个不成熟的 python 项目一定不去用。调试 python 依赖消耗很长时间。


启动本地开发服务器：
pnpm run dev
# 或
# npm run dev


构建生产版
  
pnpm run build
构建完成后，会在项目根目录生成 `dist/`（或类似名称）输出文件夹，里面包含打包好的 `manifest.json`、脚本文件和静态资源。


Chrome：`chrome://extensions`

 点击「加载已解压的扩展程序」（Chrome/Edge）或「临时加载附加组件」（Firefox），选择第 4 步中生成的 `dist/` 目录

安装完成后即可在浏览器工具栏看到 Boos 直聘助手图标，并在打开 Boss 直聘页面时生效


我看到问题了。
你使用的是 TypeScript 5.6.3，这个版本太新了，
与当前的 vue-tsc 不兼容。让我们降级 TypeScript 版本：

pnpm install typescript@5.5.4 -D

现在你可以：

1. 使用 pnpm dev 进行本地开发

1. 使用 pnpm build:noTsc 构建生产版本

1. 将生成的 dist/boos-helper.user.js 文件加载到浏览器

```

阅读资料：https://developer.aliyun.com/article/1581010



        
## 链接我

正在挑战365天认知马拉松：  
✍️ 晨间写作（6:30-7:30）   
🏃 夜间健行（20:00-21:00）


![如果有疑问添加个人微信](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)


如果您觉得阅读本文对您有帮助， 
请点一下“**点赞，转发**” 按钮
 您的“**点赞，转发**” 将是我最大的写作动力！






