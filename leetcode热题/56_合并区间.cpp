/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-11 10:52:57
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
https://leetcode.cn/problems/merge-intervals/

以数组 intervals 表示若干个区间的集合，其中单个区间为 intervals[i] = [starti, endi] 。请你合并所有重叠的区间，并返回 一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间 。

 

示例 1：

输入：intervals = [[1,3],[2,6],[8,10],[15,18]]
输出：[[1,6],[8,10],[15,18]]
解释：区间 [1,3] 和 [2,6] 重叠, 将它们合并为 [1,6].
示例 2：

输入：intervals = [[1,4],[4,5]]
输出：[[1,5]]
解释：区间 [1,4] 和 [4,5] 可被视为重叠区间。
 

提示：

1 <= intervals.length <= 104
intervals[i].length == 2
0 <= starti <= endi <= 104
*/

/*
1. 按照区间开始位置排序
2. 维护一个右边界，当遍历一个区间的开头在右边界左边或相等时，则合并，即将右边界移动到新的区间结尾；
    否则当一个区间的开头在右边界的，则开启来一个新的区间，将前一个区间结算答案。
*/

struct cmp {
    bool operator()(const vector<int>& a, const vector<int>& b)  const {
        return a[0] < b[0]; //前面的小于后面的 
    }
};
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        int len = intervals.size();
        if (len ==0 ){
            return vector<vector<int>>();
        }
        sort(intervals.begin(), intervals.end(), cmp());//对二维向量进行排序

        vector<vector<int>> res;

        int r = intervals[0][1], start = intervals[0][0];
        for (int i=1;i<len;i++){
            if (intervals[i][0] <= r) {
                r = max(intervals[i][1], r);
            } else { //结算答案
                res.push_back({start, r});
                start = intervals[i][0];
                r = intervals[i][1];
            }
        }

        //最后会有一个区间没有结算
        res.push_back({start, r});
        return res;
    }
};
int main()
{

    Solution sol;
    return 0;
}