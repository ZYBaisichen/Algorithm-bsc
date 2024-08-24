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

https://leetcode.cn/problems/daily-temperatures/description/

给定一个整数数组 temperatures ，表示每天的温度，返回一个数组 answer ，其中 answer[i] 是指对于第 i 天，下一个更高温度出现在几天后。如果气温在这之后都不会升高，请在该位置用 0 来代替。

 

示例 1:

输入: temperatures = [73,74,75,71,69,72,76,73]
输出: [1,1,4,2,1,1,0,0]
示例 2:

输入: temperatures = [30,40,50,60]
输出: [1,1,1,0]
示例 3:

输入: temperatures = [30,60,90]
输出: [1,1,0]
 

提示：

1 <= temperatures.length <= 105
30 <= temperatures[i] <= 100
*/

/*
遍历到i位置，需要找到右边第一个比自己大的值。使用单调栈。
1. 栈从底到上递减。从后往前遍历
2. 遍历到i位置的时候，如果栈顶元素比nums[i]小，则出栈，直到找到第一个大于nums[i]的值，这个就是右边第一个大于nums[i]的数
    如果出栈到空都没找到比nums[i]大的数，则使用0代替。
*/
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int len = temperatures.size();
        if (len==0) {
            return vector<int>();
        }
        vector<int> answer(len, 0);
        stack<int> st;
        st.push(len-1);
        for (int i=len-2;i>=0;i--) {
            while (!st.empty() && temperatures[st.top()] <= temperatures[i]) {
                st.pop();
            }
            if (st.empty()) {
                answer[i] = 0;
            } else {
                answer[i] = st.top() - i;
            }
            st.push(i);
        }
        return answer;
    }
    //简洁版
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int len = temperatures.size();
        if (len==0) {
            return temperatures;
        }
        vector<int> res(len, 0);
        //单调递减的，弹出时结算
        stack<int> st; 
        st.push(0);
        for (int i=1;i<len;i++) {
            while (!st.empty() && temperatures[st.top()] < temperatures[i]) {
                int tmp = st.top();
                st.pop();
                res[tmp] = i-tmp; //结算
            }
            st.push(i);
        }
        return res;
    }
};

int main()
{
    Solution sol;

    return 0;
}
