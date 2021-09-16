/*
 * @lc app=leetcode.cn id=22 lang=cpp
 *
 * [22] 括号生成
 */

// @lc code=start
//https://leetcode-cn.com/problems/generate-parentheses/solution/hui-su-suan-fa-by-liweiwei1419/
class Solution {
public:
    vector<string> generateParenthesis1(int n) {
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
     //广度优先搜索。
     vector<string> generateParenthesis1(int n) {
     }
};
// @lc code=end

