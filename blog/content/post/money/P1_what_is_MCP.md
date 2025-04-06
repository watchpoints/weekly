---
title: 大模型上下文协议——MCP详解
date: 2025-03-23
draft: false
tags:
  - tools
---



##  一、这个技术出现的背景 是什么

![](https://github.com/modelcontextprotocol/.github/raw/main/profile/assets/light.png)
24年11月初的时候Anthropic发了一篇技术[博客](https://www.anthropic.com/news/model-context-protocol)，推出了他们的模型上下文协议MCP，同时也开源了基于该协议的sdk（目前只有ts和py版本的）

于是便有了这篇文章，旨在为大家介绍下[MCP协议](https://github.com/modelcontextprotocol)

并以python sdk作为切入做一些关键代码实现的剖析。





   一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

   二、这个技术的优势和劣势分别是什么

   三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

   四、技术的组成部分和关键点。

   五、技术的底层原理和关键实现

   六、已有的实现和它之间的对比
