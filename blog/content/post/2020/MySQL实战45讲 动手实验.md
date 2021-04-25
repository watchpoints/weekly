---
title: "MySQL实战45讲"
date: 2020-05-13
description: "你明明知道数据库是你弱点，你为什么不现在补救一下 必须掌握"
draft: false
tags: ["mysql"]
categories: ["中间件"]
---



 

> 问题： 假如公司没有大量业务？假如公司不重视mysql ？你就不学习了吗？
>
> 
>
>  回答：记录Mysql 学习过程。日供一卒
>
>  对于公司来说，业务，架构，甚至管理 从来不考虑这个问题，
>
>  作为普通it来说，必须掌握技能，从陌生到熟悉然后到精通。
>
> 你目标也不是DBA,你只需要完成下面四个步骤就可以了。
>
> 

# 具体例子

### 17 如何正确地显示随机消息

~~~
CREATE TABLE `words` (
`id` int(11) NOT NULL AUTO_INCREMENT,
`word` varchar(64) DEFAULT NULL,
PRIMARY KEY (`id`)
) ENGINE=InnoDB;

delimiter ;;
create procedure idata()
begin
declare i int;
set i=0;
while i<10000 do
insert into words(word) values(concat(char(97+(i div 1000)), char(97+(i % 1000 div 1
set i=i+1;
end while;
end;;
delimiter ;
call idata();

select word from words order by rand() limit 3;
~~~
~~~
# Query_time: 0.900376 Lock_time: 0.000347 Rows_sent: 3 Rows_examined: 20003
SET timestamp=1541402277;
select word from words order by rand() limit 3;

tmp_table_size 这个配置限制了内存临时表的大小，默认值是 16M。如果
临时表大小超过了 tmp_table_size，那么内存临时表就会转成磁盘临时表

set tmp_table_size=1024;
set sort_buffer_size=32768;
set max_length_for_sort_data=16;
/* 打开 optimizer_trace，只对本线程有效 */
SET optimizer_trace='enabled=on';
/* 执行语句 */
select word from words order by rand() limit 3;
/* 查看 OPTIMIZER_TRACE 输出 */
SELECT * FROM `information_schema`.`OPTIMIZER_TRACE`\G

~~~

### 18 为什么这些SQL语句逻辑相同，性能却差异巨大？

~~~
 CREATE TABLE `tradelog` (
`id` int(11) NOT NULL,
`tradeid` varchar(32) DEFAULT NULL,
`operator` int(11) DEFAULT NULL,
`t_modified` datetime DEFAULT NULL,
PRIMARY KEY (`id`),
KEY `tradeid` (`tradeid`),
KEY `t_modified` (`t_modified`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


select count(*) from tradelog where month(t_modified)=7;
~~~