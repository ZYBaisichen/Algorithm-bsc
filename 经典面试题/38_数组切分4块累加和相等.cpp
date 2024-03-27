/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-27 11:38:54
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
给定一个正数数组arr，返回该数组能不能分成4个部分，并且每个部分的累加和相等，切分位置的数不要。
比如arr=[3,2,4,1,4,9,5,10,1,2,2]，三个切分点下标为2,5,7。切分出数组[3,2], [1,4], [5], [1,2,2]
累加和为5
*/

/*
 */
class Solution
{
public:
    /*
        1. 前缀和位置记在map中。因为都是正数，所以前缀和递增不会重复
        2. 因为要切3刀，分成4个部分，所以总长度需要>=7
        3. 分析第1刀切在哪里： 从1位置枚举第一刀，可以枚举到n-6，因为还要为剩下的2刀留下足够的数(n-6 n-5 n-4 n-3 n-2 n-1)
        4. 假设以i位置(a)为第一刀，第一刀前面的累加和是x，则第二刀所在位置（b）的前缀和是2*x+a, 第三刀位置的前缀和为3*x+a+b，再验证下最后块区域和是否是x
    启发：枚举第一刀的时候，需要知道左边的累加和leftsum，往后还需要再累加leftsum个数
    */

    /*

    */
    bool can_split(vector<int> &arr, vector<int> &res)
    {
        int len = arr.size();
        if (len < 7)
        {
            return false;
        }
        map<int, int> sum_map;
        vector<int> sum_arr(len, 0);
        sum_arr[0] = arr[0];
        sum_map[sum_arr[0]] = 0;
        for (int i = 1; i < len; i++)
        {
            sum_arr[i] = sum_arr[i - 1] + arr[i];
            sum_map[sum_arr[i]] = i;
        }
        // 枚举第一刀位置
        for (int i = 1; i <= len - 6; i++)
        {
            int x = sum_arr[i - 1];
            int a = arr[i];
            // 看第二刀是否有
            int second = 2 * x + a;
            if (sum_map.find(second) != sum_map.end())
            {
                int second_idx = sum_map[second] + 1;
                if (second_idx > len - 4)
                { // 实际上有更精细的位置，比如第二刀最多到n-4(n-6 n-5 n-4 n-3 n-2 n-1)
                    continue;
                }
                int b = arr[second_idx];
                int third_sum = 3 * x + b + a;
                if (sum_map.find(third_sum) != sum_map.end())
                {
                    int third_idx = sum_map[third_sum] + 1;
                    if (third_idx > len - 2)
                    { // 第三刀最多到n-2
                        continue;
                    }
                    int c = arr[third_idx];
                    if (sum_arr[len-1] - (3 * x + a + b + c) == x)
                    { // 校验最后一部分是否等于x, 这就找到了最后一部分
                        res.push_back(i);
                        res.push_back(second_idx);
                        res.push_back(third_idx);
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

int main()
{

    Solution sol;
    vector<int> arr = {3, 2, 4, 1, 4, 9, 5, 10, 1, 2, 2};
    vector<int> res;
    cout << "can:" << sol.can_split(arr, res) << endl;
    for (auto iter:res) {
        cout << " " << iter;
    }

    cout << endl;

    return 0;
}