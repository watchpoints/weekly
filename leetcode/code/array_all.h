#include <vector>
#include <iostream>
using namespace std;

class Solution162
{
public:
    //852. 山脉数组的峰顶索引
    //162 寻找峰值
    //如果重复怎么办 ologn --o（n）
    //[1,1,1,1]
    
    int findPeakElement(vector<int>& nums)
    {
      //状态定义:这是个高中数学范围问题
      ////至少3个元素 
      //arr[i-1] < arr[i] i>0， i-1 >=0 i>=1
      //arr[i] > arr[i+1] i<n  i+1 <n  i<n-1;
     // n-1>1 n>2;
     //[1 2 1]
     //left =0,right =3 mid=1;

     //2个元素可以吗？ 
     //[1 2 1]
     //【2，1】
     //【1，2】
     //left =0,right =1 mid=0;
     //arr[i-1] < arr[i] 不行， arr[i] > arr[i+1]可以

     //【2】 循环结束。
     int left =0;
     int right =nums.size()-1;

     while(left <right)
     {    
         
         int mid =(left+right)/2; 
         //为什么mid+1是安全的，/2有关系
         if (nums[mid] >nums[mid+1])
         {
              right =mid;
              //4:7:11
              //nums[mid+1]肯定不是最大的， nums[mid] 更大
         }else if(nums[mid] <nums[mid+1] ) 
         {
             left =mid+1;
             //nums[mid]  肯定不是最大的。nums[mid+1更大
         }else
         {   
             //二分查找 变为线性查找
             if(nums[mid] == nums[left])
             {
                //[1,1,1,1,1]
                //0:2:4
                 left++;

             }
             if(nums[mid] == nums[right])
             {
                //vector<int> test={1,1,1,1,4,1,1,1,1,1,1,1}; 
                // 4:7:11
                 right--;
             }
         }

        // cout<< left << ":"<<mid<<":"<<right<<endl;
         
     }
     return nums[left];
    }
};