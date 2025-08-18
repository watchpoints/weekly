---
title: 167. 两数之和 II - 输入有序数组
date: 2025-04-07
description: c++
draft: false
tags:
  - 成为孩子榜样，为国争光
categories:
---


## 一、 刷题选做什么准备吗？

- 准备一个白纸

- 断网

- 25 分钟倒计时

**画外音：25 分钟限制**

- **相信你足够知识**：

遇到不会，不需要急匆匆 打开浏览器搜索各种答案，

你感觉难，别人感觉难，分析这个题目，最坏结果做不出来。不浪费大量时间使用浏览器搜索答案。

- **无条件支持**：不要想着成为（对面是）大厂，名校，总监，领导 才可以做

在这个赛道上，他们不是 你对手，他们根本不敢参与。他们没这个时间，只要你愿意就可以

- **先完成，在完美**：千万不要想太复杂，用巧妙解法，复杂的思路就是不是好思路，直接放弃。

## 二、题目要求

- 面试官通过口述方式，缺少测试用例和函数原型


[167. 两数之和 II - 输入有序数组](https://leetcode.cn/problems/two-sum-ii-input-array-is-sorted/)



### 听清楚后然后反问：


## 三、思路：采用什么数据结构与算法。

### 四、代码实现

#### c++版本（支持重复元素）



### Rust版本(不支持重复元素)

~~~ Rust

use std::collections::HashMap;

impl Solution {

//我必须立刻押注 Rust

// @微信公共号: 后端开发成长指南

// @微信: watchpoints

pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {

let mut hash_map = HashMap::new(); //key nums[i],value i

for (index,&key) in nums.iter().enumerate() {

if let Some(&j) = hash_map.get(&(target-key)) {

return vec![j as i32 ,index as i32]

}

hash_map.insert(key,index);

}

return vec![]

}

}

~~~

## 参考

- 左耳朵耗子叔叔 https://github.com/haoel/leetcode

- https://github.com/youngyangyang04/leetcode-master

- https://programmercarl.com/

- 宫水三叶刷题日记https://www.acoier.com/

- ppt 版本 https://github.com/liweiwei1419/LeetCode-Solution-PPT

--------------

您的**点赞，转发** 将是我最大的写作动力！

如果更多疑问，欢迎留言评论。