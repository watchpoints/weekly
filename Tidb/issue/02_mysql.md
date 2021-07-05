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

select * from user  where  id >1 limit  4;
SET @a=1;
PREPARE prepare_sql from "SELECT * FROM user where id=?";
EXECUTE prepare_sql USING @a;
~~~

