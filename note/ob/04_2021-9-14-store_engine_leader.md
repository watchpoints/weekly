https://www.oceanbase.com/training/detail?level=OBCA





# 开源数据库OceanBase代码导读

https://zhuanlan.zhihu.com/p/407940672

## （15）集群自举





### 准备



什么是 [两地三中心（2:2:2）](https://help.aliyun.com/apsara/enterprise/v_3_10_0_20200506/oceanbase/enterprise-technical-whitepaper/three-data-centers-in-the-same-city-1.html?spm=a2c4g.14484438.10001.41)



![这个是2:2:1 每个分区总共包含五个副本](https://static-aliyun-doc.oss-cn-hangzhou.aliyuncs.com/assets/img/zh-CN/4144591951/p27725.png)







- 假设深圳有两个机房，上海一个机房。对于每个分区，深圳机房分别部署两个副本，上海机房只部署一个副本，形成**2 + 2 + 1**部署。每个分区总共包含五个副本

-  2:2:2我需要创建一个集群，集群有三个zone，这三个zone分布在两个region中，每个zone内有两个observer

