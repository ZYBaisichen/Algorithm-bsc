/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-13 14:46:14
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
https://leetcode.cn/problems/unique-binary-search-trees/
给你一个整数 n ，求恰由 n 个节点组成且节点值从 1 到 n 互不相同的 二叉搜索树 有多少种？返回满足题意的二叉搜索树的种数。

 

示例 1：


输入：n = 3
输出：5
示例 2：

输入：n = 1
输出：1
 

提示：

1 <= n <= 19
*/

/*
卡特兰数的典型应用：
假设G(n)表示n个节点数能组成的二叉搜索树的种类数。f(i)表示以i为头结点时的二叉搜索树种类数, 则：
G(n) = f(1)+f(2)+...+f(n)
当以i为根节点时，左边节点个数是i-1个，右边节点个数是n-i个，所以:
f(i)=G(i-1)*G(n-i)

综合得
G(n) = G(0)*G(n-1) + G(1)*G(n-2) + .... + G(n-1)*G(0)
即为卡特兰数：
可以通过
G(n) = c(2n,n)/(n+1) 快速算出结果

*/
typedef unsigned long long ll;
class Solution
{
public:
    int numTrees_dp(int n) {
        int ans = 0;
        //0个节点数有1种，1个节点数也由1种
        vector<int> dp(n+1, 0);
        dp[0]=1;
        dp[1]=1;
        // dp[2] = dp[0]*dp[1]+dp[1]*dp[0]=2
        // dp[3] = dp[0]*dp[2] + dp[1]*d[1] + dp[2]*dp[0]=2+1+2=5

        //dp[i]表示有i个节点时的二叉搜索树种类
        for (int i=2;i<=n;i++) {
            for (int j=0;j<=i-1;j++) {
                // cout << "i:" << i <<  " j:" << j << " n-1-j:" << n-1-j << " dp[j]:" << dp[j]  << " dp[n-1-j]:" << dp[n-1-j]  << " dp[i]:" << dp[i] << endl;
                dp[i] += dp[j] * dp[i-1-j]; 
            }
        }
        // for (int i=0;i<=n;i++) {
        //     cout << dp[i] << " " ;
        // }
        // cout << endl;
        return dp[n];
    }
    int numTrees2(int n)
    {
        // G(n) = c(2n,n)/(n+1) 快速算出结果
        // 杨辉三角形(帕斯卡三角形)是一个由组合数构成的三角形。在帕斯卡三角形中，每个数字是由它上方两个数字之和得到的。例如，C(n, k) = C(n-1, k-1) + C(n-1, k)
        // c(1,1)=1,c(2,2)=1
        // c(1,1)=1, c(2,1)=2, c(n,1)=n
        // c(3,2) = c(2,1) + c(2,2) = 2 + 1 = 3
        // 依赖上一个格子和左上角的格子
        vector<vector<ll>> dp(2 * n + 1, vector<ll>(n + 1, 0));
        for (int i = 0; i <= n; i++)
        {
            dp[i][i] = 1;
        }

        // 第0列
        for (int i = 0; i <= 2 * n; i++)
        {
            dp[i][0] = 1;
        }
        // 第1列
        for (int i = 1; i <= 2 * n; i++)
        {
            dp[i][1] = i;
        }

        // for (int i=0;i<=2*n;i++) {
        //     for (int j=0;j<=n;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        for (int i = 3; i <= 2 * n; i++)
        {
            for (int j = 2; j <= i - 1 && j <= n; j++)
            {
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
            }
        }
        // for (int i=0;i<=2*n;i++) {
        //     for (int j=0;j<=n;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return dp[2 * n][n] / (n + 1);
    }
    
    //杨辉三角形，空间压缩
    int numTrees(int n)
    {
        if (n==0) {
            return 1;
        }
        // G(n) = c(2n,n)/(n+1) 快速算出结果
        // 杨辉三角形(帕斯卡三角形)是一个由组合数构成的三角形。在帕斯卡三角形中，每个数字是由它上方两个数字之和得到的。例如，C(n, k) = C(n-1, k-1) + C(n-1, k)
        // c(1,1)=1,c(2,2)=1
        // c(1,1)=1, c(2,1)=2, c(n,1)=n
        // c(3,2) = c(2,1) + c(2,2) = 2 + 1 = 3
        // 依赖上一个格子和左上角的格子
        vector<ll> dp(n + 1, 0);
        ll pre = 0 ,tmp = 0;
        dp[0] = 1;
        for (int i = 1; i <= 2 * n; i++) {
            // dp[0]永远是1
            pre = dp[1]; //记录左上角的值
            dp[1] = i; //i个数选1个，有i中选法
            for (int j = 2; j <= i && j <= n; j++) {
                tmp = dp[j];
                dp[j] = pre + dp[j];
                pre = tmp;
            }
            // cout << "i:" << i << " ";
            // for (auto it:dp) {
            //     cout << it << " ";
            // }
            // cout << endl;
        }
        return dp[n] / (n + 1);
    }

    //纯dp解法
    int numTrees_dp(int n) {
        /*
        左右有一个数，右边有n-2个数
        左边有2个数，右边有n-3个数
        左边有3个数，右边有n-4个数
        用g(n)表示n个节点有多少种二叉树，则g(n)=f(1)+f(2)+f(3)+..f(n)，f(i)表示以i节点为root时的二叉树种类数
        当以i为节点时，左边有i-1个数，右边有n-i个数，所以f(i)=g(i-1)*g(n-i)
        g(n) = g(0)*g(n-1) + g(1)*g(n-2)+...+g(n-1)*g(0)
        典型的卡特兰数
        g(n)=c(2n,n)/(n+1)

        或者用递推来求
        g(0) = 1;
        g(1) = 1;
        g(2) = g(0)*g(2-1) = g(0)*g(1)
        */
        vector<ll> g(n+1,0);
        g[0] = 1;
        g[1] = 1;
        for (int i=2;i<=n;i++) {
            for (int j=0;j<i;j++) {
                g[i] = g[i] + g[j]*g[i-j-1];
            }
        }
        return g[n];

    }
};
int main()
{

    Solution sol;
    int n=19;
    cout << sol.numTrees(n) << endl;
    cout << sol.numTrees2(n) << endl;
    return 0;
}
