/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-24 18:59:48
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
#include <sstream>

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
定义dp[i][j]表示，必须以matrix[i][j]作为右下角时，往左上角扩，正方形边长是多少。

第一列，matrix为1时，正方形变长都是1。
第一行，matrix为1时，正方形变长都是1。
依赖上方、左方、左上角3个格子，取各个格子的最小变长，然后加上自己扩出来的1，就是以i,j为右下角的正方形边长

*/


class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        }
        int m = matrix[0].size();
        if (m==0) {
            return 0;
        }
        vector<vector<int>> dp(n, vector<int>(m, 0));
        int ans = 0;
        //第一行
        for (int j=0;j<m;j++) {
            if (matrix[0][j] == '1') {
                dp[0][j] = 1;
                ans = 1;
            }
        }
        //第一列
        for (int i=0;i<n;i++) {
            if (matrix[i][0] == '1') {
                dp[i][0] = 1;
                ans= 1;
            }
        }

        
        for (int i=1;i<n;i++) {
            for (int j=1;j<m;j++) {
                if (matrix[i][j]=='1') {
                    dp[i][j] = 1 + min(dp[i-1][j], min(dp[i][j-1], dp[i-1][j-1]));
                    ans = max(ans, dp[i][j]);
                }
            }
        }
        // for (int i=0;i<n;i++) {
        //     for (int j=0;j<m;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return ans*ans;
    }
};

int main() {
    
    return 0;
}