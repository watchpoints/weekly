---
title: "myql入门"
date: 2022-03-26
description: ""
draft: false
tags: ["book"]
---






## 第一篇

### 安装

- 在CentOS7中已经不在推荐使用mysql，甲骨文公司收购了MySQL后，有将MySQL闭源的潜在风险。
- MariaDB数据库是 MySQL 的一个分支版本（branch），提供的功能可和 MySQL 完全兼容



删除mysql

```
rpm -qa |grep -i mysql
yum remove mysql-community-server-5.7.39-1.el7.x86_64
yum remove  mysql57-community-release-el7-8.noarch
yum remove  mysql-community-common-5.7.39-1.el7.x86_64
yum remove  mysql-community-client-5.7.39-1.el7.x86_64
yum remove  mysql-community-libs-5.7.39-1.el7.x86_64
yum remove  mysql-community-libs-compat-5.7.39-1.el7.x86_64
yum remove  mysql-community-devel-5.7.39-1.el7.x86_64

rm -rf /etc/my.cnf
rm -rf /var/log/mysqld.log
rm -rf /usr/share/mysql

```



MariaDB

```shell
yum -y install mariadb  mariadb-devel  mariadb-server

systemctl start mariadb.service 

修改root的密码
update mysql.user set password=PASSWORD('123456') where user='root';
flush privileges; 
use test;
 

obd cluster deploy test -c /data/beegfs/beegfs_storage303/mini-local-example.yaml
```





## 第二篇：基本概念



~~~
mysql -uroot -p
查询 sql_mode
select @@sql_mode;
SHOW VARIABLES LIKE 'sql_mode';

通过 SQL 语句设置 Session/Global 级别sql_mode
SET sql_mode = 'STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
SET GLOBAL ob_query_timeout = 20000000;


ANSI_QUOTES：
启用后，不能用双引号来引用字符串，因为"（双引号）将被解释为标识符
CREATE TABLE a ( a char(5));
INSERT INTO a SELECT 'abc';
SET SQL_MODE='ANSI_QUOTES';
select @@sql_mode;

sql_mode= STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION

    SET GLOBAL sql_mode = 'STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';


MariaDB [test]> INSERT INTO a SELECT "abc";
ERROR 1054 (42S22): Unknown column 'abc' in 'field list'

https://blog.csdn.net/u010312671/article/details/118482396
sql_mode是个很容易忽视的变量，默认值是空值，在这种设置下是可以允许一些非法操作的，比如允许一些非法数据的插入。在生产环境必须将这个值设置为严格模式，所以开发、测试环境的数据库也必须要设置，这样在开发测试阶段就可以发现问题

作者：david161
链接：https://www.jianshu.com/p/43657a601f53
来源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
~~~



![](https://upload-images.jianshu.io/upload_images/23703037-4dccbf7b13a82b7f.png?imageMogr2/auto-orient/strip|imageView2/2/w/859/format/webp)

### 构造mysql记录

~~~myql
create table user (

id int primary key,

name varchar(20),

sex varchar(5),

index(name)

)engine=innodb;

 

insert into user values(1, 'shenjian','no');

insert into user values(2, 'zhangsan','no');

insert into user values(3, 'lisi', 'yes');

insert into user values(4, 'lisi', 'no');

select * from user  limit  4;

~~~

