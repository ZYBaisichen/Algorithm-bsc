/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-13 11:29:37
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
https://leetcode.cn/problems/largest-rectangle-in-histogram/description/

给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 1 。

求在该柱状图中，能够勾勒出来的矩形的最大面积。



示例 1:



输入：heights = [2,1,5,6,2,3]
输出：10
解释：最大的矩形为图中红色区域，面积为 10
示例 2：



输入： heights = [2,4]
输出： 4


提示：

1 <= heights.length <=105
0 <= heights[i] <= 104
*/

/*
第一个想法就是用单调栈来做
1. 对于每个矩形，假设以其为高，往左右两边可以扩的最大距离为宽，就可以得到一个矩形的面积
2. 使用单调栈，从底向上是依次递增的，每个元素入栈下面的元素就是它左边第一个小于它的元素
3. 当有一个数a比栈顶元素b小时，弹出栈顶元素，此时，a是b右边第一个比b小的元素。
4. 以nums[i]为高，往左右两边扩充最多扩充到第一个比自己矮的边界处
*/

class Solution {
public:
    int largestRectangleArea1(vector<int>& heights) {
        int len = heights.size();
        if (len == 0) {
            return 0;
        }
        stack<int> st;
        st.push(0);
        int w,h,left;
        int ans = INT_MIN;
        for (int i=1;i<len;i++) {
            while (!st.empty() && heights[st.top()] > heights[i]) {
                h = heights[st.top()];
                st.pop();
                left = st.empty() ? -1 : st.top();
                w = i - left -1;
                ans = max(ans,w*h);
                
            }
            st.push(i);
        }

        while (!st.empty()) {
            h = heights[st.top()];
            st.pop();
            left = st.empty() ? -1 : st.top();
            w = len - left - 1;
            // cout << "w:" << w << " h:" << h  << " len:" << h << " st_top:" << left << endl;
            ans = max(ans, w*h);
        }
        return ans;
    }

    //先求出预处理数组，常数优化，效果没有质的变化
    int largestRectangleArea(vector<int>& heights) {
        int len = heights.size();
        if (len == 0) {
            return 0;
        }
        stack<int> st;
        st.push(0);
        int cur;
        int ans = INT_MIN;
        vector<int> left(len, 0); //左边界
        vector<int> right(len, 0); // 右边界

        for (int i=1;i<len;i++) {
            while (!st.empty() && heights[st.top()] > heights[i]) {
                cur = st.top();
                st.pop();
                left[cur] = st.empty() ? -1 : st.top();
                right[cur] = i;
                
            }
            st.push(i);
        }

        while (!st.empty()) {
            cur = st.top();
            st.pop();
            right[cur] = len;
            left[cur] = st.empty() ? -1 : st.top();
        }

        for (int i=0;i<len;i++) {
            ans = max(ans, (right[i] - left[i] - 1) * heights[i]);
        }
        return ans;
    }

};
int main()
{

    Solution sol;
    vector<int> nums = {2,4};
    cout << sol.largestRectangleArea(nums) << endl;
    return 0;
}
