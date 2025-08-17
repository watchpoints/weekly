---
title: Datawhale AI夏令营第一期，大模型技术 方向
date: 2025-05-09
description: c++
draft: false
tags:
  - 小而美
  - AIGC从青铜到王者
categories:
---
## 2025-7-6

## 今天的任务是什么

话术：
```shell

👏欢迎参加Datawhale夏令营，可以看一下群公告哦，【零基础】跑通一站式baseline！ 我们的学习已经开始了
Step1：30分钟，零基础跑通Baseline获得结果
【baseline链接】：https://www.datawhale.cn/activity/324/learn/191/4314/6/5

Step2：完成打卡，打卡就在教程右侧哦

🎁Tips：打卡完在群公告处有抽奖噢，先到先得～
```

任务地址：https://www.datawhale.cn/activity/324/learn/191/4314/6/5

baseline出来啦！大家来测试一下，
跑完的助教们接下龙说一下跑了多久+多少分+想法/反馈（专业助教们一定要跑哈）

带货视频评论洞察baseline：https://datawhaler.feishu.cn/wiki/N4cLwvLJxiArbVkeyfAcBjOmnce

宝子们！特大好消息来啦～🎉 今晚9点咱们要开启文字结营啦！从最开始的学习打卡，到攻克一个个知识难关，大家这段时间的努力真的超棒！不管是熬夜写笔记，还是反复抠知识点，每一份付出都闪闪发光～等晚上一起唠唠学习趣事，分享收获，不见不散！


@所有人 夏令营活动上线啦，各个赛道助教可以报名自己负责的方向哦～所有助教必须报名哈，否则无法看到教程和打卡

大模型技术https://www.datawhale.cn/activity/324?sourceId=1499
机器学习https://www.datawhale.cn/activity/323
MCP Serverhttps://www.datawhale.cn/activity/322

```c++
各位同学大家好！欢迎大家参加2025 Datawhale AI 夏令营- 大模型技术方向
请大家先改下群备注，姓名-学校/在职-专业/岗位
-----------------------
【开营预热】强烈推荐【零基础跑通baseline】并参与抽奖！
https://www.datawhale.cn/activity/324/learn/191/4314/6/5

大家跑通baseline后记得在赛事网页提交获得分数，教程右侧进行打卡抽奖哦

【抽奖链接】
https://exn8g66dnwu.feishu.cn/share/base/form/shrcnwWgdu6EheUUd2sY0seC7hf
-------------------------
【开营仪式】人满200人之后我们在本群进行开营仪式哦
-------------------------
【本次学习完全免费，有任何人加你们好友说要付费/推荐付费课程，都是诈骗。如有遇到，欢迎举报信息给官方企微号：小鲸鱼@Datawhale
```

# 一、赛事背景

在电商直播爆发式增长的数字化浪潮下，短视频平台积累了海量带货视频及用户互动数据。这些数据不仅是消费者对商品体验的直接反馈，更蕴含着驱动商业决策的深层价值。在此背景下，基于带货视频评论的用户洞察分析，已成为品牌优化选品策略、评估网红带货效能的关键突破口。

带货视频评论用户洞察的核心逻辑，在于对视频内容与评论数据的联合深度挖掘。通过智能识别视频中推广的核心商品，结合评论区用户的情感表达与观点聚合，企业能够精准捕捉消费者对商品的真实态度与需求痛点。这种分析方式不仅能揭示用户对商品功能、价格、服务的多维评价，还可通过情感倾向聚类，构建消费者偏好画像，为选品策略优化和网红合作评估提供数据支撑。

本挑战赛聚焦"商品识别-情感分析-聚类洞察"的完整链条：参赛者需先基于视频内容建立商品关联关系，进而从非结构化评论中提取情感倾向，最终通过聚类总结形成结构化洞察。这一研究路径将碎片化的用户评论转化为可量化分析的商业智能，既可帮助品牌穿透数据迷雾把握消费心理，又能科学评估网红的内容种草效果与带货转化潜力，实现从内容营销到消费决策的全链路价值提升。在直播电商竞争白热化的当下，此类分析能力正成为企业构建差异化竞争优势的核心武器。
# 二、赛事任务

参赛者需基于提供的带货视频文本及评论文本数据，完成以下三阶段分析任务：

1）【商品识别】精准识别推广商品；

2）【情感分析】对评论文本进行多维度情感分析，涵盖维度见数据说明；

3）【评论聚类】按商品对归属指定维度的评论进行聚类，并提炼类簇总结词。


https://www.datawhale.cn/activity/324/learn/191/4314/6/5


### 环境搭建：

 [Step2：启动魔搭Notebook！](https://www.modelscope.cn/my/mynotebook/preset)

新用户需要先注册魔搭

![](https://datawhale-business.oss-cn-hangzhou.aliyuncs.com/image/51bbb97d-df91-4ea8-abc8-ee66dc1d33f4.png)


![](https://datawhale-business.oss-cn-hangzhou.aliyuncs.com/image/20f46371-4f75-46e4-9005-c6555d4a8079.png)


选择【方式一：CPU】、启动！

![](https://datawhale-business.oss-cn-hangzhou.aliyuncs.com/image/7b16e828-c925-4b07-bfde-2cfc21b74aaf.png)

![](https://datawhale-business.oss-cn-hangzhou.aliyuncs.com/image/7d8e88a8-016c-409b-af3d-40589ae72329.png)


git lfs install
git clone https://www.modelscope.cn/datasets/Datawhale/AISumerCamp_video_comments_insights_baseline.git

![](https://datawhale-business.oss-cn-hangzhou.aliyuncs.com/dashboard/dipwap/1751709456946/image.png)


一键运行：
![](https://datawhale-business.oss-cn-hangzhou.aliyuncs.com/dashboard/dipwap/1751709542940/image.png)


恭喜！你已超越了80%新手、成功突破了**「用AI完成用户洞察」**的‘从0到1’：

✅ 从视频描述中 **抓出🔍商品**（**文本分类**）

✅ 自动**标记**用户**情绪**😄（多维度**情感分析**）

✅ 把相似评论打包🗂️ 成组（**聚类**）

稍等片刻后，刷新即可看到分数！分数应该在 176分左右~



非常欢迎大家参加本期夏令营学习助教，大家有任何问题都可以提出来~千万不要害羞

我们真诚的希望可以做到For the learner 和学习者一起学习，而这都依赖于大家的出色表现，给大家的福利加码！

①助教只要表现优秀，都可以获得积分，积分可兑换夏令营专属文创礼品（后续会公布）
②积分到一定门槛就可以直接获得【实习证明】（基本2次优秀助教就可以满足！）

【优秀助教标准】
硬指标：群打卡率40%以上
软指标：认真负责，负责人安排的事项落实到位，学习者体验良好


录制：助教培训回放
日期：2025-07-06 19:38:38
录制文件：https://meeting.tencent.com/crm/l6bG88b9f3


各位助教们好，这里是群公告小助手（有助教手册、日程安排、助教激励、名单）
助教手册：
https://exn8g66dnwu.feishu.cn/docx/XHh2d5ngjo4bcBxgiHxcTFEJnne?from=from_copylink

【Task01_零基础跑通baseline】  
截止时间：7月10日 24:00


📍今晚24:00我们方向Task01就截止打卡了

  截止时间：7月10日 24：00

⚠️结营证书、优秀学习者证书都是以打卡为准

  

@

今晚24:00我们方向Task01就截止打卡了
Task1还没打卡，✅15分钟跑一下baseline，教程右侧打卡即可

https://www.datawhale.cn/activity/324/learn/191/4314/6/5


### 第三天 7月10日

目标：# 三、你能读懂多少Baseline信息？
https://www.datawhale.cn/activity/324/learn/191/4314/6/5

打开服务端地址

https://www.modelscope.cn/my/mynotebook/preset

|                   |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              |
| ----------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **核心信息**          | **信息详情**                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
| **赛事链接**          | [https://challenge.xfyun.cn/h5/detail?type=video-comment-insight&ch=dwsfsp25-1](https://challenge.xfyun.cn/h5/detail?type=video-comment-insight&ch=dwsfsp25-1)                                                                                                                                                                                                                                                                                                                                                                                                                               |
| **赛题任务类型**        | 文本分类、文本聚类                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| **Baseline 代码仓库** | [https://www.modelscope.cn/datasets/Datawhale/AISumerCamp_video_comments_insights_baseline.git](https://www.modelscope.cn/datasets/Datawhale/AISumerCamp_video_comments_insights_baseline.git)                                                                                                                                                                                                                                                                                                                                                                                               |
| **Baseline概况**    | - 所用编程语言：python<br>    <br>- 使用的python库：pandas, sklearn<br>    <br>- 测评分数：约为176                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              |
| **代码运行环境**        | Python 3.8+8核CPU、约30分钟                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
| **方案要点**          | 1. 读取数据：`origin_comments_data.csv` 和 `origin_videos_data.csv`<br>    <br>2. 商品识别：使用视频文本信息，预测推广商品名称<br>    <br>    ```<br>    1. 使用jieba进行中文分词<br>    2. 使用TF-IDF将文本转换为特征向量（限制最多50个特征）<br>    3. 使用SGDClassifier进行分类预测<br>    ```<br>    <br>3. 情感分析：对四个评论维度（情感倾向、用户场景、用户疑问、用户建议）进行分类预测<br>    <br>    ```<br>    1. 使用jieba分词<br>    2. 使用TF-IDF向量化<br>    3. 使用SGDClassifier进行分类<br>    ```<br>    <br>4. 评论聚类：对五个聚类维度（正面、负面、用户场景、用户疑问、用户建议）聚类并生成聚类主题词<br>    <br>    ```<br>    1. 使用jieba分词<br>    2. 使用TF-IDF向量化<br>    3. 使用KMeans聚类<br>    ```<br>    <br>5. 保存结果并压缩成提交文件 `submit.zip` |
| **相关知识点**         | 1. **文本编码（Text Encoding）**<br>    <br>2. **文本分类（Text Classification）**                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |

![](https://s2.loli.net/2025/07/10/hNJ6Tu3VFl4oALx.png)

代码如下：

```python
import pandas as pd
video_data = pd.read_csv("origin_videos_data.csv")
comments_data = pd.read_csv("origin_comments_data.csv")
video_data.sample(10) 
//功能：从序列a中随机抽取n个元素，并将n个元素生以list形式返回

comments_data.head() //Return the first n rows.
video_data["text"] = video_data["video_desc"].fillna("") + " " + video_data["video_tags"].fillna("")


import jieba
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import SGDClassifier
from sklearn.svm import LinearSVC
from sklearn.cluster import KMeans
from sklearn.pipeline import make_pipeline


kmeans_predictor = make_pipeline(
    TfidfVectorizer(tokenizer=jieba.lcut), KMeans(n_clusters=2)
)

kmeans_predictor.fit(comments_data[comments_data["user_question"].isin([1])]["comment_text"])


kmeans_cluster_label = kmeans_predictor.predict(comments_data[comments_data["user_question"].isin([1])]["comment_text"])

kmeans_top_word = []
tfidf_vectorizer = kmeans_predictor.named_steps['tfidfvectorizer']
kmeans_model = kmeans_predictor.named_steps['kmeans']
feature_names = tfidf_vectorizer.get_feature_names_out()
cluster_centers = kmeans_model.cluster_centers_

for i in range(kmeans_model.n_clusters):
    top_feature_indices = cluster_centers[i].argsort()[::-1]
    top_word = ' '.join([feature_names[idx] for idx in top_feature_indices[:top_n_words]])
    kmeans_top_word.append(top_word)

comments_data.loc[comments_data["user_question"].isin([1]), "question_cluster_theme"] = [kmeans_top_word[x] for x in kmeans_cluster_label]


```

**处理缺失值​**​：

- `fillna("")`：将 `video_desc` 和 `video_tags` 列中的缺失值（NaN）替换为空字符串
- 避免因为 NaN 值导致字符串拼接失败（NaN + 字符串 = NaN）


https://scikit-learn.org/stable/


scikit-learn 是 Python 生态系统中广泛使用的一个开源库，专为机器学习相关任务而设计。

它基于 NumPy、SciPy 和 Matplotlib 构建，提供一套强大的工具和算法，用于执行数据分析、预处理、模型开发和评估等机器学习任务。