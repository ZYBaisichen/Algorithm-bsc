/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-02 16:05:06
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
给你一个整数数组 prices 和一个整数 k ，其中 prices[i] 是某支给定的股票在第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你最多可以完成 k 笔交易。也就是说，你最多可以买 k 次，卖 k 次。

注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。



示例 1：

输入：k = 2, prices = [2,4,1]
输出：2
解释：在第 1 天 (股票价格 = 2) 的时候买入，在第 2 天 (股票价格 = 4) 的时候卖出，这笔交易所能获得利润 = 4-2 = 2 。
示例 2：

输入：k = 2, prices = [3,2,6,5,0,3]
输出：7
解释：在第 2 天 (股票价格 = 2) 的时候买入，在第 3 天 (股票价格 = 6) 的时候卖出, 这笔交易所能获得利润 = 6-2 = 4 。
     随后，在第 5 天 (股票价格 = 0) 的时候买入，在第 6 天 (股票价格 = 3) 的时候卖出, 这笔交易所能获得利润 = 3-0 = 3 。


提示：

1 <= k <= 100
1 <= prices.length <= 1000
0 <= prices[i] <= 1000


https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iv/

*/

class Solution
{
public:
    /*
    一、 股票上坡数量不会超过n/2，因为大的上坡一定会有个下坡才会有新的上坡，所以当k大于n/2时，和无限次交易是一样的了，就退化成了无限次交易
    二、 当k小于n/2时：
        dp[i][j]，arr[0..i]股票随意交易，最多完成j次交易，获得的最大利润
        1. 第0列：交易0次，利润一定都是0元
        2. 第0行：第一个股票，交易多少次交易，利润都是0元
        3. 普遍位置dp[i][j]：
            a. i号股票的价格不参与交易，则依赖0...i-1所有股票<=j次的交易利润:dp[i-1][j]
            b. i号股票的价格参与交易：(贪心)只会参与最后以i号位置价格作为最终卖出时机。这就需要列举最后一次股票交易的买入在哪里
                i) 在前i个股票上凑够k-1个交易，最后一次交易是在i位置买在i位置卖：dp[i][j-1]+[i]-[i]
                ii) 在前i-1个股票上凑够k-1个交易，最后一次交易是在i-1位置买,在i位置卖：dp[i-1][j-1]+[i]-[i-1]
                ..
                k) dp[i][j]=max(dp[每个买入时机][j-1] + [i]-[k])，枚举每个买入时机，然后在i位置卖出，最后求利润max

        4. 可能性b中有枚举行为，所以二维表是N^2，加上这个枚举行为就是n^3的复杂度了。
            a. 观察dp[10][3]=max{
                dp[9][3], //不参与交易
                dp[10][2]+[10]-[10], //在10位置买在10位置卖
                dp[9][2]+[10]-[9], //在9位置买在10位置卖
                ...
                dp[0][2]+[10]-[0], //在0位置买在10位置卖
            }
            dp[11][3]=max{
                dp[10][3],
                dp[11][2]+[11]-[11],
                dp[10][2]+[11]-[10],
                dp[9][2]+[11]-[9],
                dp[0][2]+[11]-[0],
            }
            将[11]和[10]求出来就是
            dp[10][3]=max{dp[9][3], [10]+max{
                 //不参与交易
                dp[10][2]-[10], //在10位置买在10位置卖
                dp[9][2]-[9], //在9位置买在10位置卖
                ...
                dp[0][2]-[0], //在0位置买在10位置卖
            }
            }
            dp[11][3]=max{dp[10][3], [11]+max{
                dp[11][2]-[11],
                dp[10][2]-[10],
                dp[9][2]-[9],
                ...
                dp[0][2]-[0],
            }}

            所以发现两者dp[10][2]往下是有重复的。所以做抽象化就是
            dp[i-1][j]=max{
                dp[i-2][j],
                [i-1]+t
            }
            d[i][j]=max{
                dp[i-1][j],
                [i]+max(t,dp[i][j-1]-[i])
            }
            令新的t=max(t,dp[i][j-1]-[i])
            d[i+1][j]=max{
                dp[i][j],
                [i+1]+max(t,dp[i+1][j-1]-[i+1])
            }
            最终填每个格子的复杂度是O(1)了。（斜率优化）
    */
    int maxProfit(int k, vector<int> &prices) {
        int len = prices.size();
        if (k>len/2) {//退化成无限次交易，吃每个上涨
            int ans = 0;
            for (int i=1;i<len;i++) {
                if (prices[i]>prices[i-1]) {
                    ans += prices[i]-prices[i-1];
                }
            }
            return ans;
        }

        vector<vector<int>> dp(len, vector<int>(k+1, 0));
        ////第0列
        // for (int i=0;i<=len;i++) {
        //     dp[i][0] = 0;
        // }

        // //第0行
        // for (int i=1;i<=k;i++) {
        //     dp[0][i] = 0;
        // }

        /*
        dp[i][j]依赖[i-1][j-1]、[i-2][j-1]...所以大流程需要从上往下，从左往右
        但通过斜率优化得到，当推出来dp[i][j]时，想瞬时退出来dp[i+1][j]。所以外层循环是j，内存循环是i。
        按照列推
        */
       
       for (int j=1;j<=k;j++) {
            /*
            对于d[1][1]=max{
                dp[0][1],
                [1]+max{
                dp[1][0]-[1],
                dp[0][1]-[0]
                }
            }
            t=max{dp[1][0]-[1],dp[0][1]-[0]}

            对于dp[0][j]=max{
                dp[-1][j], //无意义就是0
                [0]+max{
                    dp[0][j-1]-[0]
                }
            }
            t=dp[0][j-1]-[0]
            因为下面要枚举[1][j]了，所以t应该是0位置时的t。
            */
            int t = dp[0][j - 1] - prices[0];
            for (int i=1;i<len;i++) {
                dp[i][j]=dp[i-1][j];

                int next_t = max(
                    t, dp[i][j-1]-prices[i]
                );
                dp[i][j] = max(
                    dp[i][j],
                    prices[i]+next_t
                );
                t = next_t;
            }
       }
       return dp[len-1][k];
    }
};

int main()
{

    Solution sol;
    vector<int> prices = {3,2,6,5,0,3};
    int k=2; //7

    cout << sol.maxProfit(k, prices) << endl;

    return 0;
}