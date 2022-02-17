

# Lesson 04：Execution 性能优化.

[任务地址](https://github.com/pingcap/tidb/issues?q=is%3Aopen+is%3Aissue+label%3Ahigh-performance+label%3Asig%2Fexecution)

分值：600
题目描述：
目前，聚合函数的内存追踪有待完善；
认领一个聚合函数，并根据 issue 内描述完成他的内存追踪 issues/19369


分值：3000
题目描述：
	认领一个还未进行滑动窗口优化的聚合函数，并优化他 issues/12967 


分值：3000
题目描述：
	认领一个还未 inline projection 优化的算子，并优化他 issues/14428 


分值：3000
题目描述：
	认领一个还未被实现的聚合函数，并实现他 issues/7623 


分值：600
题目描述：
认领一个表达式函数的问题，并修复 issues/11223


分值：6600
题目描述：
	使用已有的 Shuffle 算子，实现并行化 Stream Aggregate  issues/9244


分值：6600
题目描述：
	使用已有的 Shuffle 算子，实现并行化 Merge Join issues/14441 


分值：600
题目描述：
选取任意一个执行器或表达式相关的 bug issue 并修复它
help-wanted issues of execution
help-wanted issues of expressions

* 相关 issue 链接可从 PPT 直接获取


https://github.com/pingcap/tidb/issues/18693




## FQA
- https://asktug.com/t/topic/36992




# 参考
- 揭秘 TiDB 新优化器： Cascades Planner 原理解析
- TiDB 源码阅读系列文章（六）Select 语句概览
- ​	[Go夜读 第 47 期 TiDB 源码阅读之 Executor](https://www.bilibili.com/video/av55403428/) --OK 40分钟
- https://docs.qq.com/pdf/DSkNaZW9hTXNqWXVH 
- 数据库内核杂谈（四）：执行模式 --- 0% 