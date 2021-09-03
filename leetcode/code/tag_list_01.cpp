
#include <iostream>
#include <vector>
#include "list_odd.h"
#include "base.h"
using namespace std;
class Solution1 {
public:
    ListNode* reverseList(ListNode* head) {
     //01 模型 单链表 原地翻转。
     if (nullptr == head || nullptr == head->next) 
     {
         return head;//隐含条件：第一个元素假设就是翻转好。
     }

     //02 辅助空间：三路指针，帮助完成原地翻转
     
     ListNode listhead(-1);listhead.next =head; 
     ListNode* ppre = head;   
     ListNode* pcur = head->next;
    
    //03 步骤
     while(pcur)
     {
         //基本操作：删除节点2 
         //1-2-3 --> 1 -3
         ppre->next = pcur->next;
         //基本操作：插入节点2
         //2->1->3
         pcur->next = listhead.next;
         //head->2-1-3
         listhead.next =pcur;

         //迭代移动

         pcur = ppre->next;
     }

     return listhead.next;// 固定头结点插入。


    }
};

ListNode* createList( vector<int>& input)
{
    ListNode head;
    ListNode* ptail=&head;

    for(int i =0;i<input.size();i++)
    {
        
        ListNode * ptemp  =new ListNode(input[i]);
        ptail->next =ptemp;
        ptail =ptemp;
    }
    return head.next;

}

void show(ListNode* phead )
{
    while(phead)
    {
      cout<< phead->val << " ";
      phead = phead->next;
    }
    cout <<endl;

}

//86. 分隔链表
class Solution3 {
public:
    /**
     * 
     * @param head ListNode类 
     * @param x int整型 
     * @return ListNode类
     */
    ListNode* partition(ListNode* phead, int x) {
        // write code here
        
        //01 
        if (nullptr == phead || nullptr == phead->next)
        {
            return phead;
        }

        //02 返回结果 如果head小于k 返回结果就是head，大于则不是，不确定因此 why
        ListNode myhead(-1);
        myhead.next = phead;
        ListNode* ptail = &myhead;

       //03 
       ListNode* pcur =phead; // 第一个节点大小不确定，无法默认就是分割好的（反转，奇偶 插入排序）
       ListNode* ppre =&myhead;

       while(pcur)
       {
         
         if(pcur->val >= x)
         {   ////case 1 移动
             ppre = pcur ;
             pcur = pcur->next;//i++
         }else if (ptail->next == pcur)
         {  //bug1  ptail->next = pcur
             //case 2移动
             ptail = ptail->next;//move
             ppre = pcur ;
             pcur = pcur->next;//i++

         }else
         {
            //case3 反转
            ppre->next = pcur->next;
            pcur->next =ptail->next;
            ptail->next = pcur;
            ptail = pcur;//move

            pcur = ppre->next;


         }
         
       }
        return myhead.next;
    }
        
};

void show(vector<int>& input)
{  
  for(int i =0;i<input.size();i++)
  {
      cout<< input[i] << " ";
  }
  cout <<endl;
}

void test10()
{    
    vector<int> test={1,2,3,4,5};
    vector<int> right={1,3,5,2,4};

    ListNode* pInput= createList(test);
    cout<< "input: " <<endl;
    show(pInput);
    cout<< "right output: "  <<endl;
    show(right);

    Solution10 s10;
    ListNode* pOnput=s10.oddEvenList(pInput);
    cout<< "my output: " <<endl;
    show(pOnput);
}

//分割链表
void test12()
{
    //vector<int> test={1,2,3,4,5};
    vector<int> test={5,4,3,2,1};
    vector<int> right={2,1,5,4,3};

    ListNode* pInput= createList(test);
    cout<< "input: " <<endl;
    show(pInput);
    cout<< "right output: "  <<endl;
    show(right);

    Solution3 s3;
    ListNode* pOnput=s3.partition(pInput,3);
    cout<< "my outut: " <<endl;
    show(pOnput);
}


//g++ -std=c++11 tag_list_01.cpp
int main()
{   
    test10();
    return 0; 
}


