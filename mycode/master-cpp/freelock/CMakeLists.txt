cmake_minimum_required (VERSION 3.10)
project ("day1")

# 将源代码添加到此项目的可执行文件。
add_executable (day1 "cond.cpp")

# Specify libraries  to use when linking
# target_link_libraries(<target> ... <item>... ...)
# The named <target> must have been created by a command such as add_executable() or add_library() and must not be an ALIAS target.
target_link_libraries(day1 gtest gtest_main)


