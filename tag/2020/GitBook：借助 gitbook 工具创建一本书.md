> GitBook 是一个基于 Node.js 的命令行工具，可使用 Github/Git 和 Markdown 来制作精美的电子书
>
> 本书将简单介绍如何安装、编写、生成、发布一本在线图书。





# 安装步骤



- 创建一个新的仓库

  https://github.com/wangcy6/weekly_read

- 创建一本书





### GitBook 准备工作

~~~shell

### 安装 Node.js
node -v

### 安装 GitBook
npm config set registry=http://registry.npm.taobao.org -g

npm install gitbook-cli -g
gitbook -V

### Windows下GitBook客户端安装


###Gitbook 本地使用
gitbook init
gitbook serve

http://localhost:4000/
~~~



- ssh

  ~~~
  [2] SSH KEY
  
  由于本地 Git 仓库和 Github 仓库之间的传输是通过 SSH 加密的，所以连接时需要设置一下：
  
  Step1：先看一下C:\Users\wangchuanyi.GOME 有没有.ssh 目录，
  
  有的话看下里面有没有 id_rsa 和 id_rsa.pub 这两个文件，有就跳到下一步，没有就通过下面命令创建:
  
  打开 Git Bash，输入如下命令，然后连续按三个回车即可：
  
  ssh-keygen -t rsa -C "wang_cyi@163.com"
  
  这时你就会在用户下的.ssh 目录里找到 id_rsa 和 id_rsa.pub 这两个文件
  
  Step2
  ：登录 Github,找到右上角的图标，打开点进里面的 Settings，再选中里面的 SSH and GPG KEYS，点击右上角的 New 
  
  SSH key，然后 Title 里面随便填，再把刚才 id_rsa.pub 里面的内容复制到 Title 
  下面的 Key 内容框里面
  ，最后点击 Add SSH key，这样就完成了 SSH Key 的加密。
  ~~~

  



## GitHub Pages

[GitHub Pages](https://pages.github.com/) is designed to host your personal, organization, or project pages from a GitHub repository.

 Your site is published at https://wangcy6.github.io/weekly_read/









## ref

- http://www.chengweiyang.cn/gitbook/github-pages/README.html

- https://github.com/Alvinyw/GitBook
- [https://www.blackcat.monster/index.php/2020/07/26/%E6%90%AD%E5%BB%BA%E8%87%AA%E5%B7%B1%E7%9A%84gitbook%E5%B9%B6%E5%B0%86%E5%85%B6%E6%89%98%E7%AE%A1%E5%88%B0github/](https://www.blackcat.monster/index.php/2020/07/26/搭建自己的gitbook并将其托管到github/)