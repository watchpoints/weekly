/*
 * @lc app=leetcode.cn id=208 lang=cpp
 *
 * [208] 实现 Trie (前缀树)
 */

// @lc code=start

//自习室 无声音c++版本 --实现 Trie (前缀树) 

//遗留问题，假如存在重复元素怎办 aaa aabb aaacb
//01 定义存储结构
struct Node
{
    int isKey; //统计个数,重复插入记录个数
    Node* next[26];//指针数组，当作hash<char,vecotr<Node*>>

    Node()
    {
        isKey =0;
        for(int i=0;i<26;i++)
        {
            next[i] = nullptr;
        }
    }
};
class Trie {

private:
//02 Trie 是抽象，Node才是具体实现。这里用array,你也可以hash
  Node* root;

public:
    /** Initialize your data structure here. */
    Trie() {
        root = new Node();
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
     
      //上个node判断是否存在 ，下一个node 才是数据,类比B+ tree

      Node * pcur = root;  //固定头节点
      //类别 tree 深度优先搜索遍历.这里只选择一个，不需要递归回溯
      for(auto c:word)
      ////这里有一个问题 元素不能重复 假如重复怎么办 aaaaaaaaaa,我不知道该如何解决。需要在这里判断一下。
      {
          if ( nullptr == pcur->next[c-'a'])
          {
              pcur->next[c-'a'] = new Node(); 
          }
          pcur =  pcur->next[c-'a'];
      }
      pcur->isKey +=1;
      //应该变为a 16个。

    }
    
    /** Returns if the word is in the trie. */
    bool ç(string word) {
         Node * pcur = root;
         for(auto c:word)
         {
             pcur =  pcur->next[c-'a'];
             if (nullptr == pcur)
             {
                 return false;
             }
         }

        return pcur->isKey >0;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    //如果之前已经插入的字符串 word 的前缀之一为 prefix ，返回 true ；
    //否则，返回 false 。
    bool startsWith(string prefix) {
     
        Node * pcur = root;
         for(auto c:prefix)
         {
             pcur =  pcur->next[c-'a'];
             if (nullptr == pcur)
             {
                 return false;
             }
         }

        return true;
       
    }
};


