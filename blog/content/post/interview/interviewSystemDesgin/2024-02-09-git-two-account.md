---
title: "在一台电脑上同时使用多个github账号(亲测有效)"
date: 2024-02-09
draft: false
tags: ["GitHub"]

---





## what：从现象提出问题 

在一台电脑上有两个 github 账号

- git 提交代码时候 
 fatal: 无法读取远程仓库。


- 尝试 密码登陆 

今天是2021年8月13日（情人节），苦涩。
在使用账号密码push代码时，提示了如下错误：

remote: Support for password authentication was removed on August 13, 2021. 
Please use a personal access token instead.

github不再通过验证密码方式push代码

**如何 在一台电脑上同时使用多个github账号**

## why：背后原理

1， 由于 github 是使用 SSH key 的 对应的公钥id_rsa_pub来判定你是哪个账户，

而不是通过用户名。 

2. 如何提交不同主机呢

~~~
当前分支没有跟踪信息。
请指定您要合并哪一个分支。
详见 git-pull(1)。

    git pull <远程> <分支>

如果您想要为此分支创建跟踪信息，您可以执行：

    git branch --set-upstream-to=<远程>/<分支> main
    
参考默认github创建时候 提醒 

echo "# ai_tools" >> README.md
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin git@github.com:AiMoneyClub/ai_tools.git
git push -u origin main
~~~




## how：如何解决

### 步骤一：取消全局配置

~~~
git config --global --unset user.name
git config --global --unset user.email

git config  user.name "你的用户名"
git config  user.email "你的邮箱"
~~~

验证是否添加成功：

对于本地 git repo 配置，先进入 本地git项目 目录，cat git/config 文件，
~~~
[user]
    name = your-username
    email = your-email-address

~~~

### 步骤二：生成多个 SSH key ,修改config配置

~~~
ssh-keygen -t rsa -f ~/.ssh/id_rsa_one -C "one@xxx.com"
ssh-keygen -t rsa -f ~/.ssh/id_rsa_two -C "two@xxx.com"

~~~

在 ~/.ssh目录下新建 config 文件，令不同 Host 实际映射到同一 HostName，但密钥文件不同。

~~~
# one (first account)
Host one.github.com
HostName github.com
PreferredAuthentications publickey
User one
IdentityFile ~/.ssh/id_rsa_one

# two(second account) 
Host two.github.com
HostName github.com
PreferredAuthentications publickey
User two
IdentityFile ~/.ssh/id_rsa_two

~~~


### 步骤三：使用 git 重要

git 的使用一般是从其他仓库直接 clone 或本地新建，注意配置用户名和邮箱。

1、clone 到本地
原来写法
git clone git@github.com: 用户名/learngit.git

现在写法
~~~

$ git clone git@one.github.com: one的用户名/learngit.git
$ git clone git@two.github.com: two的用户名/learngit.git
~~~

2、如何提交代码
~~~

# push 到 github上去
git remote rm origin //清空原有的
git remote add origin git@one.github.com:one/test.git
//git remote add 是 Git 命令，用于添加一个新的远程仓库。
// here is 
git remote add origin git@two.github.com:AiMoneyClub/ai_tools.git


git push --set-upstream origin main
git push 

~~~



- 从 2020 年 10 月 1 日开始，GitHub 上的所有新库都将用中性词「main」命名，取代原来的「master」

这样就实现了一台电脑管理两个 github 账号，接下来可以进行 git 的其他操作。

- 参考用法：https://www.liaoxuefeng.com/wiki/896043488029600/898732864121440





## 联系我 ：



微信号：watchpoints

![image.png](https://s2.loli.net/2024/02/09/yLDuCszWrIQbfj2.jpg)





知识星球：



![image.png](https://s2.loli.net/2024/02/09/xpkYzBu93NZMv2a.png)
