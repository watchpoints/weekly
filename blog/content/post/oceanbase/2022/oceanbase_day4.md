---
title: "成为oceanbase贡献者第四天：容器与分配器"
date: 2022-10-02
description: "自我驱动学习，专业上只能靠自己"
draft: false
categories: ["oceanbase"]
---





# #一、Overview 

> 前言

大家好,今天是成为oceanbase贡献者第四天



本文 主要描述的是 

1. 通过#1038  the template parameter of ObSortedVector must be pointer type which is not resonable

- 了解模板特性  类型推导使用场景：
- 了解模板特性偏特化，萃取 使用场景，
- 了解右值引用和通用引用使用场景

2. 顺便完成

[【课后练习】从0到1数据库内核实战教程七](https://ask.oceanbase.com/t/topic/35601338)

**【说明】：** 推荐 deps/oblib/src/lib/allocator 内存分配管理相关代码；
 **推荐论文**

- 《AddressSanitizer: [A Fast Address Sanity Checker](https://github.com/tpn/pdfs/blob/master/AddressSanitizer%20-%20A%20Fast%20Address%20Sanity%20Checker%20(atc12-final39).pdf)》

- 《[Oscar](https://www.usenix.org/conference/usenixsecurity17/technical-sessions/presentation/dang)：A Practical Page-Permissions-Based Scheme for [Thwarting Dangling Pointers](https://www.youtube.com/watch?v=5HTt4eRWf3E)》

    

3. **第二期 操作系统重要项目1--协程** 

   https://wx.zsxq.com/dweb2/index/group/51122582242854

实现一个可以运行的有栈对称协程 核心功能： 

1 协程上下文切换 

 2 协程运行栈 

3 非对称调度或者对称协程调度（可选）

 4 hook系统调用（可选）（socket相关的系统调用：connect，read，write等随便选一个





**思考 60 秒 写下你疑问**





大纲





# # 二、MVP

> 最少知识



## 2.1 sql 一生 【回顾】

#### 详细流程



![入口](https://img-blog.csdnimg.cn/f1dee96b6097419c8feff51e28c3137d.png)





~~~mermaid

graph TB
    1[ObMPQuery::process]-->2[ObMPQuery::do_process ]  -->5[ObSql::handle_text_query] -->4[handle_physical_plan] -->5[ ObMPQuery::response_result] -->6[ObMPQuery::register_callback_with_async] -->7[ObCmdExecutor::execute] -->8[ObDDLOperator::alter_table_column]
~~~



- ```c
  
  
  ```
  
  ![](https://img-blog.csdnimg.cn/e423a396e3be4e659a885d0ed81fc9a8.png)

![](https://img-blog.csdnimg.cn/dcfb6a4bd280418dad2b1c30e494b010.png)



![](https://img-blog.csdnimg.cn/f43a468108504016b441bb6cef633d0f.png)







 2.2 



~~~
template <typename T, typename Allocator = PageArena<T> >
class ObVector {}
D:\oceanbase\deps\oblib\src\lib\allocator\page_arena.h

~~~



####  提问：ObVector 如何申请一个变量内存地址？

>  1号补丁：new  operator new 与malloc有什么区别？



~~~
void* ObAllocator::alloc(const int64_t size, const ObMemAttr& attr)
{
  UNUSED(attr);
  void* ptr = nullptr;
  int ret = OB_SUCCESS;
  if (OB_UNLIKELY(!is_inited_)) {
    ret = init();
  }
  if (OB_SUCC(ret)) {
    BACKTRACE(WARN, !attr.label_.is_valid(), "[OB_MOD_DO_NOT_USE_ME ALLOC]size:%ld", size);
    ObMemAttr inner_attr = attr_;
    if (attr.label_.is_valid()) {
      inner_attr.label_ = attr.label_;
    }
    AObject* obj = os_.alloc_object(size, inner_attr);
    if (NULL != obj) {
      ptr = obj->data_;
      get_mem_leak_checker().on_alloc(*obj);
    }
    if (OB_UNLIKELY(nullptr == obj) && REACH_TIME_INTERVAL(1 * 1000 * 1000)) {
      ObTenantCtxAllocator& ta = os_.get_block_mgr()->get_tenant_ctx_allocator();
      _OB_LOG(WARN, "[OOPS] alloc failed reason: %s", alloc_failed_msg());
      _OB_LOG(WARN,
          "oops, alloc failed, tenant_id=%ld, ctx_id=%ld, ctx_name=%s, ctx_hold=%ld, "
          "ctx_limit=%ld, tenant_hold=%ld, tenant_limit=%ld",
          inner_attr.tenant_id_,
          inner_attr.ctx_id_,
          common::get_global_ctx_info().get_ctx_name(inner_attr.ctx_id_),
          ta.get_hold(),
          ta.get_limit(),
          ta.get_tenant_hold(),
          ta.get_tenant_limit());
      // 49 is the user defined signal to dump memory
      raise(49);
    }
  }
  return ptr;
}

void ObAllocator::free(void* ptr)
{
  int ret = OB_SUCCESS;
  if (OB_UNLIKELY(!is_inited_)) {
    ret = init();
  }
  if (OB_SUCC(ret)) {
    if (OB_LIKELY(nullptr != ptr)) {
      AObject* obj = reinterpret_cast<AObject*>((char*)ptr - lib::AOBJECT_HEADER_SIZE);
      abort_unless(NULL != obj);
      abort_unless(obj->MAGIC_CODE_ == lib::AOBJECT_MAGIC_CODE || obj->MAGIC_CODE_ == lib::BIG_AOBJECT_MAGIC_CODE);
      abort_unless(obj->in_use_);

      get_mem_leak_checker().on_free(*obj);
      lib::ABlock* block = obj->block();
      abort_unless(block);
      abort_unless(block->check_magic_code());
      ObjectSet* os = block->obj_set_;
      abort_unless(os == &os_);
      os->free_object(obj);
    }
  }
}
~~~





## 单元测试



执行单元测试位置：

/root/src/oceanbase/build_debug/deps/oblib/unittest

代码位置：

/root/src/oceanbase/deps/oblib/unittest/lib/container

/root/src/oceanbase/deps/oblib/unittest/CMakeLists.txt









### 1. gdb调试





~~~shell


~~~

D:\oceanbase\src\sql\executor\ob_cmd_executor.cpp



~~~
set ob_enable_trace_log = 1
show trace;
obclient [oceanbase]> select * from __all_server_event_history

~~~



### 2. 环境说明

- bash build.sh debug --init --make -j8



~~~
源码编译
https://www.oceanbase.com/docs/community/observer-cn/V3.1.4/10000000000449658

git clone git@github.com:watchpoints/oceanbase.git
bash build.sh debug --init --make -j8

离线安装部署，必须禁用远程仓库：
obd mirror disable remote

/oceanbase/oceanbase/build_debug
make DESTDIR=./ install 



obd mirror create -n oceanbase-ce -V 3.1.4-1 -p ./usr/local -t my-oceanbase


https://gitee.com/oceanbase/obdeploy/blob/master/example/mini-distributed-example.yaml

obd cluster deploy test -c /oceanbase/mini-local-example.yaml
obd cluster deploy test -c ./mini-local-example.yaml
obd cluster start test
# Connect to the OceanBase Database by using a MySQL client.
mysql -h127.1 -uroot -P2883
alias cdob="obclient -uroot@sys -h127.0.0.1 -P2881 oceanbase"


~~~



- copy obd 目录

~~~c#
cd /root/oceanbase/build_debug/src/observer
cp /observer /root/.obd/repository/oceanbase-ce/3.1.4-1/my-oceanbase/bin/
~~~



 



### 3. 提交代码





~~~
git config user.name  watchpoints
git config user.email wang_cyi@163.com


## 关联远程仓库，保证代码可以和远程更新和提交
git remote add oceanbase https://github.com/oceanbase/oceanbase.git
git remote -v
#把远程仓库最新的代码及分支拉取一下 [可以在界面手工合并]
git fetch oceanbase 
git merge oceanbase/master

## 本地创建分支，进行开发
https://github.com/oceanbase/oceanbase/issues/1038
git checkout -b issue_1038
git branch -a

## 提交
git commit -m "fix Rocky is not supported yet #1014"
git push -u  origin issue_1035 

## 冲突解决


cp /root/oceanbase/build_debug/src/observer/observer /root/observer/bin/observer 
~~~







# # 三、 Conclusion:



1. 方法 不停gdb调试

2. ob代码中 主键和rowkey关系？

​    来源：https://github.com/oceanbase/oceanbase/issues/1035

3. ddl是通过rootservice（RS）执行的，所以其executor实际是发送RPC

​         src\rootserver\ob_ddl_operator.cpp



# # 四、Further reading

> 举一反三









> 慢慢来，多看、多问、多总结，肯定是可以攻克的。

### 沟通步骤

1. 准备好一个ppt，在写代码之前演示最终目标 和架构设计 就是如何去实现的 【不要说公司部门环境不对 着就是最终结果，不要试着看看，一定是可以完全上线的项目，非demo和一个知识点。自己认为真的 不是闹着玩的。。】

   一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题

   二、这个技术的优势和劣势分别是什么

   三、这个技术适用的场景。任何技术都有其适用的场景，离开了这个场景

   四、技术的组成部分和关键点。

   五、技术的底层原理和关键实现

   六、已有的实现和它之间的对比

   

2. 经过领导，专家 进行鸡蛋里挑骨头。【自己做好了别人路了胡扯，不会对别人产生任何影响，做事和做人一样，无论熟悉人，还是老师，领导，不相关人 反对 他们反馈信号，接受质疑，经过九九八十一难考验，并且你还在坚持认为对的。】

3. 最后融合别人建议，然后完善你项目。【不听老人言，吃亏在眼前，不敢接受别人批评，说明自己完全没有把握，才去否定 愤怒方式】

# # 相关文章



[1] [What is an Allocator?](https://medium.com/@vgasparyan1995/what-is-an-allocator-c8df15a93ed)

