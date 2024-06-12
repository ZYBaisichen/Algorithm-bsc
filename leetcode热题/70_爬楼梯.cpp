/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-11 13:01:44
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

typedef unsigned long long ll;
class Solution {
public:
    int climbStairs_origin(int n) {
        if (n == 1) {
            return 1;
        }
        if (n==2) {
            return 2;
        }

        int a = 1, b=2, c = 0;
        for (int i=3;i<=n;i++) {
            c = a+b;
            a = b;
            b = c;
        }
        return c;
    }

    //斐波那契数列快速幂球阀，在进阶班里讲到了
    int climbStairs(int n) {
        if (n == 1) {
            return 1;
        }
        if (n==2) {
            return 2;
        }
        vector<vector<ll>> base = {{1,1},{1,0}};
        auto t = matrix_power(base, n-2);
        return 2*t[0][0] + 1*t[0][1];
    }

    //求矩阵的n次方
    vector<vector<ll>> matrix_power(vector<vector<ll>>& base, int n) {
        vector<vector<ll>> t = base;
        vector<vector<ll>> res = {{1,0},{0,1}};
        while (n!=0) {
            // cout << "n:" << n << endl;
            if (n & 1) {
                // print(res, "res:");
                res = multy(res, t);
                // print(res, "res_after_multi:");
                
            }
            // print(t, "t:");
            t = multy(t, t);
            // print(t, "t_after_multi:");
            n = n >> 1;
        }
        return res;
    }

    void print(vector<vector<ll>> &m, string name) {
        // cout << "name:" << name << endl;
        for (int i = 0; i < m.size(); ++i) {
            for (int j = 0; j < m[i].size(); ++j) {
                cout << m[i][j] << " ";
            }
            cout << endl;
        }
    }

    vector<vector<ll>> multy(vector<vector<ll>>& a, vector<vector<ll>>& b) {
        int n = a.size();
        vector<vector<ll>> res(n ,vector<ll>(n, 0));
        for (int i=0;i<n;i++) { //枚举a的每一行
            for (int j=0;j<n;j++) { //枚举b的每一列
                int ans = 0;
                for (int k=0;k<n;k++) {
                    ans += a[i][k]*b[k][j];
                }
                res[i][j] = ans;
            }
         }
        return res;
    }

    
};
int main()
{

    Solution sol;

    vector<vector<int>> g ={{1,3,1},{1,5,1},{4,2,1}};
    cout << sol.minPathSum(g) << endl;
    return 0;
}