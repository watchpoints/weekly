
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



# 题目分析

[Bug]: DROP COLUMN unexpected error #1035
https://github.com/oceanbase/oceanbase/issues/1035



CREATE TABLE v0 ( v2 INTEGER PRIMARY KEY , v1 VARCHARACTER ( 30 ) , UNIQUE v0 ( v1 ( 30 ) ) ) ;
ALTER TABLE v0 DROP COLUMN v2 ;

https://github.com/oceanbase/oceanbase/issues/999


## mvp：一个sql执行过程



int ObMPQuery::process()


const ObMySQLRawPacket& pkt = reinterpret_cast<const ObMySQLRawPacket&>(req_->get_packet());

ObParser parser(
            THIS_WORKER.get_sql_arena_allocator(), session.get_sql_mode(), session.get_local_collation_connection());

ObParser::pre_parse(sql_, pre_parse_result))


 ret = process_single_stmt(
                ObMultiStmtItem(false, 0, sql_), session, has_more, force_sync_resp, async_resp_used, need_disconnect);
			


int ObMPQuery::process_single_stmt(
