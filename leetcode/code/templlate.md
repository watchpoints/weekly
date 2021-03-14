

###  一、题目

> 完成比完美更重要



#### [332. 重新安排行程](https://leetcode-cn.com/problems/reconstruct-itinerary/)

思考 60秒 。。。

思考 60秒 。。



### 二、思路

> 第一感觉是什么，放弃奇淫技巧。



### 图解：



#### 步骤：





 #### 复杂度：



###  三、 代码



- python

  ~~~python
  import collections
  
  
  class Solution:
      def findItinerary(self, tickets: List[List[str]]) -> List[str]:
  
          # 核心思想--深度搜索+回溯
          # 首先先把图的邻接表存进字典，然后对字典的value进行排序
          # 然后从'JFK'开始深搜，每前进一层就减去一条路径，
          # 直到某个节点不存通往其他节点的路径时，说明该节点就为此次行程的终点
          # 需要跳出while循环进行回溯，返回到上一层节点进行搜索，再次找到倒数第二个终点，依次类推
          # 设定ans为返回答案，每次找到的节点都要往头部插入
  
          # 举例说明：[["JFK","SFO"],["JFK","ATL"],["SFO","ATL"],["ATL","JFK"],["ATL","SFO"]]
          # 构建领接表的字典：{'JFK': ['SFO', 'ATL'], 'SFO': ['ATL'], 'ATL': ['JFK', 'SFO']})
          # 按照题目要求对字典的val排序：{'JFK': ['ATL', 'SFO'], 'SFO': ['ATL'], 'ATL': ['JFK', 'SFO']})
          # 开始从 JFL 开始进行dfs搜索
          # 1.JFK --> ALT  
          #   JFK pop出ALT,JFK的字典变为：'JFK': ['SFO']
          # 2.JFK --> ALT --> JFK 
          #   ALT pop出JFK,ALT的字典变为：'ALT': ['SFO']
          # 3.JFK --> ALT --> JFK --> SFO 
          #   JFK pop出SFO,JFK的字典变为：'JFK': ['']
          # 4.JFK --> ALT --> JFK --> SFO --> ATL 
          #   SFO pop出ALT,SFO的字典变为：'SFO': ['']
          # 5.JFK --> ALT --> JFK --> SFO --> ATL --> SFO
          #   ATL pop出SFO,ATL的字典变为：'ATL': ['']
          # 此时我们可以发现SFO的val为空，没有地方可以去了，说明我们已经找出了终点SFO
          # 然后向上回溯，依次将其添加到ans中
          # 最终答案为：["JFK","ATL","JFK","SFO","ATL","SFO"]
  
  
          d = collections.defaultdict(list)   #邻接表
          for f, t in tickets:
              d[f] += [t]  # 路径存进邻接表
  
          for f in d:
              d[f].sort()  # 邻接表排序
  
          ans = []
          def dfs(f):  # 深搜函数
              while d[f]:
                  dfs(d[f].pop(0))  # 路径检索            
              ans.insert(0, f)  # 放在最前
  
          dfs('JFK')
          return ans
  
  作者：charlesd
  链接：https://leetcode-cn.com/problems/reconstruct-itinerary/solution/332-zhong-xin-an-pai-xing-cheng-chao-xiang-xi-ti-j/
  来源：力扣（LeetCode）
  著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
  
  潜台词：
  
  图的存储结构主要分两种，一种是邻接矩阵，一种是邻接表
  
  ~~~

  



### 四、 举一反三




> 分享最实用的经验 ， 希望每一位来访的朋友都能有所收获！
https://github.com/wangcy6/leetcode
>大家也可以关注我的公众号《程序猿思维》，获取更多资料
> 如果有疑问请联系我，一起探讨，进步。
>