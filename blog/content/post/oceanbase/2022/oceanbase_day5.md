---
title: "成为oceanbase贡献者第5天："
date: 2023-01-04
description: "自我驱动学习，专业上只能靠自己"
draft: false
categories: ["oceanbase"]
---





# #一、Overview (what)

> 前言

大家好,今天是成为oceanbase贡献者第5天。

今天主要任务是：https://github.com/oceanbase/oceanbase/issues/1229

https://github.com/oceanbase/oceanbase/issues/1246































 













# # 二    Read（how） 

### 安装部署

~~~
mkdir -p /data/storage
mkfs.ext4 /dev/nvme3n1 
mount /dev/nvme3n1 /data/storage

vi mini-local-example.yaml
obd cluster edit-config  test
obd cluster redeploy test

obd cluster deploy test -c ./mini-local-example.yaml
obd cluster start test
# Connect to the OceanBase Database by using a MySQL client.
mysql -h127.1 -uroot -P2883
alias cdob="obclient -uroot@sys -h127.0.0.1 -P2881 oceanbase"


启动失败：
ERROR] (127.0.0.1): when production_mode is True, memory_limit can not be less then 16.0G

解决：

production_mode false


编译：
bash build.sh debug --init --make -j8

~~~





# # 三、more

> 举一反三





> 慢慢来，多看、多问、多总结，肯定是可以攻克的。

### 沟通步骤

1. 准备好一个ppt，在写代码之前演示最终目标 和架构设计 就是如何去实现的 【不要说公司部门环境不对 着就是最终结果，不要试着看看，一定是可以完全上线的项目，非demo和一个知识点。自己认为真的 不是闹着玩的。。】

   一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

   二、这个技术的优势和劣势分别是什么

   三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

   四、技术的组成部分和关键点。

   五、技术的底层原理和关键实现

   六、已有的实现和它之间的对比

   

2. 经过领导，专家 进行鸡蛋里挑骨头。【自己做好了别人路了胡扯，不会对别人产生任何影响，做事和做人一样，无论熟悉人，还是老师，领导，不相关人 反对 他们反馈信号，接受质疑，经过九九八十一难考验，并且你还在坚持认为对的。】

3. 最后融合别人建议，然后完善你项目。【不听老人言，吃亏在眼前，不敢接受别人批评，说明自己完全没有把握，才去否定 愤怒方式】
