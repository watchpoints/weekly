/*
 * @lc app=leetcode.cn id=234 lang=cpp
 *
 * [234] 回文链表
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
    bool isPalindrome(ListNode *head)
    {

        //01 check
        if (nullptr == head || nullptr == head->next)
        {
            return true;
        }

        //02 define three node

        ListNode *phead = head;

        ListNode *pslow = head;
        ListNode *pfast = head;

        ListNode *ptemp = nullptr;

        while (pfast && pfast->next)
        { //细节1  pfast->next 判断
            ptemp = pslow;
            pslow = pslow->next;
            pfast = pfast->next->next;
        }

        ptemp->next = nullptr; //变成2个链表

        //03  reverse of mid list
        phead = nullptr;

        pfast = pslow;
        ptemp = nullptr;

        while (pfast)
        {
            //phead->1(pfast)->2(ptemp)-3()
            //细节：这里不需要固定头节点，但是依然需要头节点。
            //phead 代替头节点phead =&dump。
            //防止别人看懂懂系列 认为内存泄漏什么的

            ptemp = pfast->next;

            pfast->next = phead;
            phead = pfast;

            pfast = ptemp;
        }

        //02 compare
        pslow = head;
        pfast = phead;

        while (pslow && pfast)
        {
            if (pslow->val != pfast->val)
            {
                return false;
            }

            pslow = pslow->next;
            pfast = pfast->next;
        }
        //细节：相等的去判断
        //虽然  pslow 和 pfast有可能不相等 【1 2】 【3 2 1 】【1 2 3】

        return true;
    }
};
// @lc code=end
