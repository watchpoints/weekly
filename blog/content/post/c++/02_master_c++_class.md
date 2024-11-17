---
title: "c++代码编程规范"
date: 2024-09-07
description: "c++"
draft: false
tags: ["man"]
categories: ["do book"]
---






## 函数

- 参数传递：值传递 与引用传递

~~~c++
void usage(const string &name) {

}

谈谈你对 const string &
~~~













## 数据结构

### 数组

- 初始化

~~~c++
 char buf[128];
memset(buf, 0xcc, sizeof(buf));
~~~



### map

- c++ map查找key

~~~
1 if(m.count(key)>0)
2 {
3     return m[key];
4 }
5 return null;
1 iter = m.find(key);
2 if(iter!=m.end())
3 {
4     return iter->second;
5 }
6 return null;
这里需要注意：前一种方法很直观，但是效率差很多。因为前面的方法，需要执行两次查找。因此，推荐使用后一种方法

~~~









# shell



- \# 使用 xargs 对指定目录中的所有文件计算 MD5 值 find /path/to/directory -type f | xargs md5sum
- 逗号替换空格

```bash
sed -i 's/,/ /g' example.txt
```
