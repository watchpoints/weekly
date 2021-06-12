#### gmock



有时候对于一些接口，比如向服务器发送请求。

但单元测试中有没有可用于测试的服务器，

这个时候就需要mock这个请求接口。

 **mock工具的作用是指定函数的行为（模拟函数的行为）。可以对入参进行校验，对出参进行设定，还可以指定函数的返回值。**

**[Google Mock](http://code.google.com/p/googlemock/)（简称gmock）是Google在2008年推出的一套针对C++的Mock框架，它灵感取自于[jMock](http://www.jmock.org/)、[EasyMock](http://www.easymock.org/)、[harcreat](http://code.google.com/p/hamcrest/)**







#  # **技术原理**



- book

修改代码的艺术

混沌工程 Chaos Engineering



单元测试打桩技巧总结(stub、mock)

Mock是基于`多态`实现的



# #动手实现



https://github.com/google/googletest.git



```
git clone https://github.com/google/googletest.git -b release-1.10.0
cd googletest        # Main directory of the cloned repository.
mkdir build          # Create a directory to hold the build output.
cd build
cmake ..             # Generate native build scripts for GoogleTest.

??? https://github.com/ddnet/ddnet/issues/3355
brew install cmake
brew upgrade cmake 
cmake 3.20.3 already installed
# CMake 最低版本号要求

https://discourse.cmake.org/t/how-to-fix-cmake-minimum-required-deprecation-warning/2487/6
googletest/googlemock/CMakeLists.txt
cmake_minimum_required(VERSION 2.6.4)-->cmake_minimum_required(VERSION 2.8...3.20)

/usr/local/include/gmock
/usr/local/lib/libgmock.a
```





- 测试代码

/Users/wangchuanyi/code/c++/googletest/googletest/samples





## 参考

- 注册测试：http://m0nkee.github.io/2015/08/20/play-ptrace/

- http://hongyitong.github.io/2016/12/23/%E6%B5%85%E8%B0%88mock%E5%92%8Cstub/ 

https://juejin.cn/post/6844903976765243400

https://www.dazhuanlan.com/2019/10/04/5d96f7bd23ced/



https://github.com/google/googletest/blob/master/docs/gmock_cook_book.md

https://www.cnblogs.com/welkinwalker/archive/2011/11/29/2267225.html

https://google.github.io/googletest/gmock_cook_book.html

https://github.com/google/googletest/blob/master/docs/gmock_cook_book.md

https://google.github.io/googletest/



https://fuchsia.googlesource.com/third_party/googletest/+/HEAD/googlemock/docs/cook_book.md