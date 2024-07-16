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

using namespace std;
void print(vector<int>& res, string suf = "res") {
    cout << suf;
    for (auto iter:res) {
        cout << " " << iter;
    }
    cout << endl;
}

// 最小路径和
// 给定一个包含非负整数的 m x n 网格 grid
// 请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。
// 说明：每次只能向下或者向右移动一步。
// 测试链接 : https://leetcode.cn/problems/minimum-path-sum/

/*
输入：grid = [[1,3,1],[1,5,1],[4,2,1]]
输出：7
解释：因为路径 1→3→1→1→1 的总和最小。
*/

class Solution {
public:
    // 暴力递归
	int minPathSum1(vector<vector<int>>& grid) {
        int n = grid.size();
        if (n == 0) {
            return 0;
        }
        int m = grid[0].size();
        if (m==0) {
            return 0;
        }

		return f1(grid, n-1, m-1);
	}
    
    //返回从(0,0)到i,j的最短路径之和
    int f1(vector<vector<int>>& grid, int i, int j) {
        if (i<0 || j<0 || i>=grid.size() || j>=grid[0].size()) {
            // cout << "asfsdf" <<endl;
            return INT_MAX;
        }
        //到达自己，活的grid[0][0]的分数
        if (i==0 && j==0) {
            return grid[0][0];
        }
        
        //普遍位置依赖左边和上边的最短路径+1
        int up = f1(grid, i-1, j);
        int left = f1(grid, i, j-1);
        // cout << "=======" << endl;
        // cout << "i:" << i << " j:" << j << endl;
        // cout << "up: " << up << " left:" << left  << " res:" << min(up, left) + 1<< endl;

        return min(up, left) + grid[i][j];
    }

    // 记忆化搜索
	int minPathSum2(vector<vector<int>>& grid) {
        int n = grid.size();
        if (n == 0) {
            return 0;
        }
        int m = grid[0].size();
        if (m==0) {
            return 0;
        }
        vector<vector<int>> dp(n, vector<int>(m, INT_MAX));

		return f2(grid, n-1, m-1,dp);
	}
    
    //返回从(0,0)到i,j的最短路径之和。加了个记忆化缓存dp数组
    int f2(vector<vector<int>>& grid, int i, int j, vector<vector<int>>& dp) {
        if (i<0 || j<0 || i>=grid.size() || j>=grid[0].size()) {
            // cout << "asfsdf" <<endl;
            return INT_MAX;
        }
        if (dp[i][j] != INT_MAX) {
            return dp[i][j];
        }
        if (i==0 && j==0) {
            return grid[0][0];
        }
        
        //普遍位置依赖左边和上边的最短路径+1
        int up = f2(grid, i-1, j, dp);
        int left = f2(grid, i, j-1, dp);
        // cout << "=======" << endl;
        // cout << "i:" << i << " j:" << j << endl;
        // cout << "up: " << up << " left:" << left  << " res:" << min(up, left) + 1<< endl;

        return min(up, left) + grid[i][j];
    }

    // 改动态规划
	int minPathSum3(vector<vector<int>>& grid) {
        int n = grid.size();
        if (n == 0) {
            return 0;
        }
        int m = grid[0].size();
        if (m==0) {
            return 0;
        }
        //dp[i][j]表示从(0,0)到达(i，j）的路径和
        vector<vector<int>> dp(n, vector<int>(m, INT_MAX));
        dp[0][0] = grid[0][0];
        //根据递归逻辑，依赖上方和左边的元素，所以从上到下，从左到右填格子
        for (int i=0;i<n;i++) {
            for (int j=0;j<m;j++) {
                if (i==0&&j==0) {
                    continue;
                }
                int up = INT_MAX;
                if (i-1 >= 0) {
                    up = dp[i - 1][j];
                }
                int down = INT_MAX;
                if (j-1 >= 0) {
                    down = dp[i][j-1];
                }
                // cout << "i:" << i << " j:" << j << " up:" << up << " down:" << down << endl;
                dp[i][j] = min(up, down) + grid[i][j];
            }
        }

		return dp[n-1][m-1];
	}


    // 动态规划状态压缩
	int minPathSum4(vector<vector<int>>& grid) {
        int n = grid.size();
        if (n == 0) {
            return 0;
        }
        int m = grid[0].size();
        if (m==0) {
            return 0;
        }
        // 根据递归逻辑，依赖上方和左边的元素，所以从上到下，从左到右填格子
        // 同一行只会依赖左边的元素，所以使用一个元素，从第一行开始，滚动更新到底n行

        vector<int> dp(m, INT_MAX);
        dp[0] = grid[0][0];
        for (int i=0;i<n;i++) { //滚动更新n行
            for (int j=0;j<m;j++) {
                if (i==0&&j==0) {
                    continue;
                }
                int left = INT_MAX;
                if (j-1>=0) {
                    left = dp[j-1];
                }
                // cout << "i:" << i << " j:" << j << " up:" << dp[j] << " left:" << left << endl;
                dp[j] = min(left, dp[j]) + grid[i][j]; //右边的dp[j]为第i-1行的j
            }
        }
        return dp[m-1];
	}
};

int main()
{
    Solution sol;
    // vector<ll> arr = {100,1500,500,500,1000};
    // ll big = 1000;
    // vector<vector<int>> grid = {
    //     {1, 3, 1}, 
    //     {1, 5, 1}, 
    //     {4, 2, 1}};
    vector<vector<int>> grid = {
        {1,2,3},
        {4,5,6}
        };
    cout << sol.minPathSum1(grid) << endl;
    cout << sol.minPathSum2(grid) << endl;
    cout << sol.minPathSum3(grid) << endl;
    cout << sol.minPathSum4(grid) << endl;
    return 0;
}