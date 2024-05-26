/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-25 16:21:33
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
#include <sstream>
#include <unordered_set>

using namespace std;

/*
https://leetcode.cn/problems/maximal-rectangle/description/
给定一个仅包含 0 和 1 、大小为 rows x cols 的二维二进制矩阵，找出只包含 1 的最大矩形，并返回其面积。

 

示例 1：

输入：matrix = [["1","0","1","0","0"],["1","0","1","1","1"],["1","1","1","1","1"],["1","0","0","1","0"]]
输出：6
解释：最大矩形如上图所示。
示例 2：

输入：matrix = [["0"]]
输出：0
示例 3：

输入：matrix = [["1"]]
输出：1
 

提示：

rows == matrix.length
cols == matrix[0].length
1 <= row, cols <= 200
matrix[i][j] 为 '0' 或 '1'
*/

/*
使用单调栈的思想。
对于一个直方图，使用单调栈算法：
    1. 任何位置i都可以知道左边比它小且最近的位置，右边比它小且最近的为位置。
    2. 这样就可以得到必须以arr[i]为高，往外扩，扩到的最大矩形。

对于原始矩阵matrix，依次求：
    1. 以第0行为底时，求一个单调栈直方图，得到最大面积，就是1最多的矩形。
    2. 以第1行为底时，求一个直方图，得到最大的矩形面积
    ....
    4. 一直求到m行，取最大值

matrix矩阵m行n列，遍历m次，每次o(N)的复杂度求出最大矩形。总复杂度是O(m*n)

三个知识点：
    1. 矩阵压缩：使用一维问题去解matrix的问题
    2. 可能性划分：需要以某一行为底的情况下求解
    3. 使用单调栈加速一维问题求解过程

*/

class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        }
        int m = matrix[0].size();
        if (m==0) {
            return 0;
        }
        vector<int> height(m,0);
        int ans = 0;
        for (int i=0;i<n;i++) {
            //每次压缩数组成直方图
            for (int j=0;j<m;j++) {
                if (matrix[i][j]== '0') {
                    height[j] =  0;
                } else {
                    height[j] += 1;
                }
            }
            //求出单个直方图的答案
            int cur_area = max_rect_from_buttom(height);
            // cout << "i:" << i << " cur_area:" << cur_area << endl;
            ans = max(ans, cur_area);
        }
        return ans;
    }
    int max_rect_from_buttom(vector<int>& height) {
        int len = height.size();
        if (len == 0) {
            return 0;
        }
        // cout << "cur height:" << endl;
        // for (auto iter:height) {
        //     cout << iter << " ";
        // }
        // cout << endl;
        stack<int> st;
        int ans = 0;
        for (int i=0;i<len;i++) {
            /*
            1 2 3 3 3 1
            结算第一个3时，虽然只算到了3位置，但第3个3结算的时候是可以从1位置到5位置的。
            */
            while (!st.empty() &&  height[i] <= height[st.top()]) { 
                int cur = st.top(); //结算栈顶的面积
                st.pop();
                int r = i;
                int l = st.empty() ? -1 : st.top();
                int cur_area = (r-l-1) * height[cur];
                ans = max(ans, cur_area);
            }
            st.push(i);
        }
        //结算栈中剩余的位置
        while (!st.empty()) {
            int cur = st.top();
            st.pop();
            int r = len;
            int l = st.empty() ? -1 : st.top();
            int cur_area = (r-l-1) * height[cur];
            ans = max(ans, cur_area);
        }
        return ans;
    }
};

int main() {
    Solution sol;
    /*
    输入：words = ["abcd","dcba","lls","s","sssll"]
    输出：[[0,1],[1,0],[3,2],[2,4]]
    解释：可拼接成的回文串为 ["dcbaabcd","abcddcba","slls","llssssll"]
    */
//    vector<string> words = {"abcd","dcba","lls","s","sssll"};
    vector<char> nums = {100,1,3,0,4,2,17};
    cout << "ans:" << sol.longestConsecutive(nums) << endl;
    return 0;
}