---
title: shell从青铜到王者
date: 2023-08-11
description: ""
draft: false
tags:
  - book
---


### 1. shell 命令获取随机获取 2位的整数?

方法1：使用内置变量 `$RANDOM`

```bash
echo $(( RANDOM % 90 + 10 ))
```
**原理**：  
`$RANDOM` 生成一个0到32767之间的随机数，`% 90` 得到0到89的余数，加10后范围变为10到99。


在文件中第一行添加 key
sed -i 's/^/key/g' xx



查看进程启动时间



Shell脚本实用小技巧-教你屏蔽执行命令的所有显示信息，包含错误信息

-  标准输入（STDIN），文件描述符为 0
- 标准输出（STDOUT），文件描述符为 1
- 标准错误（STDERR），文件描述符为 2

1. **重定向到 `/dev/null`**：
    - `command > /dev/null`：仅隐藏标准输出。
    - `command 2> /dev/null`：仅隐藏标准错误。
    - `command &> /dev/null` 或 `command > /dev/null 2>&1`：同时隐藏标准输出和标准错误。

























