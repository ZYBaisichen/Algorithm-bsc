/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-02 12:14:58
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
给你一个整数数组 prices ，其中 prices[i] 表示某支股票第 i 天的价格。

在每一天，你可以决定是否购买和/或出售股票。你在任何时候 最多 只能持有 一股 股票。你也可以先购买，然后在 同一天 出售。

返回 你能获得的 最大 利润 。

 

示例 1：

输入：prices = [7,1,5,3,6,4]
输出：7
解释：在第 2 天（股票价格 = 1）的时候买入，在第 3 天（股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5 - 1 = 4 。
     随后，在第 4 天（股票价格 = 3）的时候买入，在第 5 天（股票价格 = 6）的时候卖出, 这笔交易所能获得利润 = 6 - 3 = 3 。
     总利润为 4 + 3 = 7 。
示例 2：

输入：prices = [1,2,3,4,5]
输出：4
解释：在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5 - 1 = 4 。
     总利润为 4 。
示例 3：

输入：prices = [7,6,4,3,1]
输出：0
解释：在这种情况下, 交易无法获得正利润，所以不参与交易可以获得最大利润，最大利润为 0 。
 

提示：

1 <= prices.length <= 3 * 104
0 <= prices[i] <= 104


https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-ii/description/
*/

class Solution {
public:
    /*
    贪心：每个上坡时的利润都占上。
    来到i位置，只要i-1位置的价格比当前小，则加上prices[i]-prices[i-1]的利润
    */
    int maxProfit(vector<int>& prices) {

        int len = prices.size();
        if (len==0) return 0;
        int ans = 0;
        for (int i=1;i<len;i++) {
            if (prices[i] > prices[i-1]) {
                ans += (prices[i]-prices[i-1]);
            }
        }
        return ans;
    }
};

int main()
{

    Solution sol;
    vector<int> arr = {5,5,3,2,6,4,3};

    cout << sol.max_len(arr) << endl;

    return 0;
}