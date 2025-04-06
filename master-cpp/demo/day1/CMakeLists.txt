# 设置最低版本的CMake（可选）
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和版本号（可选）
project(HelloWorld VERSION 1.0)

# 查找所需的C++标准版本。这里我们使用C++11作为示例。
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#指定链接器
set(CMAKE_LINKER "/usr/local/opt/llvm/bin/ld64.lld" CACHE PATH "Linker")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")

# 添加可执行文件
add_executable(HelloWorld main.cpp)
