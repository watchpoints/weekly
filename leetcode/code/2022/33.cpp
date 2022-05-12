 /**给你一个字符串 s ，找出其中最长的回文子序列，并返回该序列的长度。
    子序列定义为：不改变剩余字符顺序的情况下，删除某些字符或者不删除任何字符形成的一个序列。

        示例 1： 输入：s = "bbbab" 输出：4
            解释：一个可能的最长回文子序列为 "bbbb" 。 示例 2：

    输入：s = "cbbd" 输出：2
        解释：一个可能的最长回文子序列为 "bb" 。

    //回文子序列

    //删除某些字符或者不删除任何字符形成的一个序列
*/
    // bbbab
    // bbbab
    // dp[i][j]  j>=i
int longsub(string input)
{
    int n = input;
    vecotr<vecotr<bool>> dp(n, vecotr<bool>(n, false)); // dp[i][j] =false
    int low = 0;
    int high = 0;

    for (int end = 0; end < n; end++)
    {
        for(int start =0;start <=end;start++)
        {
            if (input[start] == input[end])
            {    //[aa] [aba]
                 if(end -start <2)
                 {
                     dp[start][end] =true;
                 }else
                 {
                     dp[start][end] = dp[start][end-1] ==true? ;
                 }
            }else
            {
                dp[start][end] =dp[start+1][end-1] ;
            }
        }
        if (true ==dp[start][end]  &&  (end -start > high -low))
        {
             high = end;
             low = start;
        }

        return high -low+1;
    }
}
