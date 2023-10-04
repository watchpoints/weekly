---
title: "how to create virtual"
date: 2023-10-02
description: ""
draft: false
tags: ["money"]
---



## Fay项目

### 1. fork 分支代码

在fork的项目里同步别人新增分支的方法 原创

~~~
git clone git@github.com:watchpoints/Fay.git

git remote add upstream https://github.com/TheRamU/Fay.git

$ git remote -v
origin  git@github.com:watchpoints/Fay.git (fetch)
origin  git@github.com:watchpoints/Fay.git (push)
upstream        https://github.com/TheRamU/Fay.git (fetch)
upstream        https://github.com/TheRamU/Fay.git (push)

git pull // 默认是 git pull origin master，

git pull upstream  master

git pull origin与git pull upstream区别
git pull origin是拉取自己或团队项目的更新到本地；git pull upstream是拉取fork的外部上游项目的更新到本地自己仓库中的项目中。


git checkout -b fay-sales-edition 

git pull  upstream fay-sales-edition 
 
# 5.将 pull 下来的分支 push 到项目B 中去
git push origin fay-sales-edition 
~~~

git push --set-upstream origin fay-sales-edition

~~~
Code structure
.
├── main.py		    # 程序主入口
├── fay_booter.py	    # 核心启动模块
├── config.json		    # 控制器配置文件
├── system.conf		    # 系统配置文件
├── ai_module
│   ├── ali_nls.py	        # 阿里云 实时语音
│   ├── ms_tts_sdk.py       # 微软 文本转语音
│   ├── nlp_lingju.py       # 灵聚 人机交互-自然语言处理
│   ├── xf_aiui.py          # 讯飞 人机交互-自然语言处理
│   ├── nlp_gpt.py          # gpt api对接
│   ├── nlp_chatgpt.py      # chat.openai.com逆向对接
│   ├── nlp_yuan.py         # 浪潮.源大模型对接
│   ├── nlp_rasa.py         # ChatGLM-6B的基础上前置Rasa会话管理(强烈推荐)
│   ├── nlp_VisualGLM.py    # 对接多模态大语言模型VisualGLM-6B
│   ├── nlp_rwkv.py         # 离线对接rwkv
│   ├── nlp_rwkv_api.py     # rwkv server api
│   ├── yolov8.py           # yolov8资态识别
│   └── xf_ltp.py           # 讯飞 情感分析
├── bin                     # 可执行文件目录
├── core                    # 数字人核心
│   ├── fay_core.py         # 数字人核心模块
│   ├── recorder.py         # 录音器
│   ├── tts_voice.py        # 语音生源枚举
│   ├── authorize_tb.py     # fay.db认证表管理
│   ├── content_db.py       # fay.db内容表管理
│   ├── interact.py         # 互动（消息）对象
│   ├── song_player.py      # 音乐播放（暂不可用）
│   └── wsa_server.py       # WebSocket 服务端
├── gui                     # 图形界面
│   ├── flask_server.py     # Flask 服务端
│   ├── static
│   ├── templates
│   └── window.py           # 窗口模块
├── scheduler
│   └── thread_manager.py   # 调度管理器
├── utils                   # 工具模块
    ├── config_util.py      
    ├── storer.py
    └── util.py
└── test                    # 都是惊喜
~~~


### 2. Fay控制器部署及必须的key如何获取

- 视频地址：https://www.bilibili.com/video/BV1go4y1L7oe

- READ:
https://github.com/watchpoints/Fay/tree/fay-sales-edition#ai-%E6%A8%A1%E5%9D%97

#### 必须添加ai模块
- 阿里云 实时语音识别	
https://ai.aliyun.com/nls/trans

实时语音识别是对不限时长的音频流做实时识别，达到“边说边出文字”的效果，内置智能断句，可提供每句话开始结束时间。
可用于视频实时直播字幕、实时会议记录、实时法庭庭审记录、智能语音助手等场景



- 讯飞 情感分析	
https://www.xfyun.cn/service/emotion-analysis


- 讯飞自然语言处理（NLP 3选1）

讯飞已弃用，可将nlp更换为灵聚

启动Fay控制器

python main.py

启动失败缺乏依赖

~~~
https://github.com/ztosec/secscan-authcheck/issues/11
pip install werkzeug==0.16.0


~~~


### bilibili个性化数字人直播教程（不封号，有流量奖励）




# 项目：

https://luna.docs.ie.cx/
~~~
Luna AI 是一个由

ChatterBot / GPT / Claude / langchain 本地 or 云端 / chatglm / text-generation-webui / 讯飞星火 / 智谱AI

做为 大脑 驱动的虚拟主播 Live2D / Vtube Studio / UE5 + Audio2Face ，可以在 Bilibili / 抖音 / 快手 / 斗鱼 直播中与观众实时互动 或者 直接在本地和您进行聊天

它使用自然语言处理和文本转语音技术 Edge-TTS / VITS-Fast / elevenlabs / bark-gui / VALL-E-X 生成对观众问题的回答并可以通过 so-vits-svc / DDSP-SVC 变声

另外还可以通过特定指令协同 Stable Diffusion 进行画图展示。并且可以自定义文案进行循环播放

~~~
