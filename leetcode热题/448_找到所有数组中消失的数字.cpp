/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-24 11:14:25
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
#include <list>

using namespace std;

/*

https://leetcode.cn/problems/find-all-numbers-disappeared-in-an-array/
给你一个含 n 个整数的数组 nums ，其中 nums[i] 在区间 [1, n] 内。请你找出所有在 [1, n] 范围内但没有出现在 nums 中的数字，并以数组的形式返回结果。

 

示例 1：

输入：nums = [4,3,2,7,8,2,3,1]
输出：[5,6]
示例 2：

输入：nums = [1,1]
输出：[2]
 

提示：

n == nums.length
1 <= n <= 105
1 <= nums[i] <= n
进阶：你能在不使用额外空间且时间复杂度为 O(n) 的情况下解决这个问题吗? 你可以假定返回的数组不算在额外空间内
*/

/*
使用下标循环怼的技巧，在原数组上改动。
1. 每到一个位置时，nums[i]在1-n之间。假设长度为n的数组，1-n所有的数字只出现了一次，则排序后其所在的下标是idx=nums[i]-1。
2. 将nums[idx]保存到一个临时变量origin中，将nums[idx]=-1，循环往下找nums[origin]，直到nums[origin]已经是-1了
*/

class Solution
{
public:
    void print_arr(vector<int> &arr, string suff)
    {
        cout << suff;
        for (auto it : arr)
        {
            cout << it << " ";
        }
        cout << endl;
    }
    vector<int> findDisappearedNumbers(vector<int> &nums)
    {
        int len = nums.size();
        for (int i = 0; i < len; i++)
        {
            int idx = nums[i];
            if (idx == -1)
            {
                continue;
            }
            int origin = nums[idx - 1];
            while (origin != -1)
            {
                // cout << "idx:" << idx << " origin:" << origin << endl;
                nums[idx - 1] = -1;
                idx = origin;
                origin = nums[idx - 1];
            }
            // cout << "i:" << i << endl;
            // print_arr(nums, "cur:");
            // nums[i]=-1;
        }
        vector<int> res;
        // print_arr(nums, "final:");
        for (int i = 1; i <= len; i++)
        {
            if (nums[i - 1] != -1)
            {
                res.push_back(i);
            }
        }
        return res;
    }
};


int main()
{
    Solution sol;

    vector<int> nums={4,3,2,7,8,2,3,1};
    auto res = sol.findDisappearedNumbers(nums); 
    
    cout << "res:";
    for (auto it:res){
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
