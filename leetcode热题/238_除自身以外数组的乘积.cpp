/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 15:03:40
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

https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/description/

给你一个整数数组 nums，返回 数组 answer ，其中 answer[i] 等于 nums 中除 nums[i] 之外其余各元素的乘积 。

题目数据 保证 数组 nums之中任意元素的全部前缀元素和后缀的乘积都在  32 位 整数范围内。

请 不要使用除法，且在 O(n) 时间复杂度内完成此题。



示例 1:

输入: nums = [1,2,3,4]
输出: [24,12,8,6]
示例 2:

输入: nums = [-1,1,0,-3,3]
输出: [0,0,9,0,0]


提示：

2 <= nums.length <= 105
-30 <= nums[i] <= 30
保证 数组 nums之中任意元素的全部前缀元素和后缀的乘积都在  32 位 整数范围内


进阶：你可以在 O(1) 的额外空间复杂度内完成这个题目吗？（ 出于对空间复杂度分析的目的，输出数组 不被视为 额外空间。）

*/

/*
1. O(n)空间复杂度
前缀乘积数组+后缀乘积数组
遍历到i位置时，从前缀数组中拿[0..i-1]的乘积，从后缀数组中拿[i+1...len-1]的乘积, 两个乘积相乘得到除i之外的乘积
*/

class Solution
{
public:
    void print_arr(vector<int> &nums, string suff) {
        cout << suff;
        for (auto it:nums) {
            cout << it << " ";
        }
        cout << endl;
    }
    vector<int> productExceptSelf1(vector<int> &nums)
    {
        int len = nums.size();
        if (len == 0)
        {
            return vector<int>();
        }

        vector<int> pre(len + 1, 1), depre(len + 2, 1);
        // 前缀从1到len
        for (int i = 1; i <= len; i++)
        {
            pre[i] = nums[i - 1] * pre[i - 1];
        }
        // 后缀从len到1
        for (int i = len; i >= 1; i--)
        {
            depre[i] = nums[i - 1] * depre[i + 1];
        }

        // print_arr(pre, "pre:");
        // print_arr(depre, "depre:");

        vector<int> res(len, 0);
        res[0] = depre[2];
        res[len - 1] = pre[len - 1];
        for (int i = 1; i < len - 1; i++)
        {
            res[i] = pre[i] * depre[i + 2];
        }
        return res;
    }

    //取巧的O(1)空间复杂度的解法
    //因为res不算额外空间复杂度，所以可以用它来做后缀数组。前缀乘积用一个整型记录就可以
    vector<int> productExceptSelf(vector<int> &nums) {
        int len = nums.size();
        if (len == 0) {
            return vector<int>();
        }
        if (len==1) {
            return nums;
        }

        vector<int> res(len, 1);
        res[len-1] = nums[len-1];
        // 后缀从len到1
        for (int i = len-2; i >= 0; i--) {
            res[i] = nums[i] * res[i + 1];
        }

        int pre = nums[0];
        res[0] = res[1];
        for (int i=1;i<len-1;i++) {
            //i之前的乘积是pre，之后的乘积在res[i+1]中
            res[i] = pre * res[i+1];
            pre *= nums[i];
        }
        res[len-1] = pre;
        return res;
    }



};
int main()
{

    Solution sol;
    vector<int> arr = {1, 2, 3, 4};
    int k = 2;
    auto res = sol.productExceptSelf(arr);
    for (auto it:res) {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
