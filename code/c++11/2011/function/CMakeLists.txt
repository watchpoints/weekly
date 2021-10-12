
cmake_minimum_required(VERSION 3.10)
# gcc编译选项：
add_definitions(-std=c++11)

# 链接步骤
ADD_EXECUTABLE(hello bindmain.cpp)
