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
https://leetcode.cn/problems/maximum-product-subarray/description/

给你一个整数数组 nums ，请你找出数组中乘积最大的非空连续
子数组
（该子数组中至少包含一个数字），并返回该子数组所对应的乘积。

测试用例的答案是一个 32-位 整数。

 

示例 1:

输入: nums = [2,3,-2,4]
输出: 6
解释: 子数组 [2,3] 有最大乘积 6。
示例 2:

输入: nums = [-2,0,-1]
输出: 0
解释: 结果不能为 2, 因为 [-2,-1] 不是子数组。
 

提示:

1 <= nums.length <= 2 * 104
-10 <= nums[i] <= 10
nums 的任何前缀或后缀的乘积都 保证 是一个 32-位 整数
*/

/*
暴力：
 列举出所有子数组，然后求每个子数组的乘积，复杂度O(N^3)
*/
class Solution
{
public:

    /*
    前缀和思路，无法解决0的情况，比如：
    [-2,0,-1]
    [-1,0,-2,2]
    */
    void print_arr(vector<int>& nums, string suff) {
        cout << suff;
        for (auto it:nums) {
            cout << it << " ";
        }
        cout << endl;
    }
    int maxProduct1(vector<int>& nums) {
        int len = nums.size();
        if (len ==0) {
            return 0;
        }
        vector<int> multi(len+1, 1),demulti(len+2,1);
        int ans = INT_MIN;
        for (int i=1;i<=len;i++) {
            if (multi[i-1]==0) {
                multi[i] = nums[i-1];
            } else {
                multi[i] = multi[i-1] * nums[i-1];
            }
            ans = max(ans, nums[i-1]);
        }
        for (int i=len;i>=1;i--) {
            if (demulti[i+1] == 0) {
                demulti[i] = nums[i-1];
            } else {
                demulti[i] = demulti[i+1] * nums[i-1];
            }
        }
        print_arr(multi, "multi:");
        print_arr(demulti, "demulti:");

        
        for (int i=1;i<=len;i++) {
            for (int start=0;start<=i-1;start++) {
                if (multi[start] == 0) {
                    continue;
                } 
                ans = max(ans, multi[i]/multi[start]);
            }
        }

        for (int i=len;i>=1;i--) {
            for (int end=len+1;end>=i+1;end--) {
                if (demulti[end] == 0) {
                    continue;
                } 
                ans = max(ans, demulti[i]/demulti[end]);
            }
        }
        return ans;
    

    //初探dp，超时
    int maxProduct_dp1(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }

        vector<int> dp(len, INT_MIN);
        dp[0] = nums[0];
        int ans = dp[0], tmp=1;
        for (int i=1;i<len;i++) {
            dp[i] = nums[i];
            tmp = nums[i];
            for (int start = i-1;start>=0;start--) {
                tmp *= nums[start];
                dp[i] = max(dp[i], tmp);
            }
            ans = max(ans, dp[i]);
        }
        // print_arr(dp, "dp:");
        return ans;
    }


   double baoli_ans;
    struct info{
        double _max_ans;
        double _min_ans;
        info(double _max, double _min) {
            _max_ans = _max;
            _min_ans = _min;
        }
    };
    int maxProduct(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        baoli_ans = INT_MIN;
        process(nums, len-1);
        return baoli_ans;
    }
    /*
    返回0..i范围上的必须以i结尾的最大乘积和最小乘积
    */
    info process(vector<int>& nums, int i) {
        if (i==0) {
            baoli_ans = nums[0]>baoli_ans?nums[0]:baoli_ans;
            return info(nums[0],nums[0]);
        }

        double max_ans = nums[i];
        double min_ans = nums[i];

        info _info = process(nums, i-1);
        max_ans = max(max_ans, max(nums[i]*_info._max_ans, nums[i]*_info._min_ans));
        min_ans = min(max_ans, min(nums[i]*_info._max_ans, nums[i]*_info._min_ans));

        baoli_ans = max(baoli_ans, max(max_ans, min_ans));
        return info(max_ans, min_ans);
    }

    /*
    dp[i]必须以nums[i]为结尾的最大乘积和：
        1. nums[i]自己
        2. 往左扩充和dp[i-1]相乘
        两者取最大，但这样有个问题，比如[5,6,-3,4,-3]，dp求出来是[5,30,-3,4,4], 最后一个数是求不对的，答案应该是全部的乘积。
        原因就出在负数上，当为负数时，也期望前面的乘积也是负数，且尽可能的小。如果是正数，也期望前面的乘积也是正数，且尽可能的大。
        所以需要维护一个最小的dp表，用来尽可能的捕捉负数。就有了第三种情况
        3. 往左扩充和dp2[i-1]相乘，当nums[i]和dp2[i-1]都是负数时，这个将会是最优解。
    */
    int maxProduct(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        vector<double> dp(len, INT_MIN);//dp[i]表示以i为结尾的子序列的最大乘积
        vector<double> dp2(len, INT_MIN);//dp2[i]表示以i为结尾的子序列的最小乘积

        dp[0] = nums[0];
        dp2[0] = nums[0];
        double ans = dp[0];
        for (int i=1;i<len;i++) {
            dp[i] = nums[i];
            dp2[i] = nums[i];
            double val = nums[i];
            // double tmp = min(val*dp2[i-1], val*dp[i-1]);
            // cout << "nums[i]:" << nums[i] << " dp2[i]:" << dp2[i]  << " min(nums[i]*dp2[i-1], nums[i]*dp[i-1]):" << tmp << " min(tmp,dp2[i]):" << min(tmp, dp2[i])<<endl;
            dp[i] = max(dp[i], max(val*dp[i-1], val*dp2[i-1]));
            dp2[i] = min(dp2[i], min(val*dp2[i-1], val*dp[i-1]));
            // cout << "i:"<<i << ":" << dp[i]<< "," << dp2[i] << endl;
            ans = max(ans, max(dp[i], dp2[i]));
        }
        // print_arr(dp, "dp:");
        // print_arr(dp, "dp2:");
        return (int)ans;
    }

    //dp优化
    int maxProduct(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        double imax = nums[0], old_imax=0;
        double imin = nums[0];
        double ans = imax, val=0;
        for (int i=1;i<len;i++) {
            val = nums[i];
            old_imax = imax;
            imax = max(val, max(val*imax, val*imin));
            imin = min(val, min(val*imin, val*old_imax));
            ans = max(ans, max(imin, imax));
        }
        return (int)ans;
    }

    
};

int main()
{

    Solution sol;
    return 0;
}
