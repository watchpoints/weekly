---
title: "谷歌NotebookLM: 最聪明的AI笔记工具, 15分钟内学会全部功能！"
date: 2025-05-09
description: c++
draft: false
tags:
  - 小而美
  - AIGC从青铜到王者
categories:
---
playwright codegen https://www.nowcoder.com/?type=follow
各位老师好， 我是007，

目标：
<font color="#245bdb">1. 截止2028年通过Ai争取人生100万</font>
<font color="#245bdb">2. 然后毫无保留分享自己经验。</font>

方法：
1. 不要说每天加班没时，不要说我太菜，
2. 说出来 我自己都不相信，每天专注投入15分钟，就是我做事情。
3. 每天聚焦一个小事，每天改进一点点

今天第2天，2025年5月09日，

>今天主要任务 ：学习谷歌推出Notebook LM怎么使用


开始


上篇文章： 谷歌NotebookLM: 最聪明的AI笔记工具, 15分钟内学会全部功能


### 小宇宙

_小宇宙是即刻团队推出一款中文播客工_具


1. 创建节目：https://podcaster.xiaoyuzhoufm.com/create/free

![创建节目](https://s2.loli.net/2025/05/09/GRLxzo9cNODEvJg.png)


小提示：需要实名认证 

![实名认证](https://s2.loli.net/2025/05/09/GRLxzo9cNODEvJg.png)
2 录音转文字 
- https://tingwu.aliyun.com/home
- https://q0sq2eaorn.feishu.cn/minutes/home


## 功能开发： 把mp3变成mp4视频


功能：
从配置文件读取图片地址，音地址，还有文字描述
请使用moviepy把多个图片和音频变mp4视频。pyhton实现


环境准备：cursor+ python
```python
无效：
cd /Users/wangchuanyi/money/years/p1_good_night
python3 -m venv venv
source venv/bin/activate
deactivate
pip install moviepy


brew install moviepy

export https_proxy=http://127.0.0.1:7890 
http_proxy=http://127.0.0.1:7890 all_proxy=socks5://127.0.0.1:7890


brew reinstall pipx
pipx install moviepy//❎ 这个方式不行

python3 -m pip install --user moviepy

pip install playwright
playwright install Chromium Firefox WebKit

```

## 上传到自媒体平台

- 小红书 
```python

source venv/bin/activate
pip install playwright
playwright install
pip install requests
playwright codegen https://www.xiaohongshu.com/explore

```

![自动获取内容](https://s2.loli.net/2025/05/09/lUOQ5YE7MaJdu2v.png)

```html
<div class="ql-editor ql-blank"
     aria-owns="quill-mention-list"
     data-placeholder="输入正文描述"
     data-gramm="false">
</div>


page.locator("css=.ql-editor.ql-blank").fill(artile_content)

 <div class="d-button-content">
        <span>
            发布
        </span>
    </div>

page1.get_by_role("button", name="发布").click()


```


- 知乎

page.get_by_role("button", name="上传视频").click()

page.get_by_role("button", name="绑定话题（至少添加一个）").click()
page.get_by_placeholder("绑定相关话题可以被更多人看到").fill("读书笔记")
page.get_by_placeholder("绑定相关话题可以被更多人看到").press("Enter")


![下拉菜单](https://s2.loli.net/2025/05/09/8MbnRlx31uN5LCc.png)



## 链接我

**刚刚好，是最难得的美好**

正在挑战365天认知马拉松：  
✍️ 晨间写作（6:30-7:30）   
🏃 夜间健行（20:00-21:00）

与你分享工程师的跨界成长地图  
关注公众号「 后端开发成长指南」


![如果有疑问添加个人微信](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)

如果您觉得阅读本文对您有帮助，
请点一下点赞 按钮，
您的点赞 将是我最大的写作动力

## References 第一手资料















<iframe src="//player.bilibili.com/player.html?isOutside=true&aid=947706170&bvid=BV1TW4y1372y&cid=1001200489&p=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"></iframe>






