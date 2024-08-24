/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 17:38:51
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

https://leetcode.cn/problems/move-zeroes/description/
给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。

请注意 ，必须在不复制数组的情况下原地对数组进行操作。

 

示例 1:

输入: nums = [0,1,0,3,12]
输出: [1,3,12,0,0]
示例 2:

输入: nums = [0]
输出: [0]
 

提示:

1 <= nums.length <= 104
-231 <= nums[i] <= 231 - 1
*/

/*
类似荷兰国旗问题。维护一个边界是左边确定不是0的右边界.
往后遍历，当不是0时，往左交换

//因为要保持原数组元素顺序不变，所以要从左往右。不能从右往左
*/

class Solution
{
public:
    void moveZeroes(vector<int>& nums) {
        int j = 0;
        for (int i=0;i<nums.size();i++) {
            if (nums[i] != 0) {
                swap(nums[j++], nums[i]);
            }
        }
    }
};
int main()
{

    Solution sol;
    // vector<vector<int>> arr = {
    //     {0,30},
    //     {5,10},
    //     {15,20}};

    int n = 12;
    cout << sol.numSquares(n) << endl;
    return 0;
}
