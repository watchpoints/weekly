#include "base.h"
class Solution1
{
public:
    ListNode *partition(ListNode *head, int x)
    {
        // 01 check
        if (nullptr == head || nullptr == head->next)
        {
            return head; // 第一个节点 一定不需要移动
        }

        // 02
        //所有 小于 x 的节点都出现在 大于或等于 x 的节点之前
        //[已经x在前面]
        //[没有在x的前面]
        //【5，4，3，2，1】 k=3
        //【1,2,3,4,5 】 k=3;
        //不需要 保留 每个分区中各节点的初始相对位置
        ListNode *phead = head;
        ListNode *ptail = head;

        ListNode *pcur = head; //第一个节点不需要翻转
        ListNode *ppre = nullptr;

        // 03 3个情况
        while (pcur)
        {
            if (pcur->val >= x)
            {
                // case1 大于保持不变
                ppre = pcur;
                pcur = pcur->next;
            }
            else if (ptail == pcur)
            {

                ppre = pcur;
                pcur = pcur->next;
                ptail = pcur;
            }
            else
            {
                // case2 小于，并且 [已经x后面]
                //【head->5，4，3(ppre)，2(pcur)，1】 k=3
                ppre->next = pcur->next;
                pcur->next = phead;
                phead = pcur;

                // ppre 不变
                pcur = ppre->next;
            }
        }

        return phead;
    }

    ListNode *partition2(ListNode *head, int x)
    {
        // 01 check
        if (nullptr == head || nullptr == head->next)
            return head;

        // case1 [5,4,3,2,1] x=3;

        //输入：head = [1,4,3,2,5,2], x = 3
        //输出：[1,2,2,4,3,5]

        // 02 相对顺序不变
        ListNode myhead;
        myhead.next = head;

        ListNode *ptail = &myhead;
        ListNode *pcur = head;
        ListNode *ppre = ptail;

        // 03 for

        while (pcur)
        {
            if (pcur->val >= x)
            {
                ppre = pcur;
                pcur = pcur->next;
            }
            else if (ptail->next == pcur)
            {

                ppre = pcur;
                pcur = pcur->next;
                ptail = pcur;
            }
            else
            {
                ppre->next == pcur->next;
                pcur->next = ptail->next;
                ptail->next =pcur;
                ptail = pcur;

                pcur = ppre->next;
            }
        }

        return myhead.next;
    }
};

class Solution2
{
public:
    ListNode *partition(ListNode *head, int x)
    {
        if (nullptr == head || nullptr == head->next)
            return head;

        ListNode *phead = head;
        ListNode *ptail = head;

        ListNode *pcur = head;
        ListNode *ppre = nullptr;

        while (pcur)
        {
            if (pcur->val >= x)
            {
                // vector<int> input2 = {5, 4, 3, 2, 1};
                // int k2 = 1;
                ppre = pcur;
                pcur = pcur->next;
            }
            else if (ptail == pcur)
            {
                ppre = pcur;
                pcur = pcur->next;
                ptail = pcur;
                //
            }
            else
            {
                ppre->next = pcur->next;
                pcur->next = phead; //
                phead = pcur;

                pcur = ppre->next;
                // ppre stay
                // ptail stay
            }
        }

        return phead;
    }
};

int main()
{
    // case1
    vector<int> input1 = {1, 2, 3, 4, 5};
    int k1 = 5;

    // case2
    vector<int> input2 = {5, 4, 3, 2, 1};
    int k2 = 1;

    // case3
    vector<int> input3 = {2, 1};
    int k3 = 2;

    return 0;
}