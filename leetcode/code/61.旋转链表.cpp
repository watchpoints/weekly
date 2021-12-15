/*
 * @lc app=leetcode.cn id=61 lang=cpp
 *
 * [61] 旋转链表
 *
 * https://leetcode-cn.com/problems/rotate-list/description/
 *
 * algorithms
 * Medium (41.76%)
 * Likes:    681
 * Dislikes: 0
 * Total Accepted:    211.4K
 * Total Submissions: 506.3K
 * Testcase Example:  '[1,2,3,4,5]\n2'
 *
 * 给你一个链表的头节点 head ，旋转链表，将链表每个节点向右移动 k 个位置。
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：head = [1,2,3,4,5], k = 2
 * 输出：[4,5,1,2,3]
 * 
 * 
 * 示例 2：
 * 
 * 
 * 输入：head = [0,1,2], k = 4
 * 输出：[2,0,1]
 * 
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * 链表中节点的数目在范围 [0, 500] 内
 * -100 
 * 0 
 * 
 * 
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
   //小白视角分析
    // 4,5 ---怎么右移 从最后面变成最前面位置。 1,2,3  怎么右移
    // swap 还是移动 ？

    //时间复杂度：O(n)，最坏情况下，我们需要遍历该链表两次 
    //链表 不需要swap 
    
    //初级视角分析：
    //右移动 k 个位置 -->倒数第k个

    //中级视角分析：链表 环形队列的引用是
    ListNode* rotateRight(ListNode* head, int k) {
        
        if (NULL == head || NULL == head->next){
            return head;
        } 

        //构造一个环形链表 
        ListNode* ptemp = head;
        int size = 1;//最后一个不计算长度
        while (ptemp && ptemp->next )
        {
            ptemp = ptemp->next;
            size ++;
        } // ptemp ==last

        ptemp->next = head;//loop

        k = k % size;

        // 倒数第k个 前面一个位置。单链表特点  之前做法 双指针 

        while(  -- size >=k)
        {
             ptemp = ptemp->next;
        }

        ListNode* myhead = ptemp->next;
        ptemp->next = NULL;

        return myhead;
    }
};
// @lc code=end

//https://leetcode.com/problems/split-linked-list-in-parts/
