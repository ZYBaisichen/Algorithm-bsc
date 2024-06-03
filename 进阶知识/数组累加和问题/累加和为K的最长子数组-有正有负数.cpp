/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-13 20:25:09
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
// 给定一个无序数组arr, 值可能正、可能负、可能0
// 给定一个整数aim
// 求arr所有子数组中累加和为aim的最长子数组长度
// 测试链接 : https://www.nowcoder.com/practice/36fb0fd3c656480c92b569258a1223d5
// 请同学们务必参考如下代码中关于输入、输出的处理
// 这是输入输出处理效率很高的写法
// 提交以下的code，提交时请把类名改成"Main"，可以直接通过

class Solution
{
public:
    int getLongest_sub_array_length(vector<int> &arr, int k)
    {
        if (arr.size() == 0 || k < 0)
        {
            return 0;
        }
        map<int, int> tmp; // 记录求和值最早出现的位置
        tmp[0] = -1;
        int len = arr.size();
        int sum = 0;
        int res = 0; //记录满足条件的数组最大长度
        for (int i = 0; i < len; i++)
        {
            sum += arr[i];
            // cout << "sum:" << sum << " k-sum" << k-sum << endl;
            if (tmp.find(k - sum) != tmp.end())
            {
                res = max(res, i - tmp[k - sum]);
            }
            if (tmp.find(sum) == tmp.end())
            {
                tmp[sum] = i;
            }
        }
        return res;
    }

    
};
int main()
{
    vector<int> arr = {-1, 1, 1, -1, 6};
    int k = 6;
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