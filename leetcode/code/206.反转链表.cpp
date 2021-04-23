/*
 * @lc app=leetcode.cn id=206 lang=cpp
 *
 * [206] 反转链表
 *
 * https://leetcode-cn.com/problems/reverse-linked-list/description/
 *
 * algorithms
 * Easy (71.57%)
 * Likes:    1700
 * Dislikes: 0
 * Total Accepted:    523.9K
 * Total Submissions: 732.1K
 * Testcase Example:  '[1,2,3,4,5]'
 *
 * 反转一个单链表。
 * 
 * 示例:
 * 
 * 输入: 1->2->3->4->5->NULL
 * 输出: 5->4->3->2->1->NULL
 * 
 * 进阶:
 * 你可以迭代或递归地反转链表。你能否用两种方法解决这道题？
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
    /**
     *  借助三个辅助指针完成单链表翻转
     *  注意：这里是原地翻转，不需要建立新的节点。
     *   翻转后还是自己只不过head位置每次都发送变化，因此借助固定头节点方式。减少代码复杂度
     */
    ListNode* reverseList(ListNode* head)
    {
        if (nullptr == head || nullptr == head->next) {
            return head;
        }
        // ！！！！！！
        ListNode pHead(-1); //固定头节点插入
        pHead.next = head;
        //隐藏机制：第一个节点变成最后一个节点，元素位置并没有发送改变，因为单链表操作
        //因此我可以放心的默认第一个节点是已经排序号的。

        ListNode* pcur = head->next; //从第二个位置开始。
        ListNode* ppre = head;
        // 1->2(pcur)->3->4->5 -null
        while (pcur) {
            // right -->left
            ppre->next = pcur->next; //删除
            pcur->next = pHead.next;
            pHead.next = pcur;

            pcur = ppre->next;
        }

        return pHead.next;
    }
};
// @lc code=end
