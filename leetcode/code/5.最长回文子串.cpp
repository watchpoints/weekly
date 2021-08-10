/*
 * @lc app=leetcode.cn id=5 lang=cpp
 *
 * [5] 最长回文子串
 */

// @lc code=start
class Solution {
public:
    string longestPalindrome(string s) {
       //思路： 最长的回文子串 -->是回文--->[start,end] end>=start

       //细节：f(i) =
       // if s[start] ==s[end]
       //dp[start][end] =dp[start+1][end-1]; end-1 >=start+1 
        
        // if (end-start <=2) dp[start][end] = true

        int n =s.size();
        int lstart =0;
        int lend =0;
       vector<vector<bool>>  dp(n,vector<bool>(n,false));//
       
       //dp[start][end] = true  Palindrome end >=start >=0
       //dp[start][end] =dp[start+1][end-1]; Palindrome end-1 >=start+1  

       //01 n(n-1)/2 遍历无法优化
       for (int end = 0; end < n; end++)
       {
           for(int start =0;start <=end;start++)
           {
              //02 here her her 
              
              if (s[start] == s[end])
              {   //[a] [aa] 
                  if (end -start <2)
                  {
                       dp[start][end] =true;
                  }else
                  {
                      dp[start][end]= dp[start+1][end-1];
                  } //[aba] end-1>=start+1
                  
              }

              if(dp[start][end] ==true && end -start >lend -lstart)
              {
                  lend =end;
                  lstart =start;
              }
           }
       }

       return s.substr(lstart,lend-lstart+1);
    }
};

// @lc code=end

