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

https://leetcode.cn/problems/target-sum/description/
给你一个非负整数数组 nums 和一个整数 target 。

向数组中的每个整数前添加 '+' 或 '-' ，然后串联起所有整数，可以构造一个 表达式 ：

例如，nums = [2, 1] ，可以在 2 之前添加 '+' ，在 1 之前添加 '-' ，然后串联起来得到表达式 "+2-1" 。
返回可以通过上述方法构造的、运算结果等于 target 的不同 表达式 的数目。

 

示例 1：

输入：nums = [1,1,1,1,1], target = 3
输出：5
解释：一共有 5 种方法让最终目标和为 3 。
-1 + 1 + 1 + 1 + 1 = 3
+1 - 1 + 1 + 1 + 1 = 3
+1 + 1 - 1 + 1 + 1 = 3
+1 + 1 + 1 - 1 + 1 = 3
+1 + 1 + 1 + 1 - 1 = 3
示例 2：

输入：nums = [1], target = 1
输出：1
 

提示：

1 <= nums.length <= 20
0 <= nums[i] <= 1000
0 <= sum(nums[i]) <= 1000
-1000 <= target <= 1000
*/

/*
暴力找

*/
class Solution {
public:
    int findTargetSumWays1(vector<int>& nums, int target) {
        return process(nums, 0, 0, target);
    }
    /*
    来到了i位置
    当前加和是sum
    */
    int process(vector<int>& nums, int i, int sum, int target) {
        int len = nums.size();
        if (i==len) {
            // cout << "i:" << i << " j:" << sum << " ans:" << (sum == target ? 1 : 0) << endl;
            return sum == target ? 1 : 0;
        }
        //加个减号
        int sub = process(nums, i+1, sum-nums[i], target);
        //加个加号
        int add = process(nums, i+1, sum+nums[i], target);
        // cout << "i:" << i << " j:" << sum << " ans:" << sub+add << endl;
        return sub+add;
    }


    //记忆化搜索
    int findTargetSumWays2(vector<int> &nums, int target) {
        map<int, map<int,int>> _map;
        return process2(nums, 0, 0, target, _map);
    }
    /*
    来到了i位置
    当前加和是sum
    */
    int process2(vector<int> &nums, int i, int sum, int target, map<int, map<int,int>>& _map)
    {   
        if (_map.find(i) != _map.end()) {
            if (_map[i].find(sum) != _map[i].end()) {
                return _map[i][sum];
            }
        }
        int len = nums.size();
        if (i == len)
        {
            _map[i][sum] = sum == target ? 1 : 0;
            // cout << "i:" << i << " j:" << sum << " ans:" << (sum == target ? 1 : 0) << endl;
            return _map[i][sum];
        }
        // 加个减号
        int sub = process2(nums, i + 1, sum - nums[i], target, _map);
        // 加个加号
        int add = process2(nums, i + 1, sum + nums[i], target, _map);
        _map[i][sum] = sub+add;
        // cout << "i:" << i << " j:" << sum << " ans:" << sub + add << endl;
        return sub + add;
    }
    int findTargetSumWays3(vector<int> &nums, int target)
    {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        int sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += nums[i];
        }
        int a = (sum - target) / 2;
        return process3(nums, 0, 0, a);
    }
    int process3(vector<int> &nums, int i, int sum, int a) {
        int len = nums.size();
        if (i==len) {
            if (sum == a) {
                return 1;
            } else {
                return 0;
            }
        }

        int no = process3(nums, i+1, sum, a);
        int yes = process3(nums, i+1, sum+nums[i], a);
        return no+yes;

    }
    /*
    将增加和减少都转换成累加；
    nums的总和是sum，假设所有前面加了减号的元素之和为a, 则其他所有前面加了加好的元素之和是b=sum-a
    因为target=b-a， 所以target=(sum-a)-a=sum-2a；a=(sum-target)/2
    问题转化成了在nums中自由选择数字，累加和是a的组合数有多少种。
    dp[i][j]表示0.。。i-1都已经搞定，当前累加和是j，在i...len范围上继续寻找数搞定target的组合数
    1. dp[len][a] = 1
    2. 当选择i时，dp[i][j]=dp[i+1][j+nums[i]], 条件是j+nums[i]<=a
    3. 当不选择i时，dp[i][j]=dp[i+1][j]
    4. 两种可能性相加
    */
   /*就是这样写有点别扭，重新写下递归*/
    int findTargetSumWays_dp1(vector<int>& nums, int target) {
        int len = nums.size();
        if (len==0) {
            return 0;
        }
        int sum = 0;
        for (int i=0;i<len;i++) {
            sum += nums[i];
        }
        if (target > sum || (sum-target)%2!=0) {
            return 0;
        }
        int a =(sum-target)/2;
        cout << "a = " << a << endl;
        vector<vector<int>> dp(len+1, vector<int>(a+1, 0));
        dp[len][a] = 1;

        for (int i=len-1;i>=0;i--) {
            for (int j=0;j<=a;j++) {
                dp[i][j] = dp[i+1][j];//不选i
                if (j+nums[i]<=a) {
                    dp[i][j] += dp[i+1][j+nums[i]];//选i
                }
            }
        }
        for (int i=0;i<=len;i++) {
            for (int j=0;j<=a;j++) {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        return dp[0][0];
    }

    int findTargetSumWays4(vector<int> &nums, int target)
    {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        int sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += nums[i];
        }
        if (target > sum || (sum-target)%2!=0) {
            return 0;
        }
        int a = (sum - target) / 2;
        
        return process4(nums, 0, a);
    }
    /*
    当前来到了i，0...i-1都已经搞定，下面还需要再i...len上选累加和是rest的子数组

    */
    int process4(vector<int> &nums, int i, int rest) {
        int len = nums.size();
        if (i>len) {
            return 0;   
        }
        if (rest==0) {
            return 1;
        }

        int no = process4(nums, i+1, rest); //不选
        int yes = process4(nums, i+1, rest-nums[i]);
        return no+yes;
    }

    //清晰版递归改动态规划，背包问题经典解法
    int findTargetSumWays(vector<int> &nums, int target)
    {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        int sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += nums[i];
        }
        if (target > sum || (sum-target)%2!=0) { //得不到一个偶数是sum-target，则一定拼接不成
            return 0;
        }
        int a = (sum - target) / 2;

        vector<vector<int>> dp(len+1, vector<int>(a+1, 0));
        dp[len][0] = 1;
        for (int i = len-1;i>=0;i--) {
            for (int rest=0;rest<=a;rest++) {
                dp[i][rest] = dp[i+1][rest];//不选i
                if (rest-nums[i]>=0) {
                    dp[i][rest] += dp[i + 1][rest - nums[i]]; // 选i. i+1...len搞定剩下的rest-nums[i]
                }
            }
        }
        return dp[0][a];
    }

};

int main()
{
    Solution sol;

    vector<int> nums = {1, 1, 1, 1, 1};
    int t = 3;
    cout <<sol.findTargetSumWays1(nums, t) << endl;
    cout << sol.findTargetSumWays2(nums, t) << endl;
    cout << sol.findTargetSumWays3(nums, t) << endl;
    cout << sol.findTargetSumWays_dp1(nums, t) << endl;
    cout << sol.findTargetSumWays4(nums, t) << endl;
    cout << sol.findTargetSumWays(nums, t) << endl; //最终版

    return 0;
}
