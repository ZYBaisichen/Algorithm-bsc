/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-04-02 18:01:04
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
 * 给定一个二维数组matrix，再给定一个k值，返回累加和小于等于k，但离k最近的子矩阵累加和
 *
 */

/*
已经求出来了小于k的累加和问题，只需要在matrix上做矩阵压缩枚举就可以

*/
class Solution
{
public:
    int getLongest_sub_array_length(vector<vector<int>> &matrix, int k)
    {
        int n = matrix.size();
        if (n == 0 || k < 0)
        {
            return 0;
        }
        int m = matrix[0].size();
        if (m == 0)
        {
            return 0;
        }

        int ans = INT_MIN;

        for (int up = 0; up < n; up++)
        { // 枚举每个矩阵的上边界
            vector<int> help(n, 0);
            for (int down = up; down < n; down++)
            { // 枚举每个矩阵的下边界，确定了一个矩阵 O(n^2)
                // 下面计算up和down之间矩阵压缩到一起之后，累加和最接近k的子数组最大和
                help[0] += matrix[down][0];

                set<int> _sets;
                _sets.insert(0);
                _sets.insert(help[0]); // 一个数也不加的时候累加和是0
                int cur_ans = help[0] <= k ? help[0] : INT_MIN;
                int sum = help[0];
                for (int j = 1; j < m; j++)
                { // o(m*log(n))
                    help[j] += matrix[down][j];

                    sum += help[j];
                    int tmp = sum - k;
                    auto it = _sets.lower_bound(tmp); // 返回第一个大于等于tmp的数, 即当前累加到了100，要求k=20，那就要求需要找到大于等于80的数，最小是80，在小就超过20了
                    if (it != _sets.end())
                    {
                        cur_ans = max(cur_ans, sum - (*it));
                    }
                    cout << "j:" << j << " cur_sum:" << sum << " tmp:" << tmp << " it:" << *it << " ans:" << cur_ans << endl;
                    _sets.insert(sum);
                }
                ans = max(ans, cur_ans);
            }
        }
        // 总复杂度是O(n^2*m*log(m))
        return ans;
    }
};
int main()
{
    vector<vector<int>> arr =
        {
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };
    int k = 20;
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