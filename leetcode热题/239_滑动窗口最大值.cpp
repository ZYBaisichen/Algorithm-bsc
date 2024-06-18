/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 15:28:04
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

https://leetcode.cn/problems/sliding-window-maximum/description/
给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。

返回 滑动窗口中的最大值 。

 

示例 1：

输入：nums = [1,3,-1,-3,5,3,6,7], k = 3
输出：[3,3,5,5,6,7]
解释：
滑动窗口的位置                最大值
---------------               -----
[1  3  -1] -3  5  3  6  7       3
 1 [3  -1  -3] 5  3  6  7       3
 1  3 [-1  -3  5] 3  6  7       5
 1  3  -1 [-3  5  3] 6  7       5
 1  3  -1  -3 [5  3  6] 7       6
 1  3  -1  -3  5 [3  6  7]      7
示例 2：

输入：nums = [1], k = 1
输出：[1]
 

提示：

1 <= nums.length <= 105
-104 <= nums[i] <= 104
1 <= k <= nums.length

*/

/*
单调队列经典题目。rmq(区间最值问题)
同经典面试题104，之前用数组模拟的双端队列，这次用C++内置的写一遍

维护一个队列，从前到后单调递减。
1. 每次窗口右滑动一个数，当新的数比队尾的元素大时，则队尾的元素一定不可能再是窗口内的最大值了，所以需要弹出，让最新的数入进去
   假设比队尾元素小时，则直接入队
2. 窗口左边界往右滑动时，如果正好是队头元素，则出队
3. 为了防止出现重复数字，入队的始终是下标

*/

class Solution
{
public:
    void print_que(deque<int>& que, string suff) {
        cout << suff;
        // 遍历队列
        for (const auto &elem : que){
            std::cout << elem << " ";
        }
        cout << endl;
    }
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int len = nums.size();
        if (len < k) {
            return vector<int>();
        }
        deque<int> que;
        for (int i=0;i<k-1;i++) {
            if (que.empty()) {
                que.push_back(i);
            } else {
                while (!que.empty() && nums[que.back()] < nums[i]) {
                    // cout << "init111:" << nums[que.back()] << " " << nums[i] << "i:" << i << " k: " << k << endl;
                    que.pop_back();
                    // print_que(que,"while:");
                }
                que.push_back(i);
            }
        }
        // print_que(que, "init: ");
        vector<int> res(len-k+1, 0);
        for (int r=k-1,l=0;r<len;r++,l++) {
            while (!que.empty() && nums[que.back()] < nums[r]) {
                que.pop_back();
            }
            // cout << "r:" << r << endl;
            que.push_back(r);
            // print_que(que, "new_r: ");
            res[r-k+1] = nums[que.front()];
            if (l == que.front()) {
                que.pop_front();
            }
        }
        return res;
    }
};
int main()
{

    Solution sol;
    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    auto res = sol.maxSlidingWindow(arr, k);
    for (auto it:res) {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
