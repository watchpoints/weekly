# CMakeList.txt: demo1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("01-gtest")

add_executable (test "main.cpp")
#我以为动态库才可以用简写，最后发现静态库也可以的
#target_link_libraries(test PUBLIC gtest gtest_main pthread)
target_link_libraries(test PUBLIC /root/googletest-release-1.12.1/build/lib/libgtest.a /root/googletest-release-1.12.1/build/lib/libgtest_main.a pthread)
#target_include_directories(test PUBLIC /root/googletest-release-1.12.1/googletest/include/)


# TODO: 如有需要，请添加测试并安装目标。
