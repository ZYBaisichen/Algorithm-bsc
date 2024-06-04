/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-04 11:21:32
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

using namespace std;

/*
https://leetcode.cn/problems/container-with-most-water/description/
和42题很类似，只不过42题是在直方图上方盛水

给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。

找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。

返回容器可以储存的最大水量。

说明：你不能倾斜容器。

 

示例 1：



输入：[1,8,6,2,5,4,8,3,7]
输出：49 
解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。
示例 2：

输入：height = [1,1]
输出：1
 

提示：

n == height.length
2 <= n <= 105
0 <= height[i] <= 104
*/

/*
采用双指针法。
左边某个垂直线，一定和最右边，大于等于它的垂直线合到一起，构成以其为小边界时，最多的盛水量。
两个指针l和r，l指向0，r指向n-1， min(arr[l], arr[r]) * (r-l+1) 就是此时以他们两个为左右边界可以盛的水量。此时应该怎么移动指针呢？

假设l和r构成的面积是t，arr[l]<arr[r]，直觉上应该让较小的垂线下标l往右移动；
则如果将r往左移动到r'，l和r'构成的面积是t'。 则min(arr[l], arr[r']) <= min(arr[l], arr[r]), 并有r'-l+1 < r-l+1:
    1. 当arr[r']>arr[r]时, min(arr[l], arr[r']) = arr[l] == min(arr[l], arr[r])
    2. 当arr[r']<arr[r]时, min(arr[l], arr[r']) = arr[r'] <= arr[l] = min(arr[l], arr[r])
    3. 综上 min(arr[l], arr[r]) * (r-l+1) = t >= t' = min(arr[l], arr[r']) * （r'-l+1）
    4. 所以r往右移动完全不会对求最大的盛水量有任何好处。如果将l(小的)往右移动，则可能会产生其他更好的结果。即l将不会再作为边界了。
*/
class Solution
{
public:
    int maxArea(vector<int>& height) {
        int len = height.size();
        if (len == 0 || len == 1) {
            return 0;
        }
        int l = 0,r=len-1;
        int res = 0;
        while (l<r) {
            res = max(res, min(height[l], height[r])*(r-l));
            if (height[l] <= height[r]) {
                l++;
            } else {
                r--;
            }
        }
        return res;
    }
};

int main()
{

    Solution sol;
    vector<int> height = {1,8,6,2,5,4,8,3,7};
    cout << sol.maxArea(height) << endl;
    

    return 0;
}