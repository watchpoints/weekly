---
  title: "mimob代码阅读"
date: 2022-02-10
draft: false
categories: ["code_reading"]
---


> 慢慢来，多看、多问、多总结







## 预备知识：

- 源码：https://github.com/watchpoints/miniob
- 2019CMU数据库导论(intro to database systems fall 2019) https://www.bilibili.com/video/av91919329?amp%3Bseid=15982257803189242166
- 2020 MIT 6.824 分布式系统 https://www.bilibili.com/video/BV1R7411t71W/
- https://github.com/CDDSCLab/training-plan
- https://github.com/tidb-incubator/tinykv
- https://github.com/WenbinZhu/cmu-15-445 CMU 15-445 Project 1 

## 文章

- https://github.com/oceanbase/miniob/wiki/%E4%BD%BF%E7%94%A8-ASAN-%E6%B5%8B%E8%AF%95-MiniOB-%E5%86%85%E5%AD%98%E9%97%AE%E9%A2%98
- 使用 ASAN 测试 MiniOB 内存问题
- https://www.cnblogs.com/JayL-zxl/p/14324297.html
- []CMU数据库(15-445)实验2-b+树索引实现(上)](https://www.cnblogs.com/JayL-zxl/p/14324297.html)



~~~
#!/bin/bash
mkdir -p build 
rm -rf build/miniob/ 
cd build 

cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_ASAN=ON -DWITH_UNIT_TESTS=OFF
make 

yum install libasan


~~~





## select-tables多表查询

### 测试用例

~~~
用release模式编译的时候会做很多代码优化，所以如果哪里代码写得不规范的话，debug模式可能没有问题，但是release模式的编译器可能就会曲解我们的意思，编译出一些奇怪的东西

ulimit -c unlimited

flex --header-file=lex.yy.h lex_sql.l
bison -d -b yacc_sql yacc_sql.y

cd /root/github/miniob-2022/build/bin
./observer -f /root/github/miniob-2022/etc/observer.ini
/root/github/miniob-2022/build/bin/obclient

dir /root/github/miniob-2022/
set pagination off 
thread apply all break cartesian_operator.cpp:16


CREATE TABLE Select_tables_1(id int, age int, u_name char);

CREATE TABLE Select_tables_2(id int, age int, u_name char);

CREATE TABLE Select_tables_3(id int, res int, u_name char);

CREATE TABLE Select_tables_4(id int, age int, u_name char);

CREATE TABLE Select_tables_5(id int, res int, u_name char);


INSERT INTO Select_tables_1 VALUES (1,18,'a');

INSERT INTO Select_tables_1 VALUES (2,15,'b');

INSERT INTO Select_tables_2 VALUES (1,20,'a');

INSERT INTO Select_tables_2 VALUES (2,21,'c');

INSERT INTO Select_tables_3 VALUES (1,35,'a');

INSERT INTO Select_tables_3 VALUES (2,37,'a');



INSERT INTO Select_tables_4 VALUES (1, 2, 'a');

INSERT INTO Select_tables_4 VALUES (1, 3, 'b');

INSERT INTO Select_tables_4 VALUES (2, 2, 'c');

INSERT INTO Select_tables_4 VALUES (2, 4, 'd');

INSERT INTO Select_tables_5 VALUES (1, 10, 'g');

INSERT INTO Select_tables_5 VALUES (1, 11, 'f');

INSERT INTO Select_tables_5 VALUES (2, 12, 'c');


SELECT * FROM Select_tables_1,Select_tables_2,Select_tables_3;

1 | 18 | A | 1 | 20 | A | 1 | 35 | A
1 | 18 | A | 1 | 20 | A | 2 | 37 | A
1 | 18 | A | 2 | 21 | C | 1 | 35 | A
1 | 18 | A | 2 | 21 | C | 2 | 37 | A
2 | 15 | B | 1 | 20 | A | 1 | 35 | A
2 | 15 | B | 1 | 20 | A | 2 | 37 | A
2 | 15 | B | 2 | 21 | C | 1 | 35 | A
2 | 15 | B | 2 | 21 | C | 2 | 37 | A
SELECT_TABLES_1.ID | SELECT_TABLES_1.AGE | SELECT_TABLES_1.U_NAME | SELECT_TABLES_2.ID | SELECT_TABLES_2.AGE | SELECT_TABLES_2.U_NAME | SELECT_TABLES_3.ID | SELECT_TABLES_3.RES | SELECT_TABLES_3.U_NAME
SELECT Select_tables_1.id,Select_tables_2.u_name,Select_tables_3.res FROM Select_tables_1,Select_tables_2,Select_tables_3;
1 | A | 35
1 | A | 37
1 | C | 35
1 | C | 37
2 | A | 35
2 | A | 37
2 | C | 35
2 | C | 37
SELECT_TABLES_1.ID | SELECT_TABLES_2.U_NAME | SELECT_TABLES_3.RES
Select Select_tables_1.res FROM Select_tables_1,Select_tables_2,Select_tables_3;
FAILURE

2. CONDITIONAL QUERY
SELECT * FROM Select_tables_1,Select_tables_2,Select_tables_3 WHERE Select_tables_1.u_name=Select_tables_2.u_name AND Select_tables_2.u_name=Select_tables_3.u_name;
1 | 18 | A | 1 | 20 | A | 1 | 35 | A
1 | 18 | A | 1 | 20 | A | 2 | 37 | A
SELECT_TABLES_1.ID | SELECT_TABLES_1.AGE | SELECT_TABLES_1.U_NAME | SELECT_TABLES_2.ID | SELECT_TABLES_2.AGE | SELECT_TABLES_2.U_NAME | SELECT_TABLES_3.ID | SELECT_TABLES_3.RES | SELECT_TABLES_3.U_NAME
SELECT * FROM Select_tables_1,Select_tables_2,Select_tables_3 WHERE Select_tables_1.id=Select_tables_2.id AND Select_tables_3.res=35;
1 | 18 | A | 1 | 20 | A | 1 | 35 | A
2 | 15 | B | 2 | 21 | C | 1 | 35 | A
SELECT_TABLES_1.ID | SELECT_TABLES_1.AGE | SELECT_TABLES_1.U_NAME | SELECT_TABLES_2.ID | SELECT_TABLES_2.AGE | SELECT_TABLES_2.U_NAME | SELECT_TABLES_3.ID | SELECT_TABLES_3.RES | SELECT_TABLES_3.U_NAME
SELECT * FROM Select_tables_1,Select_tables_2,Select_tables_3 WHERE Select_tables_1.age<18 AND Select_tables_2.u_name='c' AND Select_tables_3.res=35 AND Select_tables_1.id=Select_tables_2.id AND Select_tables_2.id=Select_tables_3.id;
SELECT_TABLES_1.ID | SELECT_TABLES_1.AGE | SELECT_TABLES_1.U_NAME | SELECT_TABLES_2.ID | SELECT_TABLES_2.AGE | SELECT_TABLES_2.U_NAME | SELECT_TABLES_3.ID | SELECT_TABLES_3.RES | SELECT_TABLES_3.U_NAME
SELECT Select_tables_2.age FROM Select_tables_1,Select_tables_2 WHERE Select_tables_1.age<18 AND Select_tables_2.u_name='c' AND Select_tables_1.id=Select_tables_2.id;
21
SELECT_TABLES_2.AGE

3. DUPLICATE KEY QUERY
SELECT * from Select_tables_4, Select_tables_5 where Select_tables_4.id=Select_tables_5.id;
1 | 2 | A | 1 | 10 | G
1 | 2 | A | 1 | 11 | F
1 | 3 | B | 1 | 10 | G
1 | 3 | B | 1 | 11 | F
2 | 2 | C | 2 | 12 | C
2 | 4 | D | 2 | 12 | C
SELECT_TABLES_4.ID | SELECT_TABLES_4.AGE | SELECT_TABLES_4.U_NAME | SELECT_TABLES_5.ID | SELECT_TABLES_5.RES | SELECT_TABLES_5.U_NAME
select * from Select_tables_4, Select_tables_5 where Select_tables_4.id >= Select_tables_5.id;
1 | 2 | A | 1 | 10 | G
1 | 2 | A | 1 | 11 | F
1 | 3 | B | 1 | 10 | G
1 | 3 | B | 1 | 11 | F
2 | 2 | C | 1 | 10 | G
2 | 2 | C | 1 | 11 | F
2 | 2 | C | 2 | 12 | C
2 | 4 | D | 1 | 10 | G
2 | 4 | D | 1 | 11 | F
2 | 4 | D | 2 | 12 | C
SELECT_TABLES_4.ID | SELECT_TABLES_4.AGE | SELECT_TABLES_4.U_NAME | SELECT_TABLES_5.ID | SELECT_TABLES_5.RES | SELECT_TABLES_5.U_NAME

4. JOIN EMPTY TABLE
CREATE TABLE Select_tables_6(id int, res int);
SUCCESS
SELECT Select_tables_1.id,Select_tables_6.id from Select_tables_1, Select_tables_6 where Select_tables_1.id=Select_tables_6.id;
SELECT_TABLES_1.ID | SELECT_TABLES_6.ID

~~~



## 多列索引 multi-index



### 知识准备

- https://zhuanlan.zhihu.com/p/382244184
- https://www.cnblogs.com/sun-lingyu/p/15198683.html
- - 你有空可以搜索一下"mysal,多列索引,实现"这些关键字 

CMU15-445的lab与MIT6.824的lab

CMU15-445的lab与MIT6.824的lab风格很不一样。前者定义好了函数原型，提示更多，但是禁锢了思维，发挥空间变小了。后者只提供了最基础的接口，在代码架构上的可发挥性更高。
由于函数原型都给好了，我以为这个lab会简单很多。结果没成想，写着写着，发现B+树这个lab给我整不会了。花了足足一个月，才把lab写完，

1. 测试用例

~~~

flex --header-file=lex.yy.h lex_sql.l
bison -d -b yacc_sql yacc_sql.y

cd /root/gitee/miniob-2022/build/bin
./observer -f /root/gitee/miniob-2022/etc/observer.ini
/root/gitee/miniob-2022/build/bin/obclient

CREATE TABLE multi_index(id int, col1 int, col2 float, col3 char, col4 date, col5 int, col6 int);


CREATE INDEX i_1_12 ON multi_index(col1,col2);

CREATE INDEX i_1_345 ON multi_index(col3, col4, col5);

CREATE INDEX i_1_56 ON multi_index(col5, col6);

CREATE INDEX i_1_456 ON multi_index(col4, col5, col6);

SELECT * FROM multi_index;
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6

2. MULTI INDEX OF NON-EMPTY TABLE

CREATE TABLE multi_index2(id int, col1 int, col2 float, col3 char, col4 date, col5 int, col6 int);

INSERT INTO multi_index2 VALUES (1, 1, 11.2, 'a', '2021-01-02', 1, 1);

INSERT INTO multi_index2 VALUES (2, 1, 16.2, 'x', '2021-01-02', 1, 61);

INSERT INTO multi_index2 VALUES (3, 1, 11.6, 'h', '2023-01-02', 10, 17);


CREATE INDEX i_2_12 ON multi_index2(col1,col2);
SUCCESS
CREATE INDEX i_2_345 ON multi_index2(col3, col4, col5);
SUCCESS
CREATE INDEX i_2_56 ON multi_index2(col5, col6);
SUCCESS
CREATE INDEX i_2_456 ON multi_index2(col4, col5, col6);
SUCCESS
SELECT * FROM multi_index2;
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
2 | 1 | 16.2 | X | 2021-01-02 | 1 | 61
3 | 1 | 11.6 | H | 2023-01-02 | 10 | 17
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6

3. INFLUENCE OF INSERTING
CREATE TABLE multi_index3(id int, col1 int, col2 float, col3 char, col4 date, col5 int, col6 int);

CREATE INDEX i_3_i1 ON multi_index3(id,col1);


INSERT INTO multi_index3 VALUES (1, 1, 11.2, 'a', '2021-01-02', 1, 1);
SUCCESS
INSERT INTO multi_index3 VALUES (1, 1, 11.2, 'a', '2021-01-02', 1, 1);
SUCCESS
SELECT * FROM multi_index3;
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6
CREATE INDEX i_3_14 ON multi_index3(col1,col4);
SUCCESS
INSERT INTO multi_index3 VALUES (2, 1, 16.2, 'x', '2021-01-02', 1, 61);
SUCCESS
INSERT INTO multi_index3 VALUES (3, 1, 11.6, 'h', '2023-01-02', 10, 17);
SUCCESS
INSERT INTO multi_index3 VALUES (4, 2, 12.2, 'e', '2022-01-04', 13, 10);
SUCCESS
INSERT INTO multi_index3 VALUES (5, 3, 14.2, 'd', '2020-04-02', 12, 2);
SUCCESS
SELECT * FROM multi_index3;
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
2 | 1 | 16.2 | X | 2021-01-02 | 1 | 61
3 | 1 | 11.6 | H | 2023-01-02 | 10 | 17
4 | 2 | 12.2 | E | 2022-01-04 | 13 | 10
5 | 3 | 14.2 | D | 2020-04-02 | 12 | 2
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6

4. QUERY WITH INDEXS
SELECT * FROM multi_index3 WHERE id = 1;
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6


SELECT * FROM multi_index3 WHERE col1 > 1 and col4 = '2021-01-02';
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6
SELECT * FROM multi_index3 WHERE col1 <> 1 and col4 >= '2021-01-02';


4 | 2 | 12.2 | E | 2022-01-04 | 13 | 10
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6
SELECT * FROM multi_index3 WHERE col2 < 15.0 and col4 <> '2021-01-02';
3 | 1 | 11.6 | H | 2023-01-02 | 10 | 17
4 | 2 | 12.2 | E | 2022-01-04 | 13 | 10
5 | 3 | 14.2 | D | 2020-04-02 | 12 | 2
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6




5. INFLUENCE OF DELETING
DELETE FROM multi_index3 WHERE id = 1;
SUCCESS
DELETE FROM multi_index3 WHERE id = 61;
SUCCESS
SELECT * FROM multi_index3;
2 | 1 | 16.2 | X | 2021-01-02 | 1 | 61
3 | 1 | 11.6 | H | 2023-01-02 | 10 | 17
4 | 2 | 12.2 | E | 2022-01-04 | 13 | 10
5 | 3 | 14.2 | D | 2020-04-02 | 12 | 2
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6

DELETE FROM multi_index3 WHERE col3 = 'x';
SUCCESS
SELECT * FROM multi_index3;
3 | 1 | 11.6 | H | 2023-01-02 | 10 | 17
4 | 2 | 12.2 | E | 2022-01-04 | 13 | 10
5 | 3 | 14.2 | D | 2020-04-02 | 12 | 2
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6

DELETE FROM multi_index3 WHERE id = 4 and col1 = 1;
SUCCESS
DELETE FROM multi_index3 WHERE id = 90 and col1 = 13;
SUCCESS
DELETE FROM multi_index3 WHERE id = 90 and col1 = 1;
SUCCESS
DELETE FROM multi_index3 WHERE id = 4 and col1 = 13;
SUCCESS
DELETE FROM multi_index3 WHERE id = 3 and col1 = 1;
SUCCESS
DELETE FROM multi_index3 WHERE id = 3 and col1 = 1;
SUCCESS
SELECT * FROM multi_index3;
4 | 2 | 12.2 | E | 2022-01-04 | 13 | 10
5 | 3 | 14.2 | D | 2020-04-02 | 12 | 2
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6

INSERT INTO multi_index3 VALUES (1, 1, 11.2, 'a', '2021-01-02', 1, 1);
SUCCESS
INSERT INTO multi_index3 VALUES (2, 1, 11.2, 'x', '2021-01-02', 1, 61);
SUCCESS
INSERT INTO multi_index3 VALUES (3, 1, 11.2, 'h', '2023-01-02', 10, 17);
SUCCESS
SELECT * FROM multi_index3;
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
2 | 1 | 11.2 | X | 2021-01-02 | 1 | 61
3 | 1 | 11.2 | H | 2023-01-02 | 10 | 17
4 | 2 | 12.2 | E | 2022-01-04 | 13 | 10
5 | 3 | 14.2 | D | 2020-04-02 | 12 | 2
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6

6. INFLUENCE OF UPDATING
UPDATE multi_index3 SET col6=49 where id=2;
SUCCESS
UPDATE multi_index3 SET col4='1999-02-01' where id=2;
SUCCESS
UPDATE multi_index3 SET col1=2 where id=2;
SUCCESS
UPDATE multi_index3 SET col1=5 where col6=49;
SUCCESS
SELECT * FROM multi_index3;
1 | 1 | 11.2 | A | 2021-01-02 | 1 | 1
2 | 5 | 11.2 | X | 1999-02-01 | 1 | 49
3 | 1 | 11.2 | H | 2023-01-02 | 10 | 17
4 | 2 | 12.2 | E | 2022-01-04 | 13 | 10
5 | 3 | 14.2 | D | 2020-04-02 | 12 | 2
ID | COL1 | COL2 | COL3 | COL4 | COL5 | COL6

7. INFLUENCE OF DROPPING TABLE
DROP table multi_index;
SUCCESS

8. ERROR
CREATE TABLE multi_index4(id int, col1 int, col2 float, col3 char, col4 date, col5 int, col6 int);
SUCCESS

CREATE INDEX i_4_i7 ON multi_index4(id,col7);
FAILURE
CREATE INDEX i_4_78 ON multi_index4(col7,col8);
FAILURE
CREATE INDEX i_4_i78 ON multi_index4(id,col7,col8);
FAILURE


~~~



###  数据结构

~~~
/**
 * leaf page of bplus tree
 * storage format:
 * | common header | prev page id | next page id |
 * | key0, rid0 | key1, rid1 | ... | keyn, ridn |
 *
 * the key is in format: the key value of record and rid.
 * so the key in leaf page must be unique.
 * the value is rid.
 * can you implenment a cluster index ?
 */
struct LeafIndexNode : public IndexNode {
  static constexpr int HEADER_SIZE = IndexNode::HEADER_SIZE + 8;
  
  PageNum prev_brother;
  PageNum next_brother;
  /**
   * leaf can store order keys and rids at most
   */
  cha
  

flex --header-file=lex.yy.h lex_sql.l
bison -d -b yacc_sql yacc_sql.y

cd /root/gitee/miniob-2022/build/bin
./observer -f /root/gitee/miniob-2022/etc/observer.ini
/root/github/miniob-2022/build/bin/obclient


table：

//std::vector<FieldMeta> fields_;  // 包含sys_fields
//std::vector<IndexMeta> indexes_;


/**
 * the meta information of bplus tree
 * this is the first page of bplus tree.
 * only one field can be supported, can you extend it to multi-fields?
 
  里面说了是一个列 怎么变成多个列呢
 */
 
struct IndexFileHeader {
  IndexFileHeader()
  {
    memset(this, 0, sizeof(IndexFileHeader));
    root_page = BP_INVALID_PAGE_NUM;
  }
  PageNum  root_page;
  int32_t  internal_max_size;
  int32_t  leaf_max_size;
  int32_t  attr_length;
  int32_t  key_length; // attr length + sizeof(RID)
  AttrType attr_type;

  const std::string to_string()
  {
    std::stringstream ss;

    ss << "attr_length:" << attr_length << ","
       << "key_length:" << key_length << ","
       << "attr_type:" << attr_type << ","
       << "root_page:" << root_page << ","
       << "internal_max_size:" << internal_max_size << ","
       << "leaf_max_size:" << leaf_max_size << ";";

    return ss.str();
  }
};

~~~



### 算法

~~~cpp

创建索引流程：SCF_CREATE_INDEX
ExecuteStage::do_create_index-->Table::create_index -->BplusTreeIndex::create
--->RC BplusTreeIndex::create_multi
    
插入数据流程：走索引的插入
ExecuteStage::do_insert--->Table::insert_record_mult_rows-->Trx::insert_record
--->Table::insert_entry_of_indexes--->BplusTreeHandler::insert_entry

    BplusTreeHandler::make_key
查找 
ExecuteStage::do_select --->IndexScanOperator *try_to_create_index_scan_operator
  IndexScanOperator::IndexScanOperator(  
    
Table::create_index
    1. 检测字段是否存在 find_mult_index_by_field
    2. 创建b+ index->create_multi
    3. 遍历当前的所有数据，插入这个索引
    
BplusTreeIndex::create_multi
    
    

~~~

~~~mermaid

graph TD
Table::open --> B
~~~



### bug



~~~
    "indexes" :
        [
                {
                        "fields_name" :
                        [
                                {
                                        "field_name" : "col1"
                                },
                                {
                                        "field_name" : "col2"
                                }
                        ],
                        "name" : "i_1_12",
                        "unique" : false
                },
                {
                        "fields_name" :
                        [
                                {
                                        "field_name" : "col3"
                                },
                                {
                                        "field_name" : "col4"
                                },
                                {
                                        "field_name" : "col5"
                                }
                        ],
                        "name" : "i_1_345",
                        "unique" : false
                }



//  if (table_meta_.index(index_name) != nullptr || table_meta_.find_mult_index_by_field((attribute_count,attributes_name))) {
  //   LOG_INFO("Invalid input arguments, table name is %s, index %s exist or attribute %d exist index",
  //       name(),
  //       index_name,
  //       attribute_count);
  //   return RC::SCHEMA_INDEX_EXIST;
  // }
  
  
  //小王同学：唯一索引
RC DefaultHandler::create_index(
    Trx *trx, const char *dbname, const char *relation_name, const char *index_name, const char *attribute_name)
{
  Table *table = find_table(dbname, relation_name);
  if (nullptr == table) {
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  //to do 先把普通索引实现了，在回来修改唯一索引
  // return table->create_index(trx, index_name, attribute_name,false);
  return 0;
}
~~~



## ⼀次插入多条数据 insert



### 1. 测试用例



~~~
cd /root/gitee/miniob-2022/build/bin
./observer -f /root/gitee/miniob-2022/etc/observer.ini
/root/gitee/miniob-2022/build/bin/obclient

CREATE TABLE insert_table(id int, t_name char, col1 int, col2 int);

CREATE TABLE multi_index3(id int, col1 int, col2 float, col3 char, col4 date, col5 int, col6 int);
SUCCESS
CREATE INDEX i_3_i1 ON multi_index3(id,col1);
SUCCESS

INSERT INTO multi_index3 VALUES (1, 1, 11.2, 'a', '2021-01-02', 1, 1);



1. INSERT
CREATE TABLE insert_table1(id int, t_name date, col1 int, col2 int);
INSERT INTO insert_table1 VALUES (1,'2021-01-02',1,1);
INSERT INTO insert_table VALUES (1,'N1',1,1);
INSERT INTO insert_table VALUES (2,'N2',1,1),(3,'N3',2,1);


2. ERROR
INSERT INTO insert_table VALUES (4,'N4',1,1),(1,1,1);
FAILURE
INSERT INTO insert_table VALUES (4,'N4',1,1),(1,1,1,1);
FAILURE

3. SELECT
SELECT * FROM insert_table;
1 | N1 | 1 | 1
2 | N2 | 1 | 1
3 | N3 | 2 | 1
ID | T_NAME | COL1 | COL2




flex --header-file=lex.yy.h lex_sql.l
bison -d -b yacc_sql yacc_sql.y

cd /root/gitee/miniob-2022/build/bin
./observer -f /root/gitee/miniob-2022/etc/observer.ini
/root/gitee/miniob-2022/build/bin/obclient

dir /root/gitee/miniob-2022/
set pagination off 
thread apply all break execute_stage.cpp:604

RC ExecuteStage::do_insert(SQLStageEvent *sql_event)

~~~





### 2. 数据结构



~~~
typedef struct ParserContext {
  Query * ssql;
  size_t select_length;
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  Value values[MAX_NUM];
  Condition conditions[MAX_NUM];
  CompOp comp;
	char id[MAX_NUM];
} ParserContext;

class InsertStmt : public Stmt

C/C++ GNU Global

//属性值类型
typedef enum
{
  UNDEFINED,
  CHARS,
  INTS,
  FLOATS,
  DATES
} AttrType
~~~





### 3. 算法

~~~
parse.cpp -->SCF_INSERT

ResolveStage::handle_event
	-->Stmt::create_stmt
		case SCF_INSERT:
		-->InsertStmt::create(Db *db, Inserts &inserts, Stmt *&stmt)
		  --->InsertStmt::InsertStmt


ExecuteStage::handle_request
   Stmt *stmt = sql_event->stmt();
   case StmtType::INSERT:
   
ExecuteStage::do_insert
RC Table::insert_record(Trx *trx, int value_num, const Value *values) 
  

事务如何回滚
insert1
insert2
insert3 failed

~~~



### 4 工具

~~~
https://blog.csdn.net/qq_37041521/article/details/88865202
How to fix Git Error ‘Your local changes to the following files will be overwritten by merge’

Method 2: Keeping both changes

git stash
git pull origin master
git stash pop

Method 1: Forcing a pull to overwrite local changes
git reset --hard
git pull origin master


Linux Perf 性能分析工具及火焰图浅析
Perf 是内置于 Linux 内核源码树中的性能剖析（profiling）工具、
https://zhuanlan.zhihu.com/p/54276509
https://phoenixnap.com/kb/linux-perf

perf stat对全局性能进行统计；
perf top可以实时查看当前系统进程函数占用率情况；
pref record记录信息到perf.data；
perf report生成报告；
1，首先使用 perf record 命令记录进程的 CPU 使用情况
命令：

sudo perf record -e cpu-clock -g -p 1824049


2. 使用 perf script 工具对 perf.data 进行解析
命令：sudo perf script -i perf.data &> perf.unfold
3. 使用 Flame Graph 工具将 perf.unfold 中的符号折叠 //生成脚本文件
yum install perl
./FlameGraph/stackcollapse-perf.pl perf.unfold &> perf.folded


4. 使用 Flame Graph 工具将 perf.folded 生成 svg 火焰图
命令：

./FlameGraph/flamegraph.pl perf.folded > perf.svg 

//执行完成后生成perf.svg图片，可以下载到本地，用浏览器打开 perf.svg


命令：perf script | ./FlameGraph/stackcollapse-perf.pl | ./FlameGraph/flamegraph.pl > out.svg

火焰图插件下载地址：
https://github.com/brendangregg/FlameGraph
快捷命令：
git clone https://github.com/brendangregg/FlameGraph.git
火焰图原文翻译：
https://zhuanlan.zhihu.com/p/73385693

软件的性能分析，往往需要查看 CPU 耗时，了解瓶颈在哪里
http://www.ruanyifeng.com/blog/2017/09/flame-graph.html

致命报错：想生成火焰图，但是老是抛出错误：ERROR: No stack counts found，could not generate flame graph: exit status 2。
弄了很久，以为是 perf 没装，但是装了也不行，然后发现必须在访问程序时，火焰图才能生成
~~~



### day1

- [ ]  https://github.com/watchpoints/miniob_2022/blob/main/docs/how_to_build.md

- centos7.8系统【建议用8】

```shell

#命令安装版本过低
yum install  cmake
cmake --version 
2.8.12.2 //cmake 版本 >= 3.10
rpm -e cmake-2.8.12.2-2.el7.x86_64
# 下载二进制安装包
wget https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-linux-x86_64.sh
bash cmake-3.24.0-linux-x86_64.sh

export PATH=$PATH:$GOROOT/bin:/root/github/cmake-3.24.0-linux-x86_64/bin
/root/github/cmake-3.24.0-linux-x86_64/bin
cmake --version
cmake version 3.24.0

# gcc过低 需要源码安装
gcc --version
gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-44
tar xzvf gcc-11.3.0.tar.gz
cd gcc-11.3.0

# gcc不同版本切换 

Centos7 使用 yum 安装的 GCC 版本是 4.8.5，蛮老的版本了，现在稍微新一点的库都要求 GCC5 以上，但是重新编译安装最新的GCC又特别麻烦，而且不能在各个版本的 GCC 之间来回切换，不够灵活，刚好有一个叫做 SCL 的软件库可以提供不同版本 GCC 的灵活切换，可以完美解决我们的需求，安装十分简单：
————————————————
版权声明：本文为CSDN博主「thewangcj」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/wcjlyj/article/details/122844189

yum install centos-release-scl scl-utils-build
yum search devtoolset # 搜索 GCC
devtoolset-9-gcc.x86_64 : GCC version 9

yum install devtoolset-9-gcc.x86_64 
scl enable devtoolset-9 # 启用 GC9

yum install devtoolset-7-gcc.x86_64 
scl enable devtoolset-7 bash
Operation timed out 

如果要每次登录都自动切换成高版本的gcc，需要运行一下 vi /etc/profile 在最后一行加入
scl enable devtoolset-9 bash
gcc --version
gcc (GCC) 9.3.1 20200408 (Red Hat 9.3.1-2)
```



- build libevent

~~~
git submodule add https://github.com/libevent/libevent deps/libevent
cd deps
cd libevent
git checkout release-2.1.12-stable
mkdir build
cd build
cmake .. -DEVENT__DISABLE_OPENSSL=ON
make
sudo make install
~~~



- build google test

~~~
git submodule add https://github.com/google/googletest deps/googletest
cd deps
cd googletest
mkdir build
cd build
cmake ..
make
sudo make instal

 error: body of constexpr function ‘static constexpr bool testing::internal::MatcherBase<T>::IsInlined() [with M = testing::internal::EqMatcher<std::basic_string<char> >; T = const std::basic_string<char>&]’ not a return-statement
   }


~~~



#### centos7 不行升级为centos8



~~~
git clone git@github.com:watchpoints/miniob_2022.git

git submodule add https://github.com/libevent/libevent deps/libevent
cd deps
cd libevent
git checkout release-2.1.12-stable
mkdir build
cd build
cmake .. -DEVENT__DISABLE_OPENSSL=ON
make
sudo make install

git submodule add https://github.com/google/googletest deps/googletest
cd deps
cd googletest
mkdir build
cd build
cmake ..
make
sudo make instal

OpenSSL SSL_read: SSL_ERROR_SYSCALL, errno 104


git submodule add https://github.com/open-source-parsers/jsoncpp.git deps/jsoncpp
cd deps
cd jsoncpp
mkdir build
cd build
cmake -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF ..
make
sudo make install

~~~



> build miniob



~~~
mkdir build
cd build
# 建议开启DEBUG模式编译，更方便调试
cmake .. -DDEBUG=ON
make
~~~

- https://github.com/watchpoints/miniob_2022/blob/main/docs/miniob-introduction.md

#### 运行服务端

```
python3 miniob_test.py \
        --test-case-dir=./test  \
        --test-case-scores=case-scores.json \
        --test-result-dir=result \
        --test-result-tmp-dir=./result_tmp \
        --use-unix-socket \
        --git-repo=https://gitee.com/blogg9ggg/miniob-2022.git\
        --git-branch=main \
        --code-type=git \
        --target-dir=./miniob \
        --log=stdout \
        --compile-make-args=-j4

/root/gitee/miniob-2022/build/bin/observer -f /root/gitee/miniob-2022/etc/observer.ini


/root/github/miniob-2022/build/bin/obclient


oot@node1 bin]# ./observer
./observer: error while loading shared libraries: libjsoncpp.so.25: cannot open shared object file: No such file or directory 环境变量导入了 怎么执行还报错

无效：
export LD_LIBRARY=$LD_LIBRARY:/usr/local/lib64:/root/github/miniob_2022/deps/jsoncpp/build/lib

正确：
直接把那3个文件拷贝到/usr/local/lib/下试试。

```

后端开发工程师-飞书存储业务-技术专家/架构师/资深研发

Base 地：深圳 / 广州 / 武汉
特别说明：飞书 / 创作与生态 / 领域基础架构部。是结合具体场景、将业务卡点问题下沉到定制化存储系统来解决。既有完全自研的分布式数据库产品，也有和公司数据库团队合作共建的定制化分支。期待候选同学有查询优化（Cost Estimation，Cost Model，Plan Enumeration 领域有深度认知和践行），MPP 调度执行引擎、向量化计算引擎 & 算子实践，微软 Socrates / Snowflake / PrestoDB 等相关经验之一。

职位描述
1、研发基础存储组件和服务（包含分布式查询引擎和分布式存储引擎等），深度参与到存储架构设计、落地、演进的过程中，提升存储和数据服务的核心技术品质；
2、深入垂直业务领域，解决超大规模 ToB 系统的性能和稳定性问题；既可以深入到系统底层，又可以和业务架构深入协作，通过优秀的架构设计来满足多元化的扩展和部署需求，是互联网架构师成长的沃土；
3、解决大规模数据存储模型、分布式数据结构、存储稳定性和平滑扩展等领域难题。

职位要求
1、2 年及以上服务端 C++ 开发经验，对分布式系统有持之以恒的兴趣和热情；
2、 Linux 系统知识扎实，精通任意一种语言的多线程、网络编程、分布式开发；
3、深入了解数据库原理，熟练掌握分布式存储、计算、查询优化器的关键技术并有实战经验；
4、熟读过 StarRocks / Apache Doris / Rocksdb / TiDB / CockroachDB 任一代码优先；
5、开源作品、大厂分布式系统实践、ACM、国内一线技术竞赛获奖经验加分。





### 题目2：unique

#### 2.1 测试

~~~
cd /root/gitee/miniob-2022/build/bin/
./observer -f /root/gitee/miniob-2022/etc/observer.ini

/root/gitee/miniob-2022/build/bin/obclient

dir /root/gitee/miniob-2022/
set pagination off 
thread apply all break execute_stage.cpp:604
thread apply all break execute_stage.cpp:619

RC Table::insert_record(Trx *trx, Record *record)
/root/gitee/miniob-2022/src/observer/storage/index/bplus_tree_index.cpp

flex --header-file=lex.yy.h lex_sql.l
bison -d -b yacc_sql yacc_sql.y


thread apply all break bplus_tree_index.cpp:619
INITIALIZATION
CREATE TABLE unique_table(id int, col1 int, col2 int);
SUCCESS
INSERT INTO unique_table VALUES (1,1,1);
SUCCESS

1. UNIQUE TEST
CREATE UNIQUE INDEX index_id on unique_table(id);
SUCCESS
INSERT INTO unique_table VALUES (2,1,1);
SUCCESS
CREATE UNIQUE INDEX index_id on unique_table(id);
FAILURE
INSERT INTO unique_table VALUES (3,2,1);
SUCCESS
INSERT INTO unique_table VALUES (1,2,1);
FAILURE
select * from unique_table where id=1;

CREATE INDEX index_id on unique_table2(id1,id2);

CREATE TABLE unique_table2(id1 int, id2 int, id3 int);
CREATE UNIQUE INDEX index_id on unique_table2(id1,id2);
INSERT INTO unique_table2 VALUES (2,2,1,1);
INSERT INTO unique_table2 VALUES (1,1,1,1);
2. SELECT
SELECT * FROM unique_table;
1 | 1 | 1
2 | 1 | 1
3 | 2 | 1
ID | COL1 | COL2


flex --header-file=lex.yy.h lex_sql.l
bison -d -b yacc_sql yacc_sql.y

/root/gitee/miniob-2022/build/bin/observer -f /root/gitee/miniob-2022/etc/observer.ini
/root/github/miniob-2022/build/bin/obclient

dir /root/gitee/miniob-2022/
set pagination off 
thread apply all break execute_stage.cpp:604

RC ExecuteStage::do_insert(SQLStageEvent *sql_event)

miniob > select * from unique_table where id=1;
id | col1 | col2
miniob > select * from unique_table;


~~~



##### 2.2 流程图

~~~c++
[OptimizeStage]
ThreadId=SQLThreads
NextStages=ExecuteStage

[ExecuteStage]
ThreadId=SQLThreads
NextStages=DefaultStorageStage,MemStorageStage

[DefaultStorageStage]
ThreadId=IOThreads
BaseDir=./miniob
SystemDb=sys


src/observer/sql/parser/parse.cpp
void query_reset(Query *query)

src/observer/sql/executor/execute_stage.cpp
void ExecuteStage::handle_request(common::StageEvent *event)

src/observer/sql/executor/execute_stage.cpp
RC Table::create_unique_index(Trx *trx, const char *index_name, const char *attribute_name)
~~~







### 题目1 

#### 流程

~~~
[ResolveStage]
ThreadId=SQLThreads
NextStages=QueryCacheStage

[QueryCacheStage]
ThreadId=SQLThreads
NextStages=OptimizeStage


sql解析
ParseStage::handle_request
语义解析：
ResolveStage::handle_event
 
   Stmt::create_stmt

~~~



#### 代码

~~~
Stmt::create_stmt
case SCF_SELECT: {
      return SelectStmt::create(db, query.sstr.selection, stmt);
    }
    
    
sql解析
ParseStage::handle_request

Stmt::create_stmt



src/observer/sql/parser/lex_sql.l
src/observer/sql/parser/yacc_sql.y

// struct of select
typedef struct {
  size_t attr_num;                // Length of attrs in Select clause
  RelAttr attributes[MAX_NUM];    // attrs in Select clause
  size_t relation_num;            // Length of relations in Fro clause
  char *relations[MAX_NUM];       // relations in From clause
  size_t condition_num;           // Length of conditions in Where clause
  Condition conditions[MAX_NUM];  // conditions in Where clause
} Selects;

SelectStmt::create


flex --header-file=lex.yy.h lex_sql.l
bison -d -b yacc_sql yacc_sql.y
注意：当前没有把lex_sql.l和yacc_sql.y加入CMakefile.txt中，所以修改这两个文件后，需要手动生成c代码，然后再执行编译。

注意：flex 使用 2.5.35 版本测试通过，bison使用3.7版本测试通过

flex 

CentOS8设置国内阿里云镜像源2022
https://www.imhu.cn/days/857

/root/gitee/miniob-2022/src/observer/sql/parser
flex --header-file=lex.yy.h lex_sql.l
bison -d -b yacc_sql yacc_sql.y
~~~



#### 测试



~~~
git reset --hard
我的手机号是17728151873
/root/gitee/miniob-2022/build/bin/observer -f /root/gitee/miniob-2022/etc/observer.ini


/root/github/miniob-2022/build/bin/obclient
git reset --hard origin/master
select *,age from Select_meta2;
select *,age from Select_meta;
select *,id1 from Select_meta;
CREATE TABLE Select_meta(id int, age int);
SUCCESS

1. SELECT FROM A NON-EXISTENT TABLE
select * from no_table;
FAILURE

2. SELECT FROM A NON-EXISTENT COLUMN
select home from Select_meta;
FAILURE
select * from Select_meta where home='001';
FAILURE


~~~



3. SELECT * AND A NON-EXISTENT COLUMN 

   select *, age from Select_meta2;

    -FAILURE +FAILED TO PARSE SQL 4. SELECT * AND A EXISTENT COLUMN select *, age from Select_meta; -ID | AGE | AGE +FAILED TO PARSE SQL

![image-20221019115032999](assets/image-20221019115032999.png)
