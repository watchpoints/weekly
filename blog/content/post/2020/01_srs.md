---
title: "SRS官方文档和学习路径"
date: 2021-06-06
description: "高性能可扩展服务"
draft: false
categories: ["code_reading"]
---

# 卡片

SRS4.0官方文档和学习路径

https://mp.weixin.qq.com/s/AZhSQbCMBlaZv-0-myc1zg

https://ossrs.net/releases/



## 第一天：快速预览

环境：2g+Ubuntu 



- 在vpn环境完成搭建(docker安装不上)

```
git clone -b 4.0release https://gitee.com/ossrs/srs.git &&
cd srs/trunk && ./configure && make && ./objs/srs -c conf/srs.conf

/root/code/c++/srs/trunk

nohup ./objs/srs -c conf/srs.conf &
```

- 本地obs/FFMEG

~~~c++
nohup ./ffmpeg  -re -i ./hengping.mp4  -vcodec copy -acodec copy -f flv rtmp://74.120.174.137:1935/live/wangchuanyi >>./push3.log 2>&1 &
ffmpeg -re -i 1.mp4 -vcodec copy -acodec copy -f flv rtmp://74.120.174.137/live/wangchuanyi


查看srs日志 、、
cd /root/code/c++/srs/trunk/objs
tail -f srs.log  //目前看不懂 

https://gitee.com/ossrs/srs/wikis/v4_CN_SampleRTMP?sort_id=3957465
~~~



- VLC观看

~~~shell
http://74.120.174.137:8080/

1. RTMP (by VLC): rtmp://74.120.174.137:1935/live/wangchuanyi --播放正常

2. H5(HTTP-FLV): http://74.120.174.137:8080/live/wangchuanyi.flv --播放正常、
http://74.120.174.137:8080/console/ng_index.html#/streams?port=1985&schema=http&host=74.120.174.137

3. H5(HLS): http:/74.120.174.137:8080/live/wangchuanyi.m3u8 --测试不通过【相关配置和原理不清楚】
4. H5(WebRTC): webrtc://74.120.174.137/live/wangchuanyi --测试不通过【相关配置和原理不清楚】


~~~



![image-20210531211418617](C:\Users\wangchuanyi\AppData\Roaming\Typora\typora-user-images\image-20210531211418617.png)

#### 基础知识补充：*视音频数据处理入门* [雷霄骅](https://blog.csdn.net/leixiaohua1020)



音视频协议：RTMP，HTTP-FLV，HLS和WebRTC

[视音频数据处理入门：RGB、YUV像素数据处理](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fleixiaohua1020%2Farticle%2Fdetails%2F50534150)
 [视音频数据处理入门：PCM音频采样数据处理](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fleixiaohua1020%2Farticle%2Fdetails%2F50534316)
 [视音频数据处理入门：H.264视频码流解析](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fleixiaohua1020%2Farticle%2Fdetails%2F50534369)
 [视音频数据处理入门：AAC音频码流解析](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fleixiaohua1020%2Farticle%2Fdetails%2F50535042)
 [视音频数据处理入门：FLV封装格式解析](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fleixiaohua1020%2Farticle%2Fdetails%2F50535082)
 [视音频数据处理入门：UDP-RTP协议解析](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fleixiaohua1020%2Farticle%2Fdetails%2F50535230)



![image.png](https://i.loli.net/2021/06/01/HJa6f5sYhwrO2kV.png)



![image.png](https://i.loli.net/2021/06/01/xGBqrt2Ijwn5CSK.png)



- **Flash Video** is a container file format used to deliver digital video content  

  https://blog.csdn.net/leixiaohua1020/article/details/50535082?spm=1001.2014.3001.5501

### 第一天 FQA

##### 问题：协议rtmp怎么传输数据的呀？

1. 这个是谁产生的呀？摄像头吗？，ffmpeg？ obs  经过编解码了吗 

   

## 第二天（6.1） 深入场景 主要了解功能

资料：*https://gitee.com/ossrs/srs/wikis/v4_CN_Home#srs-overview*

           https://gitee.com/ossrs/srs/wikis/v4_CN_Home#solution-guides

资料：[【免费】FFmpeg/WebRTC/RTMP/NDK/Android音视频流媒体高级开发](https://ke.qq.com/course/3202131)

- SRS是一个简单高效的实时视频服务器，支持RTMP/WebRTC/HLS/HTTP-FLV/SRT/GB28181

  

### 通过下面图片看到 相关知识框架

![](https://10.idqqimg.com/qqke_course_info/ajNVdqHZLLC4FCr3MVJTPQMdriaWTtibiaGflowOyQ9pZ1N1dnKZkevcWBE9PsCMib3wZqgEibT4osXc/)



阅读：SRS支持SRT的详细步骤。 https://gitee.com/ossrs/srs/wikis/v4_CN_SampleSRT

阅读：视频播放器原理 https://www.cnblogs.com/linuxAndMcu/p/12038740.html

![](https://img-blog.csdn.net/20140201120523046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**解协议**的作用 ---->**解封装**的作用 ---**解码**的作用

- FLV 格式的数据，经过解封装操作后，输出 H.264 编码的视频码流和 AAC 编码的音频码流





### 第三天(6-8) 集群部署配置

### qa

- 问题：srs已经支持Origin Cluster，假如 3个节点做为源节点，客户端在推送时候可以随便选择任意一个节点推送？在Origin Cluster前面添加一个lvs可以了
- 问题：推流srs单点故障怎么办？









### note： 问题：推流srs单点故障怎么办？阅读

Cluster: Origin Cluster for Fault Tolarence and Load Balance, 源站热备和负载均衡集群 #464

https://github.com/ossrs/srs/issues/938

https://github.com/ossrs/srs/issues/1607#issuecomment-586543387

https://gitee.com/ossrs/srs/wikis/v4_CN_OriginCluster



Origin Cluster设计目标是<5k个流以内的集群，或者用于少量流的容灾。若需要100k流级别，请参考 [#1607 (comment)](https://github.com/ossrs/srs/issues/1607#issuecomment-586543387)

Origin Cluster设计目标是<5k个流以内的集群，或者用于少量流的容灾。若需要100k流级别，请参考 [#1607 (comment)](https://github.com/ossrs/srs/issues/1607#issuecomment-586543387)

这个方案中，每个源站互相访问，这意味着每个源站都是一个独立的服务。由于每个源站都需要对Edge提供服务，或者说源站需要被访问，每个源站都需要有个服务地址。有两种方式：

- 无状态源站集群：1~3个源站，每个源站需要单独创建Deployment和Service。好处就是它是无状态，不用互相关联，稳定性会比较高。
- 有状态源站集群：3~30个源站，只需要创建一个StatefulSets和Service。好处就是配置简单，坏处是有状态比较复杂，创建后只能更新Replicas、Template、UpdateStrategy等几个字段。

> 以上两种情况，这都需要源站配置`coworkers`，和边缘配置`origin`时，配置上所有的源站的地址。

## 无状态源站集群(Deployment)

适用于极少的流，比如<100个流，1~3个源站。

SRS的源站集群本身是有状态的，也就是请求某个流必须在某个服务器上，而不是可以随便在一个服务器就能拉到流。我们也不能将多个源站挂在SLB后面，播放某个流时SLB会随机选择一台源站，所以可能会到错误的源站上去，或者说流的状态和流的数据是在某个源站上的，并非无状态。

那么我们这里说的无状态源站集群，是源站集群以Deployment(无状态应用)方式部署。由于每个源站都需要一个独立的Deployment，每个Deployment只有一个Replicas，每个Deployment对应的Service(ClusterIP)也有独立的名称，实际上就相当于SLB后面只挂了一个源站，比如：

| 源站            | Deployment      | Service          | 域名             |
| --------------- | --------------- | ---------------- | ---------------- |
| Origin Server 0 | origin-0-deploy | origin-0-service | origin-0-service |
| Origin Server 1 | origin-1-deploy | origin-1-service | origin-1-service |
| Origin Server 2 | origin-2-deploy | origin-2-service | origin-2-service |
| Origin Server N | origin-N-deploy | origin-N-service | origin-N-service |

> Note：无状态集群的部署实例，参考[Wiki](https://github.com/ossrs/srs/wiki/v4_CN_K8s#build-srs-origin-cluster-as-deployment)。

每个源站创建一个独立的Deployment，Replicas为1，创建对应的Service类型为ClusterIP。这种方案比较麻烦，不过迁移到OCM( [#1607](https://github.com/ossrs/srs/issues/1607) )时会比较方便，源站使用的是service-name，而不是pod-name.service-name方式彼此寻址。



https://github.com/ossrs/srs/wiki/v4_CN_K8s#build-srs-origin-cluster-as-deployment



## 有状态源站集群(StatefullSet)

适用于一定数量的流，比如<5k个流，5~30个之内的源站。

在K8s中，每个源站都需要响应的Service，可选择[StatefulSets](https://v1-14.docs.kubernetes.io/docs/concepts/workloads/controllers/statefulset/)，用[HeadlessService](https://v1-14.docs.kubernetes.io/docs/concepts/services-networking/service/#headless-services)给让每个源站Pod具备寻址能力。比如：

| 源站            | StatefulSet | Service | 域名             |
| --------------- | ----------- | ------- | ---------------- |
| Origin Server 0 | origin      | service | origin-0.service |
| Origin Server 1 | origin      | service | origin-1.service |
| Origin Server 2 | origin      | service | origin-2.service |
| Origin Server N | origin      | service | origin-N.service |

> Note：有状态集群的部署实例，参考[Wiki](https://github.com/ossrs/srs/wiki/v4_CN_K8s#srs-origin-cluster-for-a-large-number-of-streams)。

只需要创建一个StatefulSets和一个Service，Replicas设置为源站的数目。

源站配置为`coworkers: origin-0.service origin-1.service origin-2.service;`
边缘配置为所有源站，或部分源站：`origin origin-0.service origin-1.service origin-2.service;`

可以看到确实会比较繁琐，而且新增源站时需要更新其他源站配置，以及边缘的配置，这个方案在30个以内的源站节点是适用的

【这个kafa的配置，redis集群配置类似呀】‘



源站集群支持<5k路流方案，参考：[#464 (comment)](https://github.com/ossrs/srs/issues/464#issuecomment-586550787)

源站集群支持<100k路流方案，参考：[#1607 (comment)](https://github.com/ossrs/srs/issues/1607#issuecomment-586543387)

关于源站集群中，源站的服务地址定义问题，参考：[#1501 (comment)](https://github.com/ossrs/srs/issues/1501#issuecomment-559946431)

关于源站集群中，多节点源站轮询问题，参考：[#1501 (comment)](https://github.com/ossrs/srs/issues/1501#issuecomment-559938654)

关于源站集群的存储问题，参考：[#1595 (comment)](https://github.com/ossrs/srs/issues/1595#issuecomment-586550583)

关于源站集群的API问题，参考：[#1607 (comment)](https://github.com/ossrs/srs/issues/1607#issuecomment-586549464)





https://github.com/ossrs/srs/wiki/v4_CN_K8s#srs-origin-cluster-for-a-large-number-of-streams





https://github.com/ossrs/srs/issues/1607:

https://github.com/ossrs/srs/wiki/v3_EN_SampleOriginCluster



源站集群主要解决大量推流的情况

https://github.com/ossrs/srs/wiki/v3_CN_OriginCluster

推荐在源站集群前面挂一系列的Edge服务器，参考[这里](https://github.com/ossrs/srs/issues/464#issuecomment-366169962)，Edge服务器可以转换协议，支持RTMP和HTTP-FLV，同时支持源站故障时自动切换，不中断客户端。



![https://wallenotes.github.io/2020/04/14/%E6%95%B0%E6%8D%AE%E5%BA%93/Cassandra/Cassandra-001/](https://wallenotes.github.io/2020/04/14/%E6%95%B0%E6%8D%AE%E5%BA%93/Cassandra/Cassandra-001/figure3.png)

- 目前源站集群(Origin Cluster)方案中，流的状态是靠源站之间互相访问获知，源站地址是通过配置在配置文件中的，扩容时需要更新这个状态

SrsRtmpConn::playing

https://github.com/ossrs/srs/wiki/v3_EN_OriginCluster

~~~
vhost __defaultVhost__ {
    # The config for cluster.
    cluster {
        # The cluster mode, local or remote.
        #       local: It's an origin server, serve streams itself.
        #       remote: It's an edge server, fetch or push stream to origin server.
        # default: local
        mode            local;

        # For origin(mode local) cluster, turn on the cluster.
        # @remark Origin cluster only supports RTMP, use Edge to transmux RTMP to FLV.
        # default: off
        # TODO: FIXME: Support reload.
        origin_cluster      on;

        # For origin (mode local) cluster, the co-worker's HTTP APIs.
        # This origin will connect to co-workers and communicate with them.
        # please read: https://github.com/ossrs/srs/wiki/v3_EN_OriginCluster
        # TODO: FIXME: Support reload.
        coworkers           127.0.0.1:9091 127.0.0.1:9092;
    }
}
~~~

- HA（High Availability 高可用方案）：这个是为了解决 NameNode 单点问题；

  两台 NameNode 形成互备，一台处于 Active 状态，为主 NameNode，另外一台处于 Standby 状态，为备 NameNode，只有主 NameNode 才能对外提供读写服务；

- 直播链路跟踪



### note：阅读：[开源流媒体服务器SRS学习笔记(4) - Cluster集群方案](https://www.cnblogs.com/yjmyzz/p/srs_study_4_cluster.html)

https://www.cnblogs.com/yjmyzz/p/srs_study_4_cluster.html

- 这个文章主要利用 http://www.haproxy.org/ 特点。

  



###  Cluster Guides

- [Origin Cluster](https://gitee.com/ossrs/srs/wikis/v4_CN_OriginCluster): 如何支持源站集群，扩展源站能力。

- [Edge Cluster: RTMP](https://gitee.com/ossrs/srs/wikis/v4_CN_SampleRTMPCluster): 如何部署RTMP分发集群，譬如CDN支持RTMP分发。

- [Edge Cluster: FLV](https://gitee.com/ossrs/srs/wikis/v4_CN_SampleHttpFlvCluster): 如何部署HTTP-FLV分发集群，譬如CDN支持HTTP-FLV分发。

- [VHOST](https://gitee.com/ossrs/srs/wikis/v4_CN_RtmpUrlVhost): 如何一个集群支持多个用户，即Vhost。

- [Reload](https://gitee.com/ossrs/srs/wikis/v4_CN_Reload): 如何不中断服务的前提下应用新的配置，即Reload。

- [Tracable Log](https://gitee.com/ossrs/srs/wikis/v4_CN_SrsLog): 如何在集群中追溯错误和日志，基于连接的日志，排错日志。

- [Log Rotate](https://gitee.com/ossrs/srs/wikis/v4_CN_LogRotate): 如何切割服务器的日志，然后压缩或者清理。

- [K8s](https://gitee.com/ossrs/srs/wikis/v4_CN_K8s): 如何使用[ACK(阿里云容器服务Kubernetes版)](https://www.aliyun.com/product/kubernetes)部署SRS集群。








#### 6-9



https://github.com/ossrs/srs/issues/1575

https://github.com/ossrs/srs/issues/1607#issuecomment-586543387





