/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-28 18:08:28
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

using namespace std;

/*
给定一个正整数的数组arr和一个整数aim，如果可以自由选择arr中的数字，想累加得到1~aim范围上所有的数，范围arr中还缺少几个数

例子：
arr=[1,2,3,7] aim15， 想要累加1-15所有的数，还差14这个数，所以结果返回1
*/
typedef long long ll;
class Solution
{
public:
    /*
    当我解决了1-range后，还差range+1的数，如果还没达到aim才能解决[range+1...range+range+1]范围上的数

    比如：解决了1，来个2可以解决[1..3]，再来个4,可以解决[1..7], 再来个8可以解决[1...15]

    对于一个数组[4,7,13,29] aim=56
    1. 最经济的完成所有目标，比如4需要完成1-3，缺1，有了1后只能解决[1..1]，还差个2，有了2可以解决[1..3]，4可以解决到[1..7]
    2. 遍历到7，要求其最经济的解决[1..6]，但现在已经解决了，所以将7加上，解决了[1..14]
    3. 遍历到13，目标解决[1..13]，使用13加上解决[1...27]
    4. 遍历到29，目标解决[1..28]，所以缺28，将28加上可以解决到[1...55], 在加上29，可以解决到[1...84]
    */

    int minPatches(vector<int> &nums, int aim)
    {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        sort(nums.begin(), nums.end());

        int range = 0; // 目前解决了[1...range]范围
        int ans = 0;   // 需要补充多少数字，就是缺了多少数字
        for (int i = 0; i < len; i++)
        {
            // nums[i]需要解决[1..nums[i]-1]的数，让后将自己补上
            while (range < nums[i] - 1)
            {
                cout << "buchong=" << range+1 << endl;
                range = range + 1;
                ans++; // 补充一个range+1的数
                if (range >= aim)
                {
                    return ans;
                }
            }

            // 解决到了nums[i]-1
            range += nums[i];
            if (range >= aim)
            {
                return ans;
            }
        }

        // 如果遍历完数组还没有解决到aim，则每次补充一个range+1
        while (range < aim)
        {
            cout << "buchong2=" << range + 1 << endl;
            range += range + 1;
            ans++;
        }
        return ans;
    }
};
int main()
{

    Solution sol;
    // vector<int> arr = {2, 6, 4, 8, 10, 9, 15};
    vector<int> arr = {4,7,13,29};
    int aim = 56;

    cout << sol.minPatches(arr,aim) << endl;
    return 0;
}