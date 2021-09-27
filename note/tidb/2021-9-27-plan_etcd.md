#Go# #源码学习#



Go Etcd 号称云原生的基石，奇伢专门开了一个专题从 Etcd 源码角度来学习这个项目。持续更新中，此篇为汇总： 从 raftexample 开始 1、[Go Etcd 源码学习【1】开篇](https://t.zsxq.com/jq7AyzB)  ✅

2、[Go Etcd 源码学习【2】编译看看](https://t.zsxq.com/qbQz3zv) 

3 [Go Etcd 源码学习【3】raftexample 学习](https://t.zsxq.com/BE6YBQJ) 









## 内容：



#### Go Etcd 源码学习【1】

开篇 代码仓库 奇伢拉了个仓库出来，用于阅读 etcd 的代码，版本是 v3.4.10 。 [https://github.com/liqingqiya/readcode-etcd-v3.4.1...](https://github.com/liqingqiya/readcode-etcd-v3.4.10) 

我会不断的在这个仓库里加注释。 

etcd etcd 说白了就是是 kv 存储，只不过这个 kv 存储是分布式的，数据是多副本冗余的，kv 是有多版本功能的。



https://github.com/wangcy6/readcode-etcd-v3.4.10









