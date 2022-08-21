
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

