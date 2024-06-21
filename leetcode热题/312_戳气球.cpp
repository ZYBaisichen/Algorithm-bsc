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
同经典面试题91题

https://leetcode.cn/problems/burst-balloons/description/
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
范围上尝试模型

dp[l][r]代表l...r上，在l-1位置和r+1位置都没有被打爆的情况下产生的最大利润
1. l==r时，表示只有一个气球，可以直接打爆。产生的硬币数是nums[l-1]*nums[l]*nums[r+1]
2. 最后打爆l时，产生的收益是dp[l+1][r]+nums[l-1]*nums[l]*nums[r+1]
3. 最后打爆r时，产生的收益是dp[l][r-1]+nums[l-1]*nums[l]*nums[r+1]
4. 最后打爆中间的一个气球i时，产生的收益是dp[l][i-1]+dp[i+1][r]+nums[l-1]*nums[i]*nums[r+1]

dp[i][j]依赖当前行前面的元素，和当且列下面的元素。所以先填对角线，再从下到上，从左到右填

*/

class Solution {
public:
    void print_m(vector<vector<int>>& dp, string suff) {
        cout << suff << endl;;
        int m = dp.size();
        int n = dp[0].size();
        for (int i=0;i<m;i++) {
            for(int j=0;j<n;j++) {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    int maxCoins(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        if (len == 1) {
            return nums[0];
        }
        vector<int> help(len+2, 1);
        /*
        3,1,5,8 > 1,3,1,5,8,1
        len = 4
        h_len = 6
        1...4是整个有效区域
        */
        for (int i=0;i<len;i++) {
            help[i+1] = nums[i];
        }
        int h_len = help.size();
        //最终结果是dp[1][h_len-2]
        vector<vector<int>> dp(h_len, vector<int>(h_len, 0));
        for (int i=1;i<=len;i++) {
            dp[i][i] = help[i-1]*help[i]*help[i+1];
        }
        // print_m(dp, "init:");
        // cout << "help:" << endl;
        // for (auto it:help) {
        //     cout << it << " ";
        // }
        // cout << endl;

        for (int l=len-1;l>=1;l--) {
            for (int r=l+1;r<=len;r++) {
                
                int ans1 = dp[l+1][r] + help[l-1]*help[l]*help[r+1];//最后打爆l 
                int ans2 = dp[l][r-1] + help[l-1]*help[r]*help[r+1];//最后打爆r 
                int ans3 = INT_MIN;
                if (r-l>1) {
                    for (int i=l+1;i<=r;i++) { //最后打爆i
                        ans3 = max(ans3, dp[l][i-1] + help[l-1]*help[i]*help[r+1] + dp[i+1][r]);
                    }
                }
                // cout << "l:" << l << " r:" << r  << " ans1:" << ans1 << " ans2:" << ans2 << " ans3:" << ans3<< endl;
                dp[l][r] = max(ans1, max(ans2, ans3));
            }
        }
        // print_m(dp, "res:");
        return dp[1][len];
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
