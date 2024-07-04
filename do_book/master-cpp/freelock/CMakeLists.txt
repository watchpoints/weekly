cmake_minimum_required (VERSION 3.10)
project ("day1")

# 将源代码添加到此项目的可执行文件。
add_executable (day1 "day1.cpp")

# Specify libraries  to use when linking
# target_link_libraries(<target> ... <item>... ...)
target_link_libraries(day1 pthread leveldb)


