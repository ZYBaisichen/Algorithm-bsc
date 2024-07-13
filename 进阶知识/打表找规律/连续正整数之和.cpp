/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-13 11:31:41
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

// 判断一个数字是否是若干数量(数量>1)的连续正整数的和
class Solution
{
public:
    bool is_sum(int num)
    {
        for (int start = 1; start <= num; start++)
        {
            int sum = start;
            for (int j = start + 1; j <= num; j++)
            {
                // cout << "sum:" << sum  << " j:" << j << " num:" << num<< endl;
                if (sum + j > num)
                {
                    break;
                }
                if (sum + j == num)
                {
                    // cout << "1111sum:" << sum << " j:" << j << " num:" << num<< endl;
                    return true;
                }
                sum += j;
            }
        }
        return false;
    }

    bool is_sum2(int num) {
        if (num == 0 || num==1) {
            return false;
        }
        int ss = (num-1)&num; //当为2的某次方时不能被连续整数累加
        // cout << "num:" << num - 1 << "," << num << ":" << ss << endl;
        if ( ((num-1)&num) == 0) {
            return false;
        }
        return true;
    }
};
int main()
{

    int n = 1000;
    Solution sol;
    int a = 1&2;


    for (int i = 0; i < n; i++)
    {
        bool ans0 = sol.is_sum(i);
        // cout << i << ","<< ans0 << endl;
        bool ans1 = sol.is_sum2(i);
        if (ans0 != ans1) {
            cout << i << ","<< ans0 << "," << ans1 << endl;
        }
    }
    return 0;
}