#include<iostream>
#include<vector>
#include<string>
using namespace std;

//定义一个统计单词个数的前缀tree
//利用trie树，关键字域存该查询串出现的次
struct Node
{
  int count;
  Node* next[26];//代表26个字母位置
  Node()
   {
        count =1;
        for(int i=0;i<26;i++)
        {
            next[i] = nullptr;
        }
    }
};
class Trie
{
private:
    Node* m_root;
public:
    //创建固定头节点的tree,第一个元素不存储任何信息。类别 b++tree or b tree
    Trie()
    {
        m_root = new Node();
    }
    //插入一个单词，如果已经插入单词有前缀有重复，需要统计重复个数

    void insert(string word)
    {
        Node* pcur = m_root;
        for(auto c:word)
        {
            //01 判断该位置存在不存在
            if (pcur->next[c-'a'] == nullptr)
            {
                pcur->next[c-'a'] =new Node(); //第一次个数为1
            }else
            {
                pcur->next[c-'a']->count++;// 
                //aa aaa
            }
            //遍历移动到下一个位置
            pcur =  pcur->next[c-'a'];

        }
    }
    //寻找最小前缀 
    //aac aab
    //a 2 a 2 c 1 b 1
    string searchPre(string word)
    {
         Node* pcur = m_root;
         Node* pnext =nullptr;
         for(int i=0;i<word.size();i++)
         {
             char cur =word[i];
             //单词长度就是tree的深度
             pnext = pcur->next[cur-'a'];
             if (pnext && pnext->count ==1)
             {   
                 return word.substr(0,i+1);
             }
             //前缀tree的遍历
             pcur = pnext;
         }
         return word;
    }
};
int main()
{   

    //01 test case
    int n =0;
    cin>>n;
    vector<string> input;
    for(int i=0;i<n;i++)
    {
       string temp;
       cin>>temp;
       input.push_back(temp);
    }

    //02 create  tree 
    Trie tr;
    for(int i=0;i<input.size();i++)
    {
        tr.insert(input[i]);
    }

    //03 visit

     for(int i=0;i<input.size();i++)
    {
       cout<<tr.searchPre(input[i])<<endl;
    }

    return 0;
}