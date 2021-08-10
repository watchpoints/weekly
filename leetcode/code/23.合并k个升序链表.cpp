/*
 * @lc app=leetcode.cn id=23 lang=cpp
 *
 * [23] 合并K个升序链表
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
class Solution
{
public:
    /**
     *   //01 定义数据结构
     *     //02 初始化数组链表
     * */

    ListNode *mergeKLists(vector<ListNode *> &lists)
    {
   
        auto comp = [&](ListNode *a, ListNode *b) {
            return a->val > b->val;
        };

      priority_queue<ListNode*, vector<ListNode *>, decltype(comp)> pq(comp);
       
      ListNode head;//排序后链表固定头节点 
      ListNode* ptail =&head;//

      for(ListNode* list:lists)
        {  
            if(list)
            {
                pq.push(list);
            }
        }

       //03 循环迭代
        while(!pq.empty())
        {
            ListNode* pcur = pq.top();
            pq.pop();

            if (pcur->next)
            {
                pq.push(pcur->next); 
            }//该链表遍历完毕 邻接节点

            //04 链表tail插入操作
             ptail->next = pcur;
             ptail =pcur;
        }
       return head.next;
    }
};
// @lc code=end
