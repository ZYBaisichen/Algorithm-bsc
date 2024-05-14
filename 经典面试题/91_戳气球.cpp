/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-11 11:59:23
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
#include <sstream>

using namespace std;

/*
https://leetcode.cn/problems/burst-balloons/

有 n 个气球，编号为0 到 n - 1，每个气球上都标有一个数字，这些数字存在数组 nums 中。

现在要求你戳破所有的气球。戳破第 i 个气球，你可以获得 nums[i - 1] * nums[i] * nums[i + 1] 枚硬币。 这里的 i - 1 和 i + 1 代表和 i 相邻的两个气球的序号。如果 i - 1或 i + 1 超出了数组的边界，那么就当它是一个数字为 1 的气球。

求所能获得硬币的最大数量。



示例 1：
输入：nums = [3,1,5,8]
输出：167
解释：
nums = [3,1,5,8] --> [3,5,8] --> [3,8] --> [8] --> []
coins =  3*1*5    +   3*5*8   +  1*3*8  + 1*8*1 = 167
示例 2：

输入：nums = [1,5]
输出：10


提示：

n == nums.length
1 <= n <= 300
0 <= nums[i] <= 100

*/

/*
递归：
1. f(l,r)在l-1和r+1位置气球没有爆的前提下，返回[l...r]范围上得到的最大硬币数。主函数调用f(1,len-1)
2. 可能性整理：按照谁是最后被打爆的气球整理
    a) l是最后一个被打爆的，得分是f(l+1,r)+[l-1]*[l]*[r+1]
    b) r是最后一个被打爆的，得分是f(l,r-1)+[l-1]*[r]*[r+1]
    c) 中间i位置的气球被最后打爆，得分是f(l,i-1)+f(i+1,r)+[l-1]*[i]*[r+1]，尝试每个i，得分求max
    d) 以上三种可能性求max

递归思考的过程：
    有可能想到l...r上的范围尝试，枚举每个气球最先打爆，先打爆气球时无法获得左右两侧的数据状况。可能需要记录左右两侧离自己最近的不被打爆气球的分数，这种记录可能是非单一参数，可以使用一个单调栈记录，但无法做动态规划。
    所以只能从最后打爆的气球开始尝试。

    如果计分方法是离自己最近且最先打爆的气球，那么可以尝试先看最先打爆的气球。

*/

class Solution
{
public:
    // 暴力递归版本
    int maxCoinsBaoli(vector<int> &nums)
    {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        // 开头结尾各补一个1，不会改变乘积得分
        vector<int> help(len + 2, 1);
        for (int i = 0; i < len; i++)
        {
            help[i + 1] = nums[i];
        }
        vector<vector<int>> dp(len+1, vector<int>(len+1, 0));
        int ans = process(help, 1, len, dp);
        cout << "baoli:" << endl;
        for (int i=1;i<=len;++i) {
            for (int j=1;j<=len;++j) {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        return ans;
    }

    int process(vector<int>& help, int l, int r,vector<vector<int>>& dp) {
        //只剩一个气球了
        if (l==r) {
            dp[l][r] = help[l - 1] * help[l] * help[r + 1];
            return dp[l][r];
        } 

        //最后打爆l
        int ans = process(help, l+1, r, dp) + help[l-1]*help[l]*help[r+1];
        //最后打爆r
        ans = max(ans, process(help, l, r-1, dp) + help[l-1]*help[r]*help[r+1]);
        //中间打爆i
        for (int i=l+1;i<r;++i) {
            ans = max(ans, process(help, l, i-1, dp)+process(help, i+1, r, dp) + help[l-1]*help[i]*help[r+1]);
        }
        dp[l][r] = ans;
        return ans;
    }


    int maxCoins(vector<int> &nums) {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        // 开头结尾各补一个1，不会改变乘积得分
        vector<int> help(len + 2, 1);
        for (int i = 0; i < len; i++)
        {
            help[i + 1] = nums[i];
        }
        //dp[i][j]代表[i...j]范围上打爆所有气球获得的最大分数
        vector<vector<int>> dp(len+1, vector<int>(len+1, 0));
        //先填对角线
        for (int i=1;i<=len;i++) {
            dp[i][i] = help[i-1]*help[i]*help[i+1];
        }
        //填次对角线
        for (int i=1;i<=len-1;i++) {
            //最后打i
            dp[i][i+1] = help[i-1]*help[i]*help[i+2] + dp[i+1][i+1];
            //最后打i+1
            dp[i][i+1] = max(dp[i][i+1], help[i-1]*help[i+1]*help[i+2] + dp[i][i]);
        }
        //l依赖l+1..., r依赖r-1....
        //从下往上，从左往右
        for (int l=len-2;l>=1;l--) {
            for (int r=l+2;r<=len;r++) {
                //最后打爆l
                int ans = dp[l+1][r] + help[l-1]*help[l]*help[r+1];
                //最后打爆r
                ans = max(ans, dp[l][r-1] + help[l-1]*help[r]*help[r+1]);
                //最后打爆中间i
                for (int i=l+1;i<r;++i) {
                    ans = max(ans, dp[l][i-1]+dp[i+1][r]+help[l-1]*help[i]*help[r+1]);
                }
                dp[l][r] = ans;
            }
        }

        // cout << "dp:" << endl;
        // for (int i=1;i<=len;++i) {
        //     for (int j=1;j<=len;++j) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        return dp[1][len];
    }
};

int main()
{

    Solution sol;
    vector<int> nums = {9, 76, 64, 21, 97};
    cout << sol.maxCoinsBaoli(nums) << endl;
    cout << sol.maxCoins(nums) << endl;
    return 0;
}