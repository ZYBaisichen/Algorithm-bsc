/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-19 11:18:38
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
#include <unordered_map>

using namespace std;

/*

https://leetcode.cn/problems/find-the-duplicate-number/description/
给定一个包含 n + 1 个整数的数组 nums ，其数字都在 [1, n] 范围内（包括 1 和 n），可知至少存在一个重复的整数。

假设 nums 只有 一个重复的整数 ，返回 这个重复的数 。

你设计的解决方案必须 不修改 数组 nums 且只用常量级 O(1) 的额外空间。



示例 1：

输入：nums = [1,3,4,2,2]
输出：2
示例 2：

输入：nums = [3,1,3,4,2]
输出：3
示例 3 :

输入：nums = [3,3,3,3,3]
输出：3




提示：

1 <= n <= 105
nums.length == n + 1
1 <= nums[i] <= n
nums 中 只有一个整数 出现 两次或多次 ，其余整数均只出现 一次


进阶：

如何证明 nums 中至少存在一个重复的数字?
你可以设计一个线性级时间复杂度 O(n) 的解决方案吗？
*/

/*

*/
class Solution
{
public:
    // 假设数组中只有一个重复数字时可以这样做，但多个的时候就找不到了
    int findDuplicate1(vector<int> &nums)
    {
        int n = nums.size() - 1;
        int sum_1_n = n * (1 + n) / 2;
        int sum = 0;
        for (int i = 0; i < nums.size(); i++)
        {
            sum += nums[i];
        }
        return sum - sum_1_n;
    }

    /*
    假设能改动数组的值，则可以使用下标循环怼，类似完美洗牌问题的解。比如[1,3,4,2,2]
    nums[0]=1是一个循环，位置正确；下一个循环入口在2位置， nums[2]=4, 令num[3]=-4， nums[3]原来位置是2;
        令nums[1]=-2, 令nums[2]=3，结束一个循环；
        下一个循环入口位置在8位置，但没有到8为止，最多到4位置, nums[4]=2，此时nums[1]=-2，已经被处理过了，说明出现了重复数字，而此时的nums[4]=2就是重复数字


    不改动数组的值：同样是再下标和值之间做文章
        1. 如果数组中没有重复的数，将下标n和nums[n]建立一个映射关系f(n)，认为f(n)是一个指针，从下表n指向nums[n]。比如[1,3,4,2]
            0->1, 1->3, 2->4, 3->2， 总结成链表就是0->1->3->2->4。
        2. 如果数组中存在重复的数，同样将下标n和nums[n]建立映射关系f(n)，比如[1,3,4,2,2]
            0->1,1->3,2->4,3->2,4->2。 变成链表就是0->1->3->2->4->2。 存在了环路
        3. 综上，判读是否有环可以用快慢指针，然后找到入环点就是重复数字

    */
    int findDuplicate(vector<int> &nums)
    {
        int len = nums.size();
        if (len == 0)
        {
            return -1;
        }
        int fast = 0, slow = 0;
        while (fast < len && nums[fast] < len && slow < len) {
            slow = nums[slow];
            fast = nums[nums[fast]];
            if (fast == slow) {
                break;
            }
        }
        // 存在环
        if (fast == slow)
        {
            fast = 0;
            while (fast != slow)
            {
                fast = nums[fast];
                slow = nums[slow];
            }
            return fast;
        }
        return 0;
    }
};
int main()
{

    Solution sol;
    vector<int> nums = {1, 3, 4, 2, 2};
    cout << sol.findDuplicate(nums) << endl;
    return 0;
}
