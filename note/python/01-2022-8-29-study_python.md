

### day3



### 提问2/100： - how to build rpm packages



查看`.rpm` 包依赖 缺少 打的rpm是否正确

-  rpm -qpR 

- yum deplist 

SPEC 阶段与目录的对应关系[#](https://www.cnblogs.com/michael-xiang/p/10500704.html#1297686658)

- %install: 安装步骤,此时需要指定安装路径，创建编译时自动生成目录，复制配置文件至所对应的目录中（这一步比较重要！）

-  %py3_install

  Runs the `setup.py install` command with arguments suitable for an RPM package

  些文件就是用户安装 RPM 后，最终得到的文件。注意一个奇怪的地方: 最终安装目录 不是 构建目录。通过执行类似 make install 的命令实现。

  - %build #执行编译命令，编译后会在BUILD目录下存在暂时文件

  









----过程

- https://access.redhat.com/sites/default/files/attachments/rpm_building_howto.pdf

- https://docs.fedoraproject.org/en-US/packaging-guidelines/Python_Appendix/

- https://rogerwelin.github.io/rpm/rpmbuild/2015/04/04/rpmbuild-tutorial-part-1.html
- https://unix.stackexchange.com/questions/226319/rpm-and-defattr
- https://www.golinuxcloud.com/rpmbuild-command-in-linux/
- https://www.cnblogs.com/zhangxinglong/p/11904922.html
- https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/9/html/installing_and_using_dynamic_programming_languages/assembly_packaging-python-3-rpms_installing-and-using-dynamic-programming-languages





#### 问题1/100：如何安装mpi4py

回答：





过程：

- rpm打包流程步骤 https://blog.csdn.net/LvJzzZ/article/details/114328421

- rpmbuild tutorial - how to build rpm packages

- 

  

  1.  	下载源码包时，需要 保留时间戳。如果有多个源码包，请用 Source1，Source2 等依次列出。如果你需要添加额外文件，请将它们列在后面

  ```text
  python setup.py bdist_rpm
  ```

~~~

[root@h12-meta01 SOURCES]# pip3 install --help |grep target
  -t, --target <dir>          Install packages into <dir>. By default this will not replace existing files/folders in <dir>. Use --upgrade to replace existing packages in <dir> with new versions.


~~~



- This is the **MPI for Python** package

https://mpi4py.readthedocs.io/en/latest/install.html#envvar-MPI4PY_BUILD_BACKEND

- pip3 install  mpi4py-3.1.3.tar.g

- ```
  python -c "import mpi4py"
  
  依赖：mpich
  一般情况下会默认安装在 /usr/local
  ./configure --prefix=/usr/local/mpich
  make CFLAGS='$CFLAGS -fpic' 
  //添加动态库标记
  
  export PATH=$PATH:/usr/local/mpich/bin
  export LD_LIBRARY=$LD_LIBRARY:/usr/local/mpich/lib
  
  pip3 install mpi4py-3.1.3.tar.gz
  python -c "import mpi4py"
  ```



~~~
rpm -qa |grep mpich
mpich-3.3.2-9.el8.x86_64
mpich-devel-3.3.2-9.el8.x86_64

+ mv /root/rpmbuild/BUILDROOT/beegfs-move-utils-1.0-1.el8.x86_64/etc/beegfs/move/beegfs_move.conf /root/rpmbuild/BUILDROOT/beegfs-move-utils-1.0-1.el8.x86_64/etc/beegfs/move/beegfs_move.conf.rpmsave
+ exit 0
[root@h12-meta01 SPECS]# python3 -m site
sys.path = [
    '/root/rpmbuild/SPECS',
    '/usr/lib64/python36.zip',
    '/usr/lib64/python3.6',
    '/usr/lib64/python3.6/lib-dynload',
    '/usr/local/lib64/python3.6/site-packages',
    '/usr/local/lib/python3.6/site-packages',
    '/usr/lib64/python3.6/site-packages',
    '/usr/lib/python3.6/site-packages',
]
USER_BASE: '/root/.local' (exists)
USER_SITE: '/root/.local/lib/python3.6/site-packages' (doesn't exist)

python3 -m site -help
/usr/lib64/python3.6/site.py [--user-base] [--user-site]


~~~



如何查看依赖？



pip3 freeze >requirements.txt 

a

pip3 install --no-index --find-links=packages -r requirements.txt 

**6）通过shell脚本安装离线包**

具体shell脚本命令如下：

pip install --no-index --find-links=packages -r requirements.txt 

***说明：\***

首先将packages文件夹和requirements.txt拷贝至离线服务器：packages文件夹放在任意目录下，requirements.txt放在同级目录





    from src.config.policy import get_policies_string


ModuleNotFoundError: No module named 'src'

添加pth文件之后里面的路径会自动添加到sys.path

src目录加到环境变量PYTHONPATH里也可以

![image-20220927092246527](assets/image-20220927092246527.png)

```
export PYTHONPATH=$PYTHONPATH:/root/rpmbuild/SOURCES/migration-engine-utils-1.0/src
```

# day2 





How to  change access and modified time of file  In Python

- [ Linux Programmer's Manual :: futimes, lutimes - change file timestamps](https://man7.org/linux/man-pages/man3/futimes.3.html)

  int setTimeRes = futimes(fd, timeValues); 

- 









~~~python
def utime(*args, **kwargs): # real signature unknown
    """
    Set the access and modified time of path.
    
    路径可以始终指定为字符串。
    path may always be specified as a string.
    On some platforms, path may also be specified as an open file descriptor.
      If this functionality is unavailable, using it raises an exception.
    
    If times is not None, it must be a tuple (atime, mtime);
        atime and mtime should be expressed as float seconds since the epoch.
    If ns is specified, it must be a tuple (atime_ns, mtime_ns);
        atime_ns and mtime_ns should be expressed as integer nanoseconds
        since the epoch.
    If times is None and ns is unspecified, utime uses the current time.
    Specifying tuples for both times and ns is an error.
    
    If dir_fd is not None, it should be a file descriptor open to a directory,
      and path should be relative; path will then be relative to that directory.
    If follow_symlinks is False, and the last element of the path is a symbolic
      link, utime will modify the symbolic link itself instead of the file the
      link points to.
    It is an error to use dir_fd or follow_symlinks when specifying path
      as an open file descriptor.
    dir_fd and follow_symlinks may not be available on your platform.
      If they are unavailable, using them will raise a NotImplementedError.
    """
    pass
~~~





> [How to Install Latest Version Of Python 3 on CentOS 7](https://phoenixnap.com/kb/how-to-install-python-3-centos-7#:~:text=Installing%20Python%20on%20CentOS%207%20There%20are%20two,package%20manager.%20Downloading%20it%20from%20the%20source%20code.)



![](https://phoenixnap.com/kb/wp-content/uploads/2021/08/how-to-install-python-on-centos7-1.png)





## **Installing Python on CentOS 7**

There are two ways to install Python 3 on your CentOS system:

- Using the official CentOS package manager.

- Downloading it from the source code.

  

~~~
sudo yum install https://repo.ius.io/ius-release-el$(rpm -E '%{rhel}').rpm
sudo yum update -y
sudo yum install -y python3
python3 --version

~~~



~~~

sudo yum groupinstall "Development Tools" -y
sudo yum install gcc open-ssl-devel bzip2-devel libffi-devel -y
wget https://www.python.org/ftp/python/3.6.8/Python-3.6.8.tgz
./configure --enable-optimizations

yum install epel-rpm-macros
+make %{?_smp_mflags}

# 生成编译文件清单，prefix指定安装路径，默认是/usr/local
# sudo ./configure --prefix=/usr/local/python3.7.9 --with-openssl=/usr/local/openssl --enable-shared  # 适用于自编译openssl的情况
 
# 直接使用系统库的ssl更便捷
sudo ./configure --prefix=/usr/local/python3.7.9 --with-ssl --enable-shared 
 
# 编译安装
make install
 
# 建立快捷方式
ln -s /usr/local/python3.7.9/bin/python3.7 /usr/bin/python3
 
# 设置动态链接库路径
echo "/usr/local/python3.7.9/lib/" >> /etc/ld.so.conf
 
# 刷新动态链接库路径缓存
ldconfig
 
# 验证命令
python3 --version
 
# 切换源为国内
python3 -m pip config set global.index-url https://mirrors.aliyun.com/pypi/simple/

~~~



seq 1000 |xargs touch

 

Python作为一门动态语言，变量本身的类型是不固定的，因此更加灵活。那Python到底是值传递还是引用传递呢？



https://blog.csdn.net/hohaizx/article/details/78427406


2. Python中的传址和传值是怎样的呢？

Python是不允许程序员选择采用传值还是传址的。Python参数传递采用的肯定是“传对象引用”的方式。实际上，这种方式相当于传值和传址的一种综合。

如果函数收到的是一个可变对象（比如字典或者列表）的引用，就能修改对象的原始值——相当于传址。如果函数收到的是一个不可变对象（比如数字、字符或者元组）的引用（其实也是对象地址！！！），就不能直接修改原始对象——相当于传值。

所以python的传值和传址是根据传入参数的类型来选择的

传值的参数类型：数字，字符串，元组（immutable）

传址的参数类型：列表，字典（mutable）









target_link_libraries 和link_libraries区别
[root@node1 local]# find ./ -name  libmariadb.so.3
./oceanbase/deps/devel/lib/mariadb/libmariadb.so.3


#Generate the static library from the library sources
add_library(hello_library STATIC
        src/hello.cpp
        )


mpirun进程在其他主机节点没有启动


把别人知道的东西能不能做到

避免外部打扰
1. 断点续传，记录下来进展
2. 带上降噪耳机
3 在会议室，走廊 开辟一个空间



温柔的坚持。
河水目的是奔向大海，而不是和石头对抗

时间管理是个入口，可以应用到其他方面
时间管理是个入口，可以应用到其他方面


不是对抗，默默做，把成果展示出来

python3 -m pip install ./mpi4py-3.1.3.tar.gz
python3 -c "from mpi4py import MPI"

ssh-keygen -t rsa
ssh-copy-id 10.141.162.45
ssh-copy-id 10.141.162.13

python3程序打包

核心：   
        puytho3项目如何打包？
		python3 源码用rpm-build打包,pip3安装的依赖程序如何打包,说明 假如生产不允许pip3安装依赖

原理：

2个方式 
1. rpm-build工具：定义spec文件，rpmbuild -ba xx.spec使用任何语言
2. setuptools工具 定义 setup.py文件，python3 setup.py bdist_rpm 安装在python包的统一管理路径下
   例如 /usr/local/lib64/python3.6/site-packages

spec文件格式：
BuildRoot: 这个是安装或编译时使用的“虚拟目录”
%files 放到rpm包里面去 


而pip安装默认是按照wheel的方式。

build过程，对应C/C++项目中的./configure, make。对Python项目来说一般可以跳过，因为setup.py都会搞定。
也可以选择在这个阶段使用setup.py进行build，build出一个wheel包来，
这是我推荐的方法，因为wheel包和直接setup.py install的目录结构有一些差异，
	
步骤：

spec文件格式（gcc makefile）

## 依赖：pip安装的第三方库

pip3 install mpi4py
pip3 uninstall mpi4py
pip3 uninstall apscheduler

mpi4py https://pypi.org/project/mpi4py/#files 
https://github.com/mpi4py/mpi4py/releases/tag/3.1.3
python setup.py bdist_rpm
psutil
apscheduler:
https://pypi.org/project/APScheduler/#files
https://github.com/agronholm/apscheduler/releases/tag/3.8.1

pip3 install APScheduler-3.9.1-py2.py3-none-any.whl

阅读：
https://www.cnblogs.com/dechinphy/p/setup.html ok

https://www.zhihu.com/question/51645857/answer/130745920

///////////////////////////////

提问：
 rpm 如何打包  

 python3 源码用rpm-build打包,pip3安装的依赖程序如何打包,说明 假如生产不允许pip3安装依赖
 依赖软件怎么安装
 https://blog.csdn.net/livingbody/article/details/83989306

原理：
https://docs.python.org/2.0/dist/creating-rpms.html

python setup.py bdist_rpm
yum install mpich-devel


https://mpi4py.readthedocs.io/en/stable/appendix.html#building-mpi-master-slave

Dependencies
Python 3.6 or above, or PyPy 7.2 or above.
An MPI implementation like MPICH or Open MPI built with shared/dynamic libraries.
To work with the in-development version, you need to install Cython.



https://github.com/mpi4py/mpi4py










/usr/bin/python3  /opt/beegfs/move/src/beemove.py -c /etc/beegfs/move/beegfs_move.conf
查看安装包的路径
pip3 uninstall mpi4py
pip3 uninstall apscheduler
/usr/local/lib64/python3.6/site-packages

pip3 install --target=/root/rpmbuild/site-packages mpi4py
pip3 install --target=/root/rpmbuild/site-packages apscheduler

root@h12-storage01 ~]# pip3 install mpi4py
WARNING: Running pip install with root privileges is generally not a good idea. Try `pip3 install --user` instead.
Collecting mpi4py
  Using cached https://files.pythonhosted.org/packages/20/50/d358fe2b56075163b75eca30c2faa6455c50b9978dd26f0fc4e3879b1062/mpi4py-3.1.3.tar.gz
Installing collected packages: mpi4py
  Running setup.py install for mpi4py ... done

pip3 install apscheduler
/usr/local/lib/python3.6/site-packages
pip uninstall mpi4py

pip3 install mpi4py
pip3 install apscheduler
pip3 uninstall apscheduler

/usr/local/lib64/python3.6/site-packages
/usr/local/lib64/python3.6/site-packages


查看安装包的路径
pip3 install mpi4py
pip3 uninstall apscheduler
/usr/local/lib64/python3.6/site-packages


[root@h12-storage01 ~]# python3 -m site
sys.path = [
    '/root',
    '/usr/lib64/python36.zip',
    '/usr/lib64/python3.6',
    '/usr/lib64/python3.6/lib-dynload',
    '/usr/local/lib64/python3.6/site-packages',
    '/usr/local/lib/python3.6/site-packages',
    '/usr/lib64/python3.6/site-packages',
    '/
	
	
目前采取了一个曲折的方法解决了。首先把所有项目依赖下载下来，项目本身也通过setup.py打包成tar包。在本地，新建一个build/usr文件夹，作为pip的安装目标路径，然后使用pip安装（需要指定源地址为本地路径）你的项目，将会把你的项目包及其依赖一股脑到安装到本地build/usr文件夹。安装完毕，你会看到类似build/usr/lib/python2.7/site-pakges/...这样的结构。然后开始写rpm打包脚本，指定build为根目录，最后使用rpmbuild工具打包，也可以使

作者：斑马
链接：https://www.zhihu.com/question/51645857/answer/130745920
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



依赖软件怎么安装
目前采取了一个曲折的方法解决了。首先把所有项目依赖下载下来，项目本身也通过setup.py打包成tar包。在本地，新建一个build/usr文件夹，作为pip的安装目标路径，然后使用pip安装（需要指定源地址为本地路径）你的项目，将会把你的项目包及其依赖一股脑到安装到本地build/usr文件夹。安装完毕，你会看到类似build/usr/lib/python2.7/site-pakges/...这样的结构。然后开始写rpm打包脚本，指定build为根目录，最后使用rpmbuild工具打包，也可以使

作者：斑马
链接：https://www.zhihu.com/question/51645857/answer/130745920
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


https://blog.csdn.net/livingbody/article/details/83989306
beelive
yum install mpich-devel
pip3 uninstall mpi4py
pip3 install mpi-master-slave
export PATH=$PATH:/usr/lib64/mpich/bin/
export LD_LIBRARY=$LD_LIBRARY:/usr/lib64/mpich/
yum  uninstall mpich-devel
pip3 uninstall mpi4py
pip3 uninstall mpi-master-slave
python3 -c "from mpi4py import MPI"

https://www.cnblogs.com/lxmzq/p/12706524.html

BuildRoot: 这个是安装或编译时使用的“虚拟目录”，考虑到多用户的环境

%files 放到rpm包里面去 放到rpm包里面 放到rpm包里面 放到rpm包里面

mpich
/usr/lib64/mpich/bin/mpirun
mpi4py
export PATH=$PATH:/usr/lib64/mpich/bin/ 
pip3 install apscheduler

export LD_LIBRARY=$LD_LIBRARY:/usr/lib64/mpich/lib
pip3 install apscheduler

yum install mpich-devel
pip3 uninstall mpi4py
pip3 uninstall apscheduler
pip3 install mpi-master-slave
export PATH=$PATH:/usr/lib64/mpich/bin/
export LD_LIBRARY=$LD_LIBRARY:/usr/lib64/mpich/

pip3 uninstall mpi4py
pip3 uninstall mpi-master-slave


https://blog.csdn.net/TomorrowAndTuture/article/details/118608708



spec 源码方法打包文件

https://docs.python.org/3/distutils/sourcedist.html#


Source0 is important, here you write the name of the source tar file in the SOURCES directory. You can write multiply Source in the specfile like this; Source0

python 如何连同依赖打包发布以及python的构建工具？
https://www.zhihu.com/question/21639330/answer/41047469



https://www.zhihu.com/question/51645857
build过程，对应C/C++项目中的./configure, make。对Python项目来说一般可以跳过，因为setup.py都会搞定


https://blog.csdn.net/qq_42303254/article/details/89035054

https://blog.csdn.net/weixin_30807779/article/details/96799186?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-96799186-blog-81868456.t5_layer_targeting_sa_randoms&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-96799186-blog-81868456.t5_layer_targeting_sa_randoms&utm_relevant_index=2
G.     %file部分

这一部分定义哪些文件或者目录会被放入到rpm中

yum install rpm-build

https://blog.csdn.net/arv002/article/details/123546081


依赖软件怎么安装
mpich
/usr/lib64/mpich/bin/mpirun
mpi4py
export PATH=$PATH:/usr/lib64/mpich/bin/ 
export LD_LIBRARY=$LD_LIBRARY:/usr/lib64/mpich/lib
/root/mpi4py-3.1.3
python程序打包rpm包 
pyinstaller打包脚本及其依赖
python编译及打包 
https://www.cnblogs.com/ITer-jack/p/9342946.html#:~:text=yum%20install%20rpmdevtools%20yum%20install%20-y,rpm-%20build%20rpmdev-setuptree%20%E4%BB%A5%E4%B8%8A%E5%91%BD%E4%BB%A4%E8%BF%90%E8%A1%8C%E6%88%90%E5%8A%9F%E5%90%8E%E4%BC%9A%E5%9C%A8%E5%BD%93%E5%89%8D%E7%94%A8%E6%88%B7%E7%9B%AE%E5%BD%95%E4%B8%8B%E7%94%9F%E6%88%90rpmbuild%E7%9B%AE%E5%BD%95%EF%BC%8C%E8%AF%A5%E7%9B%AE%E5%BD%95%E4%BC%9A%E5%8C%85%E5%90%AB%E4%BB%A5%E4%B8%8B%E5%AD%90%E7%9B%AE%E5%BD%95%20--BUILD%20%23%E7%BC%96%E8%AF%91%E4%B9%8B%E5%89%8D%EF%BC%8C%E5%A6%82%E8%A7%A3%E5%8E%8B%E5%8C%85%E5%90%8E%E5%AD%98%E6%94%BE%E7%9A%84%E8%B7%AF%E5%BE%84
---通过setppy可以打包 
rpmbuild -ba SPECS/test.spec
pyinstaller -F test.py 带来可以打包？

除python自带的模块、包之外（自带是指，pip install xxx 安装上的），其他的例如dll文件类，pyinstaller不负责打包进去，所以需要指定文件所在路径。帮助打包，否则importerror。
2、所有的外部参数，都可以在第一次打包后的scripts文件夹下的spec文件内修改
————————————————
版权声明：本文为CSDN博主「Mooney安」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/Iv_zzy/article/details/107462210

https://zhuanlan.zhihu.com/p/276461821
 Creating a Source Distribution

python setup.py bdist_rpm --release=20200617
如何使用 setup.py 构建包
https://docs.python.org/3/distutils/sourcedist.html
https://docs.python.org/3/distutils/introduction.html#distutils-simple-example


花了两天，终于把 Python 的 setup.py 给整明白了
这个文件没有说明清楚 依赖是如何安装的
////////////////////////
回答：
///////////////////////////////////////////////////////////////////
echo "export PATH=$PATH:/usr/lib64/mpich/bin/" >>~/.bashrc
echo "export LD_LIBRARY=$LD_LIBRARY:/usr/lib64/mpich/lib" >>~/.bashrc
source ~/.bashrc

export PATH=$PATH:/usr/lib64/mpich/bin/ >>~/.bashrc


提问：为什么log不产生日志文件，我之前都是这样写的

回答：


在logging.debug(value) 假设 前面设置了 logging.basicConfig 

你没设置怎办，万能在logging不能不用了呀，
我假设有一个，即使后面补上不行，我打开日志就唯一的。
因此去掉logging.basicConfig 之前的logging.debug(

原理：


https://blog.csdn.net/shuifa2008/article/details/9713561

https://blog.csdn.net/JackLang/article/details/107991964?utm_medium=distribute.pc_relevant_download.none-task-blog-baidujs-5.nonecase&depth_1-utm_source=distribute.pc_relevant_download.none-task-blog-baidujs-5.nonecase

[BeeGfs]
#定时器1,分级策略，最多支持100个
policy1=priority:1,name:exe,slowpool:1,fastpool:3
policy2=priority:2,path:/mnt/beegfs/10,slowpool:1,fastpool:3
#定时器2, 业务短时间造成pool慢，优先迁移大文件
gsize =10485761
poolLow=0
poolHigh=0
fastPool=3
slowPool=1

- 读取不存在配置文件，报错 该怎么办

. 检查section或option是否存在
ConfigParserObject.has_section(section)
ConfigParserObject.has_option(section, option)
返回bool值，若存在返回True，不存在返回Fals

作者：rr1990
链接：https://www.jianshu.com/p/417738fc9960
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



- subprocess.run 函数调用参数很重要需要【】包含起来才有返回一个类对象
https://docs.python.org/3/library/subprocess.html


~~~python
# Migrate one file from fast pool to slow pool
def doMigrate(path, rank_id, opt):
  
    process = subprocess.run([cmdMigrate], shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
    if process.returncode == 0:
      
    else:
        logging.error(" stderr:" + str(process.stderr))
        logging.error(" stdout:" + str(process.stdout))
        logging.error("doMigrate err:" + cmdMigrate + " returncode=" + str(process.returncode))

    return ret
~~~



https://docs.python.org/3.9/library/subprocess.html#subprocess.call

```
run(...).returncode

 process = subprocess.run([cmdMigrate], shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
    if process.returncode == 0:
        body = str(process.stdout, encoding='utf-8')
        
```

### 第一天 安装python3环节



```powershell
docker pull python
docker run   -itd --name python3  python:latest bash

docker exec -it 93052d64ad1c bash

```

