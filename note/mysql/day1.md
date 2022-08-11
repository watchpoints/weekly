Rocky Linux 8 上安装 MySQL

sudo dnf check-update
sudo dnf module list mysql
sudo dnf install mysql
sudo dnf module enable mysql:8.0

sudo systemctl start mysqld
sudo systemctl enable mysqld
sudo systemctl status mysqld

Failed to start mysql.server.service: Unit not found.
https://www.cnblogs.com/yuanchaoyong/p/9749060.html

yum install mariadb-server -y //如果已安装可以省略
systemctl start mariadb.service //启动服务
systemctl enable mariadb.service //开机启动服务
mysql -u root -p //登录mysql

update user set password=password("123456")where user='root';

flush privileges;
mysql -u root -p


~~~
MariaDB [(none)]> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |

create database test;

CREATE TABLE sales(
    sales_employee VARCHAR(50) NOT NULL,
    fiscal_year INT NOT NULL,
    sale DECIMAL(14,2) NOT NULL,
    PRIMARY KEY(sales_employee,fiscal_year)
);
INSERT INTO sales(sales_employee,fiscal_year,sale)
VALUES('Bob',2016,100),
      ('Bob',2017,150),
      ('Bob',2018,200),
      ('Alice',2016,150),
      ('Alice',2017,100),
      ('Alice',2018,200),
       ('John',2016,200),
      ('John',2017,150),
      ('John',2018,250);
 
select * from sales;

理解窗口函数可能更容易从聚合函数开始。
SELECT  SUM(sale) FROM sales;

SELECT 
    fiscal_year, 
    SUM(sale)
FROM
    sales
GROUP BY 
    fiscal_year;
    
与带有GROUP BY子句的聚合函数一样，窗口函数也对行的子集进行操作
但它们不会减少查询返回的行数。

SELECT 
    fiscal_year, 
    sales_employee,
    sale,
    SUM(sale) OVER (PARTITION BY fiscal_year) total_sales
FROM
    sales;
~~

https://dev.mysql.com/doc/refman/8.0/en/window-function-descriptions.html

CREATE TABLE employees (
emp_no int(11) NOT NULL,
birth_date date NOT NULL,
first_name varchar(14) NOT NULL,
last_name varchar(16) NOT NULL,
gender char(1) NOT NULL,
hire_date date NOT NULL,
PRIMARY KEY (emp_no));
