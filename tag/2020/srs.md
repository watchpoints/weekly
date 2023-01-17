





### 转码服务

|              | 目录                  | go1.15.1.linux-amd64.tar.gz |
| ------------ | --------------------- | --------------------------- |
| 10.115.37.59 | /home/wangchuanyi/rts |                             |
| 10.115.37.60 |                       |                             |
|              |                       |                             |

~~~go
export GOROOT=/app/local/go
export GOBIN=$GOROOT/bin
export GOPATH=/app/code
export PATH=$PATH:$GOROOT/bin:$GOBIN

配置文件：
host: localhost:1024
log: /home/wangchuanyi/rts/rts.log
ffmpeg: /app/local/ffmpeg
timeout: 100
kafka: 10.115.37.48:9092
url: zhibo.gome.com.cn
trytimes: 30
rts:
    redisIp: 10.115.37.51:6379
    redisIndex: 0
    orgtopic: GoodsGo
    pushserver: http://glsin.prod.ds.gome.com.cn/Live_Push/push
    getAccessByInput: http://glsin.prod.ds.gome.com.cn/access/getaccessbyinput
~
*/12 * * * *  sh /home/wangchuanyi/rts/monitor_paimai.sh


cat /proc/cpuinfo | grep "cores"|uniq
cpu cores       : 4

~~~

