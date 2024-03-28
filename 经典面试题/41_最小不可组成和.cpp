/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-28 17:01:47
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
给定一个全是正数的数组arr，定义一下arr的最小不可组成和的概念：
1，arr的所有非空子集中，把每个子集内的所有元素加起来会出现很多的值，其中最小的 记为min，最大的记为max；
2，在区间[min,max]上，如果有一些正数不可以被arr某一个子集相加得到，那么这些正数中最小的那个，就是arr的最小不可组成 和；
3，在区间[min,max]上，如果所有的数都可以被arr的某一个子集相加得到，那么max+1是arr的最小不可组成和；
举例： arr = {3,2,5} arr的min为2， max为10，在区间[2,10]上，4是不能被任何一个子集相加得到的值中最小的，所以4是arr的最小不可组成和；
arr = {3,2,4} arr的min为2，max为9，在区间 [2,9]上，8是不能被任何一个子集相加得到的值中最小的，所以8是arr的最小不可组成和；
arr = {3,1,2} arr的min为1，max为6，在区间[2,6]上，任何数都可以 被某一个子集相加得到，所以7是arr的最小不可组成和；
请写函数返回arr的最小不可组成和。

进阶问题：
已知正数数组中肯定有1这个数，是否能更快地得到最小不可组成和

https://blog.csdn.net/lil_ghost_/article/details/130115060
*/
typedef long long ll;
class Solution
{
public:
    //  暴力方法
    int unformedSum1(vector<int> &nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        int min_num = INT_MAX;
        for (int i=0;i<len;i++) {
            min_num = min(min_num, nums[i]);
        }

        set<long long> sum_set;  
        long long max_sum = INT_MIN;      
        f(nums, 0, sum_set, 0, max_sum);

        for (long long i = min_num;i<=max_sum;i++) {
            if (sum_set.find(i) == sum_set.end()) {
                return i;
            }
        }
        return max_sum + 1;
    }

    //[0...idx]的子数组都已经决策好了，到了idx位置做决策
    //将子序列的和放在sum_set中
    void f(vector<int> &nums, int idx, set<long long>& sum_set, long long sum, long long& max_sum) {
        if (idx == nums.size()) {
            // cout << "sum:" << sum << endl;
            max_sum = max(max_sum, sum);
            sum_set.insert(sum);
            return;
        }
        f(nums, idx+1, sum_set, sum, max_sum); //不加idx
        f(nums, idx+1, sum_set, sum + nums[idx], max_sum); //加上idx的数
    }
    

    //长度为idx时的子序列，能否组成min_sum到range的数， 构想
    // bool g(vector<int> &nums, int idx, ll range) {
    //     cout << "min_sum:" << min_sum << ",idx:" << idx << ",range:" << range << endl;
    //     if (idx==0) {
    //         if (range == nums[idx]) {
    //             return true;
    //         }else {
    //             return false;
    //         }
    //     }
    //     bool no = g(nums, min_sum, idx-1, range); //不将idx放入子序列，依赖前面的idx-1能不能解决min_sum range
    //     bool yes = g(nums, min_sum, idx-1, range-nums[idx]); //将idx放入子序列，依赖前面idx-1能不能解决min_sum range-nums[idx]
    //     return no || yes;
    // }
    /*
    dp[i][j]代表长度为i-1的子序列，能否能否组成0...j的sum和
    01背包问题：
    1. 加上nums[i]后，依赖前面的[0...i-1]解决j-nums[i]范围上的数
    2. 不加nums[i]，依赖前面的[0...i-1]解决j范围上的数
    3. 两种情况有一个符合就置为true
    */
    int unformedSum2(vector<int> &nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        
        int min_num = INT_MAX;
        ll sum = 0;
        for (int i=0;i<len;i++) {
            min_num = min(min_num, nums[i]);
            sum+=nums[i];
        }

        vector<vector<bool>> dp(len, vector<bool>(sum+1, false));
        //怎么都能组成0
        for (int i=0;i<len;i++) {
            dp[i][0] = true;
        }
        dp[0][nums[0]] = true;
        for (int i=1;i<len;i++) {
            for (int j=1;j<=sum;j++) {
                dp[i][j] = dp[i-1][j]; //和当前的数无关
                if (j-nums[i] >= 0) {
                    dp[i][j] = dp[i][j] || dp[i-1][j-nums[i]]; //和当前的数有关
                }
            }
        }

        for (ll i=min_num;i<=sum;i++) {
            if (!dp[len-1][i]) {
                return i;
            }
        }

        return sum+1;
    }

    /*
    进阶问题：假设arr中肯定有1。
    因为有1存在，且都是正数，所以数组排序后肯定在1位置，第一个能到达的数肯定是1
    流程：
    1. 令range=1，代表当前能来到的最大范围数
    2. 当遍历到第i个数时：
        a. nums[i] <= range+1时：
            i) 比如range到了99，来了个100，那么100是可以加出来。100加前面的1也可以，最多加到199
            ii) range来到99，来了个89，则90可以，91可以，最多到99+89=188
            ii) 综上当nums[i]<=range+1时，[range+1...nums[i]+range]所有的数都可以得到
        b. nums[i]>range+1时：
            i) range来到99，来了个101，那么100怎么都不可能加出来
            ii) 所以此时最小的不可累加和是range+1
    */
    int unformedSum3(vector<int> &nums)
    {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        sort(nums.begin(), nums.end());
        int range = 1;
        for (int i = 1; i < len; i++)
        {
            if (nums[i] <= range + 1)
            {
                range += nums[i];
            }
            else
            {
                return range + 1;
            }
        }
        return range + 1;
    }
};
int main()
{

    Solution sol;
    // vector<int> arr = {2, 6, 4, 8, 10, 9, 15};
    vector<int> arr = {1,2,3,9,89,100};

    cout << sol.unformedSum1(arr) << endl;
    cout << sol.unformedSum2(arr) << endl;
    cout << sol.unformedSum3(arr) << endl;
    return 0;
}