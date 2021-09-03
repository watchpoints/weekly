
#include <iostream>
#include <vector>
#include "base.h"
#include "list_split.h"
using namespace std;
//如果插入一个链表
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


void show(vector<int>& input)
{  
  for(int i =0;i<input.size();i++)
  {
      cout<< input[i] << " ";
  }
  cout <<endl;
}



//分割链表
void test86()
{
    vector<int> test={1,2,3,4,5};
    vector<int> right={1,2,3,4,5};

    //vector<int> test={5,4,3,2,1};
    //vector<int> right={2,1,5,4,3};

    ListNode* pInput= createList(test);
    cout<< "input: " <<endl;
    show(pInput);
    cout<< "right output: "  <<endl;
    show(right);

    Solution86 s3;
    ListNode* pOnput=s3.partition(pInput,3);
    cout<< "my outut: " <<endl;
    show(pOnput);
}


//g++ -std=c++11 tag_list_01.cpp
//bug2： 忘记-  std=c++11
//bug3: g++ tag_list_01.cpp
int main()
{   
    test86();
    return 0; 
}


