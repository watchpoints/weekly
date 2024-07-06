#include <thread>
#include <mutex>
#include <condition_variable>
#include <gtest/gtest.h>

using namespace std;
bool flag = false;
std::mutex m;
std::condition_variable cv;
//C++ Multi-Threading: Efficient Use of Condition Variables

//带着问题去看书 notify_one 使用之前为什么加锁
//假设你在旅游，而且正在一辆在夜间运行的火车上。在夜间，如何在正确的站点下车呢？
//理想的方式是，无论是早或晚，只要当火车 到站的时候，有人或其他东西能把你唤醒就好了。


void thead_a_wait(void)
{
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [](){return flag;});
        //t0: 线程A判断!gDoneFlag条件成立，准备wait
        // t2: 线程A wait阻塞
        //why 导致了signal丢失，线程A无法唤醒
        cout << "get data " << endl;
}

void thread_b_notify(void)
{       
        //std::unique_lock<std::mutex> lk(m);
        flag = true;
        cv.notify_one(); // 如果 notify_one 被调用时没有线程等待，那么这个通知就会丢失，因为没有线程可以接收这个通知
        //t1: 线程B置gDongFlag为true，并notify
        cout << "gen data " << endl;
}

TEST(notify_test, T01)
{
        flag = false;

        std::thread tProd(thead_a_wait);
        std::thread tCons(thread_b_notify);

        tProd.join();
        tCons.join();
}

int main(int argc, char* argv[])
{
        testing::InitGoogleTest(&argc, argv);

        return RUN_ALL_TESTS();
}

/**
【昵称】小王同学
【坐标】山东
【自我介绍】
    1. 高级后端工程师,擅长c++并发编程。
    2. 专注分布式存储，分布式数据库。
    3. 时间的践行者，立志成为一名讲师。
【我的成绩】
    1.  为了走出农村，2次高考
         一次考研失败,
         遇挫之后不知道干什么,开启被动之旅。
    2. 为了找到合适工作,   
        深入研究c++书籍和leetcode 200题目
    3. 为了提高项目能力，参与开源项目建设。
    4. 为了把简单事情说清楚/要干啥
        按照《只管去做》,《福格行为模型>>方法。
        纸上得来终觉浅，绝知此事要躬行
        做一个践行者。
【我能提供】
    1.  后端程序员的简历优化+就业辅导+职业规划
    2.  全栈工程师(c++,rust,go,python )项目开发
    3. 一年践行12本书践行记录。
【希望一起解决什么，开启破圈之旅】
    1. 交接更多朋友，抱团取暖。
        寻找意义本身就更加有意义。
    2. 无法做整个系统，聚焦一个模块
         道可道也，非恒道也 
         名可名也,非恒名也。
         无名 万物之始也
         有名 万物之母也
         别想太多，只管去做，躬身入局
     
链接我： # 微信（github):watchpoints   
        # 公众号：后端开发成长指南
**/