---
<<<<<<< HEAD
title: 167. 两数之和 II - 输入有序数组
=======
title: 成为孩子榜样，为国争光
>>>>>>> 997b2afada26e8cba441316dd4588e810e40ba32
date: 2025-04-07
description: c++
draft: false
tags:
  - 成为孩子榜样，为国争光
categories:
---

<<<<<<< HEAD

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
=======
# 一、背景

大家都知道，**手写算法题**是大厂面试的必考环节，俗称“压轴题”。  
这就像游戏里打怪升级，遇到的第一个大BOSS——

**蓝色大头幽魂上线！不打过它，通关别想。**。

![黑神话：悟空 大头怪](https://cdn.nlark.com/yuque/0/2025/png/215144/1754441434080-c12d5b0f-40a7-43a5-a0a4-56b3bed63c03.png?x-oss-process=image%2Fformat%2Cwebp)


让我们来分析一下，**为什么这个“幽魂BOSS(数据结构与算法)”如此难打？**

明知道刷题很重要，**却总是迟迟不动手**；  
哪怕一次次在面试里栽跟头，转身也没多少改变；
是不是我太懒了？

>真相可能不是“懒”，而是想的太多：
- 一看到题量上千，就觉得“这辈子也刷不完”
- 做一道题一小时都没思路，挫败感直线上升
- 刷题像在黑屋子里搬砖，短期看不到回报，脑子直接罢工

于是我们的大脑自我保护，

悄悄切换成“拖延模式”：“今天太累了，明天再战。”

## ❶ 题太多，直接劝退

一说“刷题”，很多人脑子里立马闪过 “LeetCode” 

一看题量就懵了：1千多道题，一个题做一周，刷完得 19 年……

好家伙，还没开始面试，人已经先缴械投降了。

干脆放弃，去摆个小摊，卖煎饼可能都比这快。

## ❷ 题目类型太多，刷题像走迷宫

经验丰富的刷题党普遍认为：

> 稳定掌握 LeetCode 前 150 题，能够总结出自己的解题思路和套路，才是高效准备 BAT 等大厂面试的关键。

稍微有心从网络搜索以下，不少刷过1千题人总结 刷题路线，面试经典高频 150 题，让新手避免很多坑，甚至组建付费刷题小分队。解决大部分人问题
- 代码随想录
- 小林Coding    
- 宫水三叶
-  花花酱
- Michelle
现实很骨感。
**996程序员每天累成狗**，就算开始刷题，也很难坚持三四个月。  
更别说每天不同题目，不同的讨论 刷着刷着半途而废 

完整地址：https://docs.qq.com/sheet/DY1NwcGZjUXpYb01j?tab=oqi27u

![这个是信奥赛的](https://cdn.nlark.com/yuque/0/2025/png/215144/1754443382876-111493a1-e094-4221-b2f3-9abe2be58747.png?x-oss-process=image%2Fformat%2Cwebp)
![leetcode](https://pica.zhimg.com/v2-b9c3e4f8c0b1a740c55f7387f7d63774_r.jpg?source=1def8aca)



## ❸ 刷题不是刷“量”，而是刷“熟”

不少人分享过：  
自己只刷了 50～60 题，就成功拿到了大厂 offer。

秘诀其实就两个字：**重复！**

一个题刷三遍，也能刷出“150题”的水准。

>有没有可能，眼前那密密麻麻的 1000 道题，突然都消失了，只剩下一道？  
你专心把这一道题反复练，练到极致：

- 第一次，做了一周
    
- 第二次，花三天复刷
    
- 第三次，三个小时搞定
    
- 第四次，只用十分钟就能写出最优解
    
这才是真正的成长路线。  
刷题，不是比谁刷得多，而是比谁刷得**深、刷得熟**。
别总幻想“下周就能“高屋建瓴吊打面试官”，  

就像“八段锦”这套功法，全套只有八个动作。  
刷题也一样。  
与其每天换新题、贪多求快，不如挑出经典题，**反复刷、认真练**。


# 二、无重复字符的最长子串

## 2.1 题目信息

给定一个字符串 `s` ，请你找出其中不含有重复字符的 **最长 子串** 的长度。
**示例 1:**

**输入:** s = "abcabcbb"
**输出:** 3 
**解释:** 因为无重复字符的最长子串是 `"abc"`，所以其长度为 3。


**示例 3:**
**输入:** s = "pwwkew"
**输出:** 3
**解释:** 因为无重复字符的最长子串是 `"wke"`，所以其长度为 3。
     请注意，你的答案必须是 **子串** 的长度，`"pwke"` 是一个_子序列，_不是子串

**提示：**
- `0 <= s.length <= 5 * 104`
- `s` 由英文字母、数字、符号和空格组成


分析：
![](https://cdn.nlark.com/yuque/0/2025/png/215144/1754448356828-dc8c7050-fba6-4a3d-b1b4-03b1380ab7d7.png?x-oss-process=image%2Fformat%2Cwebp)
## 2.2 c语言实现


![0ms击败100.00](https://cdn.nlark.com/yuque/0/2025/png/215144/1754452508598-d6b7c449-c3ed-4d6e-8e34-e31143ce74d9.png?x-oss-process=image%2Fformat%2Cwebp)

```C
int lengthOfLongestSubstring(char* s)

{
    //定义数据结构
    int hashMap[128];  //key 字符 value 字符对应的下标 定义不是 char hashMap[128];
    //为什么是128 ASCII 字符集标准中定义了 128 个字符，对应编号 0 ~ 127
    memset(hashMap,-1,sizeof(hashMap));
    //unsigned char 是唯一可以无损表示、访问任意类型对象的内存的类型。
    //According to the C and C++ standards
    //The object representation of an object is the sequence of unsigned char stored in memory
    //https://en.cppreference.com/w/cpp/string/byte/memset

    char* startPtr = s; // 窗口起点指针

    int startIndex = 0; // 记录窗口起点的索引

    char* endPtr = s;   // 窗口终点指针,因为指针结构 不清楚当前字符串的长度。

    int maxLen = 0;// 保存最大长度
    char tempChar;

    //在C语言设计之初，计算机的内存资源非常有限。

    // 为了最大限度地节省内存，C语言选择了使用一个单一的字节（'\0'）来标记字符串结束，

    // 而不是像一些其他语言那样记录字符串的长度。如果采用记录长度的方式，

    // 可能需要额外的几个字节来存储长度信息

    //数组像制作一样使用。这里为体现c语言特性 故意没有

    while(*endPtr != '\0')

    {
        tempChar= *endPtr; //字符--位置

        //重复,原来重复字符串不符合要求

        if(hashMap[tempChar] != -1 && hashMap[tempChar] >= (startPtr -s )){

            // char* s = "hello";

            // char* start = s + 2; // 指向字符 'l'，即索引为2的位置

            // int offset = start - s; // offset == 2

             startPtr = s + hashMap[tempChar] + 1; // 指针跳转
        }

        // 更新字符最新出现位置

        hashMap[tempChar] = (int)(endPtr - s); //指针可以之间相减

  

        int curLen = (int)(endPtr - startPtr) + 1;

        if (curLen > maxLen) {

            maxLen = curLen;

        }

        endPtr++;
    }
    return maxLen;

}
```

| 优点            | 说明                                   |
| ------------- | ------------------------------------ |
| ✅ 指针操作高效      | 不使用数组索引，完全用指针运算，贴近底层，效率高             |
| ✅ ASCII字符支持完整 | 使用大小为128的数组支持全部可打印和控制字符（含空格、制表符等）    |
| ✅ 无需额外库       | 无需引入哈希表、链表等复杂结构，代码简洁、纯 C 实现          |
| ✅ 逻辑清晰易于维护    | 使用 `-1` 表示“字符未出现”，避免使用偏移量+1等技巧，判断更直观 |
| ✅ 最优时间复杂度     | 滑动窗口双指针做到了 O(n)，适合大多数面试/竞赛/实际场景      |
| ✅ 可扩展性好       | 稍作修改即可支持更多字符集（如 Unicode、UTF-8）       |

指针操作 vs 数组索引：本质是一样的

- **指针操作**在底层编程、系统编程、操作内存时更常用，更能体现“内存地址”概念。
- **数组索引**对很多人来说更直观、易读，尤其是初学者
## 2.3 c++实现
```c++
class Solution {

public:

	int lengthOfLongestSubstring(string s) {
	
		int ret =0;
		
		int begin =0; //开始位置
		
		int end =0;
		
		vector<int> hash(128,-1); //hash[s[i]]=i;
		
		for(end =0;end<s.size();end++)
		
		{
			char temp =s[end];
			
			if(hash[temp]!=-1 && hash[temp] >=begin)
			{
				begin=hash[temp]+1;
			}
			
			ret =max(ret,end-begin+1);
			
			hash[temp]=end;
		
		}
		return ret;
	}
};
```


## 2.4 Go实现

```go
func lengthOfLongestSubstring(s string) int {
    //定义数据结构
    const asciiSize = 128
    hashMap := [asciiSize]int{}
    for i := range hashMap {
        hashMap[i] = -1
    }
    start, maxLen := 0, 0
    // 实现算法
    for i, ch := range s {
        if hashMap[ch] >= start {
            start = hashMap[ch] + 1
        }
        hashMap[ch] = i
        maxLen = max(maxLen, i - start + 1)
    }
    return maxLen
}

func max(a, b int) int {
    if a > b {
        return a
    }
    return b
}

```

## 2.5 Rust实现


```rust
use std::collections::HashMap;

impl Solution {
    pub fn length_of_longest_substring(s: String) -> i32 {
        let mut map = HashMap::new(); // 存字符 -> 最新索引
        let (mut start, mut max_len) = (0usize, 0usize);

        for (i, c) in s.chars().enumerate() {
        //Rust 的模式匹配写法，用来**判断 
        //HashMap 中是否有键 `c` 的值**，并把值绑定给 `prev_index`。
            if let Some(&prev_index) = map.get(&c) {
                if prev_index >= start {
                    start = prev_index + 1;
                }
            }
            map.insert(c, i);
            max_len = max_len.max(i - start + 1);
            //`.max()` 方法是 Rust 标准库 `Ord` trait 自带的
        }

        max_len as i32 //返回 `i32`
    }
}

```
## 2.6 Python实现


```python
class Solution:
    def lengthOfLongestSubstring(self, s: str) -> int:
        char_index = {} //用大括号 `{}` 创建空字典
        start = 0
        max_len = 0
         //`enumerate` 是 Python 内置函数，
         //常用来在遍历序列（如列表、字符串）时，同时获得元素的索引和值
        for i, c in enumerate(s):
            //有重复元素
            if c in char_index and char_index[c] >= start:
                start = char_index[c] + 1
            char_index[c] = i
            max_len = max(max_len, i - start + 1)

        return max_len

```



### 参考
- 刷到什么程度才能去大厂面试？https://www.zhihu.com/question/28580777
-   如何在最短的时间内搞定数据结构和算法，应付面试? 
- 黑神話悟空最強BOSS排名TOP10
- 【黑神话：悟空】4K 最高画质 全收集 全结局 全剧情流程通关攻略
- https://www.bilibili.com/video/av1256385034
- https://leetcode.cn/problems/longest-substring-without-repeating-characters/description/
- https://www.youtube.com/watch?v=IWzpZUbyCGg&ab_channel=Stacy
>>>>>>> 997b2afada26e8cba441316dd4588e810e40ba32
