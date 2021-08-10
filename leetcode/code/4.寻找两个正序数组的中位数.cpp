/*
 * @lc app=leetcode.cn id=4 lang=cpp
 *
 * [4] 寻找两个正序数组的中位数
 */

// @lc code=start
class Solution
{
public:
    double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2)
    {

        //01 思路
        //中位数  --topk ->n个有序数组 --合并n个有序数组  比较n个大小,n是有序数组。2个 多个？

        int length = nums1.size() + nums2.size();
        //偶数
        //if (length % 2 == 0)
        if ((length & 1) == 0)
        {
            //int top1 = length /2;
            int top1 = length >> 1;
            int top2 = (length >> 1) + 1;
            //【1，2】 【3,4】 length=4 lenght/2 =2 top1 =2; top3 =3;
            return (topK(nums1, nums2, top1) + topK(nums1, nums2, top2)) / 2.0;
        }
        else
        {
            //int top = length/2 +1;
            int top = (length >> 1) + 1;
            //【1，2】 【3】 length=3 lenght/2 =1 top =2;
            return topK(nums1, nums2, top);
        }
        //02 细节
        // 双指针
        // m 和n大小不一样。
        // min（a,b）
        //小区，城市 全国 全世界 第一名 不一样的。
        //公式：f（k） = min(a,b) 
        //f(k) = a+b
    }
    /** 
      *  k 是个数，不是索引  
      *  执行次数：f（n） =k
      */
    int topK(vector<int> &nums1, vector<int> &nums2, int k)
    {
        int index1 = 0;
        int index2 = 0;
        int p1 = 0;
        int p2 = 0;

        int ret;
        for (int i = k; i >= 1; i--)
        {
            p1 = index1 < nums1.size() ? nums1[index1] : INT_MAX;
            p2 = index2 < nums2.size() ? nums2[index2] : INT_MAX;
            if (p1 <= p2)
            {
                index1++; //
                ret = p1;
                //cout <<  index1 << ">1<" <<index2 <<  " k= " << k << "ret="<<ret << endl;
            }
            else
            {
                index2++;
                ret = p2;
                //cout <<  index1 << " >2>" <<index2 <<  " k=" << k << "ret="<<ret << endl;
            }
        }

        return ret;
    }
};
// @lc code=end
