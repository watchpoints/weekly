#   学习思路



### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题



### 二、这个技术的优势和劣势分别是什么 

### 三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

### 四、技术的组成部分和关键点。

### 五、技术的底层原理和关键实现

### 六、已有的实现和它之间的对比

| 产品 | 一致性 | 角色 |      |      |
| ---- | ------ | ---- | ---- | ---- |
|      |        |      |      |      |
|      |        |      |      |      |
|      |        |      |      |      |









## 挑战1:Peer-to-peer





### 三.这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

- 例子：

 那为啥微信客户端不能替你直接呼叫你的好友，非要让微信服务器代你呢？

有许多原因，原因之一是你不知道好友的ip地址。

。。。

这个理由看似合情合理，那如果知道呢？也不行，因为IP地址是地理地址，每个省每个市都有不同的ip地址（比如南京市电信就有超过65536个地址），你可以做一趟地铁，每过5站查一次stun服务，每次都能看到不同的ip地址。

- 别人怎么做的 Peer-to-peer communications with WebRTC

  

  RFC 5389定义 https://zh.wikipedia.org/wiki/STUN  【累计消耗 5*25分钟】

**STUN**（**Session Traversal Utilities for NAT**，NAT会话穿越应用程序）是一种[网络协议](https://zh.wikipedia.org/wiki/网络协议)，它允许位于[NAT](https://zh.wikipedia.org/wiki/网络地址转换)（或多重NAT）后的客户端找出自己的公网地址，查出自己位于哪种类型的NAT之后以及NAT为某一个本地端口所绑定的Internet端端口。这些信息被用来在两个同时处于NAT路由器之后的主机之间创建UDP通信。

**https://tools.ietf.org/html/rfc3489**

```
 STUN - Simple Traversal of User Datagram Protocol (UDP)
               Through Network Address Translators (NATs)
  https://tools.ietf.org/html/rfc3489
```



- 如果给NAT下一个定义，

  NAT是一种虚拟化技术，用于将虚拟化的子网接入到父级网络（通常就是互联网）。

  什么是虚拟化技术呢？虚拟化就是伪装欺骗的艺术，比如在一台电脑上开多台虚拟机，每台虚拟机都分配一个ip，那么在外界看来就存在多台电脑，但实际上只有一台：虚拟机容器将一台电脑虚拟成多台电脑。

  

  

### 挑战2 区块链技术细节：哈希算法

- Merkle Root
- 比特币的交易模型
- 比特币的 hash 算法





问答

- 区块链和P2P不都是去中心化吗，二者有什么联系吗，现在有点搞不清楚了。？

https://www.zhihu.com/question/297410734



