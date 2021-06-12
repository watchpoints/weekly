---
title: "leetcode_hard_0-60"
date: 2021-06-01
description: ""
draft: false
tags: ["Interview Question"]
categories: ["Leetcode"]
---


| 开始日期     | 结束日期 | 备注     |
| --------- | ------------ | -------- |
| 2021-6-1 |  |  |
## 前言：换个马甲你不认识了。

- 当面试问过上学时候，有没有学习数据结构? 

>后记：自己心里轻视，这么简单问题，还为 例如排序，数组，链表 tree 图特点
>我已经完全掌握了,100%失败。
- 他考察范围课本上知识，确实你根本回答不上来


- 因此基本知识自己没想清楚。出现错误理解，浅显理解等 混淆理解，你完全卡在这里了。

## 容易混淆的认知，当你决策时候傻傻分不清楚

> 容易混淆的认知，当你决策时候傻傻分不清楚







## 23.合并K个升序链表

### 数据模型:



~~~tex
输入：lists = [[1,4,5],[1,3,4],[2,6]]
输出：[1,1,2,3,4,4,5,6]
解释：链表数组如下：
[
  1->4->5,
  1->3->4,
  2->6
]
将它们合并到一个有序链表中得到。
1->1->2->3->4->4->5->6
~~~



> 脑洞大开：

- 把链表想象成堆

横着看多个升序链表，竖着看就是堆



![](https://gitee.com/alan-tang-tt/yuan/raw/master/%E6%AD%BB%E7%A3%95%20%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84%E7%B3%BB%E5%88%97/resource/heap3.png)

> 计算机问题：top k

- 从多个有序链表 看出一个堆
- 从无序数据中构建堆，



###  数据结构：定义

- #####  Priority Queue  不是二叉排序树，概念不要混了，递归折半查找算法是错误的。

- Priority Queue   是**完全二叉树(Complete Binary Tree)**，特点叶子在n n-1层。这个在堆上发挥重要优势。例如查找最小值

- Priority Queue 不是层次遍历，上层数据一定大于下层，是当前节点 和子节点关系。

- 关注路径 从root到叶子节点 有什么特点，是不是像单链表。

- top k 不要从海量归并排序，利用并发实现，我就从最简单数据结构开始，妙不可言。



![](https://files.mdnice.com/user/5197/9790c1d9-a6a7-4b45-8e5a-f9118bc82c71.png)

- This implies that as you follow any path from a leaf to the root the keys appear in (nonstrict) increasing order

  

heap order

This implies that as you follow any path from a leaf to the root the keys
appear in (nonstrict) increasing order. 
Notice that this implies that the root is necessarily the largest
element

~~~c++

https://leetcode.com/problems/merge-k-sorted-lists/discuss/10528/A-java-solution-based-on-Priority-Queue
java看着很舒服并且比c++难度降低90%。思路更加清晰了。

   public ListNode mergeKLists(List<ListNode> lists) {
        if (lists==null||lists.size()==0) return null;
        
        PriorityQueue<ListNode> queue= new PriorityQueue<ListNode>(lists.size(),new Comparator<ListNode>(){
            @Override
            public int compare(ListNode o1,ListNode o2){
                if (o1.val<o2.val)
                    return -1;
                else if (o1.val==o2.val)
                    return 0;
                else 
                    return 1;
            }
        });
        
        ListNode dummy = new ListNode(0);
        ListNode tail=dummy;
        
        for (ListNode node:lists)
            if (node!=null)
                queue.add(node);
            
        while (!queue.isEmpty()){
            tail.next=queue.poll();
            tail=tail.next;
            
            if (tail.next!=null)
                queue.add(tail.next);
        }
        return dummy.next;
    }
}
https://leetcode-cn.com/submissions/detail/165996284/ c++ 同样思路

Kth Least Element in a Min Heap in C++  这写的很巧妙
https://www.tutorialspoint.com/compile_cpp_online.php
struct Heap {
	vector<int> elemets;
	int n;

	Heap(int i = 0): n(i) {
		elemets = vector<int>(n);
	}
};

inline int leftIndex(int i) {
	return 2 * i + 1;
}

inline int rightIndex(int i) {
	return 2 * i + 2;
}

int findKthGreatestElement(Heap &heap, int k) {
	 priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>queue;
	 queue.push(make_pair(heap.elemets[0], 0));

	for (int i = 0; i < k - 1; ++i) {
		int node = queue.top().second;
		queue.pop();
		int left = leftIndex(node), right = rightIndex(node);
		if (left < heap.n) {
			queue.push(make_pair(heap.elemets[left], left));
		}
		if (right < heap.n) {
			queue.push(make_pair(heap.elemets[right], right));
		}
	}
	return queue.top().first;
}

https://editor.mdnice.com/?outId=053e7962091542a98ace1cc9cbecf3dd



~~~



###  算法描述



- 求top 看，如果是无序数组 ，time：k*n，一次遍历选择最小1个，选择k个。

- 求top 看，如果是堆。time：k*logn  ，破坏原来结构。

- 求top 看，如果是堆。time：k \* log k ，heap保持不看，增加一个堆，k个有序链表。

  

### 第k小数。**Time Complexity: O(k \* log k)**

K-th Greatest Element in a Max-Heap in C++ 【查找算法，不修改原来值】

In this tutorial, we are going to write a program that finds the **k-th** largest element from the max-heap.

We will use priority queue to solve the problem. Let's see the steps to complete the program.

- Initialise the max-heap with correct values.  建堆的时间复杂度就是O(n)。
- Create a priority queue and insert the root node of the max-heap. 
- Write a loop that iterates k - 1 times.  0（k）
  - Pop the greatest element from the queue. 0（1）
  - Add the left and right nodes of the above node into the priority queue. o（k）
- The greatest element in priority queue is the k-th greatest element now. o（1）
- Return i



补充：堆排序

https://afteracademy.com/blog/heap-building-and-heap-sort

~~~shell
T(Heap Sort) = T(build Heap) + (N-1)*T(down_heapify)
             = O(N) + (N-1)*O(logN)
             = O(N) + O(NlogN)
             = O(NlogN)

https://www.geeksforgeeks.org/kth-least-element-in-a-min-heap/

K’th Least Element in a Min-Heap
https://www.tutorialspoint.com/minimum-element-in-a-max-heap-in-cplusplus

~~~



#### Time Complexity

The complexity of the build_heap is O(N). 

down_heapify() function has complexity logN a

nd the build_heap functions run only N/2 times, 

but the amortized complexity for this function is actually linear i.e. O(N)
For more details, you can refer to [this](http://www.cs.umd.edu/~meesh/351/mount/lectures/lect14-heapsort-analysis-part.pdf).



### 最小：

~~~
https://www.tutorialspoint.com/minimum-element-in-a-max-heap-in-cplusplus

~~~





###  举一反三

- 有序*矩阵*中第K小的元素堆*topK*
- K-th Greatest Element in a Max-Heap in C++

> 思路：约束条件：max heap  已经给出。
>
> https://www.tutorialspoint.com/k-th-least-element-in-a-min-heap-in-cplusplus

- #### [373. 查找和最小的K对数字](https://leetcode-cn.com/problems/find-k-pairs-with-smallest-sums/)

  > 思路：2个数组--一个矩阵---n个有序链表---top k问题。
  >
  > https://leetcode-cn.com/problems/find-k-pairs-with-smallest-sums/solution/cha-zhao-he-zui-xiao-de-kdui-shu-zi-by-lenn123/
  >
  > 
  >
  > 忽视就被挨打：换个马甲自己不认识了。
  >
  > 过去无数次面试，从n个有序数组选择top k，矩阵到自己总结了，3个月过去了。自己忘记了。堆才就是背后的n个生序的链表。
  >
  > 其实还是不会
  >
  > 







### 塔山之石

1. 拜托，面试别再问我堆（排序）了！
- https://www.cnblogs.com/tong-yuan/p/Heap.html
-  每个字都看，看三遍，可以培养你耐心。
-  即使你懂了也看。更加培养你耐心
- 输出：开始中--


2. 刷题理解
- https://afteracademy.com/tech-interview/ds-algo-concepts/
3. 完整课程
- http://www.cs.umd.edu/~meesh/351/mount/lectures/lect14-heapsort-analysi
s-part.pdf

- http://www.cs.umd.edu/~meesh/351/mount/lectures/
- 
![](https://files.mdnice.com/user/5197/f93b6aca-937a-4e75-8ad0-887b1dfc4c10.png)



4. 刻意练习：ok

- 资料：
https://www.tutorialspoint.com/convert-min-heap-to-max-heap-in-cplusplus
- 你一定每个题目做一次。

- 输出：开始中--完成。

1. https://www.tutorialspoint.com/minimum-element-in-a-max-heap-in-cplusplus 完成
http://tpcg.io/Te9Xc4e8
