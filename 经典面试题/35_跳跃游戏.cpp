/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-26 21:52:25
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
给定一个长度为 n 的 0 索引整数数组 nums。初始位置为 nums[0]。

每个元素 nums[i] 表示从索引 i 向前跳转的最大长度。换句话说，如果你在 nums[i] 处，你可以跳转到任意 nums[i + j] 处:

0 <= j <= nums[i] 
i + j < n
返回到达 nums[n - 1] 的最小跳跃次数。生成的测试用例可以到达 nums[n - 1]。

 

示例 1:

输入: nums = [2,3,1,1,4]
输出: 2
解释: 跳到最后一个位置的最小跳跃数是 2。
     从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。
示例 2:

输入: nums = [2,3,0,1,4]
输出: 2

提示:

1 <= nums.length <= 104
0 <= nums[i] <= 1000
题目保证可以到达 nums[n-1]

https://leetcode.cn/problems/jump-game-ii/description/

*/

/*
从左往右的尝试模型，因为每个位置都是跳大于等于0步，从0开始往右跳时，没有必要往左跳；

*/
class Solution
{
public:
    int jump(vector<int>& nums) {
        int len = nums.size();
        if (len==0) {
            return 0;
        }
        
        int cur_r = 0; //目前可以到的最右边界
        int step = 0;//到达cur_r跳了step步
        int next = -1; //记录每次到达一个位置可能的最右边界

        for (int i=0;i<len;i++) {
            if (cur_r < i) { //当越过前面能够到达的右边界时，step必须+1了。多走一步能到达的最右位置存在了next中
                step++;
                cur_r = next;
            } 
            int next_r = nums[i]+i;
            next = max(next_r, next);
        }
        return step;

    }
};

int main()
{

   Solution sol;
   vector<int> nums = {1,2};
   cout << sol.jump(nums) << endl;

    // cout << s.mox_xor_zero2(nums) << endl;
    // cout << s.max_xor2(nums) << endl;
    // cout << s.max_xor3(nums) << endl;

    return 0;
}