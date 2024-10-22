/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 16:07:42
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
1. 给定一个数组，如果某个数出现次数超过了数组长度的一半，打印这个数，没有则不打印

2.给定一个数组arr和整数k，arr长度为n，如果有某些数出现次数超过了n/k，打印这些数，如果没有则不打印

著名的水王问题：
https://blog.csdn.net/qq_45173404/article/details/117809364

仓库管理员以数组 stock 形式记录商品库存表。stock[i] 表示商品 id，可能存在重复。请返回库存表中数量大于 stock.length / 2 的商品 id。

 

示例 1:

输入: stock = [6, 1, 3, 1, 1, 1]
输出: 1
 

限制：

1 <= stock.length <= 50000
给定数组为非空数组，且存在结果数字
 

注意：本题与主站 169 题相同：https://leetcode-cn.com/problems/majority-element/
超级水王问题：给你一个数组，出现次数大于数组长度的一半的元素称之为水王数，怎么能快速找到水王数？

内存限制：时间复杂度O(n)，额外空间复杂度O(1)——也就是遍历数组的次数为有限次，申请的变量数为有限个

*/


class Solution
{
public:
    /*
    因为一次删除两个数字只关注两个数字是不是相等，所以就引出了一个投票问题
    国家票选领导，一个人必须超过一半的人投票才能当选领导。为了公平，只需要提供一个机器，放两张票上去可以看投票的id是否一样。
    就是这个流程
    */
    // https://leetcode.cn/problems/shu-zu-zhong-chu-xian-ci-shu-chao-guo-yi-ban-de-shu-zi-lcof/description/
    int inventoryManagement(vector<int> &nums)
    {
        // 如果一次删除两个不同的数，最后如果不剩下数，则说明没有数在一半以上
        // 如果剩下了数，则遍历一遍考察是否大于一半

        int len = nums.size();

        // 候选cand
        int cand = 0;
        int hp = 0;
        for (int i = 0; i < len; i++)
        {
            if (hp == 0)
            {
                cand = nums[i];
                hp = 1;
            }
            else if (cand == nums[i])
            {
                hp++;
            }
            else
            {
                hp--;
            }
        }

        int times = 0;
        // 比如[1,2,3,4,5]每次都会有新的候选，最后剩5，所以cand如果不是0，不一定是大于一半的数
        // 所以还需要再统计一遍
        for (int i = 0; i < len; i++)
        {
            if (cand == nums[i])
            {
                times++;
            }
        }
        if (times > len / 2)
        {
            // cout << cand << endl;
            return cand;
        }
        return -1;
    }
};

int main()
{

    Solution sol;

    // string s="12##3##";
    vector<int> nums = { 6, 1, 3, 1, 1, 1 };

    cout << sol.inventoryManagement(nums) << endl;

    return 0;
}