---
title: Golang简明教程（1）
date: 2025-04-12
description: c++
draft: false
tags:
  - 小而美
categories:
  - do book
---


小王同学开发日记第四天

## 我的目标 成为   ContextOS布道师


✅ 任务1： 阅读 ContextOS 云操作系统 提供云研发小程序

✅ 任务2：熟悉框架：后端冰山架( Golang)，前端火山架(JS+CSS)使用方法

✅ 任务3：探索如何模块化方式 从0到1快速开发一个小程序

由于时间有限，如有误，恳请大家不吝指教，万分感激。


# 一、今天任务 花费30分钟了解golang语法

## 1.1 golang 结构体定义（c++中的类）


在Go语言中，类型声明的语法确实与许多其他编程语言不同。
Go语言采用了"**名称在前，类型在后**"的声明方式，这种设计有其特定的原因和优势。

小故事：

在访谈中，当被问及创造 Go 的初衷时，Rob Pike 给出了一个近乎“玩笑”却又无比真实的答案：

> **“我们厌倦了等待 C++ 的编译。”**

他生动地描绘了当时在 Google 内部的日常：为了构建一个巨大的 C++ 二进制文件，团队成员不得不在庞大的计算集群上等待超过一个小时。

更令人抓狂的是失控的依赖管理。Rob Pike 提到，他的同事 Mike Burrows（Chubby 的作者）在一次漫长的编译中发现，一个他从未听说过的、与项目毫无关系的头文件，竟然被重复编译了 **37,000 次**！
**在 Go 中，我们试图采取完全相反的方式：尽可能地移除东西，将其简化到骨架，只保留你构建一切所需的绝对最小值**



```go
type Books struct {  
   title string  
   author string  
   subject string  
   book_id int  
}
//c++ 语法是 class  books
//在 Go 语言中，`type Books struct` 是定义结构体（Struct）的基础语法，用于创建自定义的复合数据类型
```

为什么Go语言把类型放在后面?

 ​**​提升代码可读性（从左到右的自然阅读顺序）​**​
- ​**​符合自然语言习惯​**​：类型后置的声明方式（`变量名 + 类型`）更接近英语的表达习惯（例如 “x is an integer”）。开发者先看到变量名，再了解其类型，符合“名称-属性”的逻辑顺序

在golang 为什么没有class 这关键字?
- Go语言采用组合而不是继承来实现代码重用
- Go通过​**​接口（interface）​**​ 实现多态，而非类的继承体系
 总结：Go的无类设计 vs 传统OOP

|​**​特性​**​|​**​传统OOP（如Java）​**​|​**​Go语言​**​|
|---|---|---|
|​**​代码复用​**​|继承（`extends`）|组合（结构体嵌入）|
|​**​多态​**​|类继承+接口|接口（隐式实现）|
|​**​运行时开销​**​|虚函数表、动态分派|静态绑定、连续内存|
|​**​设计目标​**​|强类型层级|简洁性、高效性、解耦|

## 1.2 类成员函数定义

#### 为什么golang 函数不像c++，java在类中声明？

**无类（No Class）​**​：Go语言​**​没有“类”的概念​**​，而是通过​**​结构体（struct）封装数据​**​，通过​**​方法（method）关联行为​**​，但方法并非在结构体内部声明，而是通过​**​显式接收者（receiver）绑定​**​到类型

设计哲学：组合优于继承，类型与方法解耦
 Composition and forwarding 组合

- 在面向对象的世界中，对象由更小的对象组合而成。
- 术语：对象组合或组合
- Go通过结构体实现组合（composition）。
- Go提供了“嵌入”（embedding）特性，它可以实现方法的转发（forwarding）
- 组合是一种更简单、灵活的方式。


```go
type Circle struct { Radius float64 }
// 方法绑定：通过接收者关联到结构体
func (c Circle) Area() float64 {
    return math.Pi * c.Radius * c.Radius
}

```
## 参考
- https://www.runoob.com/go/go-structures.html
-  探索 Go 语言的无类设计：从 Struct 到组合的优雅之道

## 1.3 Embedding

Go 语言并不支持传统意义上的继承，相反，它提倡通过组合来扩展类型的功能。这并不是 Go 所特有的概念，继承之上的组合是 OOP 的一个众所周知的原则，

在《设计模式》一书的第一章就有介绍。Embedding(嵌入)是 Go 语言一个重要的特性


- 嵌入类型的所有方法会被"提升"到外部结构体
- 外部结构体可直接调用嵌入类型的方法
- 编译器会自动处理接收者类型转换：

```go

package main
import "fmt"
// 基础结构体：人员信息

type Person struct {
    Name string
    Age  int
}

// Person 的方法
func (p Person) Introduce() {
    fmt.Printf("大家好，我叫 %s，今年 %d 岁。\n", p.Name, p.Age)
}

// 外部结构体：员工信息（嵌入 Person）
type Employee struct {
    Person     // 匿名嵌入 Person
    Company string
    EmployeeID int
}

// Employee 的专属方法

func (e Employee) Work() {
    fmt.Printf("%s 正在 %s 公司工作（工号：%d）\n", e.Name, e.Company, e.EmployeeID)

}
func main() {

    // 创建 Employee 实例
    emp := Employee{

        Person: Person{

            Name: "张三",

            Age:  30,

        },

        Company:    "腾讯",

        EmployeeID: 1001,

    }

    // 直接访问嵌入结构体的字段（提升字段）

    fmt.Println("姓名:", emp.Name)       // 直接访问 Person.Name

    fmt.Println("年龄:", emp.Age)        // 直接访问 Person.Age

  

    // 调用嵌入结构体的方法（提升方法）

    emp.Introduce()  // 等价于 emp.Person.Introduce()

    // 调用 Employee 自身的方法

    emp.Work()

}

//在TiDB的源码中，确实广泛使用了Go的结构体嵌入特性来实现组合和代码复用


```


## 二、了解golang框架


## 2.1 10分钟了解 GORM 看那些资料
- b站视频【GORM简明教程】关于GORM你看这一个就够了
- 阅读官方文档 # GORM 指南 https://gorm.io/zh_CN/docs/
- https://www.bilibili.com/video/BV1U7411V78R
- https://www.liwenzhou.com/about/
- https://www.bilibili.com/video/BV1U7411V78R Go语言最流行的ORM框架--GORM简明教程，持续更新中...


## 2.2 GORM支持Migration特性【岗位分工：DBA 和开发工程师 他们不互通】

在 Go 语言中，GORM支持Migration特性，

支持根据Go Struct结构自动生成对应的表结构,

使用 `GORM` ORM 库的 `AutoMigrate` 方法可以自动创建数据库表，确保数据库结构与定义的模型结构一致。

`AutoMigrate` 方法非常方便，特别适合在开发阶段进行快速迭代

GORM支持Migration特性，支持根据Go Struct结构自动生成对应的表结构。

```
注意：GORM 的AutoMigrate函数，仅支持建表，不支持修改字段和删除字段，避免意外导致丢失数据。
```

 一、自动建表

通过AutoMigrate函数可以快速建表，如果表已经存在不会重复创建。

```
// 根据User结构体，自动创建表结构.
db.AutoMigrate(&User{})

// 一次创建User、Product、Order三个结构体对应的表结构
db.AutoMigrate(&User{}, &Product{}, &Order{})

// 可以通过Set设置附加参数，下面设置表的存储引擎为InnoDB
db.Set("gorm:table_options", "ENGINE=InnoDB").AutoMigrate(&User{})
```

 二、Schema方法

 1、检测表是否存在

```
// 检测User结构体对应的表是否存在
db.Migrator().HasTable(&User{})

// 检测表名users是否存在
db.Migrator().HasTable("users")
```

 2、建表

```
// 根据User结构体建表
db.Migrator().CreateTable(&User{})
```

 3、删除表

```
// 删除User结构体对应的表
db.Migrator().DropTable(&User{})

// 删除表名为users的表
db.Migrator().DropTable("users")
```

 4、删除字段

```
// 删除User结构体对应表中的description字段
db.Migrator().DropColumn(&User{}, "Name")
```

 5、添加索引

```
type User struct {
  gorm.Model
  Name string `gorm:"size:255;index:idx_name,unique"`
}

// 为 Name 字段创建索引
db.Migrator().CreateIndex(&User{}, "Name")
db.Migrator().CreateIndex(&User{}, "idx_name")

// 为 Name 字段删除索引
db.Migrator().DropIndex(&User{}, "Name")
db.Migrator().DropIndex(&User{}, "idx_name")

// 检查索引是否存在
db.Migrator().HasIndex(&User{}, "Name")
db.Migrator().HasIndex(&User{}, "idx_name")

type User struct {
  gorm.Model
  Name  string `gorm:"size:255;index:idx_name,unique"`
  Name2 string `gorm:"size:255;index:idx_name_2,unique"`
}
// 修改索引名
db.Migrator().RenameIndex(&User{}, "Name", "Name2")
db.Migrator().RenameIndex(&User{}, "idx_name", "idx_name_2")
```


