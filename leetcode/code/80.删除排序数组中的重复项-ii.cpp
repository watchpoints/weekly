/*
 * @lc app=leetcode.cn id=80 lang=cpp
 *
 * [80] 删除排序数组中的重复项 II
 */

// @lc code=start
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
      //01 三路排序 适合链表和数组 类比插入排序。
       int length =nums.size();
       if (length <=2)
       {
           return length;
       }

       int cur =2;
       int tail =1; //[0....tail] 有序
       int pre =0;

       while (cur < length)
       {
          //如果相同，代表重复至少三个. [tail.....cur]之际内容可以被替换。
          pre = tail -1;
          if (nums[pre] == nums[cur])
          {
              //tail保持不变
              cur++;
          }else
          {
              nums[++tail] = nums[cur++];
          }
          
       }
       return tail+1;
    }
};
// @lc code=end

