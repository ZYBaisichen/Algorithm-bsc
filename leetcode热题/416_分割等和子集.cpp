/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-23 12:59:02
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

https://leetcode.cn/problems/partition-equal-subset-sum/description/
给你一个 只包含正整数 的 非空 数组 nums 。请你判断是否可以将这个数组分割成两个子集，使得两个子集的元素和相等。

 

示例 1：

输入：nums = [1,5,11,5]
输出：true
解释：数组可以分割成 [1, 5, 5] 和 [11] 。
示例 2：

输入：nums = [1,2,3,5]
输出：false
解释：数组不能分割成两个元素和相等的子集。
 

提示：

1 <= nums.length <= 200
1 <= nums[i] <= 100
*/

/*
一开始想到从大到小排序，然后两个桶，哪个桶小就扔到哪个桶里。但行不通，看到题解之后才知道是一个np问题。

dp解法：
1. 因为要找到一种分法，将数组元素分成累加和相同的两部分。假设数组元素综合时sum，问题可以转化为在数组中找到一些元素，其累加和是sum/2
这就转化成了01背包问题的变体，即需要判断在0...i范围上能否找到一个组合刚好等于sum/2
2. 特殊情况，假设sum是奇数，不可能分成两个部分；假设最大值要比sum/2大，也不可能找到。
3. dp[i][j]代表在0...i范围上自由选择，是否可以组成累加和j
    a. 当nums[i]>j时，不能取nums[i]，依赖dp[i-1][j]
    b. 当nums[i]<=j时:
        取nums[i]，依赖dp[i-1][j-nums[i]]
        不取nums[i], 以来dp[i-1][j]
    c. 第一行当nums[0]==j时，dp[0][j]=true
    d. 第一列当nums[i]==0时，dp[i][0]=true;
*/
class Solution {
public:
    bool canPartition1(vector<int>& nums) {
        int len = nums.size();
        if (len <=1) {
            return false;
        }
        int sum = 0,max_num=INT_MIN;
        for (auto it:nums) {
            sum+=it;
            max_num = max(max_num, it);
        }
        if (sum%2==1) {
            return false;
        }
        if (max_num > sum/2) {
            return false;
        }
        
        int target = sum/2;
        vector<vector<int>> dp(len, vector<int>(target+1, 0));
        //第一列
        for (int i=0;i<len;i++) {
            if (nums[i] == 0) {
                dp[i][0] = true;
            }
        }
        //第一行
        for (int j=0;j<=target;j++) {
            if (nums[0] == j) {
                dp[0][j] = true;
            }
        }
        //依赖上一行的所有数
        for (int i=1;i<len;i++) {
            for (int j=1;j<=target;j++) {
                if (nums[i] > j) {
                    dp[i][j] = dp[i-1][j];
                }  else {
                    dp[i][j] = dp[i-1][j-nums[i]] || dp[i-1][j];
                }
            }
        }
        return dp[len-1][target];
    }

    //空间压缩
    bool canPartition(vector<int>& nums) {
        int len = nums.size();
        if (len <=1) {
            return false;
        }
        int sum = 0,max_num=INT_MIN;
        for (auto it:nums) {
            sum+=it;
            max_num = max(max_num, it);
        }
        if (sum%2==1) {
            return false;
        }
        if (max_num > sum/2) {
            return false;
        }
        
        int target = sum/2;
        vector<int> dp(target+1, 0);
        //第一行
        for (int j=0;j<=target;j++) {
            if (nums[0] == j) {
                dp[j] = true;
            }
        }
        //依赖上一行的所有数
        for (int i=1;i<len;i++) {
            //因为依赖的是j-nums[i]，只依赖左上的数，所以每一行需要从大到小填，保存左边上一行的值
            for (int j=target;j>=1;j--) { 
                if (nums[i] > j) {
                    dp[j] = dp[j];
                }  else {
                    dp[j] = dp[j-nums[i]] || dp[j];
                }
            }
        }
        return dp[target];
    }
};

int main()
{
    Solution sol;

    // vector<vector<string>> equations = {{"a","b"},{"b","c"}};
    // vector<double> values = {2.0,3.0};
    // vector<vector<string>> queries = {{"a","c"},{"b","a"},{"a","e"},{"a","a"},{"x","x"}};
    // 输出：[6.00000,0.50000,-1.00000,1.00000,-1.00000]

    vector<vector<int>> people = {{7,0},{4,4},{7,1},{5,0},{6,1},{5,2}};
    auto res = sol.reconstructQueue(people);
    for (auto& it:res) {
        cout << it[0] << "," << it[1] << endl;
    }
    cout << endl;

    // std::list<vector<int>> myList = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}};  
  
    // for (const auto& it : myList) { // 使用const auto&可以避免不必要的拷贝，并且如果元素是只读的，这是最佳选择  
    //     std::cout << it[0] << ' ' << endl;  
    // }  
    return 0;
}
