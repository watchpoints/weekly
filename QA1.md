
Q 查看cpu 类型

- lscpu
- rpm -qa |grep debug rpm -e xxx



Q: gdb 为什么显示问号？

- gdb 时候bt 查看堆栈中的函数全部是问号
- gdb 时候bt 查看部分显示问号





 产生core文件的程序 和gdb 调试程序不是同一个程序
1. file core.xxx 查看core文件的可执行程序
2. gdb /bin/xx core.XXX
