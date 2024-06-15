/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
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
和经典面试题106相同

https://leetcode.cn/problems/longest-consecutive-sequence/

给定一个未排序的整数数组 nums ，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。

请你设计并实现时间复杂度为 O(n) 的算法解决此问题。

 

示例 1：

输入：nums = [100,4,200,1,3,2]
输出：4
解释：最长数字连续序列是 [1, 2, 3, 4]。它的长度为 4。
示例 2：

输入：nums = [0,3,7,2,5,8,4,6,0,1]
输出：9
 

提示：

0 <= nums.length <= 105
-109 <= nums[i] <= 109
*/

/*
hash表：
两个hash表，_map1,_map2
_map1[i]表示以i开头的区间，长度为val
_map2[i]表示以i结尾的连续区间，长度是val

每次一个新的数a=nums[i], 到来后，先增加[a,a]到两个map。
1. 然后再_map1中查看是否存在以a+1开头的区间，如果存在，则删除_map1[a+1]，_map2[a]。即删除以a+1开头的区间和以a结尾的区间。
    
2. 在_map2中查看是否有a-1结尾的区间，如果存在，则删除_map2[a-1],_map1[a]。即删除以a-1结尾，以a开头的区间。

*/

class Solution
{
public:
    int longestConsecutive1(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        map<int,int> _map1,_map2;
        int a = 0;
        for (int i=0;i<len;i++) {
            a = nums[i]
            _map1[a] = 1;
            _map2[a] = 1;

            if (_map1.find(a+1) != _map1.end()) {
                int end = a+1 + _map1[a+1] - 1; //找到以a+1开头的结尾位置
                _map1[a] += _map1[a+1]; //将新的长度更新到以a为开头的区间上
                _map2[end] = _map1[a];
                _map1.erase(a+1);
                _map2.erase(a);
            }

            if (_map2.find(a-1) != _map2.end()) {
                int start = a-1 - _map2[a-1] + 1;//找到以a-1为结尾的开头位置
                _map2[a] += _map2[a-1];
                _map1[start] = _map2[a];
                _map2.erase(a-1);
                _map1.erase(a);
            }
        }

        int ans = 0;
        for (auto it:_map1) {
            ans = max(ans, it.second);
        }
        return ans;
    }

    /*
    首次出现
    */
    int longestConsecutive(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        map<int,int> _map;
        int start = -1, end = -1;
        int ans = INT_MIN;
        for (int i=0;i<len;i++) {
            int a = nums[i];
            if (_map.find(a) == _map.end()) {
                _map[a] = 1;
                start = a;
                end = a;
                //往前合并
                if (_map.find(a-1) != _map.end()) {
                    //a-1如果存在，则一定是结尾
                    start = a-1-_map[a-1]+1;
                    _map[start] += 1;
                    _map[a] = _map[start];//代表结尾
                }

                //从start，往后合并
                if (_map.find(a+1) != _map.end()) {
                    int len = _map[a+1];
                    //a+1如果存在，则一定是开头
                    end = a+1+_map[a+1]-1;
                    _map[end] += _map[a];
                    _map[start] += len;
                }
                ans = max(ans, max(_map[start], _map[end]));
            }
        }
        return ans;
    }
};

int main()
{

    Solution sol;
    return 0;
}
