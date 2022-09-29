---
title: "成为tiflash贡献者第一天"
date: 2022-04-14
description: "tiflash"
draft: false
tags: ["tidb"]
---







## 	成为贡献者day1

x`x`

- [ ] 阅读资料 [TiFlash 相关的学习资料](https://asktug.com/t/topic/662813)

- [ ] ## [Build TiFlash](https://github.com/pingcap/tiflash)

~~~shell
# 环境依赖
cmake version 3.21.2
curl https://sh.rustup.rs -sSf | sh -s -- -y --profile minimal --default-toolchain nightly
source $HOME/.cargo/env

alias python="/usr/local/bin/python3"
python -V
Python 3.9.9
g++ --version
Apple clang version 13.1.6 (clang-1316.0.21.2)
# 编译
git clone git@github.com:watchpoints/tiflash.git
https://github.com/watchpoints/tiflash.git
##当使用git clone下来的工程中带有submodule时，初始的时候，submodule的内容并不会自动下载下来的
git submodule update --init --recursive
##标准流程
cd build
cmake ..
make
~~~

x
