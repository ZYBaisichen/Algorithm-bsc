/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-27 13:36:08
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

/*
给你一个整数数组 nums ，你需要找出一个 连续子数组 ，如果对这个子数组进行升序排序，那么整个数组都会变为升序排序。

请你找出符合题意的 最短 子数组，并输出它的长度。



示例 1：

输入：nums = [2,6,4,8,10,9,15]
输出：5
解释：你只需要对 [6, 4, 8, 10, 9] 进行升序排序，那么整个表都会变为升序排序。
示例 2：

输入：nums = [1,2,3,4]
输出：0
示例 3：

输入：nums = [1]
输出：0


提示：

1 <= nums.length <= 104
-105 <= nums[i] <= 105


进阶：你可以设计一个时间复杂度为 O(n) 的解决方案吗？
https://leetcode.cn/problems/shortest-unsorted-continuous-subarray/description/
*/

/*
流程：
    1. 从左往右遍历，记录遍历过程中的max数，如果max大于当前的数，则标记当前数为叉。（即如果排序的话，前面的max需要越过当前数放在后面）
    2. 从右往左遍历，记录遍历过程中的min数，如果min小于当前的数，则标记当前数为叉号。（即如果排序的话，后面的min要越过当前数放到前面去）
    3. 取第一遍最右边标记叉号的数(因为再往右都是有序的了)作为右边界，第二遍的最左边记为叉号的数作为左边界（因为再往左都是有序的了）
    4. 两遍的打叉号的数列只会交错一个。第一遍最左边的叉号的数前面一个位置其实就是第二遍的最左边打叉号的数；
 */
class Solution
{
public:
    int findUnsortedSubarray(vector<int> &nums)
    {
        int right = -1;
        int left = -1;
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }

        int max_num = INT_MIN;
        for (int i = 0; i < len; i++)
        {
            if (max_num > nums[i])
            {
                right = i;
            }
            max_num = max(max_num, nums[i]);
        }

        int min_num = INT_MAX;
        for (int i = len - 1; i >= 0; i--)
        {
            if (min_num < nums[i])
            {
                // cout << "min_num:" << min_num << " nums[i]:" << nums[i] << endl;
                left = i;
            }
            min_num = min(min_num, nums[i]);
        }

        // cout << "right: " << right << " left:" << left<< endl;
        if (right!=-1&&left!=-1) {
            return right - left + 1;
        } else {
            return 0;
        }
    }
};
int main()
{

    Solution sol;
    // vector<int> arr = {2, 6, 4, 8, 10, 9, 15};
    vector<int> arr = {1,2,3,4};

    cout << sol.findUnsortedSubarray(arr) << endl;
    return 0;
}