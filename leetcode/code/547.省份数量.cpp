/*
 * @lc app=leetcode.cn id=547 lang=cpp
 *
 * [547] 省份数量
 *
 * 1. 无向图的邻接矩阵
 *
 * 
 * 
 * 2. 防止死循环。
 * 1 --2
 * 2 --1 
 * 
 * 3. 这里为什么不（x,y) 顺时针遍历方式消除相连 visited。
 *
 */
// @lc code=start

class Solution
{
public:
    int findCircleNum(vector<vector<int>> &isConnected)
    {
        int count = 0;                     //最坏结果 n
        int rows = isConnected.size();     // rows ==cols
        vector<bool> visited(rows, false); //直接相连

        for (int i = 0; i < rows; i++)
        {
            if (visited[i] == false)
            {
                dfs(i, isConnected, visited);
                count++;
            }
        }

        return count;
    }

    void dfs(int i, vector<vector<int>> &isConnected, vector<bool> &visited)
    {
        visited[i] = true;

        for (int j = 0; j < isConnected.size(); j++)
        { // isConnected[i][j] = 1 表示第 i 个城市和第 j 个城市直接相连
            if (isConnected[i][j] == 1 && visited[j] == false)
            {
                // a[0][0] 自己和自己组成了一个朋友圈
                dfs(j, isConnected, visited);

            }
        }
        Ó
    }
};

func findCircleNum(isConnected [][]int) int {

    rows :=len(isConnected)
    //https://www.cnblogs.com/sparkdev/p/10704614.html
    visited :=make([]bool,rows); //n 个城市，
    count :=0
    /////// 这些写阅读不方便，但是节省2个参数传递
    var dfs func(k int)
	dfs = func(k int) {
        visited[k] = true 
		for i := range isConnected[k] {
			c := isConnected[k][i]
			if c == 0 || visited[i] {
				continue
			}
			dfs(i)
		}
	}
    ////////

    for i:=0;i<rows;i++{
        if visited[i]!=true{
          count++
          dfs(i)
        }
    }

    return count
}


class Solution {
    public int findCircleNum(int[][] isConnected) {
        // int[][] isConnected 是无向图的邻接矩阵，n 为无向图的顶点数量
        int n = isConnected.length;
        // 定义 boolean 数组标识顶点是否被访问
        boolean[] visited = new boolean[n];
        // 定义 cnt 来累计遍历过的连通域的数量
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            // 若当前顶点 i 未被访问，说明又是一个新的连通域，则遍历新的连通域且cnt+=1.
            if (!visited[i]) { 
                cnt++;
                dfs(i, isConnected, visited);
            }
        }
        return cnt;
    }

    private void dfs(int i, int[][] isConnected, boolean[] visited) {
        // 对当前顶点 i 进行访问标记
        visited[i] = true;
        
        // 继续遍历与顶点 i 相邻的顶点（使用 visited 数组防止重复访问）
        for (int j = 0; j < isConnected.length; j++) {
            if (isConnected[i][j] == 1 && !visited[j]) {
                dfs(j, isConnected, visited);
            }
        }
    }
}
时间复杂度：O(n^2)其中 n 是城市的数量。需要遍历矩阵 n 中的每个元素。
空间复杂度：O(n)，其中 n 是城市的数量。需要使用数组标记是否访问。 

// @lc code=end
