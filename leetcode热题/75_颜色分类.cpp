/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-11 13:01:44
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
https://leetcode.cn/problems/sort-colors/description/
给定一个包含红色、白色和蓝色、共 n 个元素的数组 nums ，原地对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。

我们使用整数 0、 1 和 2 分别表示红色、白色和蓝色。

必须在不使用库内置的 sort 函数的情况下解决这个问题。

 

示例 1：

输入：nums = [2,0,2,1,1,0]
输出：[0,0,1,1,2,2]
示例 2：

输入：nums = [2,0,1]
输出：[0,1,2]
 

提示：

n == nums.length
1 <= n <= 300
nums[i] 为 0、1 或 2
 

进阶：

你能想出一个仅使用常数空间的一趟扫描算法吗？
*/


/*
经典荷兰国旗问题
*/


class Solution {
public:
    void print_arr(vector<int>& nums, string suff) {
        cout << suff;
        for (auto it:nums) {
            cout << it << " ";
        }
        cout << endl;
    }
    void sortColors(vector<int>& nums) {
        int len = nums.size();
        int left=-1, right=len;
        int i=0;
        while (i < len && i<right) {
            if (nums[i] < 1) {
                // print_arr(nums, "little:");
                swap(nums[++left], nums[i++]);
                // print_arr(nums, "little_after:");
            } else if (nums[i]>1) {
                // print_arr(nums, "big:");
                swap(nums[--right], nums[i]);
                // print_arr(nums, "big_after:");
            } else {
                i++;
            }
        }
        // cout << "left:" << left << " right:" << right << endl;
    }
};
int main()
{

    Solution sol;
    vector<int> nums = {2,0,2,1,1,0};
    sol.sortColors(nums);
    for (auto it:nums) {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
