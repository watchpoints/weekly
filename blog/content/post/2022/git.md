---
title: "git入门"
date: 2022-03-26
description: ""
draft: false
tags: ["book"]
---





# 冲突解决

### [放弃本地修改](https://zhuanlan.zhihu.com/p/547287193)（粗暴）

1. 修改了本地文件，但是没有使用git add

​      git checkout .



通过git sta[sh命令](https://so.csdn.net/so/search?q=sh命令&spm=1001.2101.3001.7020)，把工作区的修改提交到栈区，目的是保存工作区的修改；
通过git pull命令，拉取远程分支上的代码并合并到本地分支，目的是消除冲突；
通过git stash pop命令，把保存在栈区的修改部分合并到最新的工作空间中；



# 编码 教训

- 不能在window 修改代码，然后上传到liunx提交代码 【大量混乱 不行】【不运行这样操作发送】

  纠正过去错误习惯：window 开发 然后上传liunx编译开发 【学会远程开发工具IDE 和 vi等命令】

## 操作



```bash
git stash
```

###  解决冲突



Please, commit your changes or stash them before you can switch branches



- [07★Git入门★比较修改内容 git diff](https://www.bilibili.com/video/BV1sf4y1D7bF/?vd_source=432dc5326188b5e1aa7119a815a8b105)

​      [git必知必会第三课：git status与git diff

  

`1. git diff -- 文件名`：查看具体某个文件 在工作区和暂存区之间的差异

 git diff -- ob_system_variable.cpp



如何回退代码

https://www.bilibili.com/video/BV1T5411j76k/?spm_id_from=333.788.recommend_more_video.4&vd_source=432dc5326188b5e1aa7119a815a8b105

https://www.bilibili.com/video/BV19B4y1u7vm/?spm_id_from=333.788.recommend_more_video.3&vd_source=432dc5326188b5e1aa7119a815a8b105 	

https://www.cnblogs.com/haima/p/13365483.html



https://www.bilibili.com/video/BV14i4y1C7tM/?vd_source=432dc5326188b5e1aa7119a815a8b105

https://www.liaoxuefeng.com/wiki/896043488029600/1216289527823648



https://juejin.cn/post/6844904089600409613



https://www.bilibili.com/video/BV1cv411u7wd/?spm_id_from=333.788.recommend_more_video.-1&vd_source=432dc5326188b5e1aa7119a815a8b105

- 如何合并代码

  git checkout -b newB

  git checkout master

  git merge newB



- 如何提交代码？

  git push <远程主机名> <本地分支名>:<远程分支名>

  git push -u origin newB







## doc

1. https://git-scm.com/docs/git-diff



# Vim 编译器

1. 显示行号

vim ~/.vimrc  #内容如下 set nu





# lin.vim使用



```
    git clone https://github.com/linrongbin16/lin.vim ~/.vim && cd ~/.vim && ./install.sh
    https://zhuanlan.zhihu.com/p/145350469
```







































