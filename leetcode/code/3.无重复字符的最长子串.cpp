/*
 * @lc app=leetcode.cn id=3 lang=cpp
 *
 * [3] 无重复字符的最长子串
 */

// @lc code=start
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
      //故事：
      //思路：只要获取无重复最长子串开始位置 和结束问题。-》最长子串 的长度
     
      //有个问题导致这个思路无法实现：
      //一个字符串长度n ，共n*(n+1)/2 子串
      //每个子串都判断全部，这个肯定不行的

      //细节：概念清楚 --如何表示---如何体现变化
       //???abc???【不重复】
      //???abca??? 【重复】
      //???abcab??? 【重复】

      //end 结尾。重复单词可能出现在任意位置
      
      int start =0; //无重复最长子串开始位置
      int end =0; 
      int  ret =0;

      vector<int> hash(128,-1); 
      // hash[key]=-1    [end]之外 无重复   
      // hash[key] <start。 【start】之外 不重复
      // hash[key]>=start 【start --end】 重复

      // 思考：60秒 这里为什么不统计每个单词出现个数呢？
      //【start end】
      
      // 如何判断重复，
      //按照s[end]去寻找【start end】需找到就重复
      //如何重复元素变成不重复的元素。
      //hash[s[end]] 寻找到下一个位置开始。
      while (end <s.size())
      {
          char key =s[end];

          if (hash[key] >=start)
          {
              start = hash[key]+1; 
          }

          ret =max(ret,end-start+1);//思考60秒：执行条件是什么？
         
          hash[key] =end;
          end++;   
      }

      return ret;
    }
};
// @lc code=end

