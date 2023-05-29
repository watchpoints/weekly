---
title: "成为ob贡献者第三天"
date: 2022-09-25
draft: false
categories: ["oceanbase"]
---



> 多看、多问、多总结，肯定是可以攻克的。

# 一、Overview





- https://github.com/oceanbase/oceanbase/issues/1038

  

## 提出疑问

设计一个类是需要需要考虑什么问题

- 这个和侯捷的《STL源码剖析》提到知识有关系吗？

  1. 【stl源码】https://github.com/wangcy6/STL

     ​                https://github.com/wangcy6/reading_code_note/tree/master/SGI-STL

     

  2. 【课程：侯捷 - STL和泛型编程】https://www.youtube.com/watch?v=IoXNDKyI1L0&list=PLRTJhCIMo8HPW3OA74uVWIFWDCGRVckUV

  ​       16 vector深度探索

  3. 常见误区

  

- 这个和c++11 新特性 std::move std:forward 使用场景？  右值引用（T&&）用使用场景？

  1. https://www.youtube.com/watch?v=TJIb9TGfDIw&list=PL-X74YXt4LVYo_bk-jHMV5T3LHRYRbZoH
  2. 【资料】02_60秒思考：C++11新特性.xmind
  3.  【资料】[Effective Modern C++](https://app.yinxiang.com/shard/s39/nl/8226829/9ee26731-54a3-4e77-925f-309f897d9a1a/)
  4. 【资料】移动语义 Move semantics和完美转发问题。
  5.   概念错误理解：[std:move 就是左值变成右值](https://app.yinxiang.com/fx/6217a104-f50a-4e2d-87cc-db72b713dd94)
  6. 【找答案】https://github.com/watchpoints/microsoft_STL

  > 补丁 根据上面内容 单链表 和 vector怎么设计

  

  

  

## 大纲




~~~forth

~~~





# 二、MVP
### 2.1 流程图

![](https://cdn.nlark.com/yuque/__puml/a487d04a1c46cdb2d6493645a0b41219.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxuc3RhcnRcbjpvYl9yYXdfZXhwcl9yZXNvbHZlcl9pbXBsLmNwcDtcbjrkuIAu6KGo6L6-5byP55qE6Kej5p6QOuWug-i-k-WFpU9iUGFyc2VOb2RlIOagke-8jOi-k-WHuuihqOi-vuW8j-agkSBPYlJhd0V4cHI7XG46MS4gIE9iUmF3RXhwclJlc29sdmVySW1wbDo6ZG9fcmVjdXJzaXZlX3Jlc29sdmUoY29uc3QgUGFyc2VOb2RlKiBub2RlLCBPYlJhd0V4cHIqJiBleHByKVxuOjEuMSBPYlJhd0V4cHJSZXNvbHZlckltcGw6OnByb2Nlc3NfZnVuX3N5c19ub2RlKGNvbnN0IFBhcnNlTm9kZSogbm9kZSwgT2JSYXdFeHByKiYgZXhwcik7XG46MS4yIE9iUmF3RXhwclJlc29sdmVySW1wbDo6cHJvY2Vzc19kbGxfdWRmX25vZGUoY29uc3QgUGFyc2VOb2RlKiBub2RlLCBPYlJhd0V4cHIqJiBleHByKTtcblxuOjEuMS4xIE9iRXhwck9wZXJhdG9yRmFjdG9yeTo6Z2V0X3R5cGVfYnlfbmFtZShmdW5jX25hbWUpO1xuOjEuMS4yICBPYkV4cHJPcGVyYXRvckZhY3Rvcnk6OnJlZ2lzdGVyX2V4cHJfb3BlcmF0b3JzKClcbiAgICAgICAgICAgUkVHX09QKE9iRXhwckRheU9mTW9udGgpXG4gICAgICAgICAgIFJFR19PUChPYkV4cHJEYXlPZldlZWspXG4gICAgICAgICAgIFJFR19PUChPYkV4cHJEYXlPZlllYXIpO1xuc3RvcFxuXG5AZW5kdW1sIiwidXJsIjoiaHR0cHM6Ly9jZG4ubmxhcmsuY29tL3l1cXVlL19fcHVtbC9hNDg3ZDA0YTFjNDZjZGIyZDY0OTM2NDVhMGI0MTIxOS5zdmciLCJpZCI6IldXeGtGIiwibWFyZ2luIjp7InRvcCI6dHJ1ZSwiYm90dG9tIjp0cnVlfSwiaGVpZ2h0IjozNjMuOTkxMDY1OTc5MDAzOSwiY2FyZCI6ImRpYWdyYW0ifQ==)
### - 类图


![](https://cdn.nlark.com/yuque/__puml/0483ed3290f54635666954a237b8d373.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxuY2xhc3MgUGFyc2VOb2Rle1xuXHQgIyDoioLngrnnsbvlnotcblx0T2JJdGVtVHlwZSB0eXBlXztcbiAgIyDlrZDoioLngrnjgJDlkI7pnaLlnKjor6bnu4bnnIvjgJFcbiAgc3RydWN0IF9QYXJzZU5vZGUqKiBjaGlsZHJlbl87XG59XG5jbGFzcyBPYkV4cHJPcGVyYXRvciB7XG4jIOihqOi-vuW8j-WQjeWtlyBcbiMg6KGo6L6-5byP57G75Z6LXG5jb25zdCBPYkV4cHJPcGVyYXRvclR5cGUgdHlwZV87IFxuLyogdHlwZSBkZWZpbmVkIGluIHNxbC9vYl9pdGVtX3R5cGUuaCAqL1xufVxuY2xhc3MgT2JSYXdFeHBye1xuIFxufVxuY2xhc3MgT2JFeHBye1xuZXZhbF9mdW5jX1xu6L-Z5LiqIE9iUmF3RXhwciDku4XnlKjkuo7kvJjljJblmajpmLbmrrXnmoTor63kuYnliIbmnpDkvJjljJbvvIxcbuWcqOeUn-aIkOeJqeeQhuaJp-ihjOiuoeWIkuWQjuS8muiiq-i9rOaNouS4uuS4uuaJp-ihjOaViOeOh-S8mOWMlueahFxuT2JFeHByIOOAglxufVxuY2xhc3MgT2JFeHByVGltZUJhc2Uge1xuIyB2aXJ0dWFsIGludCBjZ19leHByXG59XG5PYkV4cHJPcGVyYXRvciAqLWRvd24tIE9iUmF3RXhwclxuXG5cblxuQGVuZHVtbCIsInVybCI6Imh0dHBzOi8vY2RuLm5sYXJrLmNvbS95dXF1ZS9fX3B1bWwvMDQ4M2VkMzI5MGY1NDYzNTY2Njk1NGEyMzdiOGQzNzMuc3ZnIiwiaWQiOiJFRTlGbSIsIm1hcmdpbiI6eyJ0b3AiOnRydWUsImJvdHRvbSI6dHJ1ZX0sImNhcmQiOiJkaWFncmFtIn0=)### 2.2 整体流程

-  parser阶段 ：func(arg1, arg2) 类型的表达式函数 统一parser 成T_FUN_SYS 类型，函数名存在子结点里

- 【resolver 模块】: 根据节点 T_FUN_SYS 类型 执行 process_fun_sys_node函数

- 【resolver 模块】: 根据函数名字查找类型：ObExprOperatorFactory::get_type_by_name(func_name)

- 【ob_expr_operator_factory.cpp】表达式名字 和类型 通过自定义实现类 提前注册REG_OP
```
#define REG_OP(OpClass)                                                \
  do {                                                                 \
    OpClass op(alloc);                                                 \
    if (OB_UNLIKELY(i >= EXPR_OP_NUM)) {                               \
      LOG_ERROR("out of the max expr");                                \
    } else {                                                           \
      NAME_TYPES[i].name_ = op.get_name();                             \
      NAME_TYPES[i].type_ = op.get_type();                             \
      OP_ALLOC[op.get_type()] = ObExprOperatorFactory::alloc<OpClass>; \
      i++;                                                             \
    }                                                                  \
  } while (0)
```

- 【ob_expr_time.cpp】自定义表达式类：在定义构造时候 初始化了 表达式的名字 和类型【写死了】
```
ObExprDayName::ObExprDayName(ObIAllocator& alloc)
    : ObExprTimeBase(alloc, DT_DAY_NAME, T_FUN_SYS_DAY_NAME, N_DAY_NAME){};
#define N_DAY_NAME "dayname"
```

- [表达式内置函数] virtual  cg_expr 注册自己实现函数到 eval_func_
- 【表达式内置函数】提供函数注册是的内存分配，类型定义

calc_result_type1  返回值的类型
calc_result_type2 返回值的类型


看到这里可用了 下面个人测试 和阅读记录

---

### 2.3 详细描述

#### ob_raw_expr_resolver_impl.cpp

```c
#781
 case T_FUN_SYS: {
          if (OB_FAIL(process_fun_sys_node(node, expr))) {
            if (ret != OB_ERR_FUNCTION_UNKNOWN) {
              LOG_WARN("fail to process system function node", K(ret), K(node));
            } else if (OB_FAIL(process_dll_udf_node(node, expr))) {
              if (ret != OB_ERR_FUNCTION_UNKNOWN) {
                LOG_WARN("fail to process dll user function node", K(ret), K(node));
              } else {
                ParseNode* udf_node = NULL;
                ObString empty_db_name;
                ObString empty_pkg_name;
                bool record_udf_info = true;
                if (OB_FAIL(ObResolverUtils::transform_func_sys_to_udf(
                        &ctx_.expr_factory_.get_allocator(), node, empty_db_name, empty_pkg_name, udf_node))) {
                  LOG_WARN("transform func sys to udf node failed", K(ret));
                } else if (OB_ISNULL(expr)) {
                  ret = OB_ERR_FUNCTION_UNKNOWN;
                  LOG_WARN("function does not exist", K(node->children_[0]->str_value_));
                  LOG_USER_ERROR(
                      OB_ERR_FUNCTION_UNKNOWN, (int32_t)node->children_[0]->str_len_, node->children_[0]->str_value_);
                }
              }
            }
          }
          break;
        }

else if (OB_ISNULL(expr)) {
                  ret = OB_ERR_FUNCTION_UNKNOWN;
                  LOG_WARN("function does not exist", K(node->children_[0]->str_value_));
                  LOG_USER_ERROR(
                      OB_ERR_FUNCTION_UNKNOWN, (int32_t)node->children_[0]->str_len_, node->children_[0]->str_value_);
                }
```

```c
  REG_OP(ObExprDayOfWeek);

class ObExprDayOfWeek : public ObExprTimeBase {
public:
  ObExprDayOfWeek();
  explicit ObExprDayOfWeek(common::ObIAllocator& alloc);
  virtual ~ObExprDayOfWeek();
  static int calc_dayofweek(const ObExpr& expr, ObEvalCtx& ctx, ObDatum& expr_datum);

private:
  DISALLOW_COPY_AND_ASSIGN(ObExprDayOfWeek);
};

int ObExprDayOfWeek::calc_dayofweek(const ObExpr& expr, ObEvalCtx& ctx, ObDatum& expr_datum)
{
  int ret = OB_SUCCESS;
  if (ObExprTimeBase::calc(expr, ctx, expr_datum, DT_WDAY, true)) {
    LOG_WARN("calc day of week failed", K(ret));
  } else if (!expr_datum.is_null()) {
    expr_datum.set_int32(expr_datum.get_int32() % 7 + 1);
  }
  return ret;
}
```

#### src\sql\engine\expr\ob_expr_time.cpp

```c
/root/src/oceanbase/deps/oblib/src/lib/timezone

deps\oblib\src\lib\timezone\ob_time_convert.h

int ObExprTimeBase::cg_expr

 case DT_MON_NAME:
        rt_expr.eval_func_ = ObExprMonthName::calc_month_name;
        break;
/root/src/oceanbase/src/sql/engine/expr

in ObExprTimeBase::calc_result1(ObObj& result, const ObObj& obj, ObExprCtx& expr_ctx) const


    static ObExpr::EvalFunc g_expr_eval_functions[] 
```


```c
int ObExprTimeBase::calc_result1(ObObj& result, const ObObj& obj, ObExprCtx& expr_ctx) const
{
  int ret = OB_SUCCESS;
  if (OB_UNLIKELY(obj.is_null())) {
    result.set_null();
  } else {
    ObTime ot;
    bool with_date = false;
    switch (dt_type_) {
      case DT_HOUR:
      case DT_MIN:
      case DT_SEC:
      case DT_USEC:
        with_date = false;
        break;
      case DT_YDAY:
      case DT_WDAY:
      case DT_MDAY:
      case DT_YEAR:
      case DT_MON:
        with_date = true;
        break;
      case DT_MON_NAME:
      case DT_DAY_NAME:
        with_date = true;
        break;
              
      default:
        LOG_WARN("ObExprTimeBase calc result1 switch default", K(dt_type_));
    }
    bool ignoreZero = (DT_MDAY == dt_type_ || DT_MON == dt_type_ || DT_MON_NAME == dt_type_);
    if (OB_FAIL(ob_expr_convert_to_dt_or_time(obj, expr_ctx, ot, with_date, ignoreZero))) {
      LOG_WARN("cast to ob time failed", K(ret), K(obj), K(expr_ctx.cast_mode_));
      if (CM_IS_WARN_ON_FAIL(expr_ctx.cast_mode_)) {
        LOG_WARN("cast to ob time failed", K(ret), K(expr_ctx.cast_mode_));
        LOG_USER_WARN(OB_ERR_CAST_VARCHAR_TO_TIME);
        ret = OB_SUCCESS;
        result.set_null();
      }
    } else {
      if (with_date && DT_MDAY != dt_type_ && ot.parts_[DT_DATE] + DAYS_FROM_ZERO_TO_BASE < 0) {
        result.set_null();
      } else if (DT_WDAY == dt_type_) {
        int32_t res = ot.parts_[DT_WDAY];
        res = res % 7 + 1;
        result.set_int32(res);
      } else if (DT_MON_NAME == dt_type_) {
        int32_t mon = ot.parts_[DT_MON];
        if (mon < 1 || mon > 12) {
          LOG_WARN("invalid month value", K(ret), K(mon));
          result.set_null();
        } else {
          const char* month_name = ObExprMonthName::get_month_name(ot.parts_[DT_MON]);
          result.set_string(common::ObVarcharType, month_name, strlen(month_name));
          result.set_collation_type(result_type_.get_collation_type());
          result.set_collation_level(result_type_.get_collation_level());
        }
      } else {
        result.set_int32(ot.parts_[dt_type_]);
      }
    }
  }
  return ret;
}

ObExprMonthName::ObExprMonthName(ObIAllocator& alloc)
    : ObExprTimeBase(alloc, DT_MON_NAME, T_FUN_SYS_MONTH_NAME, N_MONTH_NAME){};
#define N_MONTH_NAME "monthname"

int ObExprDayOfWeek::calc_dayofweek(const ObExpr& expr, ObEvalCtx& ctx, ObDatum& expr_datum)
{
  int ret = OB_SUCCESS;
  if (ObExprTimeBase::calc(expr, ctx, expr_datum, DT_WDAY, true)) {
    LOG_WARN("calc day of week failed", K(ret));
  } else if (!expr_datum.is_null()) {
    expr_datum.set_int32(expr_datum.get_int32() % 7 + 1);
  }
  return ret;
}
```
### src\sql\parser\ob_item_type.h
```c
// system function for mysql only
T_FUN_SYS_MONTH_NAME = 708, //sql 命令 怎么知道那个函数执行
#define N_MONTH_NAME "monthname"


ObExprMonthName::ObExprMonthName(ObIAllocator& alloc)
    : ObExprTimeBase(alloc, DT_MON_NAME, T_FUN_SYS_MONTH_NAME, N_MONTH_NAME){};

ObExprIsIpv4::ObExprIsIpv4(ObIAllocator& alloc)
    : ObFuncExprOperator(alloc, T_FUN_SYS_IS_IPV4, N_IS_IPV4, 1, NOT_ROW_DIMENSION)
{}
```
## 2.3 测试过程

- 根据日志看函数调用关系。
```c

 stmt_query (ob_sql.cpp:171) [112642][466][YB427F000001-0005E8344D6D28DD] [lt=3] [dc=0] fail to handle text query(stmt=SELECT DAYNAME("2017-06-15"), ret=-5055)

     
else if (OB_FAIL(handle_text_query(stmt, context, result))) {
    if (OB_EAGAIN != ret && OB_ERR_PROXY_REROUTE != ret) {
      LOG_WARN("fail to handle text query", K(stmt), K(ret));
    }
  }

int ObSql::handle_text_query(

    [root@h12-storage03 log]# tail -f observer.log |grep "not exist"
[2022-09-09 11:17:54.141943] INFO  [SHARE.SCHEMA] ob_schema_getter_guard.cpp:6335 [112642][466][YB427F000001-0005E8344D6D28DF] [lt=9] [dc=0] udf not exist(tenant_id=1, name=dayname)
[2022-09-09 11:17:54.141953] WARN  [SQL.RESV] do_recursive_resolve (ob_raw_expr_resolver_impl.cpp:798) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=7] [dc=0] function does not exist(node->children_[0]->str_value_="DAYNAME")
[2022-09-09 11:17:54.141966] WARN  do_recursive_resolve (ob_raw_expr_resolver_impl.cpp:800) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=11] [dc=0] FUNCTION DAYNAME does not exist
[2022-09-09 11:17:54.142091] INFO  [SERVER] obmp_base.cpp:1237 [112642][466][YB427F000001-0005E8344D6D28DF] [lt=4] [dc=0] send error package.(user_error_code=1305, err=-5055, sql_state="42000", message=FUNCTION DAYNAME does not exist
)


[5]+  Stopped                 tail -f observer.log | grep --color=auto "not exist"
[root@h12-storage03 log]# grep "YB427F000001-0005E8344D6D28DF" observer.log
[2022-09-09 11:17:54.141719] INFO  [SQL] ob_sql.cpp:163 [112642][466][YB427F000001-0005E8344D6D28DF] [lt=8] [dc=0] Begin to handle text statement(trunc_stmt=SELECT DAYNAME("2017-06-15"), sess_id=3221487631, execution_id=194262)
[2022-09-09 11:17:54.141943] INFO  [SHARE.SCHEMA] ob_schema_getter_guard.cpp:6335 [112642][466][YB427F000001-0005E8344D6D28DF] [lt=9] [dc=0] udf not exist(tenant_id=1, name=dayname)
[2022-09-09 11:17:54.141953] WARN  [SQL.RESV] do_recursive_resolve (ob_raw_expr_resolver_impl.cpp:798) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=7] [dc=0] function does not exist(node->children_[0]->str_value_="DAYNAME")
[2022-09-09 11:17:54.141966] WARN  do_recursive_resolve (:800) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=11] [dc=0] FUNCTION DAYNAME does not exist
[2022-09-09 11:17:54.141975] WARN  [SQL.RESV] resolve_sql_expr (ob_dml_resolver.cpp:208) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=2] [dc=0] fail to exec expr_resolver.resolve( &node, expr, *output_columns, sys_vars, sub_query_info, aggr_exprs, win_exprs, op_exprs, user_var_exprs)(ret=-5055, &node=0x7f07f2e6aa18, expr=NULL, *output_columns=[], sys_vars=[], sub_query_info=[], aggr_exprs=[], win_exprs=[], op_exprs=[], user_var_exprs=[])
[2022-09-09 11:17:54.141985] WARN  [SQL.RESV] resolve_field_list :1399) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=5] [dc=0] resolve sql expr failed(ret=-5055)
[2022-09-09 11:17:54.141989] WARN  [SQL.RESV] resolve_normal_query (ob_select_resolver.cpp:801) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=2] [dc=0] fail to exec resolve_field_list(*(parse_tree.children_[PARSE_SELECT_SELECT]))(ret=-5055)
[2022-09-09 11:17:54.141992] WARN  [SQL.RESV] resolve (ob_select_resolver.cpp:934) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=2] [dc=0] resolve normal query failed(ret=-5055)
[2022-09-09 11:17:54.141996] WARN  [SQL.RESV] select_stmt_resolver_func (ob_resolver.cpp:142) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=2] [dc=0] execute stmt_resolver failed(ret=-5055, parse_tree.type_=3035)
[2022-09-09 11:17:54.142005] WARN  [SQL] generate_stmt (ob_sql.cpp:1443) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=2] [dc=0] failed to resolve(ret=-5055)
[2022-09-09 11:17:54.142010] WARN  [SQL] generate_physical_plan (ob_sql.cpp:1531) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=3] [dc=0] Failed to generate stmt(ret=-5055, result.get_exec_context().need_disconnect()=false)
[2022-09-09 11:17:54.142014] WARN  [SQL] handle_physical_plan (ob_sql.cpp:3231) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=2] [dc=0] Failed to generate plan(ret=-5055, result.get_exec_context().need_disconnect()=false)
[2022-09-09 11:17:54.142018] WARN  [SQL] handle_text_query (ob_sql.cpp:1212) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=2] [dc=0] fail to handle physical plan(ret=-5055)
[2022-09-09 11:17:54.142023] WARN  [SQL] stmt_query (ob_sql.cpp:171) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=2] [dc=0] fail to handle text query(stmt=SELECT DAYNAME("2017-06-15"), ret=-5055)
[2022-09-09 11:17:54.142035] WARN  [SERVER] test_and_save_retry_state (ob_query_retry_ctrl.cpp:446) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=3] [dc=0] do not need retry(client_ret=-5055, err=-5055, expected_stmt=true, THIS_WORKER.get_timeout_ts()=1662693484141630, retry_type_=0, result.get_stmt_type()=1, result.get_exec_context().need_change_timeout_ret()=true, session->get_retry_info().get_last_query_retry_err()=0)
[2022-09-09 11:17:54.142042] INFO  [SERVER] ob_query_retry_ctrl.cpp:460 [112642][466][YB427F000001-0005E8344D6D28DF] [lt=4] [dc=0] check if need retry(client_ret=-5055, err=-5055, retry_type_=0, retry_times=1, multi_stmt_item={is_part_of_multi_stmt:true, seq_num:0, sql:"SELECT DAYNAME("2017-06-15")"})
[2022-09-09 11:17:54.142049] WARN  [SERVER] do_process (obmp_query.cpp:638) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=4] [dc=0] run stmt_query failed, check if need retry(ret=-5055, cli_ret=-5055, retry_ctrl_.need_retry()=0, sql=SELECT DAYNAME("2017-06-15"))
[2022-09-09 11:17:54.142063] WARN  [SERVER] do_process (obmp_query.cpp:745) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=4] [dc=0] query failed(ret=-5055, retry_ctrl_.need_retry()=0)
[2022-09-09 11:17:54.142083] INFO  [SERVER] obmp_base.cpp:1163 [112642][466][YB427F000001-0005E8344D6D28DF] [lt=4] [dc=0] sending error packet(err=-5055, bt="0xb553645 0xa899d40 0xa92d3cd 0xa8c70ce 0xa8c151b 0xb6d881d 0xa740c40 0xa71d3f9 0xa73e9d6 0xa71b2ff 0xa71b7cc 0x1e7613c 0x1e75fcd 0x1e7a00e 0xb4fbd75 0xb4fa515 0xb20b1cf", extra_err_info=0x7f07f2e578c8)
[2022-09-09 11:17:54.142091] INFO  [SERVER] obmp_base.cpp:1237 [112642][466][YB427F000001-0005E8344D6D28DF] [lt=4] [dc=0] send error package.(user_error_code=1305, err=-5055, sql_state="42000", message=FUNCTION DAYNAME does not exist)
[2022-09-09 11:17:54.142161] WARN  [SERVER] process (obmp_query.cpp:291) [112642][466][YB427F000001-0005E8344D6D28DF] [lt=3] [dc=0] fail execute sql(sql_id="", sql=SELECT DAYNAME("2017-06-15"), sessid=3221487631, ret=-5055, ret="OB_ERR_FUNCTION_UNKNOWN", need_disconnect=false)
[root@h12-storage03 log]#

SELECT DAYNAME("2022-09-20");
SELECT DAYOFWEEK('2022-09-20');
SELECT MONTHNAME('2022-09-20');



cp /root/src/oceanbase/build_debug/src/observer/observer /root/src/observer/bin/observer
obd cluster stop test
```
第二次测试

```c
[root@h12-storage03 log]# tail -f observer.log |grep DAYNAME
[2022-09-20 21:04:42.773166] INFO  [SQL] ob_sql.cpp:163 [437909][466][YB427F000001-0005E91B467985A3] [lt=9] [dc=0] Begin to handle text statement(trunc_stmt=SELECT DAYNAME("2022-09-20"), sess_id=3221487618, execution_id=62341)
[2022-09-20 21:04:42.773450] WARN  [SQL.RESV] do_recursive_resolve (ob_raw_expr_resolver_impl.cpp:798) [437909][466][YB427F000001-0005E91B467985A3] [lt=7] [dc=0] function does not exist(node->children_[0]->str_value_="DAYNAME")
[2022-09-20 21:04:42.773456] WARN  do_recursive_resolve (ob_raw_expr_resolver_impl.cpp:800) [437909][466][YB427F000001-0005E91B467985A3] [lt=4] [dc=0] FUNCTION DAYNAME does not exist
[2022-09-20 21:04:42.773517] WARN  [SQL] stmt_query (ob_sql.cpp:171) [437909][466][YB427F000001-0005E91B467985A3] [lt=2] [dc=0] fail to handle text query(stmt=SELECT DAYNAME("2022-09-20"), ret=-5055)
[2022-09-20 21:04:42.773537] INFO  [SERVER] ob_query_retry_ctrl.cpp:460 [437909][466][YB427F000001-0005E91B467985A3] [lt=5] [dc=0] check if need retry(client_ret=-5055, err=-5055, retry_type_=0, retry_times=1, multi_stmt_item={is_part_of_multi_stmt:true, seq_num:0, sql:"SELECT DAYNAME("2022-09-20")"})
[2022-09-20 21:04:42.773542] WARN  [SERVER] do_process (obmp_query.cpp:638) [437909][466][YB427F000001-0005E91B467985A3] [lt=4] [dc=0] run stmt_query failed, check if need retry(ret=-5055, cli_ret=-5055, retry_ctrl_.need_retry()=0, sql=SELECT DAYNAME("2022-09-20"))
[2022-09-20 21:04:42.773995] INFO  [SERVER] obmp_base.cpp:1237 [437909][466][YB427F000001-0005E91B467985A3] [lt=4] [dc=0] send error package.(user_error_code=1305, err=-5055, sql_state="42000", message=FUNCTION DAYNAME does not exist)
[2022-09-20 21:04:42.774084] WARN  [SERVER] process (obmp_query.cpp:291) [437909][466][YB427F000001-0005E91B467985A3] [lt=7] [dc=0] fail execute sql(sql_id="", sql=SELECT DAYNAME("2022-09-20"), sessid=3221487618, ret=-5055, ret="OB_ERR_FUNCTION_UNKNOWN", need_disconnect=false)
[2022-09-20 21:04:45.975760] INFO  [SQL] ob_sql.cpp:163 [437909][466][YB427F000001-0005E91B467985A4] [lt=8] [dc=0] Begin to handle text statement(trunc_stmt=SELECT DAYNAME("2022-09-20"), sess_id=3221487618, execution_id=62397)
[2022-09-20 21:04:45.976064] WARN  [SQL.RESV] do_recursive_resolve (ob_raw_expr_resolver_impl.cpp:798) [437909][466][YB427F000001-0005E91B467985A4] [lt=8] [dc=0] function does not exist(node->children_[0]->str_value_="DAYNAME")
[2022-09-20 21:04:45.976072] WARN  do_recursive_resolve (ob_raw_expr_resolver_impl.cpp:800) [437909][466][YB427F000001-0005E91B467985A4] [lt=5] [dc=0] FUNCTION DAYNAME does not exist
[2022-09-20 21:04:45.976166] WARN  [SQL] stmt_query (ob_sql.cpp:171) [437909][466][YB427F000001-0005E91B467985A4] [lt=4] [dc=0] fail to handle text query(stmt=SELECT DAYNAME("2022-09-20"), ret=-5055)
[2022-09-20 21:04:45.976194] INFO  [SERVER] ob_query_retry_ctrl.cpp:460 [437909][466][YB427F000001-0005E91B467985A4] [lt=6] [dc=0] check if need retry(client_ret=-5055, err=-5055, retry_type_=0, retry_times=1, multi_stmt_item={is_part_of_multi_stmt:true, seq_num:0, sql:"SELECT DAYNAME("2022-09-20")"})
[2022-09-20 21:04:45.976203] WARN  [SERVER] do_process (obmp_query.cpp:638) [437909][466][YB427F000001-0005E91B467985A4] [lt=6] [dc=0] run stmt_query failed, check if need retry(ret=-5055, cli_ret=-5055, retry_ctrl_.need_retry()=0, sql=SELECT DAYNAME("2022-09-20"))
[2022-09-20 21:04:45.976258] INFO  [SERVER] obmp_base.cpp:1237 [437909][466][YB427F000001-0005E91B467985A4] [lt=4] [dc=0] send error package.(user_error_code=1305, err=-5055, sql_state="42000", message=FUNCTION DAYNAME does not exist)
[2022-09-20 21:04:45.976355] WARN  [SERVER] process (obmp_query.cpp:291) [4

cp /root/src/oceanbase/build_debug/src/observer/observer /root/src/observer/bin/observer

                                                                         
obclient [oceanbase]> SELECT DAYOFWEEK('2022-09-20');
+-------------------------+
| DAYOFWEEK('2022-09-20') |
+-------------------------+
|                       3 |
+-------------------------+
Returns the weekday index for date (1 = Sunday, 2 = Monday, …, 7 = Saturday). These index values correspond to the ODBC standard. Returns NULL if date is NUL
SELECT MONTHNAME('2022-09-20');
SELECT DAYNAME("2022-09-20");
select dayname("1962-03-01");

cp /root/oceanbase/build_debug/src/observer/observer /root/observer/bin/observer
                                                                          
```
# 三、 Conclusion


- [https://github.com/pingcap/tidb/pull/10732](https://github.com/pingcap/tidb/pull/10732)
- [https://open.oceanbase.com/blog/10900229?currentPage=1](https://open.oceanbase.com/blog/10900229?currentPage=1)
- [https://open.oceanbase.com/blog/8600156?currentPage=undefined](https://open.oceanbase.com/blog/8600156?currentPage=undefined)
- [https://dev.mysql.com/doc/refman/5.7/en/date-and-time-functions.html#function_dayname](https://dev.mysql.com/doc/refman/5.7/en/date-and-time-functions.html#function_dayname)
- [https://dev.mysql.com/doc/refman/8.0/en/date-and-time-functions.html](https://dev.mysql.com/doc/refman/8.0/en/date-and-time-functions.html)
