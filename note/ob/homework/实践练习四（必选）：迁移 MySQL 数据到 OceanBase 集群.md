https://open.oceanbase.com/answer/detail?id=20400030

# 题目

- 练习内容
请记录并分享下列内容：
~~~
（必选）使用 mysqldump 将 mysql的表结构和数据同步到 OceanBase 的MySQL 租户中。

（必选）使用 datax 配置至少一个表的 MySQL 到 OceanBase 的 MySQL 租户的离线同步。

（可选）使用 datax 配置至少一个表的 OceanBase 到 CSV 以及 CSV 到 OceanBase 的离线同步。

（可选）使用 canal 配置 MySQL 的增量同步到 OceanBase 的 MySQL 租户。
~~~



## 练习1：使用 mysqldump 将 mysql的表结构和数据同步到 OceanBase 的MySQL 租户中。


### 第一步：阅读资料 熟悉工具


~~~

社区版官网-文档-学习中心-入门教程：4.2 如何使用 mysqldump 迁移 MySQL 表 OceanBase 。

https://open.oceanbase.com/docs/tutorials/quickstart/V1.0.0/4-2

社区版官网-博客-入门实战：4.2：如何使用 mysqldump 迁移 MySQL 表 OceanBase

- https://open.oceanbase.com/blog/10900160



~~~




### 第二步： 准备数据
- 基础库
~~~
yum install mysql-devel

不然：
load.c:18:19: fatal error: mysql.h: No such file or directory

~~~

- 安装mysql

~~~
wget http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm

sudo rpm -ivh mysql-community-release-el7-5.noarch.rpm

sudo yum install mysql-server


重启服务：
service mysqld restart

mysql -u root

set password for 'root'@'localhost'=password('123456');

mysql -u root -p

~~~

- 问题
CentOS 7为什么放弃了MySQL，而改使用MariaDB？

mysql的作者另起炉灶的开源版本：maria DB

- ·安装tpcc

~~~
wget http://imysql.com/wp-content/uploads/2014/09/tpcc-mysql-src.tgz
gunzip tpcc-mysql-src.tgz
tar xf tpcc-mysql-src.tar
cd tpcc-mysql/src 
make

~~~


> 小贴士

- tpcc-mysql有几个脚本很重要

create_table.sql：创建所需的数据库表，共9个表

add_fkey_idx.sql：创建索引和外键

drop_cons.sql：删除约束


- 对100个数据仓库，预热120秒，100个并发连接，运行3600秒， 结果存放在当前文件tpcc100_20130325

#./tpcc_start -hlocalhost -d tpcc100  -u root  -p 'rootpwd' -w 100 -c 100 -r 120 -l 3600 -f tpcc100_20130325


- 例如：创建100个数据仓库并填充数据    
[root@racdb2 tpcc-mysql]# ./tpcc_load  localhost  tpcc100  root  'rootpassword'  100


- 初始化测试库，并在测试库中创建表，
  即执行create_table.sql测试数据导入mysql数据库的tpcc数据库

~~~

mysql -u root -p
CREATE DATABASE `tpcc` DEFAULT CHARACTER SET utf8;

mysql  -u root -p  tpcc < create_table.sql 

Database changed
mysql> show tables;
+----------------+
| Tables_in_tpcc |
+----------------+
| customer       |
| district       |
| history        |
| item           |
| new_orders     |
| order_line     |
| orders         |
| stock          |
| warehouse      |
+----------------+
9 rows in set (0.00 sec


~~~
- 灌入测试数据

~~~


./tpcc_load 127.0.0.1:3306 tpcc root 123456 1 

127.0.0.1:3306 : 指定mysql的IP和端口号

tpcc : 指定数据库的名字

1 ： 仓库数

真实测试场景中，仓库数一般不建议少于100个



mysql> select count(*) from customer;
+----------+
| count(*) |
+----------+
|    30000 |
+----------+
1 row in set (0.01 sec

~~~




- 查看测试业务库数据表


### 第三步：使用 mysqldump 迁移 MySQL 表 OceanBase

- 安装mysqldump

~~~
yum install mysql

which mysqldump
/bin/mysqldump
~~~

- mysql导出指定数据库的表结构（不包括数据）
~~~
mysqldump -h 127.0.0.1 -uroot -P3306 -p123456 -d tpcc --compact > tpcc_ddl.sql

cat  tpcc_ddl.sql

CREATE TABLE `warehouse` (
  `w_id` smallint(6) NOT NULL,
  `w_name` varchar(10) DEFAULT NULL,
  `w_street_1` varchar(20) DEFAULT NULL,
  `w_street_2` varchar(20) DEFAULT NULL,
  `w_city` varchar(20) DEFAULT NULL,
  `w_state` char(2) DEFAULT NULL,
  `w_zip` char(9) DEFAULT NULL,
  `w_tax` decimal(4,2) DEFAULT NULL,
  `w_ytd` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`w_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client 

~~~
- mysql导出指定数据库的表数据（不包括结构）
~~~
mysqldump -h 127.0.0.1  -uroot -P3306 -p123456 -t tpcc  > tpcc_data.sql
~~~

-  导入表结构和数据到OceanBase 
~~~

mysql -h 127.0.0.1  -u root@sys#obdemo -P 2883 -p123456 -c -A tpcc

source tpcc_ddl.sql

source tpcc_data.sql

在 obclient 客户端里通过 source 命令可以执行外部 SQL 脚本文件。

~~~


## （必选）使用 datax 配置至少一个表的 MySQL 到 OceanBase 的 MySQL 租户的离线同步。

### Datax安装

- DataX 是阿里云 DataWorks数据集成的开源版本，在阿里巴巴集团内被广泛使用的离线数据同步工具/平台

- 开源地址：git clone git@github.com:alibaba/DataX.git。
- JDK(1.8以上，推荐1.8)

- 通过maven打包
 mvn -U clean package assembly:assembly -Dmaven.test.skip=true
- https://open.oceanbase.com/docs/tutorials/quickstart/V1.0.0/4-6-how-to-use-datax-to-load-csv-data-files

验证
~~~
cd /app/local/datax/bin
python datax.py ../job/job.json 
~~~

### DATAX 配置文件

python ./bin/datax.py -r mysqlreader -w oceanbasev10writer

~~~
{
    "job": {
        "setting": {
            "speed": {
                "channel": 4 
            },
            "errorLimit": {
                "record": 0,
                "percentage": 0.1
            }
        },
        "content": [
            {
                "reader": {
                    "name": "mysqlreader",
                    "parameter": {
                        "username": "tpcc",
                        "password": "********",
                        "column": [
                            "*"
                        ],
                        "connection": [
                            {
                                "table": [
                                    "bmsql_oorder"
                                ],
                                "jdbcUrl": ["jdbc:mysql://127.0.0.1:3306/tpccdb?useUnicode=true&characterEncoding=utf8"]
                            }
                        ]
                    }
                },

                "writer": {
                    "name": "oceanbasev10writer",
                    "parameter": {
                        "obWriteMode": "insert",
                        "column": [
                            "*"
                        ],
                        "preSql": [
                            "truncate table bmsql_oorder"
                        ],
                        "connection": [
                            {
                                "jdbcUrl": "||_dsc_ob10_dsc_||obdemo:oboracle||_dsc_ob10_dsc_||jdbc:oceanbase://127.0.0.1:2883/tpcc?useLocalSessionState=true&allowBatch=true&allowMultiQueries=true&rewriteBatchedStatements=true",
                                "table": [
                                    "bmsql_oorder"
                                ]
                            }
                        ],
                        "username": "tpcc",
                        "password":"********",
                        "writerThreadCount":10,
                        "batchSize": 1000,
                        "memstoreThreshold": "0.9"
                    }
                }
            }
        ]
    }
}


~~~


- oceanbasev10reader 和 oceanbasev10writer 插件找不到


### 执行命令
~~~
 bin/datax.py ./job/bmsql_oorder_csv2ob.json
~~~