/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
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
https://leetcode.cn/problems/house-robber/description/
你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。

给定一个代表每个房屋存放金额的非负整数数组，计算你 不触动警报装置的情况下 ，一夜之内能够偷窃到的最高金额。

 

示例 1：

输入：[1,2,3,1]
输出：4
解释：偷窃 1 号房屋 (金额 = 1) ，然后偷窃 3 号房屋 (金额 = 3)。
     偷窃到的最高金额 = 1 + 3 = 4 。
示例 2：

输入：[2,7,9,3,1]
输出：12
解释：偷窃 1 号房屋 (金额 = 2), 偷窃 3 号房屋 (金额 = 9)，接着偷窃 5 号房屋 (金额 = 1)。
     偷窃到的最高金额 = 2 + 9 + 1 = 12 。
 

提示：

1 <= nums.length <= 100
0 <= nums[i] <= 400
*/

/*


*/

class Solution {
public:
    int rob_baoli(vector<int>& nums) {
        int len = nums.size();
        if (len==0) {
            return 0;
        }
        return process(nums, 0);
    }
    /*
    每个位置的房屋都可以偷或者不偷。
    来到了i位置, 0...i范围上的房屋都已经决定好了偷或者不偷。返回i...len范围上能偷到的最大价值。
    当来能够来到最后的时候，已经没东西可以偷了，所以返回价值0。
    可能性划分：
        1. 偷i位置的房子，产生的价值再加上偷[i+2...len]范围上产生的最大价值
        2. 不偷i位置的房子，直接依赖偷[i+1...len]范围上产生的最大价值
    */
    int process(vector<int>& nums, int i) {
        if (i>=nums.size()) {
            return 0;
        }

        int no = process(nums, i+1);
        int yes = nums[i] + process(nums, i+2);
        return max(no,yes);
    }
    /*
    改dp。
    dp[i]表示[i...len]范围上可以自由选择偷还是不偷，产生的最大价值。
    dp[len]=0;
    返回dp[0]，依赖后面的值，所以从后往前填
    */
    int rob_dp(vector<int>& nums) {
        int len = nums.size();
        if (len==0) {
            return 0;
        }
        vector<int> dp(len+2, 0);//因为dp[len-1]有可能依赖dp[len+1]

        for(int i=len-1;i>=0;i--) {
            dp[i] = max(dp[i+1], nums[i]+dp[i+2]);
        }
        return dp[0];
    }

    //dp优化
    int rob(vector<int>& nums) {
        int len = nums.size();
        if (len==0) {
            return 0;
        }
        int i_1 = 0, i_2=0; //i+1和i+2的值初始为0
        int ans = 0;
        for(int i=len-1;i>=0;i--) {
            ans = max(i_1, nums[i]+i_2);
            i_2=i_1;
            i_1=ans;
        }
        return ans;
    }

};

int main()
{

    Solution sol;
    return 0;
}
