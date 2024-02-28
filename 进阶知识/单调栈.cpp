#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <set>
#include <ctime>
#include <cstdlib>
#include <map>
#include <limits.h>
using namespace std;

//单调栈是一种特殊的数据结构，它基于栈的基本特性，即先进后出，并在此基础上添加了一个额外的特性：栈顶到栈底的元素必须是严格递增或递减。单调栈分为单调递增栈和单调递减栈，单调递增栈是指从栈顶到栈底的元素是单调递增的关系（包括相等），而单调递减栈则是指从栈顶到栈底的元素是单调递减的关系（包括相等）。
// 滑动窗口最大值（单调队列经典用法模版）
// 给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧
// 你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。
// 返回 滑动窗口中的最大值 。
// 测试链接 : https://leetcode.cn/problems/sliding-window-maximum/
// 这样的双端队列，可以推广到滑动窗口任意滑的情况。比如l加多次，伸缩窗口。但注意l不能回退
#define MAXN 100001
class Solution
{
public:
    //[3,5,2,7,1,6]
    // 双端队列每个位置最多进一次，最多出一次，所以总复杂度是O(N)。单次窗口的最大值获得复杂度平均下来是O(1)
    vector<int> maxSlidingWindow(vector<int> &nums, int k)
    {
        int len = nums.size();
        // 单调递减的双端队列，存储滑动窗口哪些数最大的优先级
        int* deque = new int[MAXN];
        int h = 0, t = 0;
        for (int i = 0; i < k - 1; i++)
        {
            // 依次弹出，直到找到一个比arr[i]大的数
            while (h < t && nums[deque[t - 1]] <= nums[i])
            {
                t--;
            }
            deque[t++] = i; // 入队列
        }
        int m = len - k + 1; // 答案长度为m
        int* ans = new int[m];
        // 当前窗口是k-1，所以l是0，r是k-2，但从k-1开始产生答案
        for (int l = 0, r = k - 1; l < m; l++, r++)
        {
            while (h < t && nums[deque[t - 1]] <= nums[r])
            {
                t--;
            }
            deque[t++] = r; // 入队列
            // 收集答案
            ans[l] = deque[h];
            // l右移，说明l位置的数过期了，如果deque的头是l位置的数，就弹出
            if (deque[h] == l)
            {
                h++;
            }
        }
        // 构造答案
        vector<int> res(m, 0);
        for (int i = 0; i < m; i++)
        {
            res[i] = nums[ans[i]];
        }
        return res;
    }
};

int main()
{
    Solution sol;
    //22
    vector<int> arr = {3, 5, 2, 7, 1, 6};
    int k=3;
    
    const vector<int>& ans0 = sol.maxSlidingWindow(arr, k);

    for (int i=0; i<ans0.size(); i++) {
        cout << ans0[i] << " ";
    }
    cout << endl;
    cout << "测试结束" << endl;
}
