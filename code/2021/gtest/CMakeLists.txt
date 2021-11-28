# CMake最低版本要求
cmake_minimum_required(VERSION 3.21)
# 项目名称
project("gtest")
add_definitions(-std=c++11)
# 指定生成目标
ADD_EXECUTABLE(Hello
    test_demo.cpp
)

# 添加链接库
target_link_libraries(Hello "/usr/local/lib/libgtest.a")

# 添加头文件搜索逻辑 "/usr/local/include"
target_include_directories(Hello PUBLIC 
                            "/usr/local/include"
                           )