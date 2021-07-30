
---
title: "leetcode_300-400"
date: 2021-05-18
description: ""
draft: false
tags: ["Interview Question"]

categories: ["Leetcode"]
---


| 开始日期     | 更新日期     | 备注     |
| --------- | ------------ | -------- |
| 2020-6-10 | 2020-6-10 | 300-400中等难度总结|





# 总结：

Happy coding! Enjoy Algorithms.

Happy coding! Enjoy Algorithms.

Happy coding! Enjoy Algorithms.

1. 刷题不需要刷那么多！！不需要那么多！！不需要辣么多！！ 刷熟常用300题，刷5遍，效果 >>>>>(远好于） 我这样刷到900+题。

2. 个人感觉： 熟练度 >>>>> （远重要于）抖小聪明。

 流畅的表达，简介明了的思路远好于在白板上磨磨蹭蹭的推导。这也回到第一点，反复刷同样的题非常重要。

其实，8-10遍高频效果是最好的，这基本就是到另一个境界了。

当然，大家没有那么多时间，3-5遍也可以去面试了。

3. Talk out loud，这个不仅仅是说在面试的时候你要说出你的思路。

刷题的时候也是！你第一遍可以不开口，但后面几遍，一定要开口说话！！

刷题是为了面试，面试第一看的是communication skill! 

不在刷题的时候开口，到面试的时候才意识到沟通不畅就太晚了。

4. 关于具体刷题，我只说第一遍，我是怎么刷的。

前500题：抄。就是直接看答案抄！完美解决没基础看不懂，不会做，做题枯燥等种种问题。

其实，我觉得，在刚开始的时候完全没必要思考，做题。因为初学者啥也不会。

自己拼命想，写两题就被劝退不想再做了。

1. https://afteracademy.com/tech-interview/ds-algo-concepts 题目总结和这个对比起来。
2. https://lucifer.ren/blog/2020/11/03/monotone-stack/





栈和队列

#### **Applications of Stack Data Structure**

- An "undo" mechanism in text editors
- Forward and backward feature in web browsers
- Check for balanced parentheses in an expression【ok】
- Expression evaluation and syntax parsing【ok】
- **Backtracking**. This is a process when you need to access the most recent data element in a series of elements. Think of a maze - how do you find a way from an entrance to an exit? Once you reach a dead end, you must backtrack. But backtrack to where? to the previous choice point. Therefore, at each choice point, you store on a stack all possible choices. Then backtracking simply means popping a next choice from the stack.
- We use a stack for the Iterative implementation of several recursive programs like tree traversals, DFS traversal in a graph, etc.
- For solving several problems in algorithms, we use a stack as the principle data structure with which they organize their information.
- Memory management: Any modern computer environment uses a stack as the primary memory management model for a running program.

#### Suggested Problems to solve

- [Find next greater element in an array](https://afteracademy.com/blog/find-the-next-greater-element-for-every-element-in-an-array)
- [Min Stack Problem](https://afteracademy.com/problems/min-stack-problem)
- [Check for balanced parentheses in an expression](https://afteracademy.com/problems/check-for-balanced-parentheses-in-an-expression)
- [Largest Rectangle in a Histogram](https://afteracademy.com/problems/largest-rectangle-in-histogram)
- [Trapping Rainwater](https://afteracademy.com/problems/trapping-rain-water)
- [Reverse a stack using recursion](https://afteracademy.com/problems/reverse-a-stack-using-recursion)
- [Validate Stack Sequences](https://afteracademy.com/problems/validate-stack-sequences)
- [Merge overlapping intervals](https://afteracademy.com/problems/merge-overlapping-intervals)
- [Sort a stack using another stack](https://afteracademy.com/problems/sort-a-stack-using-another-stack)
- Check if a string is a palindrome using stacks





#  题目汇总

- 寻找两个有序数组的中位数  

- [K个有序数组的中位数](https://www.lintcode.com/problem/median-of-k-sorted-arrays/description)

- 有序矩阵中第K小的元素

- #### [496. 下一个更大元素 I](https://leetcode-cn.com/problems/next-greater-element-i/)

- #### [494. 目标和](https://leetcode-cn.com/problems/target-sum/) ！！！ 递归回溯方法，在这里又不会了

   [77. 组合](https://leetcode-cn.com/problems/combinations/solution/hui-su-suan-fa-jian-zhi-python-dai-ma-java-dai-ma-/)

  

  #### [491. 递增子序列](https://leetcode-cn.com/problems/increasing-subsequences/)

  

  





# 分析



## 494. 目标和

### 一、**题目信息**

今天来做这个题目，主要考察的

> - 链接：494. 目标和
> - 来源：Interview Question
> - 难度：Hard

### 二、**题目描述**

> 算法是问题求解过程的一种描述，看看这个题目是怎么描述的吧。



- 回溯算法是在一棵树上的 **深度优先遍历**（**因为要找所有的解，所以需要遍历**）

  题目： https://leetcode-cn.com/submissions/detail/143251467/

- 小王遇到的误区：看到表达式，然后想到stack 求表这个题目走远了呀，然后ko此地，思维中断了。---此路不不会变通了。

- 小王遇到的挑战：组合和队列 n个节点。n可能性，tree n是高度，不是长度。

  ![](https://pic.leetcode-cn.com/1603889327-igUOld-77.%E7%BB%84%E5%90%88.png)

- 不会优化了  

https://leetcode-cn.com/problems/target-sum/solution/gong-shui-san-xie-yi-ti-si-jie-dfs-ji-yi-et5b/

https://leetcode-cn.com/problems/target-sum/solution/gong-shui-san-xie-yi-ti-si-jie-dfs-ji-yi-et5b/

 [（题解）403. 青蛙过河](https://leetcode-cn.com/problems/frog-jump/solution/gong-shui-san-xie-yi-ti-duo-jie-jiang-di-74fw/) 



输入：nums = [1,1,1,1,1], target = 3
输出：5
解释：一共有 5 种方法让最终目标和为 3 。
-1 + 1 + 1 + 1 + 1 = 3

+1 - 1 + 1 + 1 + 1 = 3
+1 + 1 - 1 + 1 + 1 = 3

+1 + 1 + 1 - 1 + 1 = 3
+1 + 1 + 1 + 1 - 1 = 3



来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/target-sum
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

进一步思考，对nums[i]选择处理策略后，紧接着对nums[i+1]进行处理，以此类推，这就是 DFS 的过程。而 DFS 的优化，有两个方向：

记忆法，即记录执行过的计算结果，下次 DFS 再进来，直接返回，避免重复计算；
剪枝法，即通过逻辑判断当前 DFS 是否有必要进行，无必要则结束；
记忆法对于本题，是简单可行的：只需要缓存记录下各个数组偏移量和不同目标和，所计算得到的结果值；
而剪枝法不适用本题，因为 DFS 到某个数组元素时，并不能确定后续的元素是否能够构成目标和。



作者：eequalmcc
链接：https://leetcode-cn.com/problems/target-sum/solution/ji-yi-fa-dfs-he-dong-tai-gui-hua-de-si-l-aexr/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

- 用空间换时间。

  for循环无法实现---改成递归---优化递归

#### [322. 零钱兑换](https://leetcode-cn.com/problems/coin-change/)



### 三、**算法描述**

> 能说出基本思想即可,实在不行动手画个流程图把。





### 四、**参考代码**

> 放轻松，虽然是c++实现，我们一贯宗旨是拒绝奇技淫巧，不懂代码一看就明白






### 五、举一反三

> 别人最佳答案并不重要，关键是自己理解。
>
> 

----------------------------

[496. 下一个更大元素 I](https://leetcode-cn.com/problems/next-greater-element-i/)

### 一、**题目信息**(Understanding the Problem)

今天来做这个题目，主要考察的heap 比较大小。

> - 链接：[496. 下一个更大元素 I](https://leetcode-cn.com/problems/next-greater-element-i/)
> - 输出：https://leetcode-cn.com/problems/next-greater-element-i/solution/dan-diao-zhan-zhan-ye-ke-yi-yong-lai-pai-or0u/
> - **Level**：easy
> - Asked in: Amazon, Microsoft

### 二、**题目描述**(**Problem Description**)

> 算法是问题求解过程的一种描述，看看这个题目是怎么描述的吧。





**Possible questions to ask the interviewer:-**

- What is the range of elements in the array? (**Ans:** The array consists of positive integers less than 10⁵)
- What should we do in the case when no greater element exists on the right side of the element in the array? (**Ans:** The next greater element for such element would be -1)
- Can the array contain duplicates? (**Ans**: Sure, that's a possibility)



### 三、**算法描述**（Solutions）

> 能说出基本思想即可,实在不行动手画个流程图把。

- 比较大小：就是一个元素和整个数组比较 这个。怎么还有更好方式，完全有序，headp每个链表完全有序【大家都知道的】
- 为什么要全部比较：我不清楚争个元素情况，只能这样。
- 新的规定是是什么？没有完美情况：完全有序

- 单调栈与排序关系

https://lucifer.ren/blog/2020/11/03/monotone-stack/

#### [面试题 03.05. 栈排序](https://leetcode-cn.com/problems/sort-of-stacks-lcci/)

https://leetcode-cn.com/problems/evaluate-reverse-polish-notation/solution/xiao-bai-ye-neng-dong-yong-zhan-mo-ni-ni-i0hl/

https://blog.csdn.net/is_zhoufeng/article/details/8040804





Time Complexity ： O(n²)

Space Complexity:  O(n)



> https://afteracademy.com/blog/find-the-next-greater-element-for-every-element-in-an-array

#### 2. Using Stack Data Structure

- Time Complexity: Initializing ans[] array with -1 + Traversal of the array and Stack Manipulation = O(n) + O(2*n) (**Why?**) = O(n)

- Space Complexity: Stack of size n + ans[] array of size n = O(n) + O(n) = O(n)



For example [9, 8, 7, 3, 2, 1, 6]

The stack will first contain [9, 8, 7, 3, 2, 1] and then we see 6 which is greater than 1 so we pop 1 2 3 whose next greater element should be 6





### 四、**参考代码**

> 放轻松，虽然是c++实现，我们一贯宗旨是拒绝奇技淫巧，不懂代码一看就明白











- https://leetcode-cn.com/problems/next-greater-element-i/solution/zhan-xia-yi-ge-geng-da-yuan-su-i-by-demi-cumj/




### 五、举一反三(Comparison of solutions)

> 别人最佳答案并不重要，关键是自己理解。





## 378. 有序矩阵中第K小的元素

### 一、**题目信息**
今天来做这个题目，主要考察的heap 比较大小。
> - 链接：玻璃球
> - 来源：Interview Question
> - 难度：Hard

### 二、**题目描述**
> 算法是问题求解过程的一种描述，看看这个题目是怎么描述的吧。

### 三、**算法描述**
> 能说出基本思想即可,实在不行动手画个流程图把。



### 四、**参考代码**
> 放轻松，虽然是c++实现，我们一贯宗旨是拒绝奇技淫巧，不懂代码一看就明白




### 五、举一反三
> 别人最佳答案并不重要，关键是自己理解。



