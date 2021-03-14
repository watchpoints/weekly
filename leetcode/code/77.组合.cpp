/*
 * @lc app=leetcode.cn id=77 lang=cpp
 *
 * [77] 组合
 * //https://leetcode-cn.com/problems/combinations/solution/hui-su-suan-fa-jian-zhi-python-dai-ma-java-dai-ma-/

*  你一个题目还做，就说后面100个题目没意思，太简单了
   这个感觉不正缺的，你必须把100个题目全部都做一遍，
  你做一遍了吗？没有。

 */


// @lc code=start
class Solution {
public:
     //思路：组合是一个数学公式---tree结构表示---递归回溯遍历
    vector<vector<int>> combine(int n, int k) {
       
      //01定义vector，tree 数据结构
      vector<vector<int>>  result;
      vector<int> path;//组合就是路径
      //02 递归回溯遍历方法。
      dfs(1,k,n,path,result);
      return result;
     
    }

    void dfs(int start,int k,int n,vector<int>& path,vector<vector<int>>& result)
    {    
        if( path.size() == k)
        {  
           result.push_back(path);
           return ;//end .只保存k个记录
        }

        for( int i =start;i <= n;i++)
        {
           path.push_back(i);
           dfs(i+1,k,n,path,result);//顺序处理
           path.pop_back();
        }
    }
};
// @lc code=end

