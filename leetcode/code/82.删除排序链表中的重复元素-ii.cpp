/*
 * @lc app=leetcode.cn id=82 lang=cpp
 *
 * [82] 删除排序链表中的重复元素 II
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
 *    输入：head = [1,2,3,3,4,4,5]
      输出：[1,2,5]
 * };
 */
//[1,1]  []
class Solution {
public:
    //难点：【1 ,1 ,1】 全部删除 最后null 怎么处理。
    ListNode* deleteDuplicates(ListNode* head) {
        
        if (nullptr ==head || nullptr ==head->next)
        {
           return head;
        }
        ListNode  myhead;
        myhead.next = head; //细节1 why myhead ? 第一个元素 可能重复 ，也可能不重复不清楚.需要固定不变头节点。

        ListNode* pcur =head;
        ListNode* ppre = &myhead;
        // 细节2 为什么   while (pcur && pcur->next) 而不是 while (pcur)?
        // 你会问最后一个元素能访问吗？不处理.
        //这里思路 不一样，一般遍历值考虑当前元素。
        while (pcur && pcur->next)
        {
            if (pcur->val != pcur->next->val) // 根本不需要担心core问题 
            {
                ppre = pcur;
                pcur = pcur->next;
            }else
            {
                // 细节3 这里必须while 处理掉全部重复元素。? 

                //[1 1]
                while (pcur && ppre->next->val == pcur->val)
                {
                    pcur = pcur->next;
                    //free
                }
                // 细节4  while (pcur && ppre->next->val == pcur->val) 为？pcur
                ppre->next = pcur;
                //  //[1 1] pcur =null
            }
        }

        return myhead.next;
    }
};
// @lc code=end

