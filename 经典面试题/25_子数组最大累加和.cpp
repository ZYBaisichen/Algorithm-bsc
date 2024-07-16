/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 23:05:28
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

using namespace std;
// 子数组最大累加和
// 给你一个整数数组 nums
// 返回非空子数组的最大累加和
// 测试链接 : https://leetcode.cn/problems/maximum-subarray/

class Solution {
public:
    //dp
    int maxSubArray2(vector<int>& nums) {
        int len = nums.size();
        if(len == 0) {
            return 0;
        }
        vector<int> dp(len, 0); //dp[i]表示必须以i结尾的最大累加和是多少
        dp[0] = nums[0]; 
        int ans = dp[0];
        for (int i=1;i<len;i++) {
            dp[i] = max(nums[i], nums[i] + dp[i-1]); //加上前面的元素有利可图则加，无利可图则不加
            ans = max(ans, dp[i]);
            // cout << "i:" << i<<" dp[i]:" << dp[i] << endl;
        }
        return ans;
    }

    /*
    贪心流程：
    准备变量cur和max，初始cur=0， max=INT_MIN。从左到右遍历
    1. 当遍历到i位置时，将nuns[i]加到cur上，如果cur大于max则更新max
    2. 如果cur加到了0以下，则将cur重置为0
    3. 最后返回max

    
    a. 当数组中全是负数时: [-3,-2,-5,-1,-4]成立，最终输出结果为-1
    b. 当数组中有正数（假设答案法）
        i) 假设有一段子数组不仅是累加和最大的，也是所有最大累加和数组中最长的子数组。下面只要证明，上面的流程能抓到这个答案就可以（不错过这个答案）
        ii) 假设这个子数组是以i开头，以j结尾。 则i开头的任何前缀数组[i...k]和都是大于0的。因为如果小于0的话，则[k..j]的和要比[i...j]大了。
        iii) i之前的某个位置k到i-1组成的子数组累加和不可能大于0（因为这样的话累加和最大数组就是[k...j]了）； 同时也不可能等于0，如果等于0，最长的数组是[k...j]
        iv) 综上，cur将要累加i时一定是等于0的状态。所以从i加到j时，都会更新max。所以不会错过[i...j]这个答案

    这里是先告诉流程再使用假设答案法证明。正常流程是使用假设答案法分析各种可能，然后根据各个可能设计流程
    */
    int maxSubArray(vector<int>& nums) {
        int len = nums.size();
        if(len == 0) {
            return 0;
        }
        int cur = 0, max_num=INT_MIN;
        for (int i=0;i<len;i++) {
            cur += nums[i];
            max_num = max(max_num, cur);
            if (cur<0) {
                cur = 0;
            }
        }
        return max_num;
    }


};

int main()
{
    Solution sol;
    vector<int> arr= {-2,1,-3,4,-1,2,1,-5,4};
    //输出6
    cout << sol.maxSubArray(arr) << endl;
    return 0;
}