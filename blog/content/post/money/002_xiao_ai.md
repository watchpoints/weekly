---
title: 魔改小爱音箱接入 ChatGPT 和豆包，打造你的专属语音助手（保姆级教程建议收藏）
date: 2023-05-13
draft: false
tags:
  - tools
---
魔改小爱音箱接入 ChatGPT 和豆包，打造你的专属语音助手（保姆级教程建议收藏）


耗时一天 放弃，被pyton版本搞怕了。


大家好，我是小王同学，希望帮助掌握更好定位

- **昵称**：20点下班就是一件很幸福事情
- **愿景**：帮助百万人一天节省1秒钟。
- **相关作品**：无
- **如果有需求请留言**：
1. Wechat/GitHub：watchpoints

## 一. 用户故事





相关术语

1. 定义：用户故事 在软件开发和产品管理中，
     一个用户故事是一个非正式的，自然语言描述的一个或多个软件系统功能。” - 维基百科

2. 如何获取：交谈（Conversation）
    用户故事背后的细节来源于和客户或者产品负责人的交流沟通；
    确保各方对故事的理解正确。
3. 用户故事组成：卡片(Card)

卡片的正面书写故事的描述，格式为：
- 作为一个<角色>, 我想要<完成活动>, 以便于<实现价值>描述需求；
- 卡片背面书写完成用户故事的规则和完成标准，格式为：Given…When…Then。

![输入图片说明](https://foruda.gitee.com/images/1738925684729386144/1652974e_116887.png "屏幕截图")
![输入图片说明](https://foruda.gitee.com/images/1738925744177262269/8945a7ca_116887.png "屏幕截图")
4. 确认（Confirmation）形成闭环

每个故事应具有验收标准（验收条件），能够确认被正确完成。
- 以始为终，先行确认以怎样的结果，来判断开发任务的完成；
- 它保证每个故事都是独立的、完整的逻辑，可以单独交付；
- 它为驱动测试驱动开发、行为驱动开发和持续集成提供可能

  
  
# 二、 系统设计（这个方案可行）

 整体流程


# 三、 模块设计（怎么做到）

# 四、操作手册

### 方案1 https://github.com/yihong0618/xiaogpt/fork

小米小爱音箱接入了新的 ChatGPT api 现在跟实际对话一样了，太流畅了，欢迎来玩
https://www.youtube.com/watch?v=K4YA8YwzOOA


1.. 下载代码：
  git clone git@github.com:watchpoints/xiaogpt.git

遇到拦路虎
ssh: connect to host github.com port 22: Connection timed out
致命错误：无法读取远程仓库

- 通过设置host方式无效 路径受阻 换个路径
- scp方式 mac下载上传服务器
   scp ./xiaogpt-main.zip root@101.43.xx.xx:/root/money
   `scp`（Secure Copy Protocol，安全复制协议）是 Linux 和 macOS 上用于**远程安全传输文件**的命令。它基于 SSH（Secure Shell）协议，因此传输数据是**加密的**，保证了文件安全性

2. 编译代码

python 版本：
sudo yum install -y python3.9
sudo alternatives --config python
python -m pip install --upgrade pip


pip install -r requirements.txt


遇到拦路虎：解决
~~~
https://github.com/yihong0618/xiaogpt/issues/570
https://packaging.python.org/en/latest/tutorials/installing-packages/

修改版本：3.9


国内镜像：
cat ~/.pip/pip.conf
[global]
index-url = http://mirrors.tencentyun.com/pypi/simple

trusted-host = mirrors.tencentyun.com


python -m site
~~~



## 3 .取小米音响DID



这个小爱音箱app 绑定小米音箱

![输入图片说明](https://foruda.gitee.com/images/1739086639294215177/190e61dd_116887.png "屏幕截图")


| 系统和Shell   | Linux *sh                                      | Windows CMD用户                            | Windows PowerShell用户                           |
| ---------- | ---------------------------------------------- | ---------------------------------------- | ---------------------------------------------- |
| 1、安装包      | `pip install miservice_fork`                   | `pip install miservice_fork`             | `pip install miservice_fork`                   |
| 2、设置变量     | `export MI_USER=xxx`  <br>`export MI_PASS=xxx` | `set MI_USER=xxx`  <br>`set MI_PASS=xxx` | `$env:MI_USER="xxx"`  <br>`$env:MI_PASS="xxx"` |
| 3、取得MI_DID | `micli list`                                   | `micli list`                             | `micli list`                                   |
| 4、设置MI_DID | `export MI_DID=xxx`                            | `set MI_DID=xxx`                         | `$env:MI_DID="xxx"`                            |
上面的方法 运行结果不正确 去源码 执行
https://github.com/Yonsm/MiService
```
https://github.com/Yonsm/MiService

pip install aiohttp aiofiles miservice
micli.py list


[

  {

    "name": "小米AI音箱",

    "model": "xiaomi.wifispeaker.s12",

    "did": "xxxxxx",

    "token": "xxxxxx"

  }

]


micli.py mina
 

// 先设置账号  这个小爱音箱app 绑定小米音箱的账号
export MI_USER=<Username>
export MI_PASS=<Password>


  "deviceID": "1184405a-e530-418e-808e-723eb68f0554",

    "serialNumber": "18090/981833983",

    "name": "小米AI音箱",

    "alias": "小米AI音箱",

    "current": false,

    "presence": "online",

    "address": "112.36.205.231",

    "miotDID": "113966988",

    "hardware": "S12A",
    
```


可能遇到的问题：# 验证出错：KeyError: 'userId'
https://github.com/yihong0618/xiaogpt/issues/6

这个 是云主机上被限制 ，在mas上么事 ，太坑人来哦


#### 调用通义千问API
https://help.aliyun.com/zh/model-studio/getting-started/first-api-call-to-qwen

sk-a2a0a2d883224c338dcc62e786b360cb





#### 启动：

//使用 `venv` 安装
python3 -m venv myenv 

Virtualenv是一个用于创建独立的Python环境的工具，主要目的是解决不同项目间的依赖、版本以及权限问题。通过virtualenv，每个项目可以拥有自己的依赖，而不会影响到其他项目，这对于管理复杂的Python应用来说极为重要


source myenv/bin/activate

python3 xiaogpt.py --config xiao_config.yaml


启动相关问题：

~~~
Exception: Error [https://api2.mina.mi.com/admin/v2/device_list?master=0&requestId=app_ios_yKOqps4gU21xidmhAjol7IWeJ38nMT](https://api2.mina.mi.com/admin/v2/device_list?master=0&requestId=app_ios_yKOqps4gU21xidmhAjol7IWeJ38nMT): Login failed

这是不是被小米屏蔽了...同样的部署在本地mac可以，在腾讯云windows服务器上就不行了。
https://github.com/yihong0618/xiaogpt/issues/240


~~~


解决办法： 改为macos执行。云主机不行。



遇到问题1：

~~~
	你的 macOS 可能是通过 `brew` 安装的 Python，而 `brew` 现在默认将 Python 设为“外部管理环境”（**externally managed environment**），不允许直接用 `pip` 安装全局包。你可以使用以下方法之一来解决这个问题。

export https_proxy=http://127.0.0.1:7890 
http_proxy=http://127.0.0.1:7890 all_proxy=socks5://127.0.0.1:7890

取消代理
unset https_proxy
unset http_proxy 
unset all_proxy



### **方法 1：使用 `venv` 安装（推荐 ✅）**

由于 Homebrew 不允许直接安装 `PyYAML`，建议使用 `venv` 创建一个虚拟环境，然后在里面安装：

sh

`python3 -m venv myenv source myenv/bin/activate  # 激活虚拟环境 pip install pyyaml`


这个方式不行


### **方法 3：使用 `pipx` 安装（适用于命令行工具）**

如果你只需要 `PyYAML` 用于某个 Python 工具，而不是作为开发库，可以使用 `pipx`：

sh

复制编辑

`brew install pipx pipx install pyyaml`


/usr/local/lib/python3.11/site-packages/dataclasses.py"
~~~~

问：pip3 默认路径/usr/local/bin 不是 /usr/bin 目录下的

设置环境变量：
export PATH=/usr/bin:/usr/local/bin:$PATH



问：python多版本管理：

brew install pyenv
pyenv 2.5.2
pyenv install -l # 查看 pyenv 可以安装哪些Python版本
pyenv versions # 所有已经安装的版本
pyenv install 3.9.10
pyenv local 3.7.3 # 当前目录及其目录切换 
python -V # 验证一下是否切换成功 
pyenv local --unset # 解除local设置


方法2:https://www.youtube.com/watch?v=WYyJ8UQuydI



## 类似产品
https://github.com/lss233/chatgpt-mirai-qq-bot
一键部署！真正的 AI 聊天机器人！支持ChatGPT、DeepSeek、Claude、Gemini、ChatGLM、文心一言、讯飞星火，多账号，人设调教，虚拟女仆、图片渲染、语音发送 | 支持 QQ、Telegram、Discord、微信 等平台


相关资料
- https://github.com/ruanyf/weekly/issues/4582
- https://github.com/yihong0618/xiaogpt
- https://github.com/idootop/mi-gpt?tab=readme-ov-file
- [小爱音箱第三期答疑解惑](https://web.vip.miui.com/page/info/mio/mio/detail?isTop=0&postId=48906984&fromBoardId=&fromPage=mioPcSearch&fromPathname=mioPcSearch&app_version=dev.230112)
- [小米小爱音箱和小米AI音箱有何区别？](https://www.zhihu.com/question/277413232)  名字上区别，太忽悠人了。
- 把小爱音箱接入大语言模型：打造你的语音助手 | 开源日报 No.258
- https://blog.laoda.de/archives/docker-install-mi-gpt

【1】 https://www.youtube.com/watch?v=K4YA8YwzOOA【正在看】
- https://www.cnblogs.com/137point5/p/15000954.html 【Python 修改 pip 源为国内源 done】
- https://pypi.org/project/aiohappyeyeballs/#files [done]
[2] 小米音箱型号
- https://github.com/yihong0618/xiaogpt/issues/323
- 登录验证失败 https://github.com/yihong0618/xiaogpt/issues/21
-  验证出错：KeyError: 'userId' https://github.com/yihong0618/xiaogpt/issues/6 [云主机不行]
- https://github.com/yihong0618/xiaogpt/issues/540 【架构流程】


耗时一天 放弃，被pyton版本搞怕了。

