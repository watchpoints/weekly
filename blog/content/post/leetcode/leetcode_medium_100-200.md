
---
title: "leetcode_medium_100-200"
date: 2020-05-18
description: ""
draft: false
tags: ["Interview Question"]
categories: ["Leetcode"]
---


| 开始日期     | 更新日期     | 备注     |
| --------- | ------------ | -------- |
| 2020-6-10 | 2020-6-10 | 300-400中等难度总结|

#  题目汇总




## 378. 有序矩阵中第K小的元素

### 一、**题目信息**
今天来做这个题目，主要考察的xxx 
> - 链接：玻璃球
> - 来源：Interview Question
> - 难度：Hard

### 二、**题目描述**
> 算法是问题求解过程的一种描述，看看这个题目是怎么描述的吧。

### 二、**题目描述**
> 算法是问题求解过程的一种描述，看看这个题目是怎么描述的吧。

### 三、**算法描述**
> 能说出基本思想即可,实在不行动手画个流程图把。

由于优先队列使用堆实现，可保证队列头是最小值

设矩阵有n行，这队列中有n个元素，每个元素都来自其中一行。初始化时，将每行的第0列压入

每次取出一个元素，根据元素是哪一行的，再将其所在行的下一个元素加入

执行k次出队列操作，即可得到结果

队列中每个元素为pair<int,pair<int,int>>，外层pair的int表示矩阵中的值，内层pair的first表示第几行，内层pair的second表示该行的第几列

### 四、**参考代码**
> 放轻松，虽然是c++实现，我们一贯宗旨是拒绝奇技淫巧，不懂代码一看就明白

~~~
//168 ms
class Solution
{
public:
    //仿函数
    struct compare
    {
        bool operator()(pair<int, pair<int, int>> &p1, pair<int, pair<int, int>> &p2)
        {
            // “大于” 构造的是最小堆
            return p1.first > p2.first;
        }
    };
    int kthSmallest(vector<vector<int>> &matrix, int k)
    {
        priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, compare> pq;
        int cols, rows;
        rows = matrix.size();
        cols = matrix[0].size();
        //将每一行的第0列入队
        for (int i = 0; i < rows; ++i)
            pq.push(make_pair(matrix[i][0], make_pair(i, 0)));

        int res = 0;
        while (k)
        {
            res = pq.top().first;
            //记录堆顶元素所在的行、列
            int r = pq.top().second.first;
            int c = pq.top().second.second;
            pq.pop();
            --k;
            if (c + 1 < cols)
                pq.push(make_pair(matrix[r][c + 1], make_pair(r, c + 1)));
        }
        return res;
    }
};

class Solution2
{
public:
    int kthSmallest(vector<vector<int>> &matrix, int k)
    {
        auto comp = [&matrix](pair<int, int> p1, pair<int, int> p2) {
            return matrix[p1.first][p1.second] > matrix[p2.first][p2.second];
        };
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(comp)> que(comp);
        que.push(make_pair(0, 0));
        int count = 1;
        while (count < k)
        {
            auto temp = que.top();
            que.pop();
            cout << "temp.first= " << temp.first << " temp.second " << temp.second << " value= " << matrix[temp.first][temp.second] << endl;
            if (temp.first + 1 < matrix.size())
            {
                que.push(make_pair(temp.first + 1, temp.second));
            }
            //if (temp.first == 0 && temp.second + 1 < matrix[0].size())
            if (temp.first == 0 && temp.second + 1 < matrix[0].size())
            {
                que.push(make_pair(temp.first, temp.second + 1));
            }
            count++;
        }
        auto t = que.top();
        return matrix[t.first][t.second];
    }

    /***  次方法虽然秒，但是我看不懂，并且体会里面好出，为什么大家都这样做。
      *  利用题目给出条件：有序  采用二分查找。
      *  
      *  二分查找关键 最大值，最小值,和中间值（卡住了）
      *  求top k  k是索引。array{k} ==中间值
      *  
      * 时间复杂度其实也是很高的
      * 1.找出二维矩阵中最小的数left，最大的数right，那么第k小的数必定在left~right之间
        2.mid=(left+right) / 2；在二维矩阵中寻找小于等于mid的元素个数count
        3.若这个count小于k，表明第k小的数在右半部分且不包含mid，即left=mid+1, right=right，又保证了第k小的数在left~right之间
        4.若这个count大于k，表明第k小的数在左半部分且可能包含mid，即left=left, right=mid，又保证了第k小的数在left~right之间
        5.因为每次循环中都保证了第k小的数在left~right之间，当left==right时，第k小的数即被找出，等于right 
          (76 ms ）
      */
    int kthSmallest1(vector<vector<int>> &matrix, int k)
    {
        int rows = matrix.size();
        if (0 == rows)
            return -1; //不存在
        int cols = matrix[0].size();
        //1 5 9 10 11 11 12 13 13 15
        // k=8 13
        // matrix = [
        //     *⁠[1, 5, 9],
        //     *⁠[10, 11, 13],
        //     *⁠[12, 13, 15] *
        // ],
        int low = matrix[0][0];
        int high = matrix[rows - 1][cols - 1];

        int mid = 0; //中间数值，array中是不存在的，通过计算是否第k小，变化low 和high的范围。

        while (low < high)
        {
            cout << low << "aaa" << mid << " " << high << endl;
            mid = low + (high - low) / 2;
            int count = findNotBiggerThanMid(matrix, mid);
            if (k > count)
            {
                low = mid + 1; //此时 low 变成可能不存。
            }
            else
            {
                high = mid;
            }
            cout << low << "bbb" << mid << " " << high << endl;
        }

        return low;
    }
    //查找一个元素
    int findNotBiggerThanMid(vector<vector<int>> &matrix, int x)
    {
        int N = matrix.size();
        int i = N - 1, j = 0, count = 0;
        while (i >= 0 && j < N)
        {
            if (matrix[i][j] <= x)
            {
                count += (i + 1);
                //排除列
                j++;
            }
            else
                //排除行
                i--;
        }
        return count;
    }
};
~~~




### 五、举一反三
> 别人最佳答案并不重要，关键是自己理解。



