---
title: "Tidb入门1"
date: 2022-01-11
description: "Tidb"
draft: false
tags: ["Tidb"]
---
# 入门

tiup cluster list
~~~
Name       User  Version  Path                                            PrivateKey
----       ----  -------  ----                                            ----------
tidb-test  tidb  v5.0.2   /root/.tiup/storage/cluster/clusters/tidb-test  /root/.tiup/storage/cluster/clusters/tidb-test/ssh/id_rsa
root@money:~#

~~~
tiup cluster start 

## 调试技巧

- 启动 mdb后，客户度连接

mysql -h 127.0.0.1 -P 4000 -u root -D test

