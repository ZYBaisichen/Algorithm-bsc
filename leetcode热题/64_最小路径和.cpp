/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-11 12:02:04
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
https://leetcode.cn/problems/minimum-path-sum/description/
和经典面试题16题的题目一样，这里用另外一种思维来解。 但貌似没有从(0,0)出发的好


给定一个包含非负整数的 m x n 网格 grid ，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。

说明：每次只能向下或者向右移动一步。

示例 1：


输入：grid = [[1,3,1],[1,5,1],[4,2,1]]
输出：7
解释：因为路径 1→3→1→1→1 的总和最小。
示例 2：

输入：grid = [[1,2,3],[4,5,6]]
输出：12
 

提示：

m == grid.length
n == grid[i].length
1 <= m, n <= 200
0 <= grid[i][j] <= 200
*/


/*

暴力递归：


*/


class Solution {
public:
    int minPathSum_baoli(vector<vector<int>>& grid) {
        return dfs(grid, 0, 0);
    }
    //当前来到了(i,j)，走到(m-1,n-1)的最小路径和
    int dfs(vector<vector<int>>& grid, int i, int j) {
        int n = grid.size();
        int m = grid[0].size();
        if (i>=n || j>= m) {
            return INT_MAX;
        }
        if (i==n-1 && j == m-1) {
            return grid[i][j];
        }

        int down = dfs(grid, i+1, j); //往下走
        int right = dfs(grid, i, j+1); //往右走
        return grid[i][j] + min(down, right);
    }

    //改造成动态规划
    //dp[i][j]代表当前将要来到了i,j，走到(n-1,m-1)的最小路径和
    int minPathSum_dp(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        vector<vector<int>> dp(n, vector<int>(m, INT_MAX));
        dp[n-1][m-1] = grid[n-1][m-1];

        //最后一行
        for (int j=m-2;j>=0;j--) {
            dp[n-1][j] = dp[n-1][j+1] + grid[n-1][j];
        }

        //最后一列
        for (int i=n-2;i>=0;i--) {
            dp[i][m-1] = dp[i+1][m-1] + grid[i][m-1];
        }
        //从下往上，从右往左
        for (int i=n-2;i>=0;i--) {
            for (int j=m-2;j>=0;j--) {
                dp[i][j] = min(dp[i+1][j], dp[i][j+1]) + grid[i][j];
            }
        }

        return dp[0][0];
    }

    
    void print_arr(vector<int>& arr, int i) {
        cout << "i:"<<i << endl;
        for (auto it:arr) {
            cout << it << " ";
        }
        cout << endl;
    }
    //空间压缩
    int minPathSum(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        vector<int> dp(m, INT_MAX);
        //最后一行
        dp[m-1] = grid[n-1][m-1];

        //最后一行
        for (int j=m-2;j>=0;j--) {
            dp[j] = dp[j+1] + grid[n-1][j];
        }
        // print_arr(dp, n-1);

        //从下往上，从右往左
        for (int i=n-2;i>=0;i--) {
            for (int j=m-1;j>=0;j--) {
                if (j == m-1) {
                    dp[j] = dp[j] + grid[i][j];
                }  else {
                    dp[j] = min(dp[j], dp[j+1]) + grid[i][j];
                }
            }
            // print_arr(dp, i);
        }

        return dp[0];
    }
};
int main()
{

    Solution sol;

    vector<vector<int>> g ={{1,3,1},{1,5,1},{4,2,1}};
    cout << sol.minPathSum(g) << endl;
    return 0;
}