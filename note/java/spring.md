\---

title: "c++程序员应该认真学习一下spring系列"

date: 2021-02-09

description: "Interview Challenge"

draft: false

categories: ["java"]

\---

# c++程序员应该认真学习一下spring



- David Abrahams，Boost C++程序库开发组的一名发起成员和主持人

  

## 基础篇

### 常量

在Java程序里，常量用关键字static final修饰，常量又分为：

- 编译期常量
- 运行时常量

#### 编译期常量

下面是一个编译期常量：

```java
static final int A = 1024;
1
```

编译时，所有A的引用都将被替换成字面量（即1024），类型必须是基本类型或String。

####  运行时常量

下面就是一个运行时常量：

```java
static final int len = "Rhine".length();
```

**编译期常量**（Compile-time Constants）与**运行期常量**（Run-time Constants）

**编译期常量不依赖类，不会引起类的初始化；而运行时常量依赖类，会引起类的初始化**



~~~java
class Test {
	//静态代码块
	static {
		System.out.println("Class Test Was Loaded!");
	}
	
	//编译期常量
	public static final int num = 10;	
	//运行时常量
	public static final int len = "Rhien".length();
}

public class Main {
	public static void main(String[] args) throws Exception {
		System.out.println("num:"+Test.num);
		System.out.println("=== after get num ===");
		System.out.println("len:"+Test.len);
	}
}
/* 打印输出:
 * num:10
 * === after get num ===
 * Class Test Was Loaded!
 * len:5
 */

形式1：编译期常量=立即数
形式2：运行期常量=不能改变值的变量
~~~





## 设计篇



## 项目篇

