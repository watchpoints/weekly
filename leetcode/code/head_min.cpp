//Minimum element in a max heap in C++.

/*
1. Find first leaf in a heap and consider its value as min
2. Iterate all remaining leaves and update min value if leaf with smaller value is found
https://www.tutorialspoint.com/minimum-element-in-a-max-heap-in-cplusplus

**/
#include <iostream>
#define SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
using namespace std;
int getMinElement(int *heap, int n){
   int minElement = heap[n / 2];
   
   for (int i = n / 2 + 1; i < n; ++i) {
      minElement = min(minElement, heap[i]);
   }
   return minElement;
}

//从大heap选择最小的一个。
//01 如果小定对 的哥
//02 这里是big ，怎么办。需要根据定义遍历。
//定义是什么 平衡搜索tree吗？不是
// 假如只有2层的话，root肯定不是，需要left right选择一个最小的
//假如3层的 最小结果肯定在第三层。2层
//n n-1层
int minValue(int *heap, int n)
{
  //第n/2就是叶子节点,从叶子节点开始
  int ret = 9999999999;
  
  for(int i=n/2;i<n;i++)
  {
    
    ret =min(ret,heap[i]);
  }
  //这里没有从 0位置开始为什么。
  return ret;//自己想的超级难，结果就是遍历一次。

}
int main()
{
   int heap[] = {120, 90, 100, 70, 75, 80, 60, 25, 40, 35};
   cout << "Min value: " << getMinElement(heap, SIZE(heap)) << "\n";
   return 0;
}

#include <bits/stdc++.h>
using namespace std;
int getMaxElement(int *heap, int n) {
   int maxVal = heap[n / 2];
   for (int i = n / 2 + 1; i < n; ++i) {
      maxVal = max(maxVal, heap[i]);
   }
   return maxVal;
}
int main2() {
   int heap[] = {15, 27, 22, 35, 29, 55, 48}; int n = sizeof(heap) / sizeof(heap[0]);
   cout << "Maximum element = " << getMaxElement(heap, n) << endl;
   return 0;
}

//https://www.youtube.com/watch?v=hGK_5n81drs
//Find the k'th Largest or Smallest Element: From Sorting To Heaps To Partitioning
