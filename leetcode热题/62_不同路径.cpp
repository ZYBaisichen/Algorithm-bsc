/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-11 11:11:21
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
https://leetcode.cn/problems/unique-paths/

一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为 “Start” ）。

机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为 “Finish” ）。

问总共有多少条不同的路径？

 

示例 1：


输入：m = 3, n = 7
输出：28
示例 2：

输入：m = 3, n = 2
输出：3
解释：
从左上角开始，总共有 3 条路径可以到达右下角。
1. 向右 -> 向下 -> 向下
2. 向下 -> 向下 -> 向右
3. 向下 -> 向右 -> 向下
示例 3：

输入：m = 7, n = 3
输出：28
示例 4：

输入：m = 3, n = 3
输出：6
 

提示：

1 <= m, n <= 100
题目数据保证答案小于等于 2 * 109
*/

/*
dfs递归：但这种暴力会超时
只能往右或者往下走，dfs枚举所有的情况，且不用记录是否已经来到过，因为只向右和向下不可能重复
*/


class Solution {
public:
    int uniquePaths_baoli(int m, int n) {
        return dfs(m,n,0,0);
    }
    //当前来到了i,j位置, 走到(m-1,n-1)有多少种走法
    int dfs(int m, int n, int i, int j) {
        if (i>=m || j>=n) { 
            return 0;
        }
        if (i == m - 1 && j == n - 1) {
            //来到了末尾了，说明找到了一种走法
            return 1;
        }

        int ans = dfs(m,n,i+1,j);//往下走
        ans += dfs(m,n,i,j+1);
        return ans;
    }

    //两个可变参数，可以改成动态规划
    //dp[i][j]表示，当前来到了i,j位置，走到(m-1,n-1)有多少种走法
    //最后一行只能往右走，只有一种走法；最后一列依赖下边，且都是一种方法，就是往下走。
    //普通格子一来下一行，下一列
    int uniquePaths_dp(int m, int n) {
        vector<vector<int>> dp(m, vector<int>(n, 0));
        for (int i=0;i<m;i++) {
            dp[i][n-1] = 1;
        }
        for (int j=0;j<n;j++) {
            dp[m-1][j] = 1;
        }
        //从下往上，从右往左
        for (int i=m-2;i>=0;i--) {
            for (int j=n-2;j>=0;j--) {
                dp[i][j] = dp[i+1][j] + dp[i][j+1];
            }
        }
        return dp[0][0];
    }

    //数组压缩。
    int uniquePaths(int m, int n) {
        // 最后一行都是1
        vector<int> dp(n, 1);
        //从下往上，从右往左
        //dp[j]是下一行的值dp[i+1][j]
        for (int i=m-2;i>=0;i--) {
            for (int j=n-2;j>=0;j--) {
                dp[j] = dp[j] + dp[j+1];
            }
        }
        return dp[0];
    }

};
int main()
{

    Solution sol;
    return 0;
}