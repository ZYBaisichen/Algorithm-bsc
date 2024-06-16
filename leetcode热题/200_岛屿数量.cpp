/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
 * @LastEditors: baisichen
 * @Description:
 */
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <set>
#include <ctime>
#include <cstdlib>
#include <map>
#include <limits.h>
#include <deque>
#include <queue>
#include <algorithm>
#include <cmath>
#include <functional>
#include <unordered_map>

using namespace std;

/*
参考并查集笔记。

https://leetcode.cn/problems/number-of-islands/description/
给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。

岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。

此外，你可以假设该网格的四条边均被水包围。

 

示例 1：

输入：grid = [
  ["1","1","1","1","0"],
  ["1","1","0","1","0"],
  ["1","1","0","0","0"],
  ["0","0","0","0","0"]
]
输出：1
示例 2：

输入：grid = [
  ["1","1","0","0","0"],
  ["1","1","0","0","0"],
  ["0","0","1","0","0"],
  ["0","0","0","1","1"]
]
输出：3
 

提示：

m == grid.length
n == grid[i].length
1 <= m, n <= 300
grid[i][j] 的值为 '0' 或 '1'

*/

/*


*/
class Solution {
public:
    /*
    初步想法是dfs+减枝。
    1. 每次从一个没有到达过的节点出发，总结过加1；
    2. 每经过一个节点将该节点的状态置为已访问，都置为2。
    3. 从当前(i,j)节点出发，往上下左右是1的位置，扩展，直到不能再扩为止。
    每个节点最多访问两次，从其他方向过来访问一次；大流程判断是否是'1'访问一次，总复杂度是O(m*n)
    */
    int numIslands(vector<vector<char>>& grid) {
        int n = grid.size();
        if (n == 0) {
            return 0;
        }
        int m = grid[0].size(); 
        if (m==0) {
            return 0;
        }
        int ans = 0;
        for (int i=0;i<n;i++) {
            for (int j=0;j<m;j++) {
                if (grid[i][j] == '1') {
                    ans++;
                    //将这块连着的陆地置为2
                    process(grid, i, j);
                }
            }
        }
        return ans;
    }

    void process(vector<vector<char>>& grid, int i, int j) {
        int n = grid.size();
        int m = grid[0].size(); 
        if (i>=n || i<0 || j>=m || j<0) {
            return;
        }
        grid[i][j] = '2';
        if (i-1>=0 && grid[i-1][j]=='1') {
            process(grid, i-1, j);
        }
        if (i+1<n && grid[i+1][j]=='1') {
            process(grid, i+1, j);
        }
        if (j-1>=0 && grid[i][j-1]=='1') {
            process(grid, i, j-1);
        }
        if (j+1<m && grid[i][j+1]=='1') {
            process(grid, i, j+1);
        }
    }


    /*
    这个问题在单cpu下O(m*n)复杂度已经是最优的了。但在多cpu场景下可以将整个数组分成多份。
    比如有2个cpu，将数组竖着一分为二，分别交由一个cpu计算，使用并查集记录下来每个'1'节点所属集合。
    然后在两个子数组边界处，处理为'1'的节点，如果相邻，则将两个节点所在的集合合并，合并过程是O(n)。
    总体复杂度是O(n*m)，但用了两个cpu并行处理，每个cpu计算O(n*m/2)，总时间缩小了一半
    */
};

int main()
{

    Solution sol;
    return 0;
}
