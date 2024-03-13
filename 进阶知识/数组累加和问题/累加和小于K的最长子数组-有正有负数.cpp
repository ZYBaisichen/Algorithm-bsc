/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-14 00:12:28
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

using namespace std;
/*
 * 给定一个整数组成的无序数组arr，值可能正、可能负、可能0，给定一个整数值K
 * 找到arr的所有子数组里，哪个子数组的累加和<=K，并且是长度最大的，返回其长度。
 * 常见的解法就是 Code02的思想，以某个数结尾，往前找。但需要用到有序表，所以整体的时间复杂度在O(N*logN)
 * 这道题还有O(N)的解法。
 *
 */

class Solution
{
public:
    int getLongest_sub_array_length(vector<int> &arr, int k)
    {

        if (arr.size() == 0 || k < 0)
        {
            return 0;
        }
        // 以i开头的子数组，后面所有可能中，可以使累加和取的最小信息需要拿到
        // min_sum[i]，子数组以i开头，在后面所有数中，哪个子数组可以取的最小累加和
        // min_sum_end[i], 以i开头，最小累加和子数组的结束位置j在哪就记录下来
        int len = arr.size();
        vector<int> min_sum(len, 0);
        vector<int> min_sum_end(len, 0);
        min_sum[len - 1] = arr[len - 1];
        min_sum_end[len - 1] = len - 1;
        for (int i = len - 2; i >= 0; i--)
        {
            if (min_sum[i + 1] > 0)
            { // 如果右边为正数，则无利可图，不能往右扩
                min_sum[i] = arr[i];
                min_sum_end[i] = i;
            }
            else
            { // 往右扩，如果能让累加和更小，则将sum[i+1]加上来，使用i+1的end
                min_sum[i] = arr[i] + min_sum[i + 1];
                min_sum_end[i] = min_sum_end[i + 1];
            }
        }
        // for (int i=0;i<len;i++) {
        //     cout << " " << min_sum[i] << " " << min_sum_end[i] << endl;
        // }

        // 初始的思想是从0开始，一直往后扩，每次都能找到一个最小子数组和，直到超了为止。但每个位置都扩一遍，复杂度是O(N^2)的了
        // 假设从0开始，扩到了29, 但30开始的块进不来，进来就会超，接下来看sum-=arr[0]能不能让30开始的块进来
        // 舍弃可能性思想
        // 即假设从0位置开始已经扩充到了R位置，1..r位置这一段是没有必要再求的。如果1..r能再加进来下一块，则有可能超过0开头的数组长度
        int end = 0; // end表示第一次不能加进来的位置
        int sum = 0;
        int res = 0;
        /*
        -1 0
        1 3
        0 3
        -1 3
        6 4
        */
        for (int i = 0; i < len; i++)
        { // 以每个位置为开头进行扩
            while (end < len && sum + min_sum[end] <= k)
            {
                // cout << "end:" << end << endl;
                sum += min_sum[end];
                end = min_sum_end[end]+1;
            }
            // 越界end到了len, 违规end也对
            res = max(res, end - i);
            if (end > i)
            {                  // 窗口内还有数， [i, end) [4,4)
                sum -= arr[i]; // 左边缩一下位置
            }
            else
            {                // 窗口内已经没有数了，说明从i开头的所有子数组累加和都不可能<=k
                end = i + 1; // 直接到下一个位置。因为i下面也会+1
            }
        }
        return res;
    }
};
int main()
{
    vector<int> arr = {-1, 1, 1, -1,-1,6};
    int k = 0;
    Solution sol;
    cout << sol.getLongest_sub_array_length(arr, k) << endl;

    // int N,K;
    // cin >> N >> K;
    // vector<int> arr;
    // for (int i=0;i<N;i++) {
    //     int tmp;
    //     cin >> tmp;
    //     arr.push_back(tmp);
    // }
    // Solution sol;
    // cout << sol.getLongest_sub_array_length(arr, k) << endl;
    return 0;
}