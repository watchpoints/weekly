/*
 * @lc app=leetcode.cn id=5 lang=cpp
 *
 * [5] 最长回文子串
 */

// @lc code=start
<<<<<<< HEAD

class Solution
{
public:
    string longestPalindrome(string s)
    {   
        //思路： 遍历n*(n+1)/2 子串
        int startIndex = 0; //longestPalindrome
        int endIndex = 0;   //longestPalindrome

        //思路： 减少回文判断 
        int n =s.size();
        vector<vector<bool>> dp(n,vector<bool>(n,false));
        //细节2: 只有清楚db表示什么含义，才理解状态转移方程
        //dp[start][end] 表示：子串 s[start..end] 是否为回文子串
        //【start,end】  0<=start<=end<N
       
        for(int end =0;end < n;end++)
        {
            for( int start =0; start <= end;start++)
            {
                
                if ( s[start] ==s[end])
                {   //【a】 【a a】 【aba】
                    if (end - start == 0 || end - start == 1 || end - start == 3)
                    {   //[b b]
                        dp[start][end] = true; //stat =0,end=0 |stat =1,end=1
                    }else
                    {
                        dp[start][end] = dp[start + 1][end - 1];
                        //细节3：第一层该如何处理
                        // end =0 end -1 =-1 coredump

                        //dp[0][1] -->dp[1][0] BEGIN >END
                    }
                   
                   
                }else
                {
                    dp[start][end] = false; // //case1 [ba] case 2 [bca]
                }

                if (dp[start][end] == true && (end - start > endIndex - startIndex) )
                {
                    startIndex = start;
                    endIndex = end;
                }


            }
        }
        return s.substr(startIndex, endIndex - startIndex + 1);
    }
};

















class Solution1 {
public:
    string longestPalindrome(string s) {
        //暴力判断 寻找每个子串
        //细节1：
        //只有清楚知道2次for循环执行次数。这样你才可以准确说出时间复杂度。
        //执行次数：
        //等差数列
        //i=0,n i=1 n-1 i =2 n-2...
        //n +n-1+n-2+2+1 = (n+1)(n)/2
        //[a b c] sn=6 { a ab abc b bc c}
        int n =s.size();
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        //细节2: 只有清楚db表示什么含义，才理解状态转移方程
        //dp[start][end] 表示：子串 s[start..end] 是否为回文子串
        //【start,end】  0<=start<=end<N

        int i = 0; //longestPalindrome 开始位置
        int j = 0; //longestPalindrome 开始位置
        for (int  end = 0; end < n; end++)
        {
            for (int start = 0; start <=end; start++)
            {
                if (s[start] !=s[end])
                {
                    dp[start][end] =false;
                }else
                {   
                    //[1]
                    //[121]
                    if (end-start <=2)
                    {
                        dp[start][end] = true;
                    }else
                    {
                        dp[start][end] = dp[start + 1][end - 1];
                        //细节3：第一层该如何处理
                        // end =0 end -1 =-1 coredump

                        //end - 1 >= start + 1
                        //end -start >=2
                        //[121]
                    }
                  
                   
                }

                if (dp[start][end] == true &&  (end -start > j-i ))
                {
                    i =start;
                    j = end;
                }
            }
        }

        return s.substr(i, j-i+1);
        }
};
// @lc code=end

/**
 
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int getLongestPalindrome (string s)
{   
   
    
    int length =0;
    int n =s.size();
    //space:
    vector<vector<bool>> dp(n,vector<bool>(n,false));
    
    for( int end  =0;end <n;end++)
    {
        for( int start =0; start <=end;start++)
        {
            if (s[start] == s[end])
            {    
                if (end -start <2 )
                {
                     // 每个子串 100%是回文。[a] [aa] [aba]
                     dp[start][end] =true;
                }else
                {
                     dp[start][end] =  dp[start+1][end-1]; 
                }
                 
                //双指针 end-1 >=start+1  end -start >=2
                
            }else
            {
                dp[start][end] = false; //[a?b]
            }
            
            if ( true == dp[start][end]  && end -start +1 > length)
            {
                length = end -start +1;
            }
        }
    }
    
    return length;
    
}


int main()
{
  string s;
  while(cin>>s)
  {
      cout << getLongestPalindrome(s)<<endl;
  }
}
*/
=======
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

>>>>>>> 48e533a1d75b62c6be4e956fb6759123788a80c6
