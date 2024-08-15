/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-05 11:27:46
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
#include <functional>

using namespace std;

/*
https://leetcode.cn/problems/next-permutation/description/
整数数组的一个 排列  就是将其所有成员以序列或线性顺序排列。

例如，arr = [1,2,3] ，以下这些都可以视作 arr 的排列：[1,2,3]、[1,3,2]、[3,1,2]、[2,3,1] 。
整数数组的 下一个排列 是指其整数的下一个字典序更大的排列。更正式地，如果数组的所有排列根据其字典顺序从小到大排列在一个容器中，那么数组的 下一个排列 就是在这个有序容器中排在它后面的那个排列。如果不存在下一个更大的排列，那么这个数组必须重排为字典序最小的排列（即，其元素按升序排列）。

例如，arr = [1,2,3] 的下一个排列是 [1,3,2] 。
类似地，arr = [2,3,1] 的下一个排列是 [3,1,2] 。
而 arr = [3,2,1] 的下一个排列是 [1,2,3] ，因为 [3,2,1] 不存在一个字典序更大的排列。
给你一个整数数组 nums ，找出 nums 的下一个排列。

必须 原地 修改，只允许使用额外常数空间。

 

示例 1：

输入：nums = [1,2,3]
输出：[1,3,2]
示例 2：

输入：nums = [3,2,1]
输出：[1,2,3]
示例 3：

输入：nums = [1,1,5]
输出：[1,5,1]
 

提示：

1 <= nums.length <= 100
0 <= nums[i] <= 100

*/

/*
暴力枚举：
    枚举出来所有的字符串，然后排序，记录nums的位置，然后找到下一个串

参考题解：
https://leetcode.cn/problems/next-permutation/solutions/80560/xia-yi-ge-pai-lie-suan-fa-xiang-jie-si-lu-tui-dao-/
核心思想是通过数学观察法得到的。
下一个排列的定义是：给定数字序列的字典序中下一个更大的排列。如果不存在下一个更大的排列，则将数字重新排列成最小的排列，即升序排列 
将问题形式化的描述为：给定若干数字，将其组合为一个整数。如果将这些数字重新排列，以得到更大的数，且尽可能的小。
                 比如123的下一个更大的数是132，如果没有更大的数，则输出最小数。


以1,2,3,4,5,6为例，排序依次是：
123456
123465
123546
123564
123645
123654
...
654321

算法推导：
1. 我们希望下一个数比当前数大，这样才能满足“下一个排列”的第一个条件。因此需要将后面的大数与前面的小数交换，可以得到一个较大的数
2. 我们还希望下一个数，增加的幅度尽可能的小，这样才能满足“与当前排列紧邻”的要求。为满足这个要求：
    a. 将一个尽可能小的'大数'与前面的'小数'交换。比如123465，下一个排列应该是把5和4交换。而不是把6和4交换
    b. 将“大数”交换到前面后，需要将"大数"后面的所有数重置为升序，因为高位已经是变的更大了，后面需要尽可能的小，而重置为升序是最小，
        比如123465将5和4交换后变成123564，然后将5后面的数排序，变成123546，显然123546<123564

算法过程：
1. 从后向前找到第一个相邻的的升序对, (i,j)，满足a[i]<a[j]，此时[j,end)必定是降序排列的。因为从后往前(i,j)是第一个升序对
    此时的a[i]就是需要和后面交换顺序的小数。
2. 在[j,end)从后往前找到第一个大于a[i]的数，因为[j,end)上是降序，第一个大于a[i]的数就是这个序列中，满足大于a[i]条件的最小的数， 假设这个数是[k]。
3. 将a[i]与a[k]交换
4. 因为交换前[j,end)逆序，而a[k]又是第一个大于a[i]的数，将a[i]换过来后，a[j...k-1]上的数都大于a[i], a[k+1...end]上的数都小于a[i]。所以[j,i]肯定是逆序，[i,end)也是逆序，所以[j,end)还是逆序的。
    所以，将a[k]往后的位置重置为升序，只需要将[j,end)上的数逆序就可以。
5. 如果在步骤1中没有找到相邻元素，说明[begin,end)上都是降序对，则直接将[begin,end)逆序，即执行步骤4

*/
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int len = nums.size();
        if (len == 0 || len == 1) {
            return;
        }

        int i=len-2,j=len-1;
        //找到第一个升序对
        while (i>0 && nums[i] >= nums[j]) {
            i--;
            j--;
        }

        //没找到升序对
        if (i==0 && nums[i]>=nums[j]) {
            reverse(nums, 0, len-1);
        } else {

            //找到了一个升序对，从后往前找到第一个大于nums[i]的数
            int k = len-1;
            while (k>=j) {
                if (nums[k] > nums[i]) {
                    break;
                }
                k--;
            }
            //交换i和k
            swap(nums[i], nums[k]);

            //逆序i之后的数
            reverse(nums, j, len-1);
        }
    }

    void reverse(vector<int>& nums, int i, int j) {
        cout << "nums len:" << nums.size() << " i:" << i << " j:" << j << endl;
        while (i<j) {
            swap(nums[i], nums[j]);
            i++;
            j--;
        }
    }
};
int main()
{

    Solution sol;
    vector<int> nums= {1,1,5};
    sol.nextPermutation(nums);
    for (auto it:nums) {
        cout << it<<" ";
    }
    cout << endl;

    return 0;
}