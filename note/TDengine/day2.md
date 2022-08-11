The TDengine service is down, and the getNumOfRowsInTimeWindow error is reported #15487
https://github.com/taosdata/TDengine/issues/15487

## 准备



- https://www.jianshu.com/p/02a5a1bf1096



~~~flow
st=>start: Start
op=>operation: Your Operation
cond=>condition: Yes or No?
e=>end
st->op->cond
cond(yes)->e
cond(no)->op
~~~





~~~flow
st=>start: query\src\qExecutor.c
op1=>operation: doIntervalAgg
op2=>operation: hashIntervalAgg
op3=>operation: createTimeIntervalOperatorInfo
op4=>operation: setupQueryRuntimeEnv
op5=>operation: doInitQInfo
e=>end

st->op5->op4->op3->op1->op2->e

~~~



## 方案


## 小总

