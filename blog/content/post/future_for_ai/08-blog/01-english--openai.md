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
OpenAI前员工亲历记，5:30AM 喂娃 → 7:00AM 办公室 → 凌晨4点部署系统 → 8:00AM 全球直播发布：

  

标题：我在OpenAI的400天：揭密全球最神秘AI公司的真实日常

  
  
  

>OpenAI 的决策非常迅速，一旦确定了方向，就会全力以赴

  
  

## 一、推门第一天：闯进「科技霍格沃茨」

  

“入职时公司1000人，1年后我竟成老员工——团队膨胀到3000人，CEO都在干完全不同的活”

  

The first thing to know about OpenAI is how quickly it's grown.

  

When I joined, the company was a little over 1,000 people.

One year later, it is over 3,000 and I was in the top 30% by tenure. Nearly everyone in leadership is doing a drastically different job than they were ~2-3 years ago.

  

关于 OpenAI，首先要知道的是它发展得有多快 。

  

我加入时，公司只有 1000 多人。一年后，公司员工人数超过 3000 人，而我按任职年限排名，位列前 30%。

几乎所有领导层的工作都与 2-3 年前截然不同

  
  
  

新手村生存指南：

#### 🔹 沟通全靠Slack：收邮件？全年不到10封！但满屏消息轰炸需极强信息过滤能力

  
  

OpenAI 的一个不同寻常之处在于 ，

  

所有东西，我指的是所有东西 ，都在 Slack 上运行 。

  

没有电子邮件。我在那里工作期间大概只收到过 10 封邮件。

  

如果你不善于组织，这会非常让人分心。如果你能管理好你的频道和通知，就能让一切变得非常便捷。

  

An unusual part of OpenAI is that everything, and I mean everything, runs on Slack. There is no email.

  

I maybe received ~10 emails in my entire time there.

  

If you aren't organized, you will find this incredibly distracting.

>If you curate your channels and notifications, you can make it pretty workable.

  
  
  
  

#### 🔹 没有KPI的神奇制度：季度计划？不存在的！好想法冒头就干（作者曾问领导遭无情反问）

  
  

OpenAI 非常注重自下而上的自律，尤其是在研究领域

  

我第一次加入时，就开始询问下一季度的路线图。得到的答案是：“这不存在”（虽然现在有了）。

  

好的想法可能来自任何地方，而且通常很难确定哪些想法会在事前证明最有成效。

  

OpenAI 并非制定宏大的“总体规划”，而是随着新研究的成果不断涌现，不断迭代和发现。

  

OpenAI is incredibly bottoms-up, especially in research. When I first showed up, I started asking questions about the roadmap for the next quarter. The answer I got was: "this doesn't exist" (though now it does). Good ideas can come from anywhere, and it's often not really clear which ideas will prove most fruitful ahead of time. Rather than a grand 'master plan', progress is iterative and uncovered as new research bears fruit.

  
  
  

##### 🔹 天才浓度超标：研究员=迷你CEO，靠“科学吸引力”组队（无聊项目无人问津）

  
  

得益于这种自下而上的文化，OpenAI 也非常注重精英管理 。

  

从历史上看，公司领导者的晋升主要基于他们是否有好点子并付诸实践的能力。

  

许多能力超群的领导者并不擅长在全体会议上进行演示或政治手腕。

  

但在 OpenAI，这一点可能不如在其他公司那么重要。最好的点子往往能赢得胜利

  
  

Andrey（Codex 负责人）曾经告诉我，应该把研究人员视为他们自己的“小主管” 。

  

>人们普遍倾向于只做自己的事情，然后看看结果如何。

  

由此推论，大多数研究都是通过“书呆子式”地吸引研究人员去解决某个特定问题来完成的。

  

如果某个问题被认为无聊或“已经解决”，那么很可能就不会有人继续研究了。

  
  
  

OpenAI 使用一个庞大的 monorepo ，它主要由 Python 编写（尽管 Rust 服务的数量正在不断增加，少量 Golang 服务也用于网络代理等）。

  

由于 Python 的编写方式多种多样，这导致了许多看起来奇怪的代码。

  

你既会遇到 Google 十年老将设计的可扩展库，也会遇到新博士们用过的 Jupyter 笔记本。

  

几乎所有代码都围绕 FastAPI 创建 API，并使用 Pydantic 进行验证。但并没有强制执行的规范。

  
  
  

## 二、颠覆认知的5大真相

  

### 1. 「混乱即生产力」

• 20+团队各自狂奔：有人造火箭，有人修自行车

  

• 案例：同一时间冒出3个“ChatGPT兄弟版”，最终Codex胜出

  
  

中文：

行动主义倾向很强 （你只要去做就可以了）。类似但不相关的团队会汇聚到各种想法上，这并不罕见。

  

我最初从事的是类似 ChatGPT Connectors 的并行（但内部）项目。

  

在我们决定推动发布之前，肯定有 3-4 个不同的 Codex 原型在流传。这些工作通常由少数人未经许可进行。当他们展现出潜力时，团队往往会迅速围绕他们组建

  

原文：

  

**There's a strong bias to action (you can just do things)**.

It wasn't unusual for similar teams but unrelated teams to converge on various ideas.

I started out working on a parallel (but internal) effort similar to ChatGPT Connectors.

There must've been ~3-4 different Codex prototypes floating around before we decided to push for a launch.

These efforts are usually taken by a small handful of individuals without asking permission. Teams tend to quickly form around them as they show promise.

  
  
  
  

## 2. 代码界的「百味杂烅」

• 博士萌新+谷歌老炮混写Python：Jupyter草稿与工业级代码共存

  

• 神比喻：“就像把梵高和毕加索的画撕碎拼贴”

  

### 3. 决策快如闪电⚡

> 晨会缺人手 → 隔天抽调ChatGPT王牌工程师 → 0审批直接开干

• 对比：普通大厂走流程平均27天

  
  

OpenAI changes direction on a dime. This was a thing we valued a lot at Segment–it's much better to do the right thing as you get new information, vs decide to stay the course just because you had a plan. It's remarkable that a company as large as OpenAI still maintains this ethos–Google clearly doesn't. The company makes decisions quickly, and when deciding to pursue a direction, goes all in.

  

OpenAI 会迅速改变方向 。

- 这是我们在 Segment 非常重视的一点——获得新信息后，选择正确的方向远比仅仅因为已有计划就坚持原有路线要好得多。

- 像 OpenAI 这样规模庞大的公司竟然还能保持这种精神，这很了不起——谷歌显然没有。OpenAI 的决策非常迅速，一旦确定了方向，就会全力以赴

  
  
  

### 4. 全民盯推特治国

• 网友吐槽上热搜 → 当天直达总裁Slack → 可能改变产品方向

  

• 内部梗：“公司命脉是推特情绪指数”

  
  

中文：

  

该公司非常关注推特 。

  

如果你发布与 OpenAI 相关的推文，并迅速走红，很有可能有人会阅读并考虑。

  

我的一个朋友开玩笑说：“这家公司靠推特氛围运转。

  
  

英文：

  

The company pays a lot of attention to twitter.

  

If you tweet something related to OpenAI that goes viral, chances are good someone will read about it and consider it.

  

A friend of mine joked, "this company runs on twitter vibes".

  

As a consumer company, perhaps that's not so wrong.

  
  
  
  

#### 5. GPU才是真BOSS

• 新功能耗电 = 创业公司全年IT开支

  
  
  
  
  

## 三、亲历Codex闪电战：7周改写历史

  

### 疯狂时间表

  

⏰ 5:30AM 喂娃 → 7:00AM 办公室 → 凌晨4点部署系统 → 8:00AM 全球直播发布

### 特种部队配置

  

>8工程师 + 4科学家 + 2设计师

= 53天造出AI编程神器

战绩：累计生成63万个代码方案 ≈ 每个工程师贡献7.8万次

  

亲历者金句：“像回到大学黑客马拉松，只是赌注是全人类的编程未来”

  

## 四、离别的顿悟：AGI竞赛的三国杀

  

公司 核心武器 作者比喻

OpenAI 全民级产品力 科技摇滚巨星

谷歌 基础设施霸权 稳重老牌贵族

Anthropic 企业级安防 精英特工小队

  

离职真相：

“不是宫斗剧！3000人巨轮转向时，怀念自己掌舵的小渔船”

  

### 五给好奇者的忠告

  

✅ 适合加入如果：

• 渴望参与定义未来

  

• 能忍受「有序混乱」

  

• 看见技术心跳会亢奋

  

❌ 谨慎考虑如果：

• 需要明确职业阶梯

  

• 习惯保姆级管理

  

• 介意凌晨修bug时被CEO

  
  

> 终极启示：这里用7周造出别家7年的产品，但你也可能7天老10岁


## 最动人的作品，为自己而写，刚刚好打动别人


（正文完）  
  
下面是乞讨信息：  
  
我，想成为一名纯粹的写手。  
  
纯粹到什么地步？  
  
靠纯手艺吃上饭！  
  
不软文，不带货，不培训。  
  
那么，只能文章有偿阅读。  
  
年费，50元。  
  
单篇，0.5元。  
  
自觉付费。（直接在文末打赏即可）  
  
不监督，不赞美，不谴责，如公交车投币。  
  
那，怎么知道年费什么时候到期？  
  
您，估摸一个时间就行。  
  
甚至，两三年，付那么一次就行。  
  
意思意思就行。  
  
感恩，所有衣食父母！


 1️⃣ 如果有更多疑问，联系小王，一起交流，进步

![个人联系方式](https://s2.loli.net/2025/08/13/veChAocQwJONWKE.png)



#### 抬头看天：走暗路、耕瘦田、进窄门、见微光
- **不要给自己这样假设**：别人完成就等着自己完成了，大家都在一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现。
- 别人不这么假设，至少本月绩效上不会写成自己的，至少晋升不是你，裁员淘汰就是你。
- 目标：在跨越最后一道坎，拿百万年薪，进大厂。

#### 低头走路：一次专注做好一个小事
- 不扫一屋 何以扫天下。
- 让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。



正在挑战365天认知马拉松：  
✍️ 晨间写作（6:30-7:30）   
🏃 夜间健行（20:00-21:00）


如果您觉得阅读本文对您有帮助， 
请点一下“**点赞，转发**” 按钮
 您的“**点赞，转发**” 将是我最大的写作动力！






