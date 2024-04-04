/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 16:34:48
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
1. 给定一个数组，如果某个数出现次数超过了数组长度的一半，打印这个数，没有则不打印

2.给定一个数组arr和整数k，arr长度为n，如果有某些数出现次数超过了n/k，打印这些数，如果没有则不打印

著名的水王问题：
https://blog.csdn.net/qq_45173404/article/details/117809364

*/

class Solution
{
public:
    vector<int> get_n_k(vector<int> &nums, int k)
    {
        /*
        超过n/2有1个候选，超过n/3有2个候选，超过n/k最多只有k-1个候选
        这次一次删掉k个数。
        使用map来记录所有k-1候选的血量，每来一个不一样的数，每个候选都降低一点血量
        */
        int len = nums.size();

        // 候选cand: 血量
        map<int, int> cand_map;//O(K)空间复杂度。k很大则可以退化使用大的hash表做
        for (int i = 0; i < len; i++)
        {
            if (cand_map.find(nums[i]) != cand_map.end()) { //候选血量增加
                cand_map[nums[i]]++;
            } else {
                if (cand_map.size() < k-1) {
                    cand_map[nums[i]] = 1;
                } else { //找齐了k-1
                    // cout << "cur_cnad: ";
                    // for (auto& item : cand_map) {
                    //     cout << item.first << ":" << item.second << " ";
                    // }
                    // cout << endl;
                    vector<int> need_delete;
                    for (auto& item : cand_map) {
                        item.second--;
                        cout << "item.second: " << item.second << endl;
                        if (item.second == 0) {
                            need_delete.push_back(item.first);
                        }
                    }
                    for (auto iter : need_delete) {
                        cand_map.erase(iter);
                    }
                }
            }
        }

        int times = 0;
        for (auto& item : cand_map) {
            item.second=0;
        }
        vector<int> res;
        // 比如[1,2,3,4,5]每次都会有新的候选，最后剩5，所以cand如果不是0，不一定是大于一半的数
        // 所以还需要再统计一遍
        for (int i = 0; i < len; i++)
        {
            if (cand_map.find(nums[i]) != cand_map.end()) { //候选血量增加
                cand_map[nums[i]]++;
                if (cand_map[nums[i]]>len/k) {
                    res.push_back(nums[i]);
                }
            }
        }
        return res;
    }
};

int main()
{

    Solution sol;

    // string s="12##3##";
    vector<int> nums = {1,1,1,1,5,6,2,2,3,3,3,3}; //12

    int k=4;
    vector<int> res = sol.get_n_k(nums, k);
    for (auto iter : res) {
        cout << iter << " ";
    }
    cout << endl;

    return 0;
}