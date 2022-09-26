### 带宽



查看网口带宽
ethtool ens17f3
可以看到是千兆带宽
Speed: 1000Mb/s

scp -l 716800 -v 文件  192.168.42.122:/root/
-l 716800 限速700M
-v 显示速度

[libpcap](https://so.csdn.net/so/search?q=libpcap&spm=1001.2101.3001.7020)是一个网络数据包捕获函数库，功能非常强大，Linux下著名的tcpdump就是以它为基础的



[yum](https://so.csdn.net/so/search?q=yum&spm=1001.2101.3001.7020) install flex byacc  libpcap ncurses ncurses-devel libpcap-devel

wget http://www.ex-parrot.com/pdw/iftop/download/iftop-1.0pre2.tar.gz





1. 安装perf

```
yum list perf
yum -y install perf.x86_64
```

1. 由于需要到github下载可视化分析工具FlameGraph，所以要先安装git

```
yum list git
yum -y install git.x86_64
git clone https://github.com/JaxYoun/FlameGraph.git
mv FlameGraph fg  //方便起见，重命名FlameGraph目录
```

1. 采集、转换、可视化【root权限】

```
perf record -F 99 -p 18733 -g -- sleep 30
perf script -i perf.data &> perf.unfold
fg/stackcollapse-perf.pl perf.unfold &> perf.folded
fg/flamegraph.pl perf.folded > perf.svg
```







### 