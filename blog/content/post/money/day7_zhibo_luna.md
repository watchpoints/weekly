---
title: "how to create virtual"
date: 2023-10-02
description: ""
draft: false
tags: ["money"]
---



## 1. 项目资料


https://luna.docs.ie.cx/
~~~
Luna AI 是一个由

ChatterBot / GPT / Claude / langchain 本地 or 云端 / chatglm / text-generation-webui / 讯飞星火 / 智谱AI

做为 大脑 驱动的虚拟主播 Live2D / Vtube Studio / UE5 + Audio2Face ，可以在 Bilibili / 抖音 / 快手 / 斗鱼 直播中与观众实时互动 或者 直接在本地和您进行聊天

它使用自然语言处理和文本转语音技术 Edge-TTS / VITS-Fast / elevenlabs / bark-gui / VALL-E-X 生成对观众问题的回答并可以通过 so-vits-svc / DDSP-SVC 变声

另外还可以通过特定指令协同 Stable Diffusion 进行画图展示。并且可以自定义文案进行循环播放

~~~

- 视频：
https://space.bilibili.com/3709626/channel/collectiondetail?sid=1422512

- 代码地址：
https://github.com/Ikaros-521/AI-Vtuber
git clone git@github.com:wangcy6/AI-Vtuber.git





### 2. 安装部署

- 【AI主播】从零开始的 环境搭建（手把手） 相关的坑太多了，整合包一直打不好，单独讲一讲具体问题如何解决

https://www.bilibili.com/video/BV1fV4y1C77r/?vd_source=d11b3b4ca9c7e3a6dd986dc2bc08040e

- 【AI主播-功能篇】新增 念弹幕 功能。在弹幕量少的情况下，可以有效的回复匹配上，也可以单独用作直播助理，帮忙念下弹幕协同直播


- https://github.com/Ikaros-521/AI-Vtuber/tree/dev

- 操作手册：
https://luna.docs.ie.cx/#_1

- 斗鱼
~~~
pip install -r requirements_douyu.txt
python main.py
http://127.0.0.1:12345/Live2D/
~~~

- 

