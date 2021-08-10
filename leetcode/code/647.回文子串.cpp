/*
 * @lc app=leetcode.cn id=647 lang=cpp
 *
 * [647] 回文子串
 */

// @lc code=start
class Solution {
public:
    int countSubstrings(string s) {
      
      int count =0;
      int n = s.size();
      vector<vector<bool>> dp(n,vector<bool>(n,false)); //[start end]
      //0<=start <<end <N

      //01 遍历 n*(n+1)/2 子串
      for( int end =0; end <n;end++)
      {
          for (int start = 0; start <= end; start++)
          {   
              // 判断是否回文。
             if(s[start] == s[end])
             {   
                 //[A] [AA] [AAA]
                 if (end -start <=2)
                 {
                     dp[start][end] = true;
                 }else
                 {
                     dp[start][end] = dp[start + 1][end - 1];
                     //core
                 }
                 
                
                 //end-1 >= start+1  end-start >=2 where
             }else
             {
                //[a?b]
                dp[start][end] =false;
             }
             //03
             if (true == dp[start][end])
             {
                 count++;
             }
          }
          
      }

      return count;
    }
};
// @lc code=end

