/*
 * @lc app=leetcode.cn id=86 lang=cpp
 *
 * [86] 分隔链表
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    /***
     *  理解需求：隔链表（不是排序）
     *  归纳分类：链表调整问题（仔细观察）
     *  数据结构：单链表（看图）
     *  a1:元素4大于3 ，元素5大3 ，翻转后位置不变。
     *  a2:大于2 2小于 3， 翻转后 都在3前面 
     *  a3:元素1小于3，翻转后，还是在 3前面。
     *  a4:元素1和元素2 和元素4关系。
     *   在4前面在1的后面。
     * 
     *  算法描述：顺序遍历
     *  a： 寻找小于x的节点pcur
     *  b： 删除pcur节点
     *  c： 插入pcur节点。
     *     从小于元素x之前位置，开始 后面追加插入
     *  复杂度：time o（n） space：o（1）
     *  
     *  输入：head = [4,3,2,5,2], x = 3
        输出：[2,2,4,3,5]
        拦路虎1：分割后链表还是原来的链表？不能构造2个链表在合并
        拦路虎2: 如何实现使得所有 小于 x 的节点都出现在 大于或等于 x 的节点之
         
         我马上想是比较大小吗？和插入排序一样 这个没问题，
         但是这里不行 【坑】
         head = [4,3,2,5,2], x = 3 加入没有1怎么办？
         2 <3 <4  
         肯定是在4的前面，head开始
        
       拦路虎3    两个分区中每个节点的初始相对位置 怎么理解？
                 输入：head = [1，4,3,2,5,2], x = 3
                 输出：[1，2,2,4,3,5]
        
        a1 注意和翻转链表区别： 1-2-3 vs 3-2-1  位置发现了改变
       
       a2: 在4前面和1的后面。
       1<3
       2<3  
       input:[1,4,3,0,2,5,2]
       output:[1,0,2,2,4,3,5]

        要点：不排序，不分割 主要和翻转链表区
     */
    ListNode* partition(ListNode* head, int x) {

        //01 定义数据结构 并且初始化 

        ListNode fixed(-1,head); //不用判断ppre是否为null
        ListNode* pcur = head;//这里不是排序，默认第一个节点
        ListNode* ppre = &fixed; //自然，前面一个节点 fixed-->head ，不用判断ppre null
        ListNode* ptail =&fixed;


        //链表原地翻转 不破坏原来结构

        while(pcur)
        {
          //easy case1 head->3->4->5
          if (pcur->val >= x)
          {
              ppre = pcur;
              pcur = pcur->next; 
              //bug3  等于x的也不需要移动
          }else if(pcur->val < x && ppre != ptail)
          { 
            //两个分区中每个节点的初始相对位置 这里假设直接从头节点不断追加
            //4位置不变，但是前面元素越来越多

            // p tail->4(ppre) --3(pcur) --2
              
            ppre->next = pcur->next ; //move 4——2

            pcur->next = ptail->next; // insert 3--4-2

            ptail->next = pcur; // 3 is ptail ->3 ->4->2

            //bug5  这个翻转链表区别，为了保证相对位置不变，是在后面追加的 而不是翻转
            //bug5  这个翻转链表区别，为了保证相对位置不变，是在后面追加的 而不是翻转
            //你写称 0 ，2 0 ，2,2 0
            ptail =ptail->next;

            //bug1 根本没移动 忘记了++操作 !!!
             pcur = ppre->next;  // pcur->2
      
          }else if (pcur->val < x && ppre == ptail)
          {
            //bug2 元素2 根本没有移动呢
            ptail = ptail->next;
            pcur = pcur->next;
            ppre = ppre->next;
            
          }
          
        }//
        //bug4
        //input:[1,4,3,0,2,5,2]
        //output:[1,0,2,2,4,3,5]

        //errout:[1,2,2,0,4,3,5] 相对位置 应该是0 2 2 

        return fixed.next;

    }
};
//总结：需求 分割链表 -->数据结构肯定是链表-->基本操作 删除和插入 ->顺序遍历删除小的元素，移动前面插入
//->std::copy (bar.begin(),bar.end(),insert_it); --> 小于1的后面，大4的前面
 //input:[1,4,3,0,2,5,2],x = 3
 //output:[1,0,2,2,4,3,5]
 
 //input [1，4,3,2,5,2], x = 3
//output:[1，2,2,4,3,5]

// @lc code=end

