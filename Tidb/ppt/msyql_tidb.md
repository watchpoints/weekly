## 测试

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

SET @a=1;
PREPARE prepare_sql from "SELECT * FROM user where id=?";
EXECUTE prepare_sql USING @a;


create table t1(id int primary key, a int, b int, c int, d int)
insert into t1 values(1,1,1,1,1),(2,2,2,2,2),(3,3,3,3,3),(4,4,4,4,4),(5,5,5,5,5)
select /*+ use_index_merge(t1, t1a, t1b) */ sum(a) from t1 where a < 2 or b > 4;

~~~

