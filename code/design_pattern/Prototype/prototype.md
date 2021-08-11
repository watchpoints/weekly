# 大话设计模式---原型模式Prototype



## 设计模式的要素

设计模式主要分为以下几个基本要素：

- **模式名称**：根据该设计模式所完成的功能，对该模式命名的一个设计模式名称
- **目的（What）**：对该设计模式的简要概括，描述了设计模式主要的功能
- **解决的问题（Why）**：描述了应该在何时使用该模式，该设计模式解决了什么问题，它包含了设计中存在的问题以及问题存在的原因
- **解决方案（How）**：描述了一个设计模式的核心部分，设计模式的组成部分之间的相互关系，各自的职责和协作方式。一般使用UML类图或是核心代码实现
- **解决效果**：描述了模式的优缺点以及在使用模式时应该权衡的问题



## 1. **目的（What）**



## 2. 解决的问题（Why）

Prototype 的意义在于，你拿到一个 Base* ，它指向某个 Derived 对象，你想克隆出 Derived 对象，

但代码中不写出 Derived 的具体类型，因为有很多派生类，这种情况下你用构造函数是搞不定的，type-switch 是 bad smells 。

另外，这里考虑 virtual 的性能损失是主次不分，构造对象需要分配内存，这开销比一次虚函数调用大多了。

Prorobuf Message 就采用了 prototype 模式，

- https://www.cnblogs.com/solstice/archive/2011/04/03/2004458.html
- https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.message



c++有拷贝构造函数。
Object *object1 = new Object();
Object *object2 = new Object(*object1);
在构造函数里实现深拷贝即可。

而网上的例子都是这样的
Object *object1 = new Object();
Object *object2 = object1->clone();

请问网上例子这样做有什么好处？
如何正确实现克隆模式内容？



1、为什么不用new直接新建对象，而要用原型模式？

首先，用new新建对象不能获取当前对象运行时的状态，其次就算new了新对象，在将当前对象的值复制给新对象，效率也不如原型模式高。

2、为什么不直接使用拷贝构造函数，而要使用原型模式？

原型模式与拷贝构造函数是不同的概念，拷贝构造函数涉及的类是已知的，原型模式涉及的类可以是未知的（基类的拷贝构造函数只能复制得到基类的对象）。

原型模式生成的新对象可能是一个派生类。拷贝构造函数生成的新对象只能是类本身。原型模式是描述了一个通用方法(或概念)，它不管是如何实现的，而拷贝构造则是描述了一个具体实现方法



- https://www.zhihu.com/question/52610176





## **3. 解决方案（How）**

![](https://img-blog.csdnimg.cn/20181111215404771.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nsb3VkVW5jbGU=,size_16,color_FFFFFF,t_70)





- **Prototype（抽象原型类）**：它是声明克隆方法的接口，是所有具体原型类的公共父类，可以是抽象类也可以是接口，甚至还可以是具体实现类。
- **ConcretePrototype（具体原型类）**：它实现在抽象原型类中声明的克隆方法，在克隆方法中返回自己的一个克隆对象。
- **Client（客户类）**：让一个原型对象克隆自身从而创建一个新的对象，在客户类中只需要直接实例化或通过工厂方法等方式创建一个原型对象，再通过调用该对象的克隆方法即可得到多个相同的对象。由于客户类针对抽象原型类 Prototype 编程，因此用户可以根据需要选择具体原型类，系统具有较好的可扩展性，增加或更换具体原型类都很方便。



## 4. **解决效果**

## c++



> *防不胜防*: 你写了一个函数， 在参数传递过程出现类型转换，想不到呀 ，客户端可能输入 不同情况。
>
> C.130: For making deep copies of polymorphic classes prefer a virtual `clone` function instead of copy construction/assignment
>
> https://www.cnblogs.com/solstice/archive/2011/04/03/2004458.html
>
> https://liam.page/2020/06/29/Dynamic-Get-Fields-in-Protobuf-by-Their-Name-in-Cpp/

~~~c++

inline google::protobuf::Message* createMessage(const std::string& type_name)
{
  google::protobuf::Message* message = NULL;
  const google::protobuf::Descriptor* descriptor =
    google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
  if (descriptor)
  {
    const google::protobuf::Message* prototype =
      google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
    if (prototype)
    {
      message = prototype->New();
    }
  }
  return message;
}



#include <iostream>
using namespace std;
 
class Prototype
{
public:
	 Prototype(){}
	 ~Prototype(){}
 
	 virtual Prototype *clone() = 0;
};
 
class ConcretePrototypeA :public Prototype
{
public:
	ConcretePrototypeA() :member(0){}
	~ConcretePrototypeA(){}
 
	ConcretePrototypeA(const ConcretePrototypeA &rhs)
	{
		member = rhs.member;
	}
 
	virtual ConcretePrototypeA* clone()
	{
		cout << "copy of self" << endl;
		return new ConcretePrototypeA(*this);
	}
 
private:
	int member;
};
 
int main(int argc, char **argv)
{
	//生成对像
	ConcretePrototypeA *conPro = new ConcretePrototypeA();
 
	//复制自身
	ConcretePrototypeA * conPro1 = conPro->clone();
 
	//复制自身
	ConcretePrototypeA * conPro2 = conPro->clone();
 
	delete conPro;
	conPro = NULL;
 
	delete conPro1;
	conPro1 = NULL;
 
	delete conPro2;
	conPro2 = NULL;
 
	return 0;
}
~~~



## go



## java

