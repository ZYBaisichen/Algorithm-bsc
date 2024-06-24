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

https://leetcode.cn/problems/subarray-sum-equals-k/description/

给你一个整数数组 nums 和一个整数 k ，请你统计并返回 该数组中和为 k 的子数组的个数 。

子数组是数组中元素的连续非空序列。

 

示例 1：

输入：nums = [1,1,1], k = 2
输出：2
示例 2：

输入：nums = [1,2,3], k = 3
输出：2
 

提示：

1 <= nums.length <= 2 * 104
-1000 <= nums[i] <= 1000
-107 <= k <= 107
*/

/*


*/


/*
使用map+前缀和
1. 当遍历到nums[i]的时候，累加和为sum，要找到以nums[i]为结尾时，和为k的子数组。
    只需要看，前方有多少累加和为sum-k的前缀数组。
2. 为了快速找到有多少个累加和为sum-k的前缀数组，使用hash表来记录，key是累加和，val是该累加和出现的次数
3. hash表初始有一个{0:1}，表示初始时什么都没加时空数组的累加和是0，有1个这样的数组。

*/
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        map<int,int> _map;
        _map[0] = 1;
        int sum = 0,ans=0;
        for (int i=0;i<len;i++) {
            sum+=nums[i];
            if (_map.find(sum-k) != _map.end()) {
                // cout << "i:" << i <<"  sum:" << sum << endl;
                ans += _map[sum-k];   
            }
            if(_map.find(sum) == _map.end()) {
                _map[sum] = 0;
            }
            _map[sum]++;
        }
        return ans;
    }
};

int main()
{
    Solution sol;

    return 0;
}
