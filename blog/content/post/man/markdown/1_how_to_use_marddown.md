---
title: "Markdown 入门指南"
date: 2024-08-03
draft: false
tags: ["tools"]
---


https://www.markdown.xyz/getting-started/
https://markdown.com.cn/


# 应用：typora（已经开始收费）

## 整体修改CSS 

1. 依次点击Typora菜单栏的“文件”-“偏好设置”，然后点击“外观”-“打开主题文件夹”
2. 下载本主题样式，解压缩至上述“主题文件夹”
 （其中“dyzj-light.css”、“dyzj-dark.css”和“source/”文件是必须的，其他可以删除），
 重启Typora，点击Typora菜单栏的“主题”，选择“Dyzj Light”（明亮模式）或“Dyzj Dark”（暗黑模式）选项即可
 https://gitee.com/LiYuncom/typora-dyzj-theme

说明：mac 下载时候 路径有空格怎么处理
cp typora-dyzj-theme-master.zip 
/Users/wangchuanyi/Library/Application\ Support/abnerworks.Typora/themes
参考：https://yefeihonours.github.io/post/mac/space_in_directory/


## 在现有主题基础上部分修改css样样式

dyzj.css 添加自定义css

~~~
vim dyzj-light.css
@import url(./source/dyzj.css);
~~~

https://github.com/Soanguy/typora-theme-autumnus

如何调试：
https://github.com/typora/typora-theme-toolkit

# 应用：Obsidian





### 学习方法

~~~
	 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

 二、这个技术的优势和劣势分别是什么 


 三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

 四、技术的组成部分和关键点。

 五、技术的底层原理和关键实现

 六、已有的实现和它之间的对比
~~~

##
- https://theme.typora.io/doc/zh/Write-Custom-Theme/ Write Custom Theme_zh
- https://theme.typora.io/doc/Write-Custom-Theme/
[1].https://blog.51cto.com/u_14315147/4578440 [done]
[2].https://blog.csdn.net/qq_36931982/article/details/122933909[done]


