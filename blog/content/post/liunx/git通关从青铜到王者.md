---
title: "git通关从青铜到王者"
date: 2023-08-11
description: ""
draft: false
tags: ["book"]
---






```shell
git cherry-pick <commit-hash>
git cherry-pick --continue
```








xmind 图片居中对齐






### 第一关：项目最新代码出现问题，你想回退，但是保留自己修改最新代码怎么办？



### 期望结果

n1（正常状态）-----n2-（出现问题，别人的）---n3（我的）

如果n3提交回到n1



步骤：



1.  git  reset --hard n1 

2.  git cherry-pick n3

3.  出现冲突， git status 手工解决冲突问题

4. git add 冲突文件

5. git cherry-pick --continue (保存继续)

6. git commit --amend

   

   

   

   https://www.kimsereylam.com/git/2021/01/01/git-cherry-pick.html

   ```
   git status
   git cherry-pick --continue
   ```

   













































