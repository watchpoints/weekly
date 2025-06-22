---
title: 读懂谷歌云7小时全球宕机报告，技术人不止懂技术
date: 2025-06-20
description: do book
draft: false
tags: 
categories:
  - do book
---

点击<font color="#245bdb">蓝色</font>关注。

><font color="#c0504d">文末有福利，先到先得</font>



知识地图：质量回溯---事故分析报告

本文主要内容
-  翻译：第一手资料  Google Cloud incident report 
-  10分钟完成定位，为何需要数小时才能恢复服务。
-  小思考：谷歌的全球单体部署模式与按区域分散部署模式，哪种更节省成本？

**核心观点:**
- **技术反思**：
  同样的配置管理问题，无论在小公司还是大公司都会遇到。
  - **对比：各省市独立部署一套系统**
    - **缺点**：开发和运维成本高，经验无法复用。
    - **优点**：代码相对简单，故障影响范围小。
  - **对比：全球统一部署一套系统**
    - **优点**：运维和管理成本低，只需维护一套系统。
    - **Spanner**：作为Google的全球级分布式数据库（Globally-Distributed Database），其强大的扩展性支撑了这一模式，可扩展至数百万台机器、数百个数据中心和上万亿行数据，是地球上最大的单一数据库。
    - **事故后果**：当故障发生时，用户不断重试请求，导致业务不断崩溃重启，最终巨大的压力压垮了Spanner数据库。

  因此，通过技术手段节约成本，充分体现了谷歌的系统设计智慧，这一点非常值得我们赞赏。

- **公关用词**：
  - 首先，坦诚地承认遇到了问题。
  - 然后，强调为了不辜负客户的信任，必须解决问题并做得更好。

希望本文对你回答经典面试题："在项目中，你印象最深刻或遇到的最大挑战是什么？"能有所帮助。



## 一、翻译：第一手资料 Google Cloud incident report 

手把手教你读懂故障报告

打开：https://status.cloud.google.com/incidents/ow5i3PPK96RduMcb1SsW


**画外音**
- 国内云厂商通常不公开事故报告，只是悄悄解决问题。
- 即使对外发表声明，也往往没有后续的详细分析。
- 不怕出事故，敢于公开并接受公众监督，这种态度值得学习。

### 1. 第一眼看到的基本信息：

报告显示该事故的开始和结束时间：

- **开始**：2025‑06‑12 10:51 
- **结束**：2025‑06‑12 18:18 

原文：Incident began at **2025-06-12 10:51** and ended at **2025-06-12 18:18** 
单词：
- Incident 事件、事故
-  began 是 begin 的过去式，描述过去某个时间点
- 暗示读者：事情已经过去了。


### 2. 其次看到事故影响的服务和地区

直接罗列受影响的范围会比较枯燥，我们用相声的形式来说明。

《这不是科幻，这是宕机》

（逗哏：小明，捧哏：老李）

**小明**：各位听众朋友们，今天我得给大家说一件——不是灵异事件、不是科幻大片、也不是 AI 起义……

**老李**：那是啥？

**小明**：——是真·全球级的"断网现场"！

**老李**：全球断网？Wi-Fi 全下线？不是你家欠费了吧！

**小明**：不是我家，是——Google Cloud！

**老李**：哎呦，那可是"云上的谷哥"！出啥事儿了？

**小明**：6月12日上午10点51分，一声不响，API Gateway"趴窝"，Cloud SQL"躺平"，Cloud Run、Cloud Firestore全"歇菜"了。

**老李**：这都是些什么"云"？天上下的那种吗？

**小明**：别瞎问，人家这云，一停，半个地球都"脱网"了。
    你想啊，Spotify不放歌，OpenAI不回话，Shopify没法卖货！

**老李**：哎呀妈呀，这不就跟我媳妇出门把饭卡带走一个道理——全家停工！

**小明**：对！Apigee失联，Vertex AI失忆，Dialogflow一句话不说……平时多能说会道的 AI，现在比哑巴还安静！

**老李**：你说这不吓人吗，原来咱不是离不开手机，是手机背后那块"云"！

**小明**：说得太对。就这次事件，谷歌技术人员集体"头顶冒烟"修系统，到18点18分才算恢复——整整7个多小时！

**老李**：这要是搁咱小时候，8小时断电就断电；搁现在，断"云"，比断氧还难受！

**小明**：你看，互联网时代嘛，啥都在云上，一宕机——满地找"神仙"。

**老李**：所以以后谁要再问我："你怕不怕断网？" 我就说："怕！因为我不光断网，我还怕断云！"

**小明**：总结一句——数字时代的"至暗时刻"，不是流量没了，是云塌了！

**老李**：谷歌这一下，让咱全世界都明白一件事：

**合**：离了云，谁都寸步难行


🎭《云崩了，全球都"罢工"了》

（逗哏：小明，捧哏：老李）



**小明**：说起这次谷歌云宕机，你知道它不仅是美国崩，全球都"中招"了。

**老李**：全球？这云这么大，断哪儿都能炸？

**小明**：那当然！你看，亚洲地区台湾（asia-east1）、香港（asia-east2）、东京（asia-northeast1）、大阪（asia-northeast2）、首尔（asia-northeast3）、孟买（asia-south1）、德里（asia-south2）、新加坡（asia-southeast1）、雅加达（asia-southeast2），哪个没"罢工"？

**老李**：这不成了"亚洲大罢工"？

**小明**：不止！南半球的澳大利亚悉尼（australia-southeast1）、墨尔本（australia-southeast2）也都瘫了。

**老李**：那大洋洲也躺枪啊！

**小明**：欧洲的更厉害，伦敦（europe-west2）、柏林（europe-west10）、巴黎（europe-west9）、米兰（europe-west8）、法兰克福（europe-west3）、荷兰（europe-west4）、苏黎世（europe-west6）、马德里（europe-southwest1）、都灵（europe-west12）、芬兰（europe-north1）、斯德哥尔摩（europe-north2）……  
就跟开了场"欧洲云宕机嘉年华"。

**老李**：这地名听得我头都大了，是不是还带上非洲约翰内斯堡（africa-south1）了？

**小明**：没错，非洲也没躲过，约翰内斯堡云服务"歇菜"了。

**老李**：那美洲咋样？

**小明**：北美蒙特利尔（northamerica-northeast1）、多伦多（northamerica-northeast2）、墨西哥（northamerica-south1）全军覆没，南美圣保罗（southamerica-east1）也"罢工"！

**老李**：这可真是全球云罢工，覆盖面堪比世界杯！

**小明**：说得好！这次宕机告诉我们，不管你在哪，是亚洲、欧洲、非洲还是美洲，谷歌云一出事，大家都得跟着"躺平"。

**老李**：以后买东西、看视频、用 AI，全得祈祷"云"稳，不然就成了"云断人生"！

**合**：云停了，全球停！数字世界的命脉就这么脆弱！


**原文： Multiple GCP products are experiencing Service issues**
单词：
1. ​**​are experiencing​**​
    - ​**​时态​**​：现在进行时（​**​are + experiencing​**​），强调故障正在发生，动态持续中。
    - ​**​动词选择​**​：用"experiencing"（经历）而非"have"（有），弱化责任归属，暗示故障是客观状态而非主观过错。
    
   - ​**​"experiencing"​**​ 强调"正在经历客观状态"，隐含故障是外部环境或不可抗力所致（如"系统遭遇问题"），而非企业主观失误。

	 ​**​对比"have"​**​：若用"have issues"则暗示企业对问题拥有所有权（"我们有问题"），易被解读为主动承认过失。

​**​Service issues​**​
    - ​**​Service​**​：首字母大写，特指 ​**​云服务可用性​**​（Service Availability），非泛指服务。
    - ​**​issues​**​：中性词，弱化严重性（比"outage"或"failure"更委婉），但实际对应 ​**​全球级联瘫痪​**​（如API返回503错误）。

| ​**​术语​**​      | 隐含严重性     | 适用场景      |
| --------------- | --------- | --------- |
| ​**​Issues​**​  | 轻度延迟至全局崩溃 | 弱化影响，避免恐慌 |
| ​**​Outage​**​  | 服务完全中断    | 明确承认重大事故  |
| ​**​Failure​**​ | 系统级功能失效   | 暗示技术根本性缺陷  |
​

画外音：是天气原因吗？
- 企业需在"真相"与"恐慌"之间寻找表达的平衡点——'issues'就是那条缝隙中的安全绳，但握太久也会磨伤手。
- ​**​"experiencing issues" 以语义弹性换取危机响应时间，是 ​**​系统运行原则​**​ 与 ​**​真诚沟通原则​**​ 的折中体现。
- ​**​适用场景​**​：  
	✅ 故障初期尚未定位根因时  
	✅ 避免引发用户恐慌或股价震荡  
	❌ 不适用于已明确人为失误的重大事故（需直接道歉）


### 3.  事件报告 先看**总结**

Incident Report- **Summary**


We **deeply** apologize for the impact this **outage** has had. 
对于此次中断造成的影响，我们深表歉意。

单词：

**outage** 通常指"服务中断"或"（电力、网络等）供应中断期间"。
- 不说 "error"/"bug"，而说 **"outage"**，把焦点放在**用户体验和服务可用性**上。
- 这是一种更**中性、但不回避责任**的措辞，既没有在技术上进行掩盖，也没有显得态度强硬。

 "the impact" 强调了 **后果** 与 **影响范围**，超越"简单故障"，暗含商业中断、信任丧失等多重损失。

Google Cloud customers and their users  **trust** their businesses to Google,
and we will do better. 

Google Cloud 客户及其用户将他们的业务托付给 Google，我们会做得更好。

单词：

- **信任的深度**："trust" 表示客户对 Google 的高度信任，尤其是在关键业务领域。
    
- **责任的承载**："their businesses" 强调了客户将其核心业务托付给 Google，暗示了 Google 承担着重要的责任。
    
- **信任的对象**："to Google" 明确了受信任的一方，突出 Google 在客户心中的重要地位。

- 这种间接的承认方式，避免了过于直白的自责。
- "we will do better" 的表述明确传达了公司对未来改进的承诺，增强了客户的信心。

We apologize for the impact this has had not only on our customers' businesses and their users but also on the trust of our systems.

我们深表歉意，此次事件不仅影响了我们客户的业务及其用户，也动摇了大家对我们系统的信任。

画外音：
- 国内公关往往强调自己做了什么、扮演了什么角色。

We are **committed** to making improvements to help avoid outages like this moving forward.
我们致力于进行改进，以帮助避免此类中断再次发生。


单词：
- committed 表示"承诺的、致力于的"，而不是代码提交（commit）的意思。


画外音：
这句话采用了"深度道歉 + 强调影响"的组合策略：
- **情感投资足**（deeply）＋ **结果聚焦**（impact）
- **语言简练、承责明确**，没有技术借口
- **符合危机沟通模型**，为后续承诺搭桥

### 重点关注 "根本原因（What happened?）


谷歌的服务是通过 Google API 实现的。为了进行收费等管理，每个API请求都需要被校验。

这项校验工作会检查请求是否符合规范，例如配额（quota）——如果你用超了，就不能再使用。

这个功能由 Service Control 组件管理。

Service Control 是一种区域性服务，它从对应区域内的数据存储中读取配额和策略信息。此数据存储中的元数据几乎可以即时在全球范围内复制，
以统一管理 Google Cloud 及客户的配额策略。

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
    - 制定"​**​红按钮​**​"方案：禁用故障代码路径，绕过配额检查；
- ​**​11:30 PDT（02:30 CST）​**​
    - 补丁全球部署生效，亚太、欧洲等小区域率先恢复。

---

####  🔄 ​**​四、恢复受阻与最终解决​**​

- ​**​12:00–16:00 PDT（03:00–07:00 CST）​**​
    - ​**​大型区域过载​**​：us-central1等区域因 ​**​"羊群效应"​**​ 延迟恢复——实例密集重启压垮Spanner数据库，缺乏指数退避机制；
    - 工程师限流重启节奏，切换多区域数据库减压；
- ​**​13:30 PDT（04:30 CST）​**​
    - 除AI/数据处理服务外，多数核心功能恢复（如Gmail、YouTube）；
- ​**​18:18 PDT（09:18 CST）​**​
    - ​**​全球服务完全恢复​**​，总耗时约7小时27分钟。

---


原文：

**On May 29, 2025, a new feature was added to Service Control for additional quota policy checks.**  
2025 年 5 月 29 日，Service Control 添加了一项用于额外配额策略检查的新功能。

This code change and binary release  went through our **region by region rollout**, but the code path that failed was never exercised during this rollout due to needing a policy change that would trigger the code. 

此次代码改动和二进制发布采用了逐区域滚动部署，
但由于需要特定的策略变更才能触发这一失败的代码路径，
因此该路径在部署过程中从未被执行。

啥意思？
意思是在测试环境根本没测出来。这种情况在很多公司的开发过程中都可能遇到。

单词：

release ：发布。指将编译好的程序进行部署发布。
rollout：推出、逐步部署

在 Google、Meta、Amazon 等公司的事故报告或技术博客中，
**rollout** 经常出现，目的是：

|目的|示例|效果|
|---|---|---|
|✅ 表示流程是 **渐进式、可控的**|"region-by-region rollout"|向公众展示公司有规范化的上线流程|
|✅ 用于 **事故甩尾说明**|"the bug was not triggered during the rollout"|表达"我们发布是正常的，只是边角情况未触发"|
|✅ 减轻公众担忧|"We paused the rollout immediately upon detecting anomalies."|



As a **safety precaution**, this code change came with a red-button to **turn off** that particular policy serving path. 
作为一项安全预防措施，此次代码改动配备了一个"红色按钮"，可以关闭该特定的策略服务路径。

啥意思：增加一个开关，可以一键关闭该功能。

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
先从内部项目开始上线，从而让我们有机会捕捉到问题。


**If this had been flag protected, the issue would have been caught in staging.**  
如果此次更改受功能标志保护，该问题本应在暂存（staging）环境中被发现。

啥意思：比如，当用户输入的字符串包含未处理的空格时，导致服务挂了。




Within 2 minutes, our Site Reliability Engineering team was **triaging** the incident. Within 10 minutes, **the root cause** was identified and the red-button (to disable the serving path) was being put in place. 
在 2 分钟内，我们的站点可靠性工程（SRE）团队对事件进行了分类。
在 10 分钟内，根本原因被确定，并立即启用了红色按钮（用于禁用该服务路径）。

The red-button was ready to roll out ~25 minutes from the start of the incident. 
红色按钮在事件开始后约 25 分钟就准备好进行部署了。

Within 40 minutes of the incident,the red-button rollout was completed, 
在事件发生后的 40 分钟内，红色按钮的部署完成，

and we started seeing recovery across regions, starting with the smaller ones first.  
我们开始看到各个区域的服务陆续恢复，首先从较小的区域开始。

Within some of our larger regions, such as us-central-1, as Service Control tasks restarted, it created a herd effect on the underlying infrastructure it depends on (i.e. that **Spanner table**), overloading the infrastructure. 

Service Control did not have the appropriate randomized exponential backoff implemented to avoid this. 


在我们的一些较大区域（例如 us-central-1）中，当 Service Control 任务重新启动时，
它对其所依赖的底层基础设施（即 Spanner 表）产生了"羊群效应"，
导致基础设施过载。

Service Control 没有实施适当的随机指数退避机制来避免这种情况。

It **took** up to ~2h 40 mins to fully **resolve** in us-central-1 as we throttled task creation to minimize the impact on the underlying infrastructure
and routed traffic to multi-regional databases to reduce the load. 

由于我们限制了任务创建以尽量减少对底层基础设施的影响，
并将流量路由到多区域数据库以减少负载，
因此在 us-central-1 区域，完全解决问题耗时约 2 小时 40 分钟。

单词：
- take  花费
- resolve 解决
- underlying infrastructure  底层基础设施，这里指数据库
- traffic 本意是交通，这里指网络流量


#  二、10分钟完成定位，为何需要数小时才能恢复服务


在故障描述中，谷歌解释了原因，这与他们的全球分布式数据库——Google Spanner有关。

Spanner 是一个可扩展的、全球分布式的数据库，由谷歌公司设计、开发和部署。

在最高抽象层面，Spanner 就是一个数据库，它将数据分片存储在许多 Paxos 状态机上，这些机器遍布全球的数据中心内。

复制技术被用来实现全球可用性和地理局部性。

客户端可以自动在副本之间进行故障恢复。随着数据的变化和服务器的增减，Spanner 会自动对数据进行重新分片，从而有效应对负载变化和处理失败。

Spanner 被设计成可以扩展到数百万个机器节点，跨越成百上千个数据中心，具备处理万亿级数据库行的规模，并能从1-2个数据中心的故障中恢复。

但这次事故中，谷歌也做了限制。

在我们的一些较大的区域（例如 us-central-1）中，当 Service Control 任务重新启动时，
它对其所依赖的底层基础设施（即 Spanner 表）产生了羊群效应，
使基础设施过载。

Service Control 没有实施适当的随机指数退避来避免这种情况。

由于我们限制了任务创建以尽量减少对底层基础设施的影响，
并将流量路由到多区域数据库以减少负载，
因此在 us-central-1 中完全解决问题最多需要约 2 小时 40 分钟。



## 三 、小思考

2分钟发现问题
10 分钟定位根本原因
40 分钟给出解决方案
7 小时全部恢复业务

这背后，我看到的是一套强大的全球分布式系统：Google Spanner，能够将变更瞬间部署到全球。
事故的根源并非基础设施问题，而是在软件层面。
这恰恰体现了谷歌通过先进技术实现"降本增效"的思路，这才是真正有效的降本增效。


----------------我是黄金分割线-----------------------------
## 最动人的作品，为自己而写，刚刚好打动别人

  

我在寻找一位积极上进的小伙伴，

一起参与**神奇早起 30 天改变人生计划**，发展个人事业，不妨 试试

  

 1️⃣关注公众号:后端开发成长指南(回复面经获取）获取过去我**全部面试录音**和大厂面试复盘攻略

![](https://s2.loli.net/2025/05/31/GRgOTiQHI456VWD.png)

2️⃣ **感兴趣的读者可以通过公众号获取老王的联系方式。**

  

 加入我的技术交流群Offer 来碗里 (回复"面经"获取），一起抱团取暖

![](https://s2.loli.net/2025/06/01/6qkOut3xrDHen8J.png)




#### 抬头看天：走暗路、耕瘦田、进窄门、见微光，
- <font color="#ff0000">我要通过技术拿到百万年薪P7职务</font>，别人投入时间完成任务，别人采取措施解决问题了，不要不贪别人功劳，
- 但是不要给自己这样假设：别人完成就等着自己完成了，大家一个集团，一个公司，分工不同，不，这个懒惰表现，这个逃避问题表现， 
- 别人不这么假设，至少kpi上不会写成自己的，至少晋升不是你，裁员淘汰是，你的整个公司ceo，整个部门总裁，整个领导不帮助一下的，他们不这么想 ，你什么没做，战略是别人10年一点带你研究的多难，项目拆分别人10年完成多少问题，项目实现10年安排组织一点点完成多少bug，多少代码，是不要给自己这样假设：你等了看了观察10年什么做 ，0 贡献，
-  但是不要给自己这样假设，别人全部市场，别人全部市场，别人占据全部客户，一切重要无比，你太差，太才，思考不行，沟通不行，认知不行，去tmd，给别人丢脸。这个方面我无法控制，在这方面经历任何问题应该的。
- 我控制 的事情是 我必须亲自了解行业遇到难题，了解有什么需求，行业解决方案，我可以从三万英尺看问题，像周围人学习，像免费公开英文资料学习，从模仿开始。然后免费公开。我要通过技术拿到百万年薪P7职务，我必须糊涂混沌中走出来
-  目标：拿百万年 想进入一线大厂，但在C++学习和应用上存在瓶颈，渴望跨越最后一道坎。
- 现状：缺乏实战，渴望提升动手能力公司的项目不会重构，没有重新设计的机会，导致难以深入理解需求。
- 成为优秀完成任务，成为团队、公司都认可的核心骨干。优秀地完成任务= 高效能 + 高质量 + 可持续 + 可度量

##### 低头走路：
- 一次专注做好一个小事。
- 不扫一屋 何以扫天下，让自己早睡，早起，锻炼身体，刷牙保持个人卫生，多喝水 ，表达清楚 基本事情做好。
- 我控制 的事情是 我通过写自己代码拿到百万收益。代码就是杠杆，我必须创造可以运行在2c2g云主机小而美产品出来（服务普通人），而不是运行构建至少10台*64cpu* 300g内存物理机大而全项目（领航者，超越其他产品，出货全球N1，这个还是有停留有限斗争游戏，为top 10人企业服务）我必须糊涂混沌中走出来