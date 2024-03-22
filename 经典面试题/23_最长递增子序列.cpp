/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 23:05:28
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
// 最长递增子序列和最长不下降子序列
// 给定一个整数数组nums
// 找到其中最长严格递增子序列长度、最长不下降子序列长度
// 测试链接 : https://leetcode.cn/problems/longest-increasing-subsequence/
class Solution {
public:
    //方法一：传统dp, 复杂度O(N^2)
    int lengthOfLIS1(vector<int>& nums) {
        int len = nums.size();
        vector<int> dp(len, 0); //dp[i]表示必须以i结尾的最长递增子序列是多少
        dp[0] = 1;
        int res = 1;
        for (int i=1;i<len;i++) {
            int len = 1;
            //看可以落在哪个字符后面
            for (int j=i-1;j>=0;j--) {
                if (nums[j] <nums[i]) {
                    len = max(len, dp[j]+1);
                }
            }
            dp[i] = len;
            res = max(res, dp[i]);
            // cout << "i:" << i << " dp[i]:" << dp[i] << endl;
        }
        return res;
    }

    //方法2，采用ends数组加速dp的寻找过程
    int lengthOfLIS(vector<int>& nums) {
        int len = nums.size();
        vector<int> dp(len, 0); //dp[i]表示必须以i结尾的最长递增子序列是多少
        vector<int> ends(len+1, 0); //ends[i]表示，长度为i+1的子序列，最后一个元素的最小值
        dp[0] = 1;
        ends[0] = nums[0];
        int e_right = 0;
        int res = 1;
        for (int i=1;i<len;i++) {
            int l = 0, r=e_right, mid=l+((r-l)>>1);
            //二分找到第一个大于等于nums[i]的数，假设这个数是b
            //b前面的数c（所在位置为d）其实也是找到了一种长度为d+1的递增序列，但因为nums[i]比c大，所以不会更新c
            int ans = -1;
            while (l<=r) {
                // cout << "l:" << l << " r:" << r << " mid:" << mid << endl;
                if (ends[mid] >= nums[i]) { //要着重注意这里的二分。最终l和r可能错过，也可能相等，这里巧妙用了个ans来记录大于等于nums[i]的情况
                    ans = mid;
                    r = mid-1;
                } else {
                    l = mid+1;
                }
                mid = l+((r-l)>>1);
            }
            // cout << "nums[i]:" << nums[i] <<" ans:" << ans << endl;
            if (ans == -1) {
                ends[++e_right] = nums[i];
                dp[i] = e_right+1;
            } else {
                ends[ans] = nums[i];
                dp[i] = ans+1;
            } 
            // for (int kk=0;kk<=e_right;kk++) {
            //     cout << " " << ends[kk];
            // }
            // cout << endl;
        }
        return e_right+1;
    }

};

int main()
{
    Solution sol;
    vector<string> arr= {"b#cst#1223#123","b#kl#fd","b#kl#io","d#", "a#de", "ab#c"};
    sol.print_contents(arr);
    return 0;
}