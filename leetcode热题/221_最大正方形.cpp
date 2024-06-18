/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 11:55:04
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


https://leetcode.cn/problems/maximal-square/description/

在一个由 '0' 和 '1' 组成的二维矩阵内，找到只包含 '1' 的最大正方形，并返回其面积。

 

示例 1：


输入：matrix = [["1","0","1","0","0"],["1","0","1","1","1"],["1","1","1","1","1"],["1","0","0","1","0"]]
输出：4
示例 2：


输入：matrix = [["0","1"],["1","0"]]
输出：1
示例 3：

输入：matrix = [["0"]]
输出：0
 

提示：

m == matrix.length
n == matrix[i].length
1 <= m, n <= 300
matrix[i][j] 为 '0' 或 '1'
*/

/*

通过枚举左上角和右下角，可以枚举出来所有的正方形。假设枚举出来一个正方形，里面全是1的数量就是它的面积。 

dp[i][j]表示以i,j为右下角时，能有最多1的数量的正方形边长。
dp[i-1][j]表示上方以[i-1,j]为右下角时，最多能往外延伸全是1的正方形边长。
dp[i][j-1]表示上方以[i,j-1]为右下角时，最多能往外延伸全是1的正方形边长。
dp[i-1][j-1]表示上方以[i-1,j-1]为右下角时，最多能往外延伸全是1的正方形边长。
以上三者取最小值然后加1，就是[i,j]为右下角时的最大边长
*/
class Solution {
public:
    int maximalSquare_dp(vector<vector<char>>& matrix) {
        int m = matrix.size();
        if (m==0) {
            return 0;
        }
        int n = matrix[0].size();
        if (n==0) {
            return 0;
        }
        vector<vector<int>> dp(m, vector<int>(n, 0));
        int ans = 0;
        //第一行
        for (int j=0;j<n;j++) {
            if (matrix[0][j] == '1') {
                dp[0][j] = 1;
                ans = 1;
            }
        }
        //第一列
        for (int i=0;i<m;i++) {
            if (matrix[i][0] == '1') {
                dp[i][0] = 1;
                ans = 1;
            }
        }

        for (int i=1;i<m;i++) {
            for (int j=1;j<n;j++) {
                if (matrix[i][j] == '1') {
                    // cout << "i:" << i << ",j:"<< j  << " :" << dp[i-1][j-1] << "," <<dp[i-1][j] << "," << dp[i][j-1] << endl;
                    dp[i][j] = min(dp[i-1][j-1], min(dp[i-1][j], dp[i][j-1])) + 1;
                    ans = max(ans, dp[i][j]);
                }
            }
        }

        // for (int i=0;i<m;i++) {
        //     for (int j=0;j<n;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return ans*ans;
    }

    //空间压缩
    int maximalSquare(vector<vector<char>>& matrix) {
        int m = matrix.size();
        if (m==0) {
            return 0;
        }
        int n = matrix[0].size();
        if (n==0) {
            return 0;
        }
        vector<int> dp(n, 0);
        int ans = 0;
        //第一行
        for (int j=0;j<n;j++) {
            if (matrix[0][j] == '1') {
                dp[j] = 1;
                ans = 1;
            }
        }

        int pre = 0;
        for (int i=1;i<m;i++) {
            //先来第一列
            pre = dp[0];//记录左上角值
            if (matrix[i][0] == '1') {
                dp[0] =  1;
                ans = max(ans, 1);
            } else {
                dp[0] = 0;
            }
            int tmp = 0;
            for (int j=1;j<n;j++) {
                if (matrix[i][j] == '1') {
                    // cout << "i:" << i << ",j:"<< j  << " :" << pre << "," <<dp[j] << "," << dp[j-1] << endl;
                    tmp = dp[j]; //下面对其修改之前，先记录下dp[j]的原始值，
                    dp[j] = min(pre, min(dp[j], dp[j-1])) + 1; //左上角，上方，右方
                    pre = tmp;
                    ans = max(ans, dp[j]);
                } else {
                    dp[j] = 0;
                }
            }
            // cout << "i:"<< i << ":";
            // for (int j=0;j<n;j++) {
            //     cout << dp[j] << " ";
            // }
            // cout << endl;
        }

        return ans*ans;
    }
};

int main()
{

    Solution sol;
    vector<vector<char>> matrix = {
        {'1','1','1','1','1'},
        {'1','1','1','1','1'},
        {'0','0','0','0','0'},
        {'1','1','1','1','1'},
        {'1','1','1','1','1'}
    }; 

    cout << sol.maximalSquare(matrix) << endl;;
    return 0;
}
