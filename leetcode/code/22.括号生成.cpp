/*
 * @lc app=leetcode.cn id=22 lang=cpp
 *
 * [22] 括号生成
 */

// @lc code=start
//https://leetcode-cn.com/problems/generate-parentheses/solution/hui-su-suan-fa-by-liweiwei1419/
class Solution {

vector<string> generateParenthesis(int n) {
     //length:m=2*n ,每个m 有2个选择。指数级别
     //有效括号：i=3时候
     //())? left:1 right 2  not math
     //(()? left:2 right 1  may math
      
      vector<string> output;
      if( 0 == n)
      {
          return output;
      }

      queue<Node> myqueue; 
      //利用队列实现tree的广度优先遍历。
      //这里tree 根本不存在，tree 高度是 2*n
    
       myqueue.push(Node(0,0,"")); //new 
       
       while(!myqueue.empty())
       {
           Node temp =myqueue.top();
           myqueue.pop();

           //判断是否结束

           if(temp.left ==n && temp.right ===n)
           {
               output.push_back(temp.m_path);
           }
           //只要大超过n就满足条件
           if(temp.left <n)
           {
                 myqueue.push(Node(temp.m_left+1,temp.m_right,temp.m_path.push_back('(')));
           }
            //1 大超过n就满足条件
            //2  right 不能超过left
            if(temp.right <n && temp.left >=temp.right)
           {
                myqueue.push(Node(temp.m_left,temp.m_right+1,temp.m_path.push_back(')')));
           }

       }
      return output;
    }

public:
    vector<string> generateParenthesis2(int n) {
     //length:m=2*n ,m(m+1)/2
     //有效括号：i=3时候
     //())? left:1 right 2  not math
     //(()? left:2 right 1  may math

     vector<string> output;
     string path;
     dfs(path,0,0,n,output);
     return output;
    }

    void dfs(string &path,int left,int right ,int n,vector<string> &output)
    {
        //condition 

        if(left ==n && right ==n )
        {
            output.push_back(path);
        }

        //condition
        if(right >left)
        {
            return ;
        }

        //condition
        if (left <n)
        {
            path.push_back('(');
            dfs(path,left+1,right,n,output);
            path.pop_back();
        }

        //condition
        if (right <n)
        {
            path.push_back(')');
            dfs(path,left,right+1,n,output);
            path.pop_back();
        }

    }
     
public：
struct Node
{
    int m_left ;
    int m_right ;
    string m_path;
    Node(int left,int right,string path)
    {
        m_left =left;
        m_right =right;
        m_path =path;
    }
};

 

};




class Solution {
public:
    vector<string> generateParenthesis(int n) {

        vector<string> output;
        string path;//递归回溯输出
        dfs(path,0,0,output,n);
       return output;
    }

    void dfs(string &path,int left,int right,vector<string> &output,int n)
    {
        if(left == n && right ==n)
        {
            output.push_back(path); //走到这里就是成功的
        }

        if(left <n)
        {
            path.push_back('(');
            dfs(path,left+1,right,output,n);
            path.pop_back();
        }

        if(right <n && left >right)
        {
            path.push_back(')'); //(?
            dfs(path,left,right+1,output,n);
            path.pop_back();
        }
    }
};
// @lc code=end

