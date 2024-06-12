/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-07 11:49:39
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
https://leetcode.cn/problems/jump-game/description/
和第45题差不多，还有经典面试题35


给你一个非负整数数组 nums ，你最初位于数组的 第一个下标 。数组中的每个元素代表你在该位置可以跳跃的最大长度。

判断你是否能够到达最后一个下标，如果可以，返回 true ；否则，返回 false 。

 

示例 1：

输入：nums = [2,3,1,1,4]
输出：true
解释：可以先跳 1 步，从下标 0 到达下标 1, 然后再从下标 1 跳 3 步到达最后一个下标。
示例 2：

输入：nums = [3,2,1,0,4]
输出：false
解释：无论怎样，总会到达下标为 3 的位置。但该下标的最大跳跃长度是 0 ， 所以永远不可能到达最后一个下标。
 

提示：

1 <= nums.length <= 104
0 <= nums[i] <= 105
*/

/*
只需要往右跳，不需要往回返。看覆盖的范围可以到达len-1，就是true，最后也没到达就返回false
*/
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int len = nums.size();
        if (len==0 || len == 1) {
            return true;
        }

        int cur_i = nums[0];

        for (int i=1;i<nums.size();i++) {
            cout << endl;
            if (cur_i < i) {
                return false; //如果到不了当前i位置，肯定不能往后再跳了
            }
            if (cur_i >= len-1) {
                return true;
            }
            cur_i = max(cur_i, i+nums[i]);
        }
        return false;
    }
};
int main()
{

    Solution sol;
    return 0;
}