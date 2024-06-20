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
即经典面试题的54..题，新的花样，增加了冷冻期

https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-cooldown/

给定一个整数数组prices，其中第  prices[i] 表示第 i 天的股票价格 。​

设计一个算法计算出最大利润。在满足以下约束条件下，你可以尽可能地完成更多的交易（多次买卖一支股票）:

卖出股票后，你无法在第二天买入股票 (即冷冻期为 1 天)。
注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

示例 1:

输入: prices = [1,2,3,0,2]
输出: 3
解释: 对应的交易状态为: [买入, 卖出, 冷冻期, 买入, 卖出]
示例 2:

输入: prices = [1]
输出: 0


提示：

1 <= prices.length <= 5000
0 <= prices[i] <= 1000

*/

/*
暴力：
每个位置可以选择卖也可以选择买。枚举路径，并携带一个标记表示前两次卖没卖


*/

class Solution
{
public:
    int maxProfit1(vector<int> &prices)
    {
        int len = prices.size();
        if (len == 0)
        {
            return 0;
        }
        return process(prices, 0, false, -1);
    }
    /*
    从左往右尝试看看
     0...i-1已经决定好了买卖
     pre_sell：前一天是否发生了卖的标记
     have_p_price： 当前如果持有股票，存储其买入价格，没有持有是-1
     返回在i...n范围上自由选择买卖，得到的最大利润

     base case: i来到了最后一天，如果手里有股票则前一天不可能卖了股票，则卖了

     可能性：
         1. 前一天如果没卖股票，且手头也没有股票持有，则可以选择买和不买
         2. 当前持有股票，则前一天不可能卖了股票，则选择卖或者不卖
         3. 前一天卖了股票，当前一天只能选择什么都不操作
    */
    int process(vector<int> &prices, int i, bool pre_sell, int have_p_price)
    {
        cout << "i:" << i << " prices[i]:" << prices[i] << " pre_sell:" << pre_sell << " have_p_price:" << have_p_price << endl;
        cout << "i == prices.size():" << (i == prices.size()) << endl;
        if (i == prices.size() - 1)
        {
            if (have_p_price != -1 && pre_sell == false) {
                return prices[i] - have_p_price;
            } else {
                return 0;
            }
        }
        if (pre_sell)
        { // 前一天卖出了，什么都不能操作
            return process(prices, i + 1, false, have_p_price);
        }
        else
        { // 前一天没卖股票
            int ans = 0;
            if (have_p_price == -1)
            { // 当前没有持有股票
                int buy = process(prices, i + 1, false, prices[i]);
                int no_buy = process(prices, i + 1, false, -1);
                ans = max(buy, no_buy);
            }
            else
            { // 当前持有股票
                int sell = prices[i] - have_p_price + process(prices, i + 1, true, -1);
                int no_sell = process(prices, i + 1, false, have_p_price);
                ans = max(sell, no_sell);
            }
            return ans;
        }
    }

    void print_m(vector<vector<int>>& m, string suff) {
        cout << suff << endl;
        for (int i=0;i<m.size();i++) {
            for (int j=0;j<m[0].size();j++) {
                cout << m[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    int maxProfit_dp(vector<int> &prices) {
        int len = prices.size();
        if (len == 0)
        {
            return 0;
        }

        //用0表示手中没有股票
        int min_prices = prices[0]+1;
        int max_prices = prices[0]+1;
        for (int i=0;i<len;++i){
            prices[i]+=1;
            min_prices = min(min_prices, prices[i]);
            max_prices = max(max_prices, prices[i]);
        }
        // cout << "min_prices:" << min_prices << " max_prices:" << max_prices << endl;

        /*
        have_p_price默认-1，取值从min(prices)到max(prices)
        i取值0...len-1
        pre_sell 取值true和false

        dp_pre_sell[i][j]: 以i-1天卖出了股票为前提，在i...len-1范围上，手中持有价格为j的股票，自由选择卖出和不卖出，得到的最大利润
        dp_no_pre_sell[i][j]: 以i-1天没有卖出了股票为前提，在i...len-1范围上，手中持有价格为j的股票，自由选择卖出和不卖出，得到的最大利润
        */

        vector<vector<int>> dp_pre_sell(len, vector<int>(max_prices+1, 0));
        vector<vector<int>> dp_no_pre_sell(len, vector<int>(max_prices+1, 0));
        //最后一行，j==0表示没有持有股票。产生的利润是0
        //如果前一天卖出股票，则产生的利润依旧是0
        for (int j=1;j<=max_prices;++j){
            dp_no_pre_sell[len-1][j] = max(prices[len-1] - j, 0);
        }
        // print_m(dp_no_pre_sell, "init dp_no_pre_sell: ");
        // print_m(dp_pre_sell, "init dp_pre_sell: ");
        /*
        pre_sel数组表示前一天卖出了股票, dp_pre_sell[i][j]: 依赖dp_no_pre_sell[i+1][j]
        pre_no_sell[i][j]，当j时0的时候，当前不持有股票，可以买也可以不买，依赖max(dp_pre_no_sell[i+1][prices[i]], dp_pre_no_sell[i+1][0])
                        当j不为0的时候，持有股票，可以卖也可以不卖，依赖min(dp_pre_sell[i+1][0], dp_pre_no_sell[i+1][j])
        
        可以看出，两个dp表均依赖下一行所有的数。而不依赖本行数。所以从下到上，从左到右填写两个dp表
        */
        for (int i=len-2;i>=0;--i){
            for (int j=0;j<=max_prices;++j){
                //前一天卖出了股票，本天是冷却期
                // cout << "i:" << i << " j:" << j << "======" << endl;
                // cout << "dp_no_pre_sell[i+1][j]: " << dp_no_pre_sell[i + 1][j]
                //      << " dp_no_pre_sell[i+1][0]: " << dp_no_pre_sell[i + 1][0]
                //      << " prices[i] - j + dp_pre_sell[i+1][0]：" << prices[i] - j + dp_pre_sell[i + 1][0]
                //      << " dp_no_pre_sell[i+1][j]:" << dp_no_pre_sell[i+1][j] << endl;
                dp_pre_sell[i][j] = dp_no_pre_sell[i + 1][j];
                // cout << "i:" << i << " j:" << j << " dp_pre_sell[i][j]" << dp_pre_sell[i][j] << endl;
                if (j==0) { //没有持有股票
                    dp_no_pre_sell[i][j] = max(dp_no_pre_sell[i + 1][prices[i]], dp_no_pre_sell[i+1][0]);
                } else {
                    dp_no_pre_sell[i][j] = max(prices[i] - j + dp_pre_sell[i + 1][0], dp_no_pre_sell[i + 1][j]);
                }
            }
        }
        // print_m(dp_no_pre_sell, "final dp_no_pre_sell: ");
        // print_m(dp_pre_sell, "final dp_pre_sell: ");
        return dp_no_pre_sell[0][0];
    }

    //空间优化
    void print_arr(vector<int>& arr, string suff) {
        cout << suff << endl;
        for (auto it:arr) {
            cout << it <<" ";
        }
        cout << endl;
    }
    int maxProfit(vector<int> &prices) {
        int len = prices.size();
        if (len == 0)
        {
            return 0;
        }

        //用0表示手中没有股票
        int min_prices = prices[0]+1;
        int max_prices = prices[0]+1;
        for (int i=0;i<len;++i){
            prices[i]+=1;
            min_prices = min(min_prices, prices[i]);
            max_prices = max(max_prices, prices[i]);
        }
        // cout << "min_prices:" << min_prices << " max_prices:" << max_prices << endl;

        /*
        have_p_price默认-1，取值从min(prices)到max(prices)
        i取值0...len-1
        pre_sell 取值true和false

        dp_pre_sell[i][j]: 以i-1天卖出了股票为前提，在i...len-1范围上，手中持有价格为j的股票，自由选择卖出和不卖出，得到的最大利润
        dp_no_pre_sell[i][j]: 以i-1天没有卖出了股票为前提，在i...len-1范围上，手中持有价格为j的股票，自由选择卖出和不卖出，得到的最大利润
        */
       
        vector<int> dp_pre_sell1(max_prices+1), dp_pre_sell2(max_prices+1);
        vector<int> dp_no_pre_sell1(max_prices + 1), dp_no_pre_sell2(max_prices + 1);
        bool flag = true;
        //最后一行，j==0表示没有持有股票。产生的利润是0
        //如果前一天卖出股票，则产生的利润依旧是0
        for (int j=1;j<=max_prices;++j){
            dp_no_pre_sell1[j] = max(prices[len - 1] - j, 0);
        }
        print_arr(dp_no_pre_sell1, "init dp_no_pre_sell1: ");
        print_arr(dp_pre_sell1, "init dp_pre_sell1: ");
        /*
        pre_sel数组表示前一天卖出了股票, dp_pre_sell[i][j]: 依赖dp_no_pre_sell[i+1][j]
        pre_no_sell[i][j]，当j时0的时候，当前不持有股票，可以买也可以不买，依赖max(dp_pre_no_sell[i+1][prices[i]], dp_pre_no_sell[i+1][0])
                        当j不为0的时候，持有股票，可以卖也可以不卖，依赖min(dp_pre_sell[i+1][0], dp_pre_no_sell[i+1][j])
        
        可以看出，两个dp表均依赖下一行所有的数。而不依赖本行数。所以从下到上，从左到右填写两个dp表
        */
        vector<int>* cur_pre_sell, *next_pre_sell;
        vector<int> *cur_no_pre_sell, *next_no_pre_sell;
        for (int i=len-2;i>=0;--i){
            if (flag) { //每一行换一次
                cur_pre_sell = &dp_pre_sell1;
                next_pre_sell = &dp_pre_sell2;
                cur_no_pre_sell = &dp_no_pre_sell1;
                next_no_pre_sell = &dp_no_pre_sell2;
            } else {
                cur_pre_sell = &dp_pre_sell2;
                next_pre_sell = &dp_pre_sell1;
                cur_no_pre_sell = &dp_no_pre_sell2;
                next_no_pre_sell = &dp_no_pre_sell1;
            }
            flag = !flag;
            for (int j=0;j<=max_prices;++j){
                
                //前一天卖出了股票，本天是冷却期
                // cout << "i:" << i << " j:" << j << "======" << endl;
                // cout << "dp_no_pre_sell[i+1][j]: " << dp_no_pre_sell[i + 1][j]
                //      << " dp_no_pre_sell[i+1][0]: " << dp_no_pre_sell[i + 1][0]
                //      << " prices[i] - j + dp_pre_sell[i+1][0]：" << prices[i] - j + dp_pre_sell[i + 1][0]
                //      << " dp_no_pre_sell[i+1][j]:" << dp_no_pre_sell[i+1][j] << endl;
                (*next_pre_sell)[j] = (*cur_no_pre_sell)[j];
                // cout << "i:" << i << " j:" << j << " dp_pre_sell[i][j]" << dp_pre_sell[i][j] << endl;
                if (j==0) { //没有持有股票
                    (*next_no_pre_sell)[j] = max((*cur_no_pre_sell)[prices[i]], (*cur_no_pre_sell)[0]);
                } else {
                    (*next_no_pre_sell)[j] = max(prices[i] - j + (*cur_pre_sell)[0], (*cur_no_pre_sell)[j]);
                }
            }

            cout << "i:" << i << endl;
            print_arr(*next_no_pre_sell, "cur next_no_pre_sell: ");
            print_arr(*next_pre_sell, "cur next_pre_sell: ");
        }
        print_arr(dp_no_pre_sell1, "final dp_no_pre_sell1: ");
        print_arr(dp_no_pre_sell2, "final dp_no_pre_sell2: ");
        if (flag) { //最后一次计算是用的next是2
            return dp_no_pre_sell1[0];
        } else {
            return dp_no_pre_sell2[0];
        }
    }
};
// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
int main()
{
    Solution sol;
    // vector<int> prices = {2,3,4,1,3};
    vector<int> prices = {1, 2, 3, 0, 2};
    cout << sol.maxProfit(prices) << endl;
    return 0;
}
