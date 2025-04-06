---
title: 源码阅读：OpenManus
date: 2025-03-22
draft: false
tags:
  - tools
---


大家好，我是小王同学，希望本文在了解OpenManus对你有帮助

- **昵称**：做一个为自己写代码的程序员
- **愿景**：成为AIGCC落地解决方案积极参与者
- **相关作品**：无
- **如果有需求请留言**：Wechat：watchpoints

# 一. 用户故事

>  这个技术出现的背景、初衷和要达到什么样的目标

![输入图片说明](https://foruda.gitee.com/images/1738925684729386144/1652974e_116887.png "屏幕截图")


### **OpenManus的定位与设计目标**

OpenManus是一个基于大语言模型（LLM）的智能体框架，它的设计目标是创建一个灵活、可扩展且功能强大的系统，使AI能够通过各种工具与外部世界交互，从而解决复杂的任务。

与传统的聊天机器人不同，OpenManus不仅能够理解和生成文本，还能够执行具体的操作，如搜索信息、浏览网页、执行代码和保存文件等。这种能力使其成为一个真正的"智能助手"，而不仅仅是一个对话系统。

OpenManus的核心理念是"思考-行动"循环，即智能体先分析当前状态和任务需求（思考），然后选择并执行适当的工具（行动），接着基于执行结果进行下一轮思考。这种循环使智能体能够逐步解决复杂问题，同时保持对任务的连贯理解。

##  阅读对象

- 新手用户
    
- 使用国内的模型
    
- 操作系统面向 Windows 10 / 11

## 教程目标

- 搭建可以正常运行的 OpenManus 环境
    
- 知道如何做对应的模型配置
    
- 了解一些 OpenManus 的基本原理

# 二、 系统设计

- 技术的组成部分和关键点

演示：请帮我诊断oceanbase数据库

标杆产品 [oracle 输出](https://blog.csdn.net/yabingshi_tech/article/details/121544370)
![](https://cdn.nlark.com/yuque/0/2025/png/22771290/1742283800556-3e6eb861-46b8-4505-bb8c-08490b7e46e6.png?x-oss-process=image%2Fformat%2Cwebp%2Fresize%2Cw_1125%2Climit_0)


# 三、 模块设计
 
### 3.1  项目结构


统计代码量-代码统计工具 CLOC 
1. sudo apt install cloc
2. 执行 cloc . 
·
``` shell
github.com/AlDanial/cloc v 1.90  T=0.14 s (408.5 files/s, 48994.5 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
Python                          36            733            509           2846
CSS                              1             95              4            606
HTML                             4             45              0            595
JavaScript                       1             88             15            594
Markdown                         7            160              0            381
YAML                             5              9              0            116
DOS Batch                        1              6              0             36
TOML                             2             11             68             25
INI                              1              2              0             12
-------------------------------------------------------------------------------
SUM:                            58           1149            596           5211
-------------------------------------------------------------------------------

```

OpenManus的项目结构清晰而模块化，主要包括以下几个部分：

``` shell
app/
├── agent/                # **智能体实现
│   ├── base.py           # **基础智能体
│   ├── react.py          # **思考-行动智能体
│   ├── toolcall.py       # **工具调用智能体
│   └── manus.py          # **Manus智能体
├── tool/                 # **工具实现
│   ├── base.py           # **基础工具
│   ├── bash.py           # **命令行工具
│   ├── browser_use_tool.py # **浏览器工具
│   ├── file_saver.py     # **文件保存工具
│   ├── python_execute.py # **Python执行工具
│   ├── terminate.py      # **终止工具
│   └── tool_collection.py # **工具集合
├── flow/                 # **流程控制
│   ├── base.py           # **基础流程
│   ├── planning.py       # **规划流程
│   └── flow_factory.py   # **流程工厂
├── prompt/               # **提示模板
│   └── manus.py          # **Manus提示
├── llm.py                # **LLM接口
├── memory.py             # **记忆系统
└── message.py            # **消息定义
main.py                   # **主入口

```






相关资料
- https://zhuanlan.zhihu.com/p/30577364301
- https://www.cnblogs.com/lissajous/articles/18770887
- https://404digital.feishu.cn/docx/TO6WdA64OoDsevxo4Zpc7HuCn4g

   一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

   二、这个技术的优势和劣势分别是什么

   三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

   四、技术的组成部分和关键点。

   五、技术的底层原理和关键实现

   六、已有的实现和它之间的对比
