





~~~
采用超级权限模式登录：(docker 一定要设置)
因为ptrace被Docker默认禁止的问题
docker exec  --privileged  -it oceanbase-ce  /bin/bash

## 设置代码目录
gdb -p xx
dir /root/oceanbase 
## 设置断点
## PROCESS
thread apply all break obmp_query.cpp:395

## handle_physical_plan
thread apply all break ob_sql.cpp:1200
thread apply all break ob_sql.cpp:3160
thread apply all break ob_sql.cpp:3217

/Users/wangchuanyi/code/src/note_oceanbase/src/observer/mysql/obmp_query.cpp


/root/observer/bin/observer -r 127.0.0.1:2882:2881 -o __min_full_resource_pool_memory=268435456,memory_limit=8G,system_memory=4G,stack_size=512K,cpu_count=16,cache_wash_threshold=1G,workers_per_cpu_quota=10,schema_history_expire_time=1d,net_thread_count=4,sys_bkgd_migration_retry_num=3,minor_freeze_times=10,enable_separate_sys_clog=0,enable_merge_by_turn=False,datafile_disk_percentage=20,enable_syslog_wf=False,enable_syslog_recycle=True,max_syslog_file_count=4 -z zone1 -p 2881 -P 2882 -c 1 -d /root/observer/store -i lo -l INFO



~~~





## 代码片段：

~~~
OB_INLINE int ObMPQuery::do_process

~~~





obclient安装

 alias cdob="obclient -u root -h 127.0.0.1 -P2881"

yum install -y libobclient

 yum install -y obclient