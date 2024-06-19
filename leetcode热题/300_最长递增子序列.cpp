/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-19 11:49:08
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

https://leetcode.cn/problems/longest-increasing-subsequence/description/

给你一个整数数组 nums ，找到其中最长严格递增子序列的长度。

子序列 是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。例如，[3,6,2,7] 是数组 [0,3,1,6,2,2,7] 的
子序列
。

 
示例 1：

输入：nums = [10,9,2,5,3,7,101,18]
输出：4
解释：最长递增子序列是 [2,3,7,101]，因此长度为 4 。
示例 2：

输入：nums = [0,1,0,3,2,3]
输出：4
示例 3：

输入：nums = [7,7,7,7,7,7,7]
输出：1
 

提示：

1 <= nums.length <= 2500
-104 <= nums[i] <= 104
 

进阶：

你能将算法的时间复杂度降低到 O(n log(n)) 吗?
*/

/*
同经典面试题23题

*/

class Solution {
public:
    //dp方法
    int lengthOfLIS(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        //dp[i]表示必须以nums[i]结尾时的递增子序列最大有多长
        vector<int> dp(len, 1); //至少是1
        int ans = 1;
        for (int i=1;i<len;i++) {
            for (int j=i-1;j>=0;j--) {
                if (nums[i] > nums[j]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            ans = max(ans, dp[i]);
        } 
        // for (int i=0;i<len;i++) {
        //     cout << dp[i] << " ";
        // }
        // cout << endl;
        return ans;
    }


    //dp加速
    int lengthOfLIS(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        //dp[i]表示必须以nums[i]结尾时的递增子序列最大有多长
        vector<int> dp(len, 1); //至少是1
        vector<int> ends(len, 0);//ends[i]表示，长度为i的子序列的最小值是多少。这里有个贪心，长度为i+1的最小值，一定比长度为i的序列的最小值大。
        ends[0] = nums[0];

        int ans = 1;
        int e_right = 0;
        for (int i=1;i<len;i++) {
            //二分查找第最左边大于nums[i]的序列
            //假设找到的这个序列长度为x，则有ends[i]>=nums，且ends[i-1]<nums
            //即nums[i]放在长度为i-1的后面，形成一个长度为i的序列，且最小值是nums[i]
            int l =0, r = e_right, mid=l+((r-l)>>1);
            int tmp = -1;
            while (l<=r) {
                if (ends[mid]>=nums[i]) {
                    tmp = mid;
                    r = mid-1;
                } else {
                    l = mid+1;
                }
                mid=l+((r-l)>>1);
            }

            if (tmp == -1) { //没有找到大于等于nums的数，则新增一个长度
                ends[++e_right] = nums[i];
                dp[i] = e_right + 1; //ends[e_right]表示长度为e_right+1的子序列最小结尾是多少
            } else {
                ends[tmp] = nums[i];
                dp[i] = tmp+1;
            }
            ans = max(ans, dp[i]);
        } 
        // for (int i=0;i<len;i++) {
        //     cout << dp[i] << " ";
        // }
        // cout << endl;
        return ans;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
int main()
{

    return 0;
}
