/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-08 20:35:50
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
#include <sstream>

using namespace std;

/*
给定一个非负数组arr，和一个整数m
返回arr的所有子序列中累加和%m之后的最大值
情况1：m不算大，arr[i]的数不大
情况2：arr[i]很大，m不算太大
情况3：m和arr[i]都很大，但arr数组长度较小，分治
*/

/*
背包问题
1. dp[i][j]：0...i的子序列能否组成和为j的值，j取值0..sum。将能搞定的j都模m，一定可以找到模m的最大值。每个格子O(1)求出，整个表复杂度是O(n*sum)
sum很大时复杂度就不太行了
2. dp[i][j]：0...i的子序列能不能在累加和在模完m后，变成j。j取值是0...m-1。
    a. 第一行：dp[0][j]：只有一个数，dp[0][arr[0]%m]=true, 其他是false
    b. 第一列：累加和为0，即一个数不选时，模m的结果是0
    c. 普遍位置dp[i][j]:
        i) 当不选i位置的数时，dp[i][j]=dp[i-1][j]
        ii) 当使用i位置的数时，依赖dp[i-1][j-arr[i]%m]
            这里需要注意，如果j-arr[j]%m>=0时，依赖dp[i-1][j-arr[i]%m]
            否则，依赖dp[i-1][m+j-arr[i]%m]， 转了一圈回到了0...m-1范围
    d. 复杂度O(n*m)
3. m和sum都很大，但n不大时。可以使用分治了
    a. 将arr数组分成两部分，每一部分的大小是N/2
    b. 两个部分分别暴力求出来所有的子序列累加和%m的结果list，分别记为list1和list2
    c. 最终的答案有可能只出现在list1或者list2中，或者两个list各取一个数来组成最接近m的累加和
    d. n不太大时，如果在n规模上枚举所有的子序列，则总复杂度可以达到2^N。但如果分治的话两边规模都是2^(n/2)，然后两边list组合也是2^(n/2)
        总复杂度是O(3*2^(n/2))，分治方法将乘积转换成了求和
*/

class Solution
{
public:
    // 解法1，以idx和sum作为行列
    int get_max_sum_m1(vector<int> arr, int m)
    {
        int len = arr.size();
        if (len == 0)
        {
            return 0;
        }
        int sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += arr[i];
        }

        vector<vector<bool>> dp(len, vector<bool>(sum + 1, false));
        // 第一行
        dp[0][arr[0]] = true;

        // 第一列
        for (int i = 0; i < len; i++)
        {
            dp[i][0] = true;
        }

        // 普遍位置
        for (int i = 1; i < len; i++)
        {
            for (int j = 1; j <= sum; j++)
            {
                dp[i][j] = dp[i - 1][j]; // 不使用arr[i]
                if (j - arr[i] >= 0)
                {
                    dp[i][j] = dp[i][j] || dp[i - 1][j - arr[i]]; // 前i-1个数搞定j-arr[i]
                }
            }
        }

        // 计算最后一行能搞出来的sum，模m后的最大值
        int ans = 0;
        for (int j = 0; j <= sum; j++)
        {
            if (dp[len - 1][j])
            {
                ans = max(ans, j % m);
            }
        }

        return ans;
    }

    // 解法2，以idx和m作为行列
    int get_max_sum_m2(vector<int> arr, int m)
    {
        int len = arr.size();
        if (len == 0)
        {
            return 0;
        }

        vector<vector<bool>> dp(len, vector<bool>(m, false));
        // 第一行
        dp[0][arr[0] % m] = true;

        // 第一列
        for (int i = 0; i < len; i++)
        {
            dp[i][0] = true;
        }

        for (int i = 1; i < len; i++)
        {
            for (int j = 1; j < m; j++)
            {
                dp[i][j] = dp[i - 1][j];
                int cur = arr[i] % m;
                if (j - cur >= 0)
                {
                    dp[i][j] = dp[i][j] || dp[i - 1][j - cur];
                }
                else
                {
                    dp[i][j] = dp[i][j] || dp[i - 1][m + j - cur]; // 转一圈
                }
            }
        }

        for (int j = m - 1; j >= 0; j--)
        {
            if (dp[len - 1][j])
            { // 从右往左找到最接近m的数
                return j;
            }
        }
        return 0;
    }

    // 解法3, 分治，n不太大时
    int get_max_sum_m3(vector<int> arr, int m)
    {
        int len = arr.size();
        if (len == 0)
        {
            return 0;
        }
        set<int> set_left;
        set<int> set_right;
        int mid = len / 2;
        int left_max = process(arr, m, 0, mid, 0, set_left);
        int right_max = process(arr, m, mid + 1, len - 1, 0, set_right);
        // cout << "set_left:" << endl;
        // for (auto iter:set_left) {
        //     cout << iter << " ";
        // }
        // cout << endl << " set_right:" << endl;
        // for (auto iter:set_right) {
        //     cout << iter << " ";
        // }
        // cout << endl;
        int ans = max(left_max, right_max);
        for (auto iter : set_left)
        {
            int need_max_num = m - 1 - iter;
            auto it = set_right.lower_bound(need_max_num); // 找到第一个大于m-1-iter的数
            if (it != set_right.begin()) {
                it--;
                // cout << "first_it:" << *it << " need_max_num:" << need_max_num << endl;
                if (*it <= need_max_num) {
                    ans = max(ans, (*it) + iter);
                }
                ans = max(ans, (*it) + iter);
            }
        }
        return ans;
    }

    int process(vector<int> &arr, int m, int cur, int e, int sum, set<int> &sum_set)
    {
        // cout << "cur:" << cur << " e:" << e << " sum:" << sum%m << endl;
        if (cur > e)
        {
            sum_set.insert(sum%m);
            return sum % m;
        }
        int p1 = process(arr, m, cur + 1, e, sum, sum_set); // 不要当前数
        int p2 = process(arr, m, cur + 1, e, sum + arr[cur], sum_set);
        return max(p1, p2);
    }
};

int main()
{

    Solution sol;
    // vector<int> arr = {112, 200, 100};
    vector<int> arr = {112, 200, 100, 50, 90, 100, 222};
    int m = 300;
    cout << sol.get_max_sum_m1(arr, m) << endl;
    cout << sol.get_max_sum_m2(arr, m) << endl;
    cout << sol.get_max_sum_m3(arr, m) << endl;

    // int test_time = 10000;
    // int max_len = 10;
    // int max_num = 1000;
    // for (int i = 1; i <= test_time; i++) {
    //     int len = (int) (rand() % max_len) + 1;
    //     vector<int> arr(len, 0);
    //     for (int j = 0; j < len; j++) {
    //         arr[j] = rand() % max_num + 1;
    //     }
    //     int ans1 = sol.get_visibale_num_baoli(arr);
    //     int ans2 = sol.get_visible_num(arr);
    //     if (ans1 != ans2)
    //     {
    //         cout << "ans1:" << ans1 << " ans2:" << ans2 << endl;
    //     }
    // }

    // // vector<int> arr = {3,1,2,4,5};
    // // cout << sol.get_visibale_num_baoli(arr) << endl;
    // // cout << sol.get_visible_num(arr) << endl;
    return 0;
}