/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 17:38:51
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

https://leetcode.cn/problems/perfect-squares/

给你一个整数 n ，返回 和为 n 的完全平方数的最少数量 。

完全平方数 是一个整数，其值等于另一个整数的平方；换句话说，其值等于一个整数自乘的积。例如，1、4、9 和 16 都是完全平方数，而 3 和 11 不是。



示例 1：

输入：n = 12
输出：3
解释：12 = 4 + 4 + 4
示例 2：

输入：n = 13
输出：2
解释：13 = 4 + 9

提示：

1 <= n <= 104
*/

/*

*/

class Solution
{
public:
    int numSquares_baoli(int n)
    {
        if (n == 0)
        {
            return 0;
        }
        return process(n);
    }
    /*
    还有rest需要组合
    返回需要几个数来组合成rest
    */
    int process(int rest)
    {
        if (rest == 0)
        {
            return 0;
        }

        int max_sq = sqrt(rest);
        int cnt = rest; // 最多rest个1
        // 从最大的数开始试
        for (int i = max_sq; i >= 1; i--)
        {
            if (i * i <= rest)
            {
                cnt = min(cnt, process(rest - i * i) + 1);
            }
        }
        return cnt;
    }

    int numSquares1(int n) {
        if (n == 0)
        {
            return 0;
        }

        vector<int> dp(n+1, n); //dp[i]表示组成i需要的最少平方数个数
        dp[0] = 0;
        int max_sq, cnt;
        for (int rest=1;rest<=n;rest++) {
            max_sq = sqrt(rest);
            cnt = rest;
            for (int i=max_sq;i>=1;i--){
                if (i*i <= rest) {
                    cnt = min(cnt, dp[rest-i*i] + 1); 
                }
            }
            dp[rest] = cnt;
        }
        // for (int i=0;i<=n;i++) {
        //     cout << dp[i] << " ";
        // }
        // cout << endl;
        return dp[n];
    }

    // 官方题解的四平方和定理：https://leetcode.cn/problems/perfect-squares/solutions/822940/wan-quan-ping-fang-shu-by-leetcode-solut-t99c/

    /*
    四平方和定理证明了任意一个正整数都可以被表示为至多四个正整数的平方和。这给出了本题的答案的上界。
    1. 当n=4^k*(8m+7)时，是4个整数；
    2. 当不等于时，判断n=a^2+b^2， 可以用两个数表示
    3. 当n=a^2时，是一个数
    4. 其他情况是三个数
    */
    bool  is_perfect_sqrt(int x) {
        int y = sqrt(x);
        return (y * y == x);
    }
    // 判断是否为4^k*(8m+7)
    bool check_4k(int x) {
        while (x % 4 == 0) { //除掉所有4
            x /= 4;
        }
        return x % 8 == 7;
    }
    int numSquares(int n) {
        if (n == 0) {
            return 0;
        }
        if (is_perfect_sqrt(n)) {
            return 1;
        }
        if (check_4k(n)) {
            return 4;
        }
        int max_sq = sqrt(n);
        for (int i=1;i<=max_sq;i++) {
            if (is_perfect_sqrt(n-i*i)) {
                return 2;
            }
        }
        return 3;
    }
};
int main()
{

    Solution sol;
    // vector<vector<int>> arr = {
    //     {0,30},
    //     {5,10},
    //     {15,20}};

    int n = 12;
    cout << sol.numSquares(n) << endl;
    return 0;
}
