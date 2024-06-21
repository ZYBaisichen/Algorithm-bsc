/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-20 14:17:01
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
https://leetcode.cn/problems/coin-change/description/

给你一个整数数组 coins ，表示不同面额的硬币；以及一个整数 amount ，表示总金额。

计算并返回可以凑成总金额所需的 最少的硬币个数 。如果没有任何一种硬币组合能组成总金额，返回 -1 。

你可以认为每种硬币的数量是无限的。

 

示例 1：

输入：coins = [1, 2, 5], amount = 11
输出：3 
解释：11 = 5 + 5 + 1
示例 2：

输入：coins = [2], amount = 3
输出：-1
示例 3：

输入：coins = [1], amount = 0
输出：0
 

提示：

1 <= coins.length <= 12
1 <= coins[i] <= 231 - 1
0 <= amount <= 104

*/

/*


*/
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        if (amount == 0) {
            return 0;
        }
        int len = coins.size();
        if (len==0) {
            return -1;
        }
        int ans = process(coins, 0, amount);
        if (ans == INT_MAX) {
            return -1;
        } else {
            return ans;
        }
    }
    /*
    来到了i位置，还有rest的零钱可以兑换
    当rest为0时，返回需要0个硬币就可以兑换出来
    */
    int process(vector<int>& coins, int i, int rest) {
        if (rest == 0) {
            return 0;
        }
        if (i==coins.size()) {
            return INT_MAX;
        }

        //不使用i号硬币，需要的钱币数
        int no = process(coins, i+1, rest);
        int yes = INT_MAX, tmp;
        for (int k=1;k*coins[i]<=rest;k++) {
            tmp = process(coins, i+1, rest-k*coins[i]);
            if (tmp != INT_MAX) {
                yes = min(yes, tmp + k);
            }
        }
        return min(no, yes)
    }


    int coinChange(vector<int>& coins, int amount) {
        if (amount == 0) {
            return 0;
        }
        int len = coins.size();
        if (len==0) {
            return -1;
        }
        /*
        i取值0...len
        rest取值0...amount
        dp[i][rest]表示，i...len自由选择，拼凑出rest最少需要几个硬币
        结果返回dp[0][amount]
        */
        vector<vector<int>> dp(len+1, vector<int>(amount+1, INT_MAX));
        //第一列
        for (int i=0;i<=len;i++) {
            dp[i][0] = 0;
        }
        for (int i=len-1;i>=0;i--) {
            for (int rest=1;rest<=amount;rest++) {
                int no = dp[i+1][rest];
                int yes = INT_MAX, tmp;
                for (int k=1;k*coins[i]<=rest;k++) {
                    tmp = dp[i+1][rest-k*coins[i]];
                    if (tmp != INT_MAX) {
                        yes = min(yes, tmp+k);
                    }
                }
                dp[i][rest] = min(no, yes);
            }
        }

        // for (int i=0;i<=len;i++) {
        //     for (int j=0;j<=amount;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        
        if (dp[0][amount] == INT_MAX) {
            return -1;
        } else {
            return dp[0][amount];
        }
    }
};


int main()
{
    Solution sol;
    // vector<int> prices = {2,3,4,1,3};
    vector<int> prices = {1, 2, 3, 0, 2};
    cout << sol.maxProfit(prices) << endl;
    return 0;
}
