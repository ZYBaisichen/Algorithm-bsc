/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-05 13:14:50
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

using namespace std;

/*
https://leetcode.cn/problems/find-first-and-last-position-of-element-in-sorted-array/description/

给你一个按照非递减顺序排列的整数数组 nums，和一个目标值 target。请你找出给定目标值在数组中的开始位置和结束位置。

如果数组中不存在目标值 target，返回 [-1, -1]。

你必须设计并实现时间复杂度为 O(log n) 的算法解决此问题。

 

示例 1：

输入：nums = [5,7,7,8,8,10], target = 8
输出：[3,4]
示例 2：

输入：nums = [5,7,7,8,8,10], target = 6
输出：[-1,-1]
示例 3：

输入：nums = [], target = 0
输出：[-1,-1]
 

提示：

0 <= nums.length <= 105
-109 <= nums[i] <= 109
nums 是一个非递减数组
-109 <= target <= 109

*/

/*
这个问题很像荷兰国旗问题，但它给出的数组已经是荷兰国旗了。所以可以直接在上面做二分查找
两次二分，分别找左边界和右边界。
注意每次要记录下来等于target的位置，比如要找左边界时：
    当nums[mid]==target时，记录ans=mid，并将r=mid-1

*/
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int len = nums.size();
        if (len==0) {
            return {-1,-1};
        }
        // if (len==1 && nums[0] == target) {
        //     return {0,0};
        // }
        int ans1=-1,ans2=-1;
        int l = 0, r=len-1,mid=0;
        while (l<=r) {
            mid = l + ((r-l) >> 1);
            // cout << "ans1: l:" << l << " r:" << r << " mid:" << mid << endl;
            if (nums[mid] == target) {
                ans1 = mid;
                cout << "ans1:" << ans1 << endl;
                r = mid-1;
            } else if (nums[mid] > target) {
                r = mid -1;
            } else {
                l = mid+1;
            }
        }

        l = 0, r=len-1,mid=0;
        while (l<=r) {  
            mid = l + ((r-l) >> 1);
            // cout <<"asn2: l:" << l << " r:" << r << " mid:" << mid << endl;
            if (nums[mid] == target) {
                ans2 = mid;
                l = mid+1;
            } else if (nums[mid] > target) {
                r = mid -1;
            } else {
                l = mid+1;
            }
        }
        return {ans1,ans2};
    }
};
int main()
{

    Solution sol;
    vector<int> nums = {5, 7, 7, 8, 8, 10};
    int t = 8;
    vector<int> res = sol.searchRange(nums, t);
    for(auto it:res) {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}