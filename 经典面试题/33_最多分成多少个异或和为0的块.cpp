/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-26 13:31:06
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
给定一个数组, 求出能最多划分多少个异或和为0的块

假设答案法：按照结尾位置讨论。dp[i]表示以i为结尾时的最大异或和为0的块数
1. 假设已经有了一个最优划分。i是最后一块区域的最后一个数字
2. 最优划分划出来的最后一个部分异或和不是0，则i位置的数要不要都行，即dp[i] = dp[i-1]
3. i所在的最后一块是异或和为0：
    a. 因为最后一块是以i结尾，需要讨论开头j的位置。
        i) 对于[j..i]，不可能存在一个数k(j<k<i)，让[j...k]的异或和为0，因为如果[j...k]异或和为0，则[k...i]异或和也是0，那就可以划分成两块了。j的位置将更靠后
        ii) 所以j的位置一定是左边离i最近的异或和为0的位置
        iii) 假设[0...i]的前缀异或和为1000，当存在一个数k[0...k]的异或和为1000，且是最右边的1000。则最后一块的范围是[k+1..i]，且异或和为0


汇总下来的转移方程是：
    dp[i]定义为以i为结尾的数组中，最多能分成多少块异或和为0的块
    dp[i] = dp[i-1] 最后一块不是异或和为0
    dp[i] = dp[j]+1 假设[0...i]的异或和为sum，上一次sum出现的位置为j，则[j+1...i]是一块，其他的依赖dp[j]
*/

class Solution
{
public:
    //dp
    int mox_xor_zero2(vector<int> &nums) {
        int len = nums.size();
        vector<int> dp(len, 0); //dp[i]表示以i为结尾时的最大异或和为0的块数
        dp[0] = nums[0] == 0 ? 1 : 0;
        int sum = nums[0];
        map<int, int> sum_map; //记录累加和最后出现的位置
        sum_map[sum] = 0;
        for (int i=1;i<len;i++) {
            sum = sum ^ nums[i];
            
            int tmp = 0;
            if (sum == 0) { //加到i异或和为0，则至少有1个
                tmp = 1;
            }
            tmp = max(tmp, dp[i-1]);
            //只有前面
            if (sum_map.find(sum) != sum_map.end()) { //如果存在，则直接更新dp[i]
                int j = sum_map[sum];
                tmp = max(tmp, dp[j]+1); //[0..j]随意划分，[j...i]划分成一个
            }
            sum_map[sum] = i;
            dp[i] = tmp;
        }
        return dp[len-1];
    }
    // 暴力枚举
    int mox_xor_zero1(vector<int> &nums)
    {
        int len = nums.size();
        int res = 0;
        for (int i = 0; i < len; i++)
        { // 枚举开头
            for (int j = i; j < len; j++)
            { // 枚举结尾
                int _xor = 0;
                for (int kk = i; kk <= j; kk++)
                {
                    _xor = _xor ^ nums[kk];
                }
                res = max(res, _xor);
            }
        }
        vector<int> parts;
        return process(nums, 0, parts);
    }

    void print_binary(int nums)
    {
        for (int i = 31; i >= 0; i--)
        {
            cout << ((nums >> i) & 1);
        }
        cout << endl;
    }

    // 枚举第一块的位置，然后继续往下递归划分
    // parts是一堆整数，代表每一块的划分结尾位置
    int process(vector<int> &nums, int idx, vector<int> &parts)
    {
        int ans = 0;
        if (idx == nums.size())
        {
            int count = 0;
            parts.push_back(nums.size());
            for (int i = 0; i < parts.size(); i++)
            {
                int start = 0;
                if (i == 0)
                {
                    start = 0;
                }
                else
                {
                    start = parts[i - 1] + 1; // 当前一块的end达到了最后一个数，则最后一块则是nums[len]到nums[len]，就不会算上了
                }

                int end = parts[i];
                int sum = 0, flag = false;
                for (int kk = start; kk <= end && kk < nums.size(); kk++)
                {
                    flag = true;
                    sum ^= nums[kk];
                }
                if (flag && sum == 0)
                { // 执行过并且异或和为0
                    count++;
                }
            }
            parts.pop_back();
            return count; // 返回本组划分结果
        }
        else
        {
            int no = process(nums, idx + 1, parts);
            parts.push_back(idx);
            int yes = process(nums, idx + 1, parts);
            parts.pop_back();
            ans = max(no, yes);
        }
        return ans;
    }
};

void generate_random_array(int max_size, int max_value, vector<int> &arr)
{
    int size = (rand() % max_size) + 1;
    for (int i = 0; i < size; i++)
    {
        arr.push_back(rand() % max_value);
    }
}

void print(vector<int> &arr)
{
    for (auto iter : arr)
    {
        cout << iter << ",";
    }
    cout << endl;
}
int main()
{

    int testTime = 100;
    int maxSize = 20;
    int maxValue = 50;
    bool succeed = true;
    Solution s;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++) {
        vector<int> arr;
        generate_random_array(maxSize, maxValue, arr);
        int ans1 = s.mox_xor_zero1(arr);
        int ans2 = s.mox_xor_zero2(arr);

        if (ans1 != ans2)
        {
            succeed = false;
            cout << "max_xor1: " << ans1 << " xor2:" << ans2 <<endl;
            break;
        }
    }
    cout << "测试结束" << endl;

    /*
    1011
    0001
    1111
    0101
    1111
    */
    // vector<int> nums = {3, 2, 9, 4, 0, 5, 5};
    // Solution s;

    // cout << s.mox_xor_zero1(nums) << endl;
    // cout << s.mox_xor_zero2(nums) << endl;
    // cout << s.max_xor2(nums) << endl;
    // cout << s.max_xor3(nums) << endl;

    return 0;
}