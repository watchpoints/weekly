 
#ifndef TEST_MEMPOOL_H
#define TEST_MEMPOOL_H
#include <string>
#include<vector>
#include<map>
#include <iostream>
using namespace  std;

/**
防止内存碎片，提供内存池功能 
候选人可以在自己熟悉的ide 上，完成代码，然后将代码拷贝过来 
题目定义好接口，麻烦候选人实现接口和增加私有成员变量， 候选人可以增加新的接口和方法, 如果觉得接口设计不合理，欢迎改进
[感觉有问题 ，申请什么类型数据]
*/

class Node
{
public:
    Node* next;
};


class Mempool {
private:

    Node m_head; // 内存池。
    


    //这是Node其他参数。
    Node* m_pcur;
    map<Node*,bool> m_used; //tag --Node*->。被重复使用。 false  没有被占用， true 被占用

    int m_number;
    int m_total;
    int m_left;
    int m_block_size;

public:

/**
* 构造函数， 不做任何动作
*/
Mempool();

/**
* 析构函数， 如果资源没有释放， 进行资源释放
*/
virtual ~Mempool();


/**
* 申请 pool_num 个 pool_size 大小的内存， 这些内存块组成内存池
* 后续每次申请内存，都是从内存池中分配block_size 大小的内存，
* @param pool_num 申请大内存的个数
* @param pool_size 申请大内存的大小
* @param block_size 用户每一次申请的block 大小
* @return ： 0 表示成功， -1 表示失败
*/
int init(int pool_num, int pool_size, int block_size);

/**
* 释放资源
*/
void cleanup();

/*
* 从内存池中申请一个大小为 block_size（在init函数中被设置） 大小的内存块
* @param tag : 表示这个内存块的标记
* @return : 失败为nullptr， 成功则返回有效值
*/
char * alloc(const char* tag);
char * alloc(size_t  size);
void free(char * pointer);

/**
* 显示当前内存池的状态， 这些状态包含
* 1. 总内存多少
* 2. 已经使用了多少内存， 还剩下多少没有被使用的内存
* 3. 列出每个在使用中内存的tag
* @return
*/
const std::string to_string();

};


#endif //TEST_MEMPOOL_H

 