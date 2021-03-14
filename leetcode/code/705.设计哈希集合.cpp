/*
 * @lc app=leetcode.cn id=705 lang=cpp
 *
 * [705] 设计哈希集合
 */
// @lc code=start
//ListNode 利用之前定义结构
class MyHashSet {
public:
//细节1： 存储什么key 和value 这里为了简化问题 假设是int类型.不然就是个模版
//细节2:MyHashSet 存储的int类型，为和内部设计了ListNode，还是链表结构，这个思路转换 想不到。为了冲突。
//：key一样会冲突，key不一样 index一样同样会冲突。 MyHashSet是上层封装。这是才是这个题目设计本意。
private:
    vector<ListNode* > hashtable;//细节2:用vector模型hash。
    //和普通数组相比，add 元素冲突 普通做法直接替换。hash不能直接替换（相同和冲突区分）

public:
    /** Initialize your data structure here. */
    MyHashSet() {
        hashtable.resize(1024,nullptr);
        //细节3:当我思考扩容时候，这说明我理解是初级水平，我是靠记忆redis和vector特性。
        //1024不是最大长度。这里简化问题 没有负载因子。key%1024，可以放任何记录。链表没有长度限制。
        //如果你构造函数让用户传递大小。这说明我理解是错误的。在这里马上被面试官给pass。gameover。

        //细节4:这里没有采用固定头节点方式，这样写代码有点麻烦。因为采取有序链表存储。

    }
    
    void add(int key) {

      int index = key%hashtable.size(); 
      //细节5:这里hash计算采取最简单方式，只输入int类型，其他类型我写不出来。考虑半天我这个题目超时完不成
      //后面阅读其他代码参考
      ListNode * phead = hashtable[index];
      ListNode* ptemp =new ListNode(key,nullptr);

      if (phead == nullptr)
      {
         hashtable[index] = ptemp;
         //细节5:reference operator[]( size_type pos ); 返回是引用类型。可以对下标直接修改。
         
         return ;// null
      }
      
     //细节6:有有序链表存储，这里为什么o（1）插入，因为还要判断是否重复
     //这里构造的有序链表，你可以升级到bst更好的结构。

      ListNode * ppre =nullptr;

      while(phead)
      {
         if(phead->val == key)
         {
              return ;//重复
         }
         //寻找合适插入位置
        if (key > phead->val)
        {
            //继续操作
              ppre = phead;
              phead = phead->next;
        }else
        {
            break; //ppre 后面就是插入位置。
        }
     

      }
      //【2】，插入元素1
      if(ppre ==nullptr)
      {
          //说明什么，这里没有固定头节点，需要在前面 插入一个元素.
          //发现还是redis代码比较好。虽然不是最佳的
           ptemp->next = hashtable[index];
           hashtable[index] =ptemp;

      }else
      {
         ptemp->next =ppre->next;
         ppre->next =ptemp;


      }
      
    }
    //删除操作：o(n)，细节：这里不仅仅遍历一次，还是记住前面一个位置。更重要前面位置初始化nul，还是假存在
    void remove(int key) {
         int index = key%hashtable.size();
          //初始化：
         ListNode * ppre = nullptr;
         ListNode * phead = hashtable[index];
        
         //该位置没有元素
         if (nullptr == phead) return ;
         //细节：删除该元素，必须知道前面一个节点
         while(phead)
         {    
              //查找到该元素
              if (phead->val == key)
              {  
                //细节7：假如删除元素就是第一个元素怎么办？
                if(ppre == nullptr)
                {    
                     //delete xxx
                     hashtable[index] =hashtable[index] ->next;    
                }else
                {
                    //1 --2--->3
                    ppre->next = phead->next;
                }
                return ;
              }
              //前进一步
              ppre = phead;
              phead = phead->next;
         }

    }
    
    /** Returns true if this set contains the specified element */
    //https://redisbook.readthedocs.io/en/latest/datatype/sorted_set.html
    //https://redisbook.readthedocs.io/en/latest/datatype/set.html
    //o(n)
    bool contains(int key) {
        int index = key%hashtable.size();
         ListNode * phead = hashtable[index];
         while(phead)
         {
            if(phead->val == key)
            {
                return true;
            }
            phead =phead->next;
         } 

         return false;
    }
};


/**
 * Your MyHashSet object will be instantiated and called as such:
 * MyHashSet* obj = new MyHashSet();
 * obj->add(key);
 * obj->remove(key);
 * bool param_3 = obj->contains(key);
 */
// @lc code=end

