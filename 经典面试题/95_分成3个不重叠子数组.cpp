/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-15 10:54:22
 * @LastEditors: baisichen
 * @Description: 指针指向有点问题，leetcode没有通过，但思想已经通了
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
https://leetcode.com/problems/maximum-sum-of-3-non-overlapping-subarrays/

给你一个整数数组 nums 和一个整数 k ，找出三个长度为 k 、互不重叠、且全部数字和（3 * k 项）最大的子数组，并返回这三个子数组。

以下标的数组形式返回结果，数组中的每一项分别指示每个子数组的起始位置（下标从 0 开始）。如果有多个结果，返回字典序最小的一个。

 

示例 1：

输入：nums = [1,2,1,2,6,7,5,1], k = 2
输出：[0,3,5]
解释：子数组 [1, 2], [2, 6], [7, 5] 对应的起始下标为 [0, 3, 5]。
也可以取 [2, 1], 但是结果 [1, 3, 5] 在字典序上更大。
示例 2：

输入：nums = [1,2,1,2,1,2,1,2,1], k = 2
输出：[0,2,4]
 

提示：

1 <= nums.length <= 2 * 104
1 <= nums[i] < 216
1 <= k <= floor(nums.length / 3)

*/

/*
大流程：
    1. 固定枚举中间的子数组，开头位置从k到n-2*k+1
    2. 假设知道了两个辅助子数组，dp和dp2，dp[i]代表0...i范围上选一个长度为k的子数组的最大和。dp2[i]代表i-1...n-1范围上选有一个长度为k的子数组的最大和。
    3. 固定好中间的子数组后，左边选k个数的子数组的最大和由dp[i]给出，右边选k个数的子数组的最大和由dp2[j]给出
    最终记录切割点

在0...i范围上选k个数，累加和最大, dp[i]有以下几种可能：
    1. 不选用arr[i]作为子数组, 依赖dp[i-1]
    2. 选用arr[i]作为子数组，则累加和一定是arr[i-k ... i]

*/
class Solution {
public:
    vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
        int len = nums.size();
        //前缀数组
        vector<int> sum(len+1, 0);
        for (int i=0;i<len;i++) {
            sum[i+1] = sum[i] + nums[i];
        }

        //计算两个dp
        //第一个元素记录最大和，第二个位置记录起始位置
        vector<vector<int>> dp(len, vector<int>(2,0));
        vector<vector<int>> dp2(len, vector<int>(2,0));
        //枚举起始位置
        int max_sum = 0;
        int max_start = 0;
        for (int i=0;i<len-k;i++) {
            int cur = sum[i+k] - sum[i]; //sum是整体往后推了一个的， 计算当前累加和
            if (cur > max_sum) { //保持最小字典序，所以没有等于
                dp[i+k-1] = {cur, i};
                max_sum = cur;
                max_start = i;
            } else {
                dp[i+k-1] = {max_sum, max_start};
            }
        }

        max_sum = 0;
        max_start = 0;
        for (int i=len-k;i>=0;i--) {
            int cur = sum[i+k] - sum[i]; //sum是整体往后推了一个的， 计算当前累加和
            if (cur >= max_sum) { //保持最小字典序，前面的优先
                dp2[i] = {cur, i};
                max_sum = cur;
                max_start = i;
            } else {
                dp2[i] = {max_sum, max_start};
            }
        }

        vector<int> ans;
        //枚举中间的子数组
        max_sum = 0;
        for (int i=k; i< len-2*k+1;i++) {
            int cur = dp[i-1][0] + dp2[i+k][0] + (sum[i+k]-sum[i]);
            if (cur > max_sum) {
                ans = {dp[i-1][1], i, dp2[i+k][1]};
                max_sum = cur;
            }
        }
        return ans;
    }
};

int main() {
    Solution sol;
    vector<int> arr = {1, 2, 1, 2, 1, 2, 1, 2, 1};
    vector<int> ans = sol.maxSumOfThreeSubarrays(arr, 2);
    cout << endl;
    for (auto iter:ans) {
        cout << iter << " ";
    }
    cout << endl;
    return 0;
}