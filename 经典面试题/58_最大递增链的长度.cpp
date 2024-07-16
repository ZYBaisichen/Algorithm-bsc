/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-07-16 11:55:39
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

using namespace std;

/*
给定一个二维数组matrix，可以从任何位置触发，每一步可以走向上下左右四个方向，返回最大递增链的长度。
比如：
matrix={
    {5,4,3},
    {3,1,2},
    {2,1,3}
}
从最中心出发，是可以走1,2,3,4,5的链，而且是最长的递增链，所以返回长度为5

*/

class Solution
{
public:
    /*
    暴力：
        从每个位置出发(n*m)，上下左右递归走(n*m)得到一个答案，求最大的，复杂度是O((n*m)*(n*m))
    */
    int max_increase_path(vector<vector<int>>& matrix) {
        int n= matrix.size();
        int m= matrix[0].size();
        //遍历每个起始位置，求最大长度
        int max_len = 0;
        for (int i=0;i<n;++i) {
            for (int j=0;j<m;++j) {
                max_len = max(max_len, process(matrix, i, j));
            }
        }
        return max_len;
    }

    int process(vector<vector<int>>& matrix, int i, int j) {
        int n=  matrix.size();
        int m=  matrix[0].size();
        if (i<0||j<0||i>n||j>m) {
            return -1;
        }
        int next1 = 0;
        int next2 = 0;
        int next3 = 0;
        int next4 = 0;
        //上
        if (i-1>=0 && matrix[i-1][j]>matrix[i][j]) {
            next1 = process(matrix, i-1, j);
        }
        //下
        if (i+1<n && matrix[i+1][j]>matrix[i][j]) {
            next2 = process(matrix, i+1, j);
        }
        //左
        if (j-1>=0 && matrix[i][j-1]>matrix[i][j]) {
            next3 = process(matrix, i, j-1);
        }
        //右
        if (j+1<m && matrix[i][j+1]>matrix[i][j]) {
            next4 = process(matrix, i, j+1);
        }

        return 1 + max(max(next1, next2), max(next3, next4));
    }

    /*
    记忆化搜索
    因为加了缓存，所以每个位置只会算一次，复杂度是O(n*m)。
    主流程每个位置调用一次：
    递归第1次的时候调用走了k1的话，后面计算规模会减k1
    递归第2个的时候调用走了k2的话，后面计算规模会减k2
    ...
    最终k1+k2+...+1=n*m，外层循环n*m次，每次的计算会递减，最终收敛于O(n*m)

    因为依赖上下左右格子，还有可能循环依赖，所以不能改经典动态规划
    */
    int max_increase_path2(vector<vector<int>>& matrix) {
        int n= matrix.size();
        int m= matrix[0].size();
        vector<vector<int>> dp(n, vector<int>(m, -1));
        //遍历每个起始位置，求最大长度
        int max_len = 0;
        for (int i=0;i<n;++i) {
            for (int j=0;j<m;++j) {
                max_len = max(max_len, process2(matrix, i, j, dp));
            }
        }
        return max_len;
    }

    int process2(vector<vector<int>>& matrix, int i, int j, vector<vector<int>>& dp) {
        int n=  matrix.size();
        int m=  matrix[0].size();
        if (i<0||j<0||i>n||j>m) {
            return -1;
        }
        int next1 = 0;
        int next2 = 0;
        int next3 = 0;
        int next4 = 0;
        //上
        if (i-1>=0 && matrix[i-1][j]>matrix[i][j]) {
            if (dp[i-1][j] != -1) {
                next1 = dp[i-1][j];
            } else {
                next1 = process2(matrix, i-1, j,dp);
            }
        }
        //下
        if (i+1<n && matrix[i+1][j]>matrix[i][j]) {
            if (dp[i+1][j] != -1) {
                next2 = dp[i+1][j];
            } else {
                next2 = process2(matrix, i+1, j,dp);
            }
        }
        //左
        if (j-1>=0 && matrix[i][j-1]>matrix[i][j]) {
            if (dp[i][j-1] != -1) {
                next3 = dp[i][j-1];
            } else {
                next3 = process2(matrix, i, j-1,dp);
            }
        }
        //右
        if (j+1<m && matrix[i][j+1]>matrix[i][j]) {
            if (dp[i][j+1] != -1) {
                next4 = dp[i][j+1];
            } else {
                next4 = process2(matrix, i, j+1,dp);
            }
        }

        int ans = 1 + max(max(next1, next2), max(next3, next4));
        dp[i][j] = ans;
        return ans;
    }
};

int main()
{

    Solution sol;
    vector<vector<int>>  matrix = {
        {5, 4, 3},
        {3, 1, 2},
        {2, 1, 3}
    }; //5

    cout << sol.max_increase_path(matrix) << endl;
    cout << sol.max_increase_path2(matrix) << endl;

    return 0;
}