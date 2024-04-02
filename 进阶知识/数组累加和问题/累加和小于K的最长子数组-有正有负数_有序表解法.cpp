/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-04-02 17:11:44
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
 *
 */

/*
这里介绍有序表解法：O(N*log(N))
还有O(N)解法是使用最小数组和辅助数组来解，在旁边文件

遍历到i，求出前缀和为sum。使用一个有序表记录前面所有的前缀和，从有序表中找有没有一个元素b, 使得sum+b最接近k。即在有序表中找<=sum-k的第一个数

*/
class Solution
{
public:
    int getLongest_sub_array_length(vector<int> &arr, int k)
    {
        int len = arr.size();
        if (len == 0 || k < 0)
        {
            return 0;
        }

        set<int> _sets;
        _sets.insert(0); // 一个数也不加的时候累加和是0

        int ans = INT_MIN;
        int sum = 0;
        for (int i = 0; i < len; i++)
        {
            sum += arr[i];
            int tmp = sum-k;
            auto it = _sets.lower_bound(tmp); // 返回第一个大于等于tmp的数, 即当前累加到了100，要求k=20，那就要求需要找到大于等于80的数，最小是80，在小就超过20了
            if (it != _sets.end())
            {
                ans = max(ans, sum-(*it));
            }
            cout << "i:" << i << " cur_sum:" << sum << " tmp:" << tmp << " it:" << *it << " ans:" << ans << endl;
            _sets.insert(sum);
        }
        return ans;
    }
};
int main()
{
    vector<int> arr = {-1, 1, 1, -1, -1, 6};
    int k = 10;
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