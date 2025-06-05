---
title: 冲刺2025年CSP-J/S和刷Leetcdoe找工作（1）：拓扑排序-课程表
date: 2025-04-07
description: c++
draft: false
tags:
  - 成为孩子榜样，为国争光
categories:
---


>知识地图：从信奥赛 CSP-J/S到Leetcode  拓扑排序-课程表
>题目难度：这个是高中省级比赛 ，题目有点难，小学组不要对号入座，

各位老师好 ，我是小义, 今天是6月4 

1. 距离2025年9月16日第38次CCF计算机软件能力考试**CSP-J/S** 还有104天
2. 考试报名：2025年5月12日--2025年6月1日 错过的，今年继续准备。
3. 孩子们，无论报名成功的还是没有报名成功的，从现在准备的充分
	去CSP-J/S战场上检验自己的学习成果。
4. 距离**2025年毕业季还有24天** （不同学校不同都在6月或者7月中旬左右）
5. 实习找工作师弟师妹 抓紧了， Leetcode刷题必不可少。

> 1. 小学初中：参加信奥比赛想要CSP-J/S 想要历年真题 
> 2. 校招：找工作实习想要内推码
> 3. 社招：大厂内推
> 联系我 ，围绕数据结构与算法 这个事情一起努力




![](https://s2.loli.net/2025/06/04/Ca5PuXSsIwdl62R.png)






本文大纲：
第一步：建立大局观  刷题有什么用
第二步：构建知识地图: 针对知识点有效训练
第三步：专心刷1题 拓扑排序-课程表

# 第一步：把枯燥刷题变成伟大事情（有啥用 阅读耗时2分钟）

成为孩子榜样，为国争光

![](https://s2.loli.net/2025/06/04/Nnv6wQHIlDkAVoW.png)
信息学奥赛，简称为“信奥”，说来话长；
1984年邓小平同志提出：“**计算机的普及要从娃娃做起。”**
于是，中国计算机学会在当年就创办了全国青少年计算机程序设计竞赛，
本质上就是，**面向中学生的计算机编程大赛**

竞赛旨
**在向那些在中学阶段学习的青少年普及计算机科学知识**
**给那些有才华的学生提供相互交流和学习的机会**；

​**​使用场景与目标人群​**​
- ​**​NOI​**​：主要服务于中学生竞赛升学
- ​**​LeetCode​**​：面向求职者（尤其是软件工程师）及编程爱好者，用于提升面试通过率或日常技能训练。

## 2.1 刷题Leetcode找工作，进大厂，去外企

- 面试大厂必要条件，太卷了
- 去亚马逊，微软等外企必要条件
- 一定要刷题吗？不一定，创业公司 更看重项目经验
- 无论什么公司，数据结构与算法都是基本功
## 2.2  辅导自己小孩，邻居小孩，社区小孩 参加CSP-J比赛


   CSP、NOIP、NOI这三个比赛都是由中国计算机学会（CCF）举办的，
   代表同学们在编程竞赛上的不同阶段：
    1. CSP-J（入门级，Junior) ：主要针对小学和初中生
    2. CSP-S（提高级，Senior）：CSP-S面向高中生
    3. NOIP：省选，NOIP名额有限,NOIP是参加NOI的必要条件
    4. NOI：国赛,为国争光

![](https://s2.loli.net/2025/05/28/Wd8VDlEsw731nmp.png)



从CSP-J/S到NOI，信竞之路一路走来实属不易。
希望走信息学路线的孩子都能抓住每一次比赛机会，夺得奖项。
纵使有成绩不如意的时候，也要戒骄戒躁，不要轻言放弃。


## 2.3 等级划分

### **CSP 入门级**

CSP 入门级（CSP-J），前身是 NOIP 普及组，是 NOI 系列赛事中难度最低，面向年龄最低的赛事，它是很多学生参与的第一个信息学的大型比赛。

- 适宜年龄：11-14 岁。
- 比赛时间：第一轮为 9 月，第二轮为 10 月。
- 比赛形式：第一轮为 2 小时的笔试，第二轮为 3.5 小时的上机测试。全国统一命题，省级评奖。
- 竞赛优惠：对小升初、初升高皆有帮助（取决于地方性政策）。部分知名初高中会对在 CSP-J 比赛中取得优秀成绩的选手（一般来说是一等）提供录取或者分班优惠政策。
- 参考训练量：于“洛谷”上完成 80 个“普及”难度的试题。历届试题可在“洛谷”上搜索。

### **CSP 提高级**

CSP 提高级（CSP-S）主要面向广大的初高中生，难度较入门级有着显著提升，其含金量也显著更高。CSP-S 的成绩是学生参与后续的 NOI 系列赛事的重要凭证。

- 适宜年龄：12-17 岁。
- 比赛时间：与 CSP 入门级相同，第一轮为 9 月，第二轮为 10 月。
- 比赛形式：第一轮为 2 小时的笔试，第二轮为 4 小时的上机测试。全国统一命题，省级评奖。
- 竞赛优惠：对初升高有很大帮助。在 CSP-S 中取得优秀成绩（一般也是一等）的学生多会被顶级高中（如北京人大附中、上海华师大二附中、浙江杭州二中）等特招。
- 参考训练量：于“洛谷”上完成 150 个“提高”难度的试题，50 个“提高+”难度的试题。历届试题可在“洛谷”上搜索。

### **NOIP**

全国青少年信息学奥林匹克联赛（NOIP）面向高中生群体，难度较 CSP-S 有一定提升，是绝大多数参与信息学竞赛的选手能够接触到的含金量最高的赛事。
- 适宜年龄：15-17 岁。
- 比赛时间：11 月中下旬。
- 比赛形式：4.5 小时的上机测试。全国统一命题，省级评奖。
- 竞赛优惠：NOIP 中获得一等奖将会在知名大学的少创班、综合评价测试、专项初审环节中取得显著优势。
- 参考训练量：于“洛谷”上完成 **200** 个“提高”难度的试题，100 个“提高+”难度的试题。历届试题可在“洛谷”上搜索。

- 
完整地址：https://docs.qq.com/sheet/DY1NwcGZjUXpYb01j?tab=oqi27u


### Leetcode刷题找工作 
- 必须完成300题目
### 资源：
- http://ybt.ssoier.cn:8088/
- https://leetcode.cn/
- https://www.luogu.com.cn/
- https://www.nowcoder.com/
- https://www.cnblogs.com/kkman2000/p/18806934
- https://github.com/liweiwei1419/LeetCode-Solution-PPT
- 大话数据结构：https://github.com/anliux/Play-With-Data-Structure
- https://github.com/youngyangyang04/leetcode-master
- https://github.com/watchpoints/LeetCode-CSP-NOI
- https://github.com/watchpoints/noi_and_leetcode/tree/main
- 数据 结构与算法（c 语言版本）https://github.com/kangjianwei/Data-Structure
https://github.com/TheAlgorithms/Python
- https://github.com/MoonShadowzzc/Data-Structure


## 第二步：构建知识地图( 怎么做，约定耗时 2分钟)


### CSP-J/S

![](https://s2.loli.net/2025/06/04/cgandCrzOXmtDiR.png)

**注意区分** 
**是小学，初中 还是高中 不同阶段，有些资料没有标准学习阶段，导致让人产生误解**
![](https://s2.loli.net/2025/06/04/FAv1R598VinfYmk.jpg)
![](https://s2.loli.net/2025/06/04/GTmdNnUDfxAws1v.jpg)

完整内容：https://docs.qq.com/sheet/DY1NwcGZjUXpYb01j

![image.png](https://s2.loli.net/2025/06/04/DR9hv2mKjUCxGeT.png)

![](https://s2.loli.net/2025/06/04/TkIg5BDArEsNQ6t.png)
### 数据结构与算法 leetcode
![](https://s2.loli.net/2025/05/29/NSihvB9Pesb4CaY.png)


## 第三步：刷题日记：专心做一题

[207. 课程表](https://leetcode.cn/problems/course-schedule/)
![image.png](https://s2.loli.net/2025/05/29/VyJIvQL415koA83.png)



There are a total of `numCourses` courses you have to take, 
labeled from `0` to `numCourses - 1`. 
You are given an array `prerequisites` where `prerequisites[i] = [ai, bi]` 
indicates that you **must** take course `bi` first if you want to take course `ai`.

For example, the pair `[0, 1]`, indicates that to take course `0` you have to first take course `1`.
Return `true` if you can finish all courses. 
Otherwise, return `false`.



![](https://s2.loli.net/2025/05/29/CzVBRsYiFTyPHpm.png)

这种叫 **有向无环图**，把一个 有向无环图 转成 线性的排序 就叫 **拓扑排序**

#### 图的存储 

![来源 小灰的数据结构与算法30讲](https://s2.loli.net/2025/05/29/gtlFDMwvP51sG8K.png)

```
名字：图（Graph）

数据对象集：G( V, E )由一个非空的有限顶点集合V和一个有限边集合E组成。

操作集合：对于任意图G∈Graph，以及v∈V，e∈E 。

Graph Create()：建立并返回空图；

Graph InsertVertex(Graph G , Vertex v)：将v插入G;

Graph InsertEdge（Graph G , Edge e）：将e挿入G;

void DES( Graph G  , Vertex v )：从顶点v出发深度优先遍历图G;

void BES( Graph G  , Vertex v )：从顶点v出发宽度优先遍历图G;

void ShortestPath( Graph G, Vertex v ,int Dist [ ] ): 
计算图G中顶点v到任意其他顶点的最短距离；

void MST( GraphG )：
计算图G的最小生成树

```

### 图遍历

![](https://s2.loli.net/2025/05/29/ZgEzvXKpNd2ToMU.png)

![图的遍历](https://s2.loli.net/2025/05/29/CT7NZwEAs6cvp5z.png)

![image.png](https://s2.loli.net/2025/05/29/U7rhsQjdM3WywYb.png)


![遇到卡点了](https://s2.loli.net/2025/05/29/I1KwSoEeaixsVPc.png)


回归：# 漫画：深度优先遍历 和 广度优先遍历
https://cloud.tencent.com/developer/article/1618700?policyId=1003



![image.png](https://s2.loli.net/2025/05/29/ejD4aNWgEVAsGRw.png)




完整代码：

```c++
class Solution {

private:

map<int,vector<int>> graph;

// 图的存储结构--邻居表 数组，链表，

//为了方面表示 用 map 代替数组，vecotr 代替 链表

//1 <= numCourses <= 2000

  

//vector<bool> visited;

// false 没有访问过 ，true访问过

//解决​​：维护visited集合或数组，标记已访问节点。

//如果有向无环图：每个节点只访问一次 ，不会重复遍历。

//如果有向有环图：出现部分节点重复遍历，死循环情况，这个存在环，结束循环。

// 踏平坎坷，成大道

// 斗罢艰险，又出发，又出发

//实际结果：

//1. 首先了解课表提供基本概念和算法 2 动手写这些算法 ，这个基本测试用例，正常情况下。

//3. 如果写更多测试用例 leetcode 提供了 写过程遇到bug 极端情况 更深入一步思考这个价值地方。【核心的地方 】

//

  

vector<int> visited; //0没有访问 1 正在访问中，包括邻居节点访问 如果回到到存在环 2 没有环，访问完成。如果下次访问时候 不需要在遍历

public:

bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {

//其中 prerequisites[i] = [数据结构,C++],

//表示如果要学习课程 数据结构则 必须 先学习课程 C++，是前置条件 ,

//如果有多个前置条件呢？例如 [数据结构,c],1

//1对多，就需要图这个结构

//疑问 1：tree 也是 1 对多为什么不用 tree 呢：tree 有严格层级关系 无法表示 一个节点和剩余 n-1 个节点直接关系。

// 图是 节点之间多对对复杂网络结构

// i:每个课程 编号 ，是不是 更抽象一层。疑问 2：最后发现可能跟课程名字没关系，为什么题目给出n 个课程 ，但是没有给出课程名字，

//疑问3: 难后容易 ，还是先容易后难，课程安排 l0 l1，l2

// 节点 与节点之间关系是：i--->j 学完课程 i 之后 ，才可以学习课程 和输入好想法。数据结果中 有向图 <--- 这个方向不好表示

  

for(auto item:prerequisites)

{

int key = item[1]; //先完成课程，容易完成

int value = item[0];//后完成课程 ，难完成的

graph[key].push_back(value); // 图的存储结构--邻居表 数组+链表

}

  

//输入：numCourses = 2, prerequisites = [[1,0],[0,1]]

//输出：false

//疑问4：图的深度有限遍历，进入死胡同 前面无路可走，和 存在环 前面有路 鬼打墙 区别？

//只访问一遍原则：有向无环图（死胡同 前面无路可走） vs 有向有环图 （ 前面有路 鬼打墙）

//回答：backtrace(栈回溯)保证 前方有路就走

// 你挑着担，我牵着马

// 迎来日出，送走晚霞

// 踏平坎坷，成大道

// 斗罢艰险，又出发，又出发

// 啦啦…… ……

// 一番番春秋、冬夏。

// 一场场酸甜、苦辣。

// 敢问路在何方 路在脚下。

// 敢问路在何方 路在脚下。

//这句词源于鲁迅《故乡》中的“其实地上本没有路，走的人多了，也便成了路”

//解决​​：维护visited集合或数组，标记已访问节点。

  

//分配大小numCourses空间，初始化每个变量为 false 类似 避免频繁内存分配带来的性能损耗

visited.resize(numCourses,0);

  
  

//疑问 5：从哪里开始，二叉树 只有一个root 节点很清楚 ，图 n 个节点？

//为什么每个节点都遍历一遍？防止孤岛，不联通情况--- 这个课程和其他课程没有任何关系。

for(int index = 0; index < visited.size(); index++)

{

if(visited[index] == 0) //no visited

{

if (false == dfs(index))

{

return false ;//# 终止“鬼打墙” a-->b-->a

}

}

// }else {

// cout<<"22222";

// return false ;//# 终止“鬼打墙” 这个行代码 永远不会执行 dfs（index）访问一次 for 全部节点 访问 1次 正常2 次 访问。

// }

}

  

return true;

  

}

//从节点 index 开始 深度优先搜索 访问相关节点

bool dfs(int index)

{

if(visited[index] == 2) {

return true;

} //why?

if(visited[index] == 1) {

return false; //a--b--a

}

  

visited[index] = 1;

vector<int> &neight = graph[index];

//change dirction

for(int i =0;i<neight.size();i++){

if (false == dfs(neight[i])) {

return false;

}

}

  

visited[index] = 2;

  

return true;

}

};
```


### 链接我

最动人的作品，为自己而写，刚刚好打动别人
刚刚好，是最难得的美好

![免费答疑](https://s2.loli.net/2025/05/29/zicSGNqxageP2os.png)

