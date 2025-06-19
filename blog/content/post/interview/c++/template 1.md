---
title: 看完谷歌云7小时全球宕机报告，我发现公关用词的小秘密
date: 2024-08-07
description: do book
draft: false
tags: 
categories:
  - do book
---
对标：https://mp.weixin.qq.com/s/qIYXDoIqILMJlRewHDkeaA
https://mp.weixin.qq.com/s/qIYXDoIqILMJlRewHDkeaA

一篇文章 
就是一个ppt，
一个短视频，
一个直播答疑 
一个付费课程咨询 
一个产品设计
一次有价值的沟通




知识地图：质量回溯---事故分析报告

本文主要内容
-  翻译：第一手资料  Google Cloud incident report 
-  10分钟完成定位，为何需要数小时才能恢复服务。
- 小思考：谷歌一套系统全球部署模式 对比 按照城市，地域 部署不同系统，那个成结束成本？

核心观点:

- 技术：
同样配置管理的问题，在小公司，和大公司都遇到，
移动每个省市部署一套系统
- 缺点 开发成本高，运维成本高，经验不复用。
- 代码简单
全球部署一套系统，
- 节省运维和管理成本，只有一套系统
- Spanner 是Google的全球级的分布式数据库 (Globally-Distributed Database) 。Spanner的扩展性达到了令人咋舌的全球级，可以扩展到数百万的机器，数已百计的数据中心，上万亿的行。地球上最大的单一数据库
- 结果：用户不断请求，业务不断崩溃重启，导致Spanner数据库撑不住压力

因此 用技术来节约成本 看出谷歌系统设计，我这个我点赞地方。

用词：
- 承认自己遇到问题
- 然后因为你们信任，为了不辜负信任，我必须解决这个问题，还要做的更好

希望在回答
- 经典面试:<font color="#245bdb">在项目中，印象最深刻/遇到最大挑战 一个问题是什么</font>？
对你有帮助



## 一、翻译：第一手资料  Google Cloud incident report 

手把手教你看故障事故报告

打开：https://status.cloud.google.com/incidents/ow5i3PPK96RduMcb1SsW


**画外音**
- 国内云厂商从来不对外事故报告，偷偷把问题解决了
- 对外发表声明，没有后续，
- 不怕有事故，敢于公开让大家监督，值得学习

### 1. 第一眼看到的基本信息：

报告显示该事故的开始和结束时间：

- **开始**：2025‑06‑12 10:51 
- **结束**：2025‑06‑12 18:18 

原文：Incident began at **2025-06-12 10:51** and ended at **2025-06-12 18:18** 
单词：
- Incident 事件、事故
-  began 是 begin 的过去式，描述过去某个时间
- 暗示读者 事情已经过去


### 2. 其次看到事故影响的服务和地区

直接罗列比较枯燥无趣，相声的形式说明

《这不是科幻，这是宕机》

（逗哏：小明，捧哏：老李）

**小明**：各位听众朋友们，今天我得给大家说一件——不是灵异事件、不是科幻大片、也不是 AI 起义……

**老李**：那是啥？

**小明**：——是真·全球级的“断网现场”！

**老李**：全球断网？Wi-Fi 全下线？不是你家欠费了吧！

**小明**：不是我家，是——Google Cloud！

**老李**：哎呦，那可是“云上的谷哥”！出啥事儿了？

**小明**：6月12日上午10点51，一声不响，API Gateway“趴窝”，Cloud SQL“躺平”，Cloud Run、Cloud Firestore全“歇菜”了。

**老李**：这都是什么云？下雨了吗？

**小明**：别瞎问，人家这云，一停，半个地球都“脱网”了。
    你想啊，Spotify不放歌，OpenAI不回话，Shopify没法卖货！

**老李**：哎呀妈呀，这不就跟我媳妇出门把饭卡带走一个道理——全家停工！

**小明**：对！Apigee失联，Vertex AI失忆，Dialogflow一句话不说……平时多能说会道的 AI，现在比哑巴还安静！

**老李**：你说这不吓人吗，原来咱不是离不开手机，是手机背后那块“云”！

**小明**：说得太对。就这次事件，谷歌技术人员集体“头顶冒烟”修系统，18点18才算恢复——整整8小时！

**老李**：这要是搁咱小时候，8小时断电就断电；搁现在，断“云”，比断氧还难受！

**小明**：你看，互联网时代嘛，啥都在云上，一宕机——满地找“神仙”。

**老李**：所以以后谁要再问我：“你怕不怕断网？” 我就说：“怕！因为我不光断网，我还怕断云！”

**小明**：总结一句——数字时代的“至暗时刻”，不是流量没了，是云塌了！

**老李**：谷歌这一下，让咱全世界都明白一件事：

**合**：离了云，谁都寸步难行


🎭《云崩了，全球都“罢工”了》

（逗哏：小明，捧哏：老李）



**小明**：说起这次谷歌云宕机，你知道它不仅是美国崩，全球都“中招”了。

**老李**：全球？这云这么大，断哪儿都能炸？

**小明**：那当然！你看，亚洲地区台湾（asia-east1）、香港（asia-east2）、东京（asia-northeast1）、大阪（asia-northeast2）、首尔（asia-northeast3）、孟买（asia-south1）、德里（asia-south2）、新加坡（asia-southeast1）、雅加达（asia-southeast2），哪个没“罢工”？

**老李**：这不成了“亚洲大罢工”？

**小明**：不止！南半球的澳大利亚悉尼（australia-southeast1）、墨尔本（australia-southeast2）也都瘫了。

**老李**：那大洋洲也躺枪啊！

**小明**：欧洲的更厉害，伦敦（europe-west2）、柏林（europe-west10）、巴黎（europe-west9）、米兰（europe-west8）、法兰克福（europe-west3）、荷兰（europe-west4）、苏黎世（europe-west6）、马德里（europe-southwest1）、都灵（europe-west12）、芬兰（europe-north1）、斯德哥尔摩（europe-north2）……  
就跟开了场“欧洲云宕机嘉年华”。

**老李**：这地名听得我头都大了，是不是还带上非洲约翰内斯堡（africa-south1）了？

**小明**：没错，非洲也没躲过，约翰内斯堡云服务“歇菜”了。

**老李**：那美洲咋样？

**小明**：北美蒙特利尔（northamerica-northeast1）、多伦多（northamerica-northeast2）、墨西哥（northamerica-south1）全军覆没，南美圣保罗（southamerica-east1）也“罢工”！

**老李**：这可真是全球云罢工，覆盖面堪比世界杯！

**小明**：说得好！这次宕机告诉我们，不管你在哪，是亚洲、欧洲、非洲还是美洲，谷歌云一出事，大家都得跟着“躺平”。

**老李**：以后买东西、看视频、用 AI，全得祈祷“云”稳，不然就成了“云断人生”！

**合**：云停了，全球停！数字世界的命脉就这么脆弱！


**原文： Multiple GCP products are experiencing Service issues**
单词：
1. ​**​are experiencing​**​
    - ​**​时态​**​：现在进行时（​**​are + experiencing​**​），强调故障正在发生，动态持续中。
    - ​**​动词选择​**​：用“experiencing”（经历）而非“have”（有），弱化责任归属，暗示故障是客观状态而非主观过错。
    
   - ​**​“experiencing”​**​ 强调“正在经历客观状态”，隐含故障是外部环境或不可抗力所致（如“系统遭遇问题”），而非企业主观失误

	 ​**​对比“have”​**​：若用“have issues”则暗示企业对问题拥有所有权（“我们有问题”），易被解读为主动承认过失

​**​Service issues​**​
    - ​**​Service​**​：首字母大写，特指 ​**​云服务可用性​**​（Service Availability），非泛指服务。
    - ​**​issues​**​：中性词，弱化严重性（比“outage”或“failure”更委婉），但实际对应 ​**​全球级联瘫痪​**​（如API返回503错误）

| ​**​术语​**​      | 隐含严重性     | 适用场景      |
| --------------- | --------- | --------- |
| ​**​Issues​**​  | 轻度延迟至全局崩溃 | 弱化影响，避免恐慌 |
| ​**​Outage​**​  | 服务完全中断    | 明确承认重大事故  |
| ​**​Failure​**​ | 系统级功能失效   | 暗示技术根本性缺  |
​

画外音：天气原因吗？
- 企业需在 ​**​真相​**​ 与 ​**​恐慌​**​ 之间寻找表达缝隙 ——‘issues’是那条缝中的安全绳，但握太久也会磨伤手
- ​**​“experiencing issues” 以语义弹性换取危机响应时间，是 ​**​系统运行原则​**​ 与 ​**​真诚沟通原则​**​ 的折中体现
- ​**​适用场景​**​：  
	✅ 故障初期尚未定位根因时  
	✅ 避免引发用户恐慌或股价震荡  
	❌ 不适用于已明确人为失误的重大事故（需直接道歉）


### 3.  事件报告 先看**总结**

Incident Report- **Summary**


We **deeply** apologize for the impact this **outage** has had. 
对于此次中断造成的影响，我们深表歉意。

单词：

**outage** 通常指“服务中断”或“（电力、网络等）供应中断期间
- 不说 “error”/“bug”，而说 **“outage”**，把焦点放在**用户体验和服务可用性**上
- 这是一种更**中性但不回避责任**的措辞，既不技术掩盖，也不过度强硬

 “the impact” 强调了 **后果** 与 **影响范围**，超越“简单故障”，暗含商业中断、信任丧失等多重损失。

Google Cloud customers and their users  **trust** their businesses to Google,
and we will do better. 

Google Cloud 客户及其用户 将他们的业务托付给 Google，我们会做得更好。

单词：

- **信任的深度**：“trust” 表示客户对 Google 的高度信任，尤其是在关键业务领域。
    
- **责任的承载**：“their businesses” 强调了客户将其核心业务托付给 Google，暗示了 Google 承担着重要的责任。
    
- **信任的对象**：“to Google” 明确了受信任的一方，突出 Google 在客户心中的

- 接承认方式避免了过于直白的自责
- “we will do better” 的表述明确传达了公司对未来改进的承诺，增强了客户的信心

We apologize for the impact this has had not only on our customers’ businesses and their users but also on the trust of our systems.

对于这不仅对我们客户的业务及其用户造成的影响，
而且对我们系统的信任造成了影响，我们深表歉意。

画外音：
- 国内公关 强调 自己做什么的，扮演什么角色

We are **committed** to making improvements to help avoid outages like this moving forward.
我们致力于进行改进，以帮助避免此类中断。


单词：
- committed 表示“承诺的、致力于的，不是提交的意思


画外音：
这句话采用了“深度道歉 + 强调影响”的组合策略：
- **情感投资足**（deeply）＋ **结果聚焦**（impact）
- **语言简练、承责明确**，没有技术借口
- **符合危机沟通模型**，为后续承诺搭桥

### 重点关注 “根本原因（What happened?）


谷歌的服务是 通过Google API实现的的，为了收费等管理，每个请求都需要校验

查看是否符合规范，例如 配额（quota），你用超了不能再用了。

这个功能被 Service Control管理，

Service Control 是一种区域性服务，它从对应区域内的数据存储中读取配额和策略信息。此数据存储中的元数据几乎可以即时在全球范围内复制，
以统一管理 Google Cloud 及客户的配额策略

时间线如下：
根据谷歌官方事故报告及多方技术分析，现将2025年6月谷歌云全球宕机事件的关键时间线整理如下（统一标注太平洋夏令时PDT和北京时间CST）：

---

#### ⏱️ ​**​一、隐患潜伏阶段​**​

- ​**​2025年5月29日​**​
    - 谷歌云为 ​**​服务控制（Service Control）​**​ 新增配额策略检查功能；
    - ​**​致命漏洞​**​：新代码未校验空白字段输入，且未启用 ​**​功能标志（Feature Flags）​**​ 分阶段测试；
    - 变更通过区域部署扩散至全球，但因未触发缺陷路径，问题未被发现。

---

####  💥 ​**​二、事故爆发阶段​**​

- ​**​2025年6月12日 10:45 PDT（6月13日 01:45 CST）​**​
    - 工程师向全球配置数据库（Spanner）插入含 ​**​空白字段​**​ 的配额策略变更，缺陷代码被激活；
- ​**​10:51 PDT（01:51 CST）​**​
    - ​**​空指针异常触发​**​：Service Control 解析策略时崩溃，陷入重启循环，全球API请求返回503错误；
    - 谷歌云50+服务瘫痪，OpenAI登录、Shopify交易等第三方平台连锁崩溃；
- ​**​10:53 PDT（01:53 CST）​**​
    - SRE团队在2分钟内启动应急响应，PagerDuty警报系统激活。

---

####  🛠️ ​**​三、应急处置阶段​**​

- ​**​11:00 PDT（02:00 CST）​**​
    - 工程师 ​**​10分钟内定位根因​**​：空白字段触发未防护的缺陷代码路径；
- ​**​11:15 PDT（02:15 CST）​**​
    - 制定“​**​红按钮​**​”方案：禁用故障代码路径，绕过配额检查；
- ​**​11:30 PDT（02:30 CST）​**​
    - 补丁全球部署生效，亚太、欧洲等小区域率先恢复。

---

####  🔄 ​**​四、恢复受阻与最终解决​**​

- ​**​12:00–16:00 PDT（03:00–07:00 CST）​**​
    - ​**​大型区域过载​**​：us-central1等区域因 ​**​“羊群效应”​**​ 延迟恢复——实例密集重启压垮Spanner数据库，缺乏指数退避机制；
    - 工程师限流重启节奏，切换多区域数据库减压；
- ​**​13:30 PDT（04:30 CST）​**​
    - 除AI/数据处理服务外，多数核心功能恢复（如Gmail、YouTube）；
- ​**​18:18 PDT（09:18 CST）​**​
    - ​**​全球服务完全恢复​**​，总耗时约7小时27分钟。

---


原文：

**On May 29, 2025, a new feature was added to Service Control for additional quota policy checks.**  
2025 年 5 月 29 日，Service Control 添加了一项用于额外配额策略检查的新功能。
【新增了一个功能】

This code change and binary release  went through our **region by region rollout**, but the code path that failed was never exercised during this rollout due to needing a policy change that would trigger the code. 

此次代码改动和二进制发布采用了逐区域滚动部署，
但由于要触发这一失败代码路径需要相应的策略更改，
该路径在部署过程中从未被执行。

啥意思：
在测试环境测试了，根本没有测试出来，这个很多公司开发过程都遇到这样情况

单词：

release ：发布 部署好的程序 编译后 然后进行发布
rollout：推出、逐步部署

在 Google、Meta、Amazon 等公司的事故报告或技术博客中，
**rollout** 经常出现，目的是：

|目的|示例|效果|
|---|---|---|
|✅ 表示流程是 **渐进式、可控的**|"region-by-region rollout"|向公众展示公司有规范化的上线流程|
|✅ 用于 **事故甩尾说明**|"the bug was not triggered during the rollout"|表达“我们发布是正常的，只是边角情况未触发”|
|✅ 减轻公众担忧|"We paused the rollout immediately upon detecting anomalies."|



As a **safety precaution**, this code change came with a red-button to **turn off** that particular policy serving path. 
作为一项安全预防措施，此次代码改动配备了一个“红色按钮”，可以关闭该特定的策略服务路径。

啥意思：增加一个开关，可以关闭该功能

单词：
- precaution 预防措施
- **came with**：附带，有配套的…


**The issue with this change was that it did not have appropriate error handling nor was it feature flag protected.**  

此更改的问题在于未设置适当的错误处理机制，
也没有通过功能标志（feature flag）进行保护。


**Without the appropriate error handling, the null pointer caused the binary to crash.**  
由于缺乏合适的错误处理，空指针异常导致该二进制程序崩溃。

**Feature flags are used to gradually enable the feature region by region per project, starting with internal projects, to enable us to catch issues.**  
功能标志用于让该功能在各项目与各区域中逐步启用，
先在内部项目上线，从而让我们有机会捕捉问题。


**If this had been flag protected, the issue would have been caught in staging.**  
如果此次更改受功能标志保护，该问题本应在暂存（staging）环境中被发现

啥意思：啥意思，例如 用户输入字符串 包含空格 没有处理好，导致服务挂了




Within 2 minutes, our Site Reliability Engineering team was **triaging** the incident. Within 10 minutes, **the root cause** was identified and the red-button (to disable the serving path) was being put in place. 
在 2 分钟内，我们的站点可靠性工程团队对事件进行了分类。
在 10 分钟内，确定了根本原因，并设置了红色按钮（用于禁用投放路径）。

The red-button was ready to roll out ~25 minutes from the start of the incident. 
红色按钮已准备好在事件开始后 ~25 分钟推出。

Within 40 minutes of the incident,the red-button rollout was completed, 
在事件发生后的 40 分钟内，红色按钮的推出完成，

and we started seeing recovery across regions, starting with the smaller ones first.  
我们开始看到各个区域的恢复，首先从较小的区域开始。

Within some of our larger regions, such as us-central-1, as Service Control tasks restarted, it created a herd effect on the underlying infrastructure it depends on (i.e. that **Spanner table**), overloading the infrastructure. 

Service Control did not have the appropriate randomized exponential backoff implemented to avoid this. 


在我们的一些较大的区域（例如 us-central-1）中，当 Service Control 任务重新启动时，
它会对它所依赖的底层基础设施（即 Spanner 表）产生羊群效应
，使基础设施过载。

Service Control 没有实施适当的随机指数回退来避免这种情况。

It **took** up to ~2h 40 mins to fully **resolve** in us-central-1 as we throttled task creation to minimize the impact on the underlying infrastructure
and routed traffic to multi-regional databases to reduce the load. 

由于我们限制了任务创建以尽量减少对底层基础设施的影响，
并将流量路由到多区域数据库以减少负载，
因此在 us-central-1 中完全解决问题最多需要约 2 小时 40 分钟

单词：
- take  花费
-  resolve
- underlying infrastructure  基础设施 就是数据库
- traffic 本来交通 这里还流量


#  二、10分钟完成定位，为何需要数小时才能恢复服务




在故障描述中解释 

## 参考资料
=- https://timesofindia.indiatimes.com/technology/tech-news/what-took-google-hours-to-fix-a-glitch-that-its-engineers-identified-in-10-minutes/articleshow/121893332.cms?utm_source=chatgpt.com