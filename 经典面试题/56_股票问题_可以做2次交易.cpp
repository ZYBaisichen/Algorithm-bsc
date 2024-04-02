/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-02 16:23:06
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
给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易。

注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

 

示例 1:

输入：prices = [3,3,5,0,0,3,1,4]
输出：6
解释：在第 4 天（股票价格 = 0）的时候买入，在第 6 天（股票价格 = 3）的时候卖出，这笔交易所能获得利润 = 3-0 = 3 。
     随后，在第 7 天（股票价格 = 1）的时候买入，在第 8 天 （股票价格 = 4）的时候卖出，这笔交易所能获得利润 = 4-1 = 3 。
示例 2：

输入：prices = [1,2,3,4,5]
输出：4
解释：在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。   
     注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。   
     因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。
示例 3：

输入：prices = [7,6,4,3,1] 
输出：0 
解释：在这个情况下, 没有交易完成, 所以最大利润为 0。
示例 4：

输入：prices = [1]
输出：0
 

提示：

1 <= prices.length <= 105
0 <= prices[i] <= 105


https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iii/

*/

class Solution {
public:
    /*
    k次交易的退化成2次交易，同样的让dp[i][j]表示[0...i]个股票完成至少j次交易产生的最大利润
    1. 第0列：交易0次，利润一定都是0元
    2. 第0行：第一个股票，交易多少次交易，利润都是0元
    3. 普遍位置dp[i][j]:
        a. i号股票的价格不参与交易，则依赖0...i-1所有股票<=j次的交易利润:dp[i-1][j]
        b. i号股票的价格参与交易：(贪心)只会参与最后以i号位置价格作为最终卖出时机。这就需要列举最后一次股票交易的买入在哪里。因为最多就交易两次，所以比较好枚举
            i) 在前i个股票上凑够j-1个交易，最后一次交易是在i位置买在i位置卖：dp[i][j-1]+[i]-[i]
            ii) 在前i-1个股票上凑够j-1个交易，最后一次交易是在i-1位置买,在i位置卖：dp[i-1][j-1]+[i]-[i-1]
            ..
            k) dp[i][j]=max(dp[每个买入时机][j-1] + [i]-[k])，枚举每个买入时机，然后在i位置卖出，最后求利润max
    4. 举个例子
        a. 观察dp[10][2]=max{
            dp[9][2], //不参与交易
            dp[10][1]+[10]-[10], //在10位置买在10位置卖
            dp[9][1]+[10]-[9], //在9位置买在10位置卖
            ...
            dp[0][1]+[10]-[0], //在0位置买在10位置卖
        }
        dp[11][2]=max{
            dp[10][2], //不参与交易
            dp[11][1]+[11]-[11], //在10位置买在10位置卖
            dp[10][1]+[11]-[10], //在9位置买在10位置卖
            ...
            dp[0][1]+[11]-[0], //在0位置买在10位置卖
        }

        同样做和57一样的斜率优化

        每次填dp[i][j]复杂度是O(1)的，所以总复杂度是O(n*2)即O(N)
    */
    int maxProfit(vector<int>& prices) {

        int len = prices.size();
        if (len==0) return 0;
        int ans = 0;
        
        vector<vector<int>> dp(len, vector<int>(3, 0));
        //0行和0列都是0，所以不用填

        for (int j=1;j<=2;j++) {
            int t = dp[0][j - 1] - prices[0];
            for (int i=1;i<len;i++) {
                dp[i][j]=dp[i-1][j];//不参与买卖
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
        return dp[len-1][2];
    }
};

int main()
{

    Solution sol;
    vector<int> arr = {1,2,3,4,5};

    cout << sol.maxProfit(arr) << endl;

    return 0;
}