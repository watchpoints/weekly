/*
 * @lc app=leetcode.cn id=5 lang=cpp
 *
 * [5] 最长回文子串
 */

// @lc code=start

class Solution
{
public:
    string longestPalindrome1(string s)
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

    //5. 最长回文子串
    string longestPalindrome(string s) 
    {
      //双指针：确定每个子串位置
      int startIndex =0;
      int endIndex =0;  

      vector<vector<bool>> dp(s.size(),vector<bool>(s.size(),false));
      //dp[start][end] end >=start
      
      //if s[end] ！=s[begin]  dp[start][end] =false

      //if s[end] ==s[begin]
      //dp[start][end] =dp[start+1][end-1]
      //end -start >=2 
      //end -start <2
      ///dp[start][end] =true

      for(int end =0;end <s.size();end++)
      {
        for(int start =0;start <=end;start++)
        {

            //最后的大 boss
            
            if(s[start] == s[end])
            {
              //[a] [aa] , [aba]
              if(end-start >=2)
              {
                  dp[start][end] =dp[start+1][end-1];
                  //end-1 >=start+1
              }else
              {  
                   dp[start][end] =true;
              }
              
            }

            if( dp[start][end] == true && (end-start > endIndex -startIndex))
            {
                endIndex =end;
                startIndex =start;

            }

        }
      }

      return s.substr(startIndex,endIndex-startIndex+1); //[start ,end)
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
