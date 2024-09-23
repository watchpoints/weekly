---
title: "git常用命令手册"
date: 2024-09-20
draft: false
tags: ["GitHub"]

---

1. 并发迁移代码实现，联调90%

2. 支持存力测试演示

   

# git撤销已经push到远端的commit



git reset 是把HEAD向后移动了一下，而git revert是HEAD继续前进，只是新的commit的内容和要revert的内容正好相反，能够抵消要被revert的内容。

提交时候需分支好



提交时候需分支好



git push  xxx origin/XXXX







## COMMIT_EDITMSG

### step1 base

Why does the COMMIT_EDITMSG open when I try to commit in vscode?

change-id



是 Git 中的一个临时文件，它用于保存提交消息（commit message）的编辑内容。当你执行` git commit` 命令时，Git 会打开一个编辑器，供你输入提交消息

其中包含

Change-Id 是 gerrit (代码审核平台)的概念，是gerrit 用以追踪具体提交的机制 与 git (版本管理) 是没有关系的.



![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/9c46ad75c8792ad2b92a573bfb6c4c82.png)

gerrit 的 Change-Id 机制

已经用 git push 将代码提交 gerrit 审核了,这时你发现代码中有疏漏,修改了一下,

执行 git commit --amend, 再次推送还可以成功. 这就是因为 gerrit 检查到两次 push 的 commit 有同一个 change-id, 

就认为是同一个提交,因此可以 amend



Gerrit 提供了标准的“commit-msg”钩子来实现。

Git 提供了4个提交[工作流](https://so.csdn.net/so/search?q=工作流&spm=1001.2101.3001.7020)钩子：pre-commit、prepare-commit-msg、commit-msg、post-commit。其中 commit-msg 钩子，会在我们执行 git commit 时被执行





### step2 do it

~~~
git checkout -b branch_name origin/branch_name
git branch


git status
git cherry-pick --continue	

要撤销git add命令，可以使用git reset命令
https://worktile.com/kb/ask/232168.html
git commit --amend
git puhs origin  xx:ref/for/XXX

~~~

### step3 future



- 一次只提交一个功能
- 

## 参考

【1】 https://www.nowcoder.com/discuss/516086200147382272





## 联系我 ：



微信号：watchpoints

![image.png](https://s2.loli.net/2024/02/09/yLDuCszWrIQbfj2.jpg)





知识星球：



![image.png](https://s2.loli.net/2024/02/09/xpkYzBu93NZMv2a.png)
