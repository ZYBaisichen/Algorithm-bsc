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
https://leetcode.cn/problems/climbing-stairs/description/

假设你正在爬楼梯。需要 n 阶你才能到达楼顶。

每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？

 

示例 1：

输入：n = 2
输出：2
解释：有两种方法可以爬到楼顶。
1. 1 阶 + 1 阶
2. 2 阶
示例 2：

输入：n = 3
输出：3
解释：有三种方法可以爬到楼顶。
1. 1 阶 + 1 阶 + 1 阶
2. 1 阶 + 2 阶
3. 2 阶 + 1 阶
 

提示：

1 <= n <= 45
*/


/*

斐波那契数列


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