/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 17:51:57
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
股票问题，和经典面试题的54-57相同

https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/
给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。

你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。设计一个算法来计算你所能获取的最大利润。

返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 0 。

 

示例 1：

输入：[7,1,5,3,6,4]
输出：5
解释：在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5 。
     注意利润不能是 7-1 = 6, 因为卖出价格需要大于买入价格；同时，你不能在买入前卖出股票。
示例 2：

输入：prices = [7,6,4,3,1]
输出：0
解释：在这种情况下, 没有交易完成, 所以最大利润为 0。
 

提示：

1 <= prices.length <= 105
0 <= prices[i] <= 104
*/

/*
贪心，假设每次都可以卖出，卖出时前面是再最低点买入的，每次更新最大的利润
*/
class Solution
{
public:
    int maxProfit(vector<int>& prices) {
        int len = prices.size();
        if (len == 0) {
            return 0;
        }
        int ans=0, cur_min=prices[0];
        for (int i=1;i<len;i++) {
            ans = max(ans, prices[i]-cur_min);
            cur_min = min(cur_min, prices[i]);
        }
        return ans;
    }
};

int main() {

    Solution sol;
    return 0;
}
