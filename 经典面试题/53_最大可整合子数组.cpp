/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-02 12:14:58
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
可整合数组的定义：如果一个数组在排序后，每相邻两个数差的绝对值都为1，则该数组为可整合数组。例如[5,3,4,6,2]排序之后为[2,3,4,5,6]，符合每相邻两个数差的绝对值为1，所以这个数组为可整合数组。
给定一个整型数组arr，请返回其中最大可整合子数组的长度，例如[5,5,3,2,6,4,3]的最大可整合子数组为[5,3,2,6,4]，所以返回为5

*/

class Solution
{
public:
    /*
    可整合数组定义较为抽象，做简单化：
        1. 自己思想：
            a. 每个子数组的最大值和最小值，在最大值-最小值如果不等于数组长度，一定不是可整合的。如果出现相同数字，则也不是可整合数组
            b. 枚举所有子数组，每次有扩时求最大值和最小值和长度，总复杂度是O(N^2)
            c. 滑动窗口，右指针每次往右扩到不能再扩时，左边界右移。每次扩充过程中得到窗口内最大值和最小值（单调栈），以及是否有重复数字(map)。
                窗口方法不太行，因为(max-min)和窗口大小没有单调性，比如从0开始扩，扩到了n-1都不达标。假设左边界右移，只能判断[1..n-1]是否达标，不能判断[1..2]、[1..3]是否达标。核心就在于(max-min)和len没有单调性，窗口内有重复数字是有单调性的(因为除非将重复的数字剔除出去，否则将一直重复)
        2. 左神思想，定义可整合数组：
            a. 数组中没有重复值
            b. 最大值-最小值=len-1
    */
    int max_len(vector<int> arr) {
        int len = arr.size();
        if (len==0) {
            return 0;
        }

        set<int> _sets;
        int ans = 0;
        //枚举每个子数组
        for (int l=0;l<len;l++) {

            int cur_max_num = arr[l];
            int cur_min_num = arr[l];
            _sets.clear();
            _sets.insert(arr[l]);
            for (int r = l + 1; r < len; r++) {
                if (_sets.find(arr[r]) != _sets.end()) { //有重复数组了，以arr[l]开头的子数组不可能是可整合数组了
                    break;
                }

                _sets.insert(arr[r]);
                int cur_len = r-l+1;
                cur_max_num = max(cur_max_num, arr[r]);
                cur_min_num = min(cur_min_num, arr[r]);
                cout <<"l:" << l<<" r:"<< r<< " cur_max_num: " << cur_max_num << " cur_min_num: " << cur_min_num << endl;
                if ((cur_max_num - cur_min_num) == cur_len - 1) {
                    ans = max(ans, cur_len);
                }
            }
        }
        return ans;
    }
};

int main()
{

    Solution sol;
    vector<int> arr = {5,5,3,2,6,4,3};

    cout << sol.max_len(arr) << endl;

    return 0;
}