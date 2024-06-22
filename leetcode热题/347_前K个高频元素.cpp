/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-22 00:27:15
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

https://leetcode.cn/problems/top-k-frequent-elements/description/

给你一个整数数组 nums 和一个整数 k ，请你返回其中出现频率前 k 高的元素。你可以按 任意顺序 返回答案。

示例 1:

输入: nums = [1,1,1,2,2,3], k = 2
输出: [1,2]
示例 2:

输入: nums = [1], k = 1
输出: [1]
 

提示：

1 <= nums.length <= 105
k 的取值范围是 [1, 数组中不相同的元素的个数]
题目数据保证答案唯一，换句话说，数组中前 k 个高频元素的集合是唯一的
 

进阶：你所设计算法的时间复杂度 必须 优于 O(n log n) ，其中 n 是数组大小。
*/

/*

*/

class Solution {
public:
    /*
    hash统计词频
    然后准备一个小顶堆，按照词频组织优先级，保持堆的大小是k，最后堆中剩下的就是结果
    */
    struct cmp {
        bool operator()(const vector<int>& a, const vector<int>& b) const{ 
            return a[1] > b[1]; //新来b，频率小时放在堆顶
        }
    };
    vector<int> topKFrequent(vector<int>& nums, int k) {
        int len = nums.size();
        if (len < k) {
            return vector<int>();
        }
        unordered_map<int, int> _map;
        for (int i=0;i<len;i++) {
            if (_map.find(nums[i]) == _map.end()) {
                _map[nums[i]] = 0;
            }
            _map[nums[i]]++;
        }

        priority_queue<vector<int>, vector<vector<int>>, cmp> pq;
        for (auto it:_map) {
            if (pq.size() < k) {
                pq.push({it.first, it.second});
            } else {
                if (pq.top()[1] < it.second) {
                    pq.pop();
                    pq.push({it.first, it.second});
                }
            }
        }
        vector<int> res;
        while (!pq.empty()) {
            res.push_back(pq.top()[0]);
            pq.pop();
        }
        return res;
    }

    /*
        堆排序的复杂度是O(N*log(N))
        数组长度n不是太大时，可以使用桶排序。
    */
    vector<int> topKFrequent(vector<int>& nums, int k) {
        int len = nums.size();
        if (len < k) {
            return vector<int>();
        }
        unordered_map<int, int> _map;
        int max_freq = 0;
        for (int i=0;i<len;i++) {
            if (_map.find(nums[i]) == _map.end()) {
                _map[nums[i]] = 0;
            }
            _map[nums[i]]++;
            max_freq = max(max_freq, _map[nums[i]]);
        }

        vector<vector<int>> bucket(max_freq+1, vector<int>());

        //入桶
        for (auto it:_map) {
            bucket[it.second].push_back(it.first);
        }

        //从尾部出桶
        vector<int> res;
        for (int i=max_freq;i>=0;i--) {
            for (auto it:bucket[i]) {
                if(res.size()>=k) {
                    break;
                }
                res.push_back(it);
            }
            if(res.size()>=k) {
                break;
            }
        }
        return res;
    }
};


int main()
{
    Solution sol;
    int n=2;
    auto res = sol.countBits(n);
    for (auto it:res) {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
