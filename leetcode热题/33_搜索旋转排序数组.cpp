/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-05 12:15:33
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
https://leetcode.cn/problems/search-in-rotated-sorted-array/
整数数组 nums 按升序排列，数组中的值 互不相同 。

在传递给函数之前，nums 在预先未知的某个下标 k（0 <= k < nums.length）上进行了 旋转，使数组变为 [nums[k], nums[k+1], ..., nums[n-1], nums[0], nums[1], ..., nums[k-1]]（下标 从 0 开始 计数）。例如， [0,1,2,4,5,6,7] 在下标 3 处经旋转后可能变为 [4,5,6,7,0,1,2] 。

给你 旋转后 的数组 nums 和一个整数 target ，如果 nums 中存在这个目标值 target ，则返回它的下标，否则返回 -1 。

你必须设计一个时间复杂度为 O(log n) 的算法解决此问题。

 

示例 1：

输入：nums = [4,5,6,7,0,1,2], target = 0
输出：4
示例 2：

输入：nums = [4,5,6,7,0,1,2], target = 3
输出：-1
示例 3：

输入：nums = [1], target = 0
输出：-1
 

提示：

1 <= nums.length <= 5000
-104 <= nums[i] <= 104
nums 中的每个值都 独一无二
题目数据保证 nums 在预先未知的某个下标上进行了旋转
-104 <= target <= 104

*/

/*
暴力肯定是排序，然后二分， O(n*log(n))，但既然题目说了是半有序数组，还是需要利用的。

精致的二分：
对于l,r,m分别表示要搜索范围上的左端点，右端点，中间位置。传统二分只需要比较nums[m]和target的大小，就可以知道target在左边还是右边范围上。
但本题有旋转， 所以还需要考虑两个边界情况
1. 当nums[mid]==target时, 返回mid
2. 因为数组发生了旋转，对于mid分隔的数组两边，至少有一边是有序的。
    a. 当左边有序，且target在[l,mid)上时，则继续在左边查找，r=mid-1；否则l=mid+1
    b. 当右边有序，且target在[mid,r]上时，则继续在右边查找，l=mid+1；否则r=mid-1
4. 循环往复1-2步骤，直到l>r结束

*/
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int len = nums.size();
        if (len == 0) return -1;
        if (len == 1) return nums[0] == target ? 0 : -1;

        int l = 0, r=len-1,mid=0;
        while (l <= r) {
            mid = (l+r)/2;
            // cout << "l:" << l << " r:" << r << " mid:" << mid << " nums[l]:" << nums[l] << " nums[r]:" << nums[r] << " nums[mid]:" << nums[mid] << endl;
            if (nums[mid] == target) {
                return mid;
            }
            if (nums[l] <= nums[mid]) { //左边是有序的
                if (nums[l] <= target && target < nums[mid]) {
                    r = mid-1;
                } else {
                    l = mid+1;
                }
            } else { //右边是有序的
                if (target > nums[mid] && target <= nums[r]) {
                    l = mid +1;
                } else {
                    r = mid-1;
                }
            }
        }
        return -1;
    }
};
int main()
{

    Solution sol;
    vector<int> nums = {4, 5, 6, 7, 8, 1, 2, 3};
    int t = 8;
    for (int i=0;i<nums.size();i++) {
        t = nums[i];
        cout <<  "cur:" << nums[i] << " :"<< sol.search(nums,t) << endl;
    }

    return 0;
}