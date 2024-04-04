/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 15:41:17
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

using namespace std;

/*
给定一个int类型的数组arr，已知除了一种数出现1次之外，所有剩下的数都出现了k次，如何使用O(1)的额外空间，找到这个数
int[] arr, int k, k>1
*/

/*
假设数组中其他数都出现了3次，只有一种数出现了1次。使用3进制的异或
准备一个不超32长度的数组，表示三进制数：t:[0,0,0....]

遍历arr
1. 14: 转换成3进制为112，加到t上[....112]
2. 6: 转换成3进制为20，加到t上[......132]
t每个位置模3，整个再转换成10进制就是只出现一次的数
如果某个数出现了3次，在对应位置上一定会加3次，一定会整除3，所以模完之后就是十进制的数

*/
class Solution
{
public:
    void set_num_to_t(int num, vector<int> &t, int k)
    {
        // 获得num的k进制的数，并将其累加到t上
        int idx = 0;
        
        while (num != 0)
        {
            t[idx] += num % k;
            num = num / k;
            idx++;
        }
    }
    int get_one_num(vector<int> &arr, int k)
    {
        int len = arr.size();
        vector<int> t(32, 0);
        for (int i = 0; i < len; i++)
        {
            set_num_to_t(arr[i], t, k);
        }

        // for (int i=0;i<32;i++) {
        //     cout << t[i] << " ";
        // }
        // cout << endl;

        int base = 1;
        int res = 0;
        for (int i = 0; i < 32; i++)
        {
            int real = t[i]%k; //将出现了k次的都去掉
            res += real * base;
            base = base * k;
        }
        return res;
    }
};

int main()
{

    Solution sol;

    // string s="12##3##";
    vector<int> arr= {1,1,1,3,3,5,5,5,3,7,8,8,8};
    // vector<int> arr = {1, 1, 1, 3};
    int k=3;
    cout << sol.get_one_num(arr, k) << endl;

    return 0;
}