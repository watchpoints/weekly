//数组中的第 k 大的数字
#include <vector>
#include <iostream>
using namespace std;

class Solution
{
public:

    //时间复杂度 平均 o（n），最坏情况o（n2）
    //非递归遍历
    int findKthLargest(vector<int> &nums, int k)
    { 
       int low = 0 ;
       int high = nums.size()-1;
       //多次分割
       while(high > low)
       {
           //一次分割分割函数
          int midIndex =0;

          int count = high -midIndex+1;
          if (count == k) return nums[midIndex];
          if (count > k) {
              low = midIndex+1;
          }else{
              high = midIndex-1;
          }
       }

       return -1;
    }

     //时间复杂度 平均 o（n），最坏情况o（n2）
    int findKthLargest1(vector<int> &nums, int k)
    { 
        //递归实现,需要重新定义新函数
        //快速选择并不递归访问双边，而是只递归进入一边的元素中继续寻找。
        //这降低了平均时间复杂度，从O(n log n)至O(n)，不过最坏情况仍然是O(n2)。
        return quickSelect(nums, 0, nums.size() - 1, k);
    }

    //严蔚敏《数据结构》标准分割函数
    // use quick sort's idea
    // put nums that are <= pivot to the left
    // put nums that are  > pivot to the right
    int quickSelect(vector<int> &nums, int low, int high, int k)
    {
        // 01 Paritition
        int pivot = nums[low];
        int left = low;   //细节1 需要定义 2个变量。
        int right = high; //细节2 ：begin == end 结束了
        while (right > left)
        {
            while (right > left && nums[right] >= pivot)
            {
                right--;
            }
            //细节3:这里没有使用swap。数组特点 不整体移动
            // left空缺 从right方向 选择.然后填补left ，
            //这个时候 right空缺 从left方向选择
            nums[left] = nums[right];

            while (right > left && nums[left] <= pivot)
            {
                left++; //符合 put nums that are <= pivot to the left
            }
            nums[right] = nums[left];
        } //这里时间复杂度 看似2个循环 最迷惑人地方  left ==right=nil

        nums[left] = pivot;
        pivot = left; //分割点

        // 02 count the nums that are > pivot from high
        int count = high - pivot + 1;
        // pivot is the one!
        if (count == k)
            return nums[pivot];
        // pivot is too small, so it must be on the right
        //【1，2，3，4，5】 targe=4 k=2 pivot=2 count=4
        if (count > k)
        {
            return quickSelect(nums, pivot + 1, high, k);
        }
        else
        {
            //如图 cout =2  k=5
            return quickSelect(nums, low, pivot - 1, k - count);
            //细节5: k发生了变化
        }

        //【1，2(targe)，3，4(pivot)，5】
        //如果目标位置在pivot left， 必然 k > count
        //【1，2(pivot)，3，4(targe)，5】
        //如果目标位置在pivot right 必然 count > k

        ////快速选择并不递归访问双边，而是只递归进入一边的元素中继续寻找。
        //这降低了平均时间复杂度，从O(nlog n)至O(n)，不过最坏情况仍然是O(n2)。
    }
};

int main()
{
    /*
    输入: [3,2,1,5,6,4] 和 k = 2
    输出: 5

    输入: [3,2,3,1,2,4,5,5,6] 和 k = 4
    输出: 4
    */
    Solution test;
    vector<int> input1 = {3, 2, 1, 5, 6, 4};
    int k1 = 2;
    vector<int> input = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    int k = 4;
    cout << "findKthLargest =" << test.findKthLargest(input, k) << endl;
}
