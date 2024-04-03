/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 12:03:35
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

using namespace std;

/*

例子：
假设nums[23,17,13,11,0,99,56,27,25]有9个数
1. 求出最小值min=0和最大值max=99。分成10等份，0-9,10-19,20-29......
2. 遍历一遍数组，将每个数放在对应的桶中，最小值一定在最左边的桶，最大值一定在最右边的桶。
    a. 因为有9个数，弄了10个桶，中间肯定有个桶是空的。
    b. 分配所有的数之后，相邻的数字可能来自桶内，也可能是跨桶之间(前一个的最大，和后一个的最小)
    c. 因为桶内最大和最小值的差值不可能超过桶的大小。所以取空桶左边的最大值和空桶右边的最小值，他们即是相邻的也是差值最大的


例子分桶，每个桶只保留桶内最小值和最大值：

min 0    11    23                56                      99
max 0    17    27                56                      99
    0-9  10-19 20-29 30-39 40-49 50-59 60-69 70-79 80-89 90-99
答案只可能在跨桶之间：11-0=11， 23-17=6， 56-27=29， 99-56=43
做空桶的目的是找到一个不算差的平凡解，但答案也有可能来自相邻的两个桶。杀死比平凡解还要差的解，即桶内部的解

https://leetcode.com/problems/maximum-gap/
*/

class Solution
{
public:
    int maximumGap(vector<int>& nums) {
        int len = nums.size();
        //分成len+1个桶。构建这种模型比较难

        int min_num = INT_MAX;
        int max_num = INT_MIN;
        
        //遍历得到最大值和最小值
        for (int i = 0; i < len; ++i) {
            if (nums[i] > max_num) {
                max_num = nums[i];
            }
            if (nums[i] < min_num) {
                min_num = nums[i];
            }
        }

        if (max_num == min_num) {
            return 0;
        }

        vector<int> has_num(len+1, 0);
        vector<int> max_num_in_bucket(len+1, INT_MIN);
        vector<int> min_num_in_bucket(len + 1, INT_MAX); //i号桶的最小值

        //将数字分配到桶里
        for (int i=0;i<len;i++) {
            int bid = bucket(nums[i], len+1, min_num, max_num);
            // cout << "i:" << i << " nums[i]:" << nums[i] <<  " bid:" << bid << endl;
            min_num_in_bucket[bid] = has_num[bid] ? min(min_num_in_bucket[bid], nums[i]) : nums[i];
            max_num_in_bucket[bid] = has_num[bid] ? max(max_num_in_bucket[bid], nums[i]) : nums[i];
            has_num[bid] = true;
        }

        for (int i=0;i<len+1;i++) { //桶编号是0,1,....len
            cout << "tong_idx:" << i << " min_num:" << min_num_in_bucket[i] << " max_num:" << max_num_in_bucket[i] << endl;
        }

        int res = 0;
        int last_max = max_num_in_bucket[0];
        for (int i=1;i<len+1;i++) { //桶编号是0,1,....len
            if (has_num[i]) {
                res = max(res, min_num_in_bucket[i] - last_max);
                last_max = max_num_in_bucket[i];
            }
        }
        return res;
    }

    int bucket(int num, int n, int min_num, int max_num) {
        //分成了n个桶，每个桶有(max_num-min_num+1)/n个数字
        double every_num = (max_num - min_num+1+0.0) / (n+0.0);
        double bid = (num - min_num) / every_num;
        cout << "num:" << num << " n:" << n << " max_num:" << max_num << " min_num:" << min_num << " every_num:" << every_num << " bid:" << bid << endl;
        return (int)bid;
    }
};

int main()
{

    Solution sol;

    // vector<vector<int>> m = {
    //     {-2, -3, 3},
    //     {-5, -10, 1},
    //     {0, 30, -5}};
    // vector<int> nums={23,17,13,11,0,99,56,27,25};
    // vector<int> nums={3,6,9,1};
    vector<int> nums = {2, 99999999};

    cout << sol.maximumGap(nums) << endl;

    return 0;
}