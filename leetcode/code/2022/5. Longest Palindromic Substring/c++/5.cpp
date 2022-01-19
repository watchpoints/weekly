// 5. Longest Palindromic Substring
class Solution
{
public:
    // 感知 图 推理 逻辑
    string longestPalindrome(string s)
    {
        //(1+n)n/2 个单词(很多) --双指针
        int start = 0; //单词开始位置
        int end = 0;   //单词结束位置
        
        vector<vector<bool>> dp(s.size(), vector<bool>(s.size(), false));
        // dp[start][end] =true   fuction
        // dp[start+1][end-1]  end-start >=2
        //一次遍历
        for (int j = 0; j < s.size(); j++)
        {
            for (int i = 0; i <= j; i++)
            {
                if (s[i] == s[j])
                {
                    //二元函数
                    if (j - i <= 2)
                    {
                        dp[i][j] = true;
                    }
                    else
                    {
                        dp[i][j] = dp[i + 1][j - 1];
                    }
                }
                else
                {
                    dp[i][j] = false;
                }

                // 一次变动

                if (true == dp[i][j] && j - i > end - start)
                {
                    end = j;
                    start = i;
                }
            }
        }
        return  end -start;
    }
}
;