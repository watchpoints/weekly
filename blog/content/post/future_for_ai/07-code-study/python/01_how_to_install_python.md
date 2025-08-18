---
title: Cursor 实战技巧：如何快速配置 Python 环境
date: 2025-04-24
description: 从青铜到王者
draft: false
tags:
  - python
categories:
---
#python入门 #日拱一卒 #从青铜到王者 



Python 作为一门功能强大的编程语言，在数据分析、人工智能、Web 开发等领域有着广泛的应用。其简洁的语法和丰富的第三方库使其成为开发者的首选语言之一。

Cursor 作为一款现代化的集成开发环境（IDE），不仅支持多种编程语言，还提供了智能代码补全、实时语法检查、代码重构等高级功能，能够显著提升开发效率。


本文将重点介绍如何在 Cursor 中配置 Python 环境.

## 1. 安装 Cursor

https://www.cursor.com/cn
## 2. 安装python

https://www.python.org/


window：

1. 环境变量设置  path:D:\tools\python\Scripts

 python --version
 Python 3.13.3

2. 依赖按照（可选）
-  
- **pip install playwright**  
    安装Playwright自动化测试框架
- **playwright install**  
    安装Playwright浏览器驱动程序：自动安装Chromium、Firefox和WebKit浏览器
- **playwright codegen**  
    一键录制：自动生成代码 后面不跟参数时默认打开Chromium的空白浏览器，如果跟一个网址参数，会自动打开此网址的浏览器


 
## 3. Cursor 配置Python开发环境

### window
三步走：
1. 在应用商店（Ctrl+Shift+X打开）安装python插件
2. 然后点ctrl+shift+p搜索python: Select Interpreter，选择解析器
3.  运行自己写pyhton程序 

Python解释器是一个可以理解和执行Python代码的程序



### 其他插件


### markdown-preview-enhanced
- https://shd101wyy.github.io/markdown-preview-enhanced/#/zh-cn/
- https://github.com/Maserhe/VScode-Markdown-theme-Maserhe
### mac 环境： Cursor配置虚拟python环境


mac环境安装第三方依赖 需要虚拟环境

``` python

删除历史虚拟环境 
rm -rf /path/to/venv

历史版本 Python 3.9.6
这是 macOS 系统自带的 Python，位于 /usr/bin/python3。
这个是系统 Python，不建议删除它，
因为一些系统功能可能依赖于它。


/usr/local/bin/python3.12 -m venv venv

source venv/bin/activate 
&& pip install apscheduler playwright
playwright install

```

选择一个 Python 解释器

- 可用解释器列表，包括虚拟环境
- 说明 在mac环境下，不允许执行 pip命令，必须设置虚拟环境，虚拟环境设置后，cursor 选择对应解释器为虚拟环境的解释器。


/usr/local/bin/python3.12  ---已经被删除了怎么处理


python3 -m playwright codegen https://www.cnblogs.com/




## 链接我 




关注公共号：后端开发成长指南  

如果更进一步交流 添加 微信：wang_cyi


![image.png](https://money-1256465252.cos.ap-beijing.myqcloud.com/2025/20250331222159.png)
