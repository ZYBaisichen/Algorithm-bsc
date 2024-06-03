/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-13 20:17:32
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

// 返回无序数组中累加和为给定值的最长子数组长度
// 给定一个无序数组arr, 其中元素均为正数
// 给定一个整数aim
// 求arr所有子数组中累加和为aim的最长子数组长度
// 测试链接 : https://www.nowcoder.com/practice/36fb0fd3c656480c92b569258a1223d5
// 请同学们务必参考如下代码中关于输入、输出的处理
// 这是输入输出处理效率很高的写法
// 提交以下的code，提交时请把类名改成"Main"，可以直接通过

/*
滑动窗口
*/
class Solution
{
public:
    int getLongest_sub_array_length(vector<int> &arr, int k)
    {
        if (arr.size() == 0 || k < 0) {
            return 0;
        }
        //左闭右开
        int l =0;
        int r = 0;
        int len  = arr.size();
        int sum = 0;
        int res = 0;
        while(r<len) {
            if (sum == k) {
                res = max(res, r-l);
                sum+=arr[r++];
            } else if (sum > k) {
                sum-=arr[l];
                l++;
            } else { //sum<k
                sum+=arr[r++];
            }
        }
        return res;
    }
};
int main()
{
    /*
     * 1 2 3 4
     * 5 6 7 8
     * 9 10 11 12
     * 13 14 15 16
     *
     *
     * 13 9 5 1
     * 14 10 6 2
     * 15 11 7 3
     * 16 12 8 4
     */

    vector<int> arr={1,2,3,1,1,1,1,1,1,1,4,5};
    int k = 6;
    Solution sol;
    cout << sol.getLongest_sub_array_length(arr, k) << endl;
    return 0;
}