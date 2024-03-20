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

using namespace std;

/*
1. 两数之和：
给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 target  的那 两个 整数，返回所有满足条件的数对。
类似题目：https://leetcode.cn/problems/two-sum/description/

2. 三数之和
给你一个整数数组 nums ，判断是否存在三元组 [nums[i], nums[j], nums[k]] 满足 i != j、i != k 且 j != k ，同时还满足 nums[i] + nums[j] + nums[k] == 0 。请

你返回所有和为 0 且不重复的三元组。

注意：答案中不可以包含重复的三元组。

https://leetcode.cn/problems/3sum/

输入：nums = [-1,0,1,2,-1,-4]
输出：[[-1,-1,2],[-1,0,1]]
解释：
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0 。
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0 。
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0 。
不同的三元组是 [-1,0,1] 和 [-1,-1,2] 。
注意，输出的顺序和三元组的顺序并不重要。

*/


class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int target = 0;
        int len = nums.size();
        vector<vector<int>> res;
        if (len == 0)
        {
            return res;
        }
        std::sort(nums.begin(), nums.end());
        // cout << "sorted:" << endl;
        // for (auto iter:nums) {
        //     cout << " "<<iter;
        // }
        // cout << endl;

        //枚举第一个数，从后面的数中选两个数，转化为在L和R数组上求两数问题
        for (int i=0;i<len;i++) {
            int l = i+1, r = len - 1;
            while (l < r) {
                int sum = nums[i] + nums[l] + nums[r];
                // cout << "i:" << i << " l: " << l << ", r: " << r << " sum:" << sum<< endl;
                if (sum > target)
                {
                    r--;
                }
                else if (sum < target)
                {
                    l++;
                }
                else
                {
                    if (i == 0) {
                        if (l == i+1 || (nums[l - 1] != nums[l])) {
                            vector<int> tmp = {nums[l], nums[r], nums[i]};
                            res.push_back(tmp);
                        }
                    } else if (nums[i-1] != nums[i]) {
                        if (l == i+1 || (nums[l - 1] != nums[l])) {
                            vector<int> tmp = {nums[l], nums[r], nums[i]};
                            res.push_back(tmp);
                        }
                    }
                    l++;
                }
            }
        }
        return res;
    }

    vector<vector<int>> twoSum(vector<int> &nums, int target)
    {
        int len = nums.size();
        vector<vector<int>> res;
        if (len == 0)
        {
            return res;
        }
        std::sort(nums.begin(), nums.end());
        int l = 0, r = len - 1;

        while (l < r)
        {
            int sum = nums[l] + nums[r];
            cout << "l: " << l << ", r: " << r << " sum:" << sum<< endl;
            if (sum > target)
            {
                r--;
            }
            else if (sum < target)
            {
                l++;
            }
            else
            {
                if (l == 0 || nums[l - 1] != nums[l])
                {
                    vector<int> tmp = {nums[l], nums[r]};
                    res.push_back(tmp);
                }
                l++;
            }
        }
        return res;
    }
};

int main()
{
    Solution sol;
    // vector<int> arr = {3,1,2,5,2,4};
    vector<int> arr = {4,5,1,3,2};
    cout << "res:" << sol.calcVigorousWater2(arr) << endl;
    return 0;
}