/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 17:03:43
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

https://leetcode.cn/problems/meeting-rooms-ii/


给你一个会议时间安排的数组 intervals ，每个会议时间都会包括开始和结束的时间 intervals[i] = [starti, endi] ，返回 所需会议室的最小数量 。

 

示例 1：

输入：intervals = [[0,30],[5,10],[15,20]]
输出：2
示例 2：

输入：intervals = [[7,10],[2,4]]
输出：1
 

提示：

1 <= intervals.length <= 104
0 <= starti < endi <= 106
*/

/*
有个问题是有很多会议，只有一个会议室。可以用贪心来做，按照结束时间排序。然后从前往后遍历，看开始时间和当前来到的时间比较，如果比上一个会议结束的时间晚，则可以排进去，否则就排不进去。
但这个问题是最大线段重合问题。

方法一：线段最大重合问题(进阶知识/线段树/最大线段重合.cpp)
1. 按照开始时间排序；每个序列遍历处理
2. 准备一个小根堆。按照结束时间优先
3. 如果当前时间区间是[l,r]。将小根堆中结束时间小于l的都弹出。即将不可能和当前会议冲突的弹出。
4. 弹出后堆中的数量即是和当前会议时间重合的会议。重合数量取最大值，就是会议室的数量

方法二：扫描线问题(上车下车问题)
类似在公交车上下车，不同的时间点，有上车的人和下车的人。会议室问题本质上就是求在车上，同一时间最多有多少人。
将所有会议的开始和结束时间放在一个x轴上，当会议开始时，计数+1，结束时计数-1。
*/

class Solution
{
public:
    struct cmp_end { //用于优先级队列
        bool operator()(const vector<int>& a, const vector<int>& b) const{ 
            return a[1] > b[1]; //新来b，end结束早时放在堆顶
        }
    };

    // 我们可以为结构体提供一个比较函数  
    static bool cmp_start(const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0]; // 后面的start要比前面的大
    }
    int minMeetingRooms_1(vector<vector<int>>& intervals) {
        int len = intervals.size();
        if (len==0) {
            return 0;
        }
        sort(intervals.begin(), intervals.end(), cmp_start);

        priority_queue<vector<int>, vector<vector<int>>, cmp_end> pq;

        int l=0,r=0;
        int ans = 0;
        for (int i=0;i<len;i++) {
            l = intervals[i][0];
            r = intervals[i][1];
            while (!pq.empty()) {
                // cout << "top:" << pq.top()[0] << "," << pq.top()[1] << endl;
                if (pq.top()[1] <= l) { //剔除不重合的会议室
                    pq.pop();
                } else {
                    break;
                }
            }
            // cout << "i:" << i << " l:" << l << " r:" << r << " pq_size:" << pq.size() << endl;
            pq.push({l,r});
            ans  = max(ans, (int)pq.size());
        }

        return ans;
    }

    int minMeetingRooms(vector<vector<int>>& intervals) {
        int len = intervals.size();
        if (len==0) {
            return 0;
        }

        vector<pair<int, int>> tmp;
        for (int i=0;i<len;i++) {
            tmp.push_back({intervals[i][0], 1});
            tmp.push_back({intervals[i][1], -1});
        }

        sort(tmp.begin(), tmp.end()); //会按照第一个元素排序
        // for (auto it:tmp) {
        //     cout << it.first << "," << it.second << endl;
        // }
        // cout << endl;
        int cnt = 0;
        int ans = 0;
        for (auto it:tmp) {
            cnt += it.second;
            ans = max(ans, cnt);
        }
        return ans;
    }
};
int main()
{

    Solution sol;
    // vector<vector<int>> arr = {
    //     {0,30},
    //     {5,10},
    //     {15,20}};


    vector<vector<int>> arr = {
        {1,5},
        {8,9},
        {8,9}};
    cout << sol.minMeetingRooms(arr) << endl;
    return 0;
}
