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

using namespace std;
//https://leetcode.cn/problems/trapping-rain-water/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china
/*
 * 给定一个数组arr， 已知其中所有的值都是非负的， 将这个数组看作一个容器，
 * 请返回容器能装多少水
 * 比如， arr = {3， 1， 2， 5， 2， 4}， 根据值画出的直方图就是容器形状， 该容
 * 器可以装下5格水
 * 再比如， arr = {4， 5， 1， 3， 2}， 该容器可以装下2格水
 * 
 * 
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

 

示例 1：



输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出：6
解释：上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 
示例 2：

输入：height = [4,2,0,3,2,5]
输出：9
 

提示：

n == height.length
1 <= n <= 2 * 104
0 <= height[i] <= 105
*/

class Solution
{
public:
    /*
    从头遍历。遍历到i位置时，往左右两边取max1和max2。则i位置的水量就是max{min(max1, max2) - arr[i], 0}
    将所有位置的水量累加到一起就是要求的水量和(sum)
    类似洗衣机问题，痛点机器就是答案。(max)

    上述方法是O(N^2)的，下面优化：
    1. 使用两个辅助数组。分别求0-i, i-n位置的最大值数组。这样每遍历到一个位置，就可以O(1)级别的拿到max(0...i-1),max(i+1...n-1)
    2. 使用左右指针，L从1开始，R从n-2开始；max_l=arr[0]开始，max_r=arr[n-1]开始。
        a. 当max_l<max_r，右边的最大数不可能再小于max_r（单调性)。则瓶颈在左边，L位置的水量就可以求出来是max_l-[L], L++, 更新max_l
        b. 当max_l>max_r，左边的最大数不可能再小于max_l（单调性)。则瓶颈在右边，R位置的水量就可以求出来是max_r-[R], R--, 更新max_r
    */
    int calcVigorousWater2(vector<int>& arr) {
        int len = arr.size();
        if (len < 3 ) {
            return 0;
        }
        int L=1,R=len-2;
        int max_l = arr[0];
        int max_r = arr[len-1];
        int res = 0;
        while (L<=R) {
            if (max_l <= max_r) {
                res += max(max_l-arr[L], 0);
                max_l = max(max_l, arr[L++]);
            } else {
                res += max(max_r-arr[R], 0);
                max_r = max(max_r, arr[R--]);
            }
        }
        return res;
    }
};

int main()
{
    Solution sol;
    // vector<int> arr = {3,1,2,5,2,4};
    vector<int> arr = {4,5,1,3,2};
    cout << "res:" << sol.calcVigorousWater2(arr) << endl;
    return 0;
}