/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-24 15:18:04
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

https://leetcode.cn/problems/shortest-unsorted-continuous-subarray/

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
*/

/*


*/


/*
和经典面试题40题一样
1. 遍历两遍数组，第一遍从左往右遍历，记录最大值max_num，当前数max_num > nums[i]说明，左边有比nuns[i]大的数，nums[i]是一个逆序，标记叉号。
2. 第二遍从右往左遍历，记录最小值min_num, 当nums[i]<min_num 时，说明右边有比nums[i]小的，nums[i]是一个逆序，标记叉号。
3. 第一遍最右边不合法的数就是最小需要排序数组的右边界；第二遍最左边的逆序的数就是最小需要排序数组的左边界

第一遍的第一个叉号位置是i，说明左边有个数比它大，i要排在它左边；最右边的叉号是j，左边有个数比它大，j要排在它左边。而j已经包含进来了，所以其作为右边界
第二遍的第一个叉号位置是j，说明左边有个数比j小，j要排在它右边，所以不能作为右边界；最后一个叉号位置i，说明右边有一个数比i小，i要排在它右边，所以可以作为左边界。

*/
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        int max_num = nums[0], right=-1;
        for (int i=1;i<len;i++) {
            if (max_num > nums[i]) {
                right = i;
            }
            max_num = max(max_num, nums[i]);
        }
        int min_num = nums[len-1], left=-1;
        for (int i=len-2;i>=0;i--) {
            // cout << "i:" << i << " nums[i]:" << nums[i] << " min_num:" << min_num << endl;
            if (nums[i] > min_num) {
                left = i;
            }
            min_num = min(min_num, nums[i]);
        }
        if (left == -1 || right==-1) {
            return 0;
        }
        // cout << "left:" << left << " right:" << right << endl;
        return right-left+1;
    }
};

int main()
{
    Solution sol;

    return 0;
}
