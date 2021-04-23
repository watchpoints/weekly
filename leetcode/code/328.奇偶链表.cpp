/*
 * @lc app=leetcode.cn id=328 lang=cpp
 *
 * [328] 奇偶链表
 *
 * https://leetcode-cn.com/problems/odd-even-linked-list/description/
 *
 * algorithms
 * Medium (65.63%)
 * Likes:    413
 * Dislikes: 0
 * Total Accepted:    105.2K
 * Total Submissions: 160.4K
 * Testcase Example:  '[1,2,3,4,5]'
 *
 * 给定一个单链表，把所有的奇数节点和偶数节点分别排在一起。请注意，这里的奇数节点和偶数节点指的是节点编号的奇偶性，而不是节点的值的奇偶性。
 * 
 * 请尝试使用原地算法完成。你的算法的空间复杂度应为 O(1)，时间复杂度应为 O(nodes)，nodes 为节点总数。
 * 
 * 示例 1:
 * 
 * 输入: 1->2->3->4->5->NULL
 * 输出: 1->3->5->2->4->NULL
 * 
 * 
 * 示例 2:
 * 
 * 输入: 2->1->3->5->6->4->7->NULL 
 * 输出: 2->3->6->7->1->5->4->NULL
 * 
 * 说明:
 * 
 * 
 * 应当保持奇数节点和偶数节点的相对顺序。
 * 链表的第一个节点视为奇数节点，第二个节点视为偶数节点，以此类推。
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
    /**
     * 观察特点：翻转奇数后，剩下的就是偶数 
     * 01 模型：
     *   原地翻转单链表，
     *   单链表特点______，
     *   因此需要三路指针
     * 02: 删除节点：删除节点编号
     *   对偶数节点采取策略是：遍历变量。
     *  
     * 03 插入节点： 在链表位节点插入
     *     1->3->5
     *    应当保持奇数节点和偶数节点的相对顺序 
     *   
     */
    ListNode* oddEvenList(ListNode* head)
    {
        if (nullptr == head || nullptr == head->next)
            return head;

        //定义辅助空间，并且初始化
        ListNode* ptail = head; //1:假设ptail有序节点最后一个元素。
        ListNode* pcur = head->next; //2
        ListNode* ppre = head; //1

        return head;
    }
};
// @lc code=end
