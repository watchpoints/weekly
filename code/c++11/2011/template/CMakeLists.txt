cmake_minimum_required(VERSION 3.10)
## clang-llvm
##SET(CMAKE_C_COMPILER "D:/Program Files/LLVM/bin/clang.exe")
##SET(CMAKE_CXX_COMPILER "D:/Program Files/LLVM/bin/clang++.exe")

##gcc
#SET(CMAKE_C_COMPILER /usr/bin/gcc)
#SET(CMAKE_CXX_COMPILER /usr/bin/g++)
# add c++11 
set(CMAKE_CXX_STANDARD 14)
# set the project name
project(template)
# add the executable 
add_executable(test 5.cpp)
