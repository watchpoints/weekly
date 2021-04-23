/*
 * @lc app=leetcode.cn id=148 lang=cpp
 *
 * [148] 排序链表
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
 * https://leetcode-cn.com/problems/insertion-sort-list/solution/wei-tu-jie-147dui-lian-biao-jin-xing-cha-ru-pai-xu/
 */
class Solution {
public:
    //我熟悉链表插入操作，因此采用插入排序，需要三指针保证链表完整性。
    //对链表进行原地翻转,不需要额外空间
    //疑问：为什么采用原理翻转，而不是分割不同链表,在合并？
    //前者每次操作后整个链表是完整的，
    //后者必须完整遍历后获取结果，第i次循环，链表不是完整的。

    ListNode* sortList1(ListNode* head)
    {

        if (head == NULL || head->next == NULL) {
            return head;
        }
        ListNode dummyHead(-1, head); //固定头节点，保持头节点位置保持不变。

        ListNode* pbefore = &dummyHead; //单链表插入，必须知道前面一个节点。不然无法完成插入操作，删除可以

        ListNode* ptail = head; //假设第一个节点已经排序完毕。
        ListNode* pcur = head->next; //当前节点 ，这是未知元素。

        while (pcur) {
            if (pcur->val >= ptail->val) {
                //case01-如果递增数据
                ptail = pcur; //有序链表范围扩大。
                pcur = pcur->next;

            } else {
                //case02 当前元素在有序链表中位置

                pbefore = &dummyHead; // 寻找pur前面一个元素位置
                while (pcur->val > pbefore->next->val) {
                    pbefore = pbefore->next;
                }

                //节点翻转
                //pbefore   ptail   pcur
                //|        |     |
                //|        |     |
                //head--- 5 --  4 --- 3 --2 ---1

                //01 删除pcur元素 4
                ptail->next = pcur->next; //5————>3

                //02 为了保证链表不被破坏 追究一元素 /4--->5
                pcur->next = pbefore->next;

                //03 为了保证链表不被破坏 追究一元素 // tail->4
                pbefore->next = pcur;

                pcur = ptail->next; //04 移动操作。
            }
        }

        return dummyHead.next;
    }
    /***
     * 遍历链表
     * 1 如果是升序（偶数）继续遍历
     * 2.如果是降序， 需要翻转。
     *  翻转需要3个复制指针
     */
    ListNode* sortList(ListNode* head)
    {
        if (nullptr == head || nullptr == head->next) {
            return head;
        }

        //01 three pointer
        ListNode myhead(-1);
        myhead.next = head; //假设一个节点是有序的

        ListNode* ptail = nullptr; // 这个题目特点：插入不是固定头节点，也不是固定位节点，而是需要判断寻找位置
        ListNode* pcur = head->next;
        ListNode* ppre = head; //这里相当于排序节点最后一个节点

        while (pcur) {

            //重点，遍历有2个逻辑
            if (pcur->val > ppre->val) {
                //升序
                ppre = pcur;
                pcur = pcur->next;
            } else {
                // 在链表那个位置插入 这里需要判断，寻找之后再插入
            }
        }

        return myhead.next;
    }
};
// @lc code=end
