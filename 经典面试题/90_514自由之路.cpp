/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-10 20:46:59
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
#include <sstream>

using namespace std;

/*
https://leetcode.cn/problems/freedom-trail/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china

电子游戏“辐射4”中，任务 “通向自由” 要求玩家到达名为 “Freedom Trail Ring” 的金属表盘，并使用表盘拼写特定关键词才能开门。

给定一个字符串 ring ，表示刻在外环上的编码；给定另一个字符串 key ，表示需要拼写的关键词。您需要算出能够拼写关键词中所有字符的最少步数。

最初，ring 的第一个字符与 12:00 方向对齐。您需要顺时针或逆时针旋转 ring 以使 key 的一个字符在 12:00 方向对齐，然后按下中心按钮，以此逐个拼写完 key 中的所有字符。

旋转 ring 拼出 key 字符 key[i] 的阶段中：

您可以将 ring 顺时针或逆时针旋转 一个位置 ，计为1步。旋转的最终目的是将字符串 ring 的一个字符与 12:00 方向对齐，并且这个字符必须等于字符 key[i] 。
如果字符 key[i] 已经对齐到12:00方向，您需要按下中心按钮进行拼写，这也将算作 1 步。按完之后，您可以开始拼写 key 的下一个字符（下一阶段）, 直至完成所有拼写。


示例 1：




输入: ring = "godding", key = "gd"
输出: 4
解释:
 对于 key 的第一个字符 'g'，已经在正确的位置, 我们只需要1步来拼写这个字符。
 对于 key 的第二个字符 'd'，我们需要逆时针旋转 ring "godding" 2步使它变成 "ddinggo"。
 当然, 我们还需要1步进行拼写。
 因此最终的输出是 4。
示例 2:

输入: ring = "godding", key = "godding"
输出: 13


提示：

1 <= ring.length, key.length <= 100
ring 和 key 只包含小写英文字母
保证 字符串 key 一定可以由字符串  ring 旋转拼出
*/

/*

1. 设函数f(i,j)，假设整体ring字符串长度为len，当前在i位置，要想将顶针拨到j位置，需要的最小步数
def f(i,j,len):
    large = max(i,j)
    small = min(i,j)
    ans1 = large-small //大减去小的差值
    ans2 = small+len-large //小的数绕一圈回来
    return min(ans1,ans2)

2. 准备一张map，表示某个字符在哪些位置上出现了
ring= "bacbcabbd"
b: {0,3,6,7}
a: {1,5}
c: {2,4}

3. 有效性检查，比如给个aim="cbd"
如果aim中的字符，有的不在ring中存在，则答案是无穷大

4. 暴力方法：对于每个aim做dfs枚举
c的所有位置做枚举
c固定时对b做枚举
cb固定时对d做枚举


*/

class Solution
{
public:
    int findRotateSteps_baoli(string ring, string key)
    {
        map<char, set<int>> _map;
        int len = ring.length();
        for (int i = 0; i < len; i++)
        {
            if (_map.find(ring[i]) != _map.end())
            {
                _map[ring[i]].insert(i);
            }
            else
            {
                _map[ring[i]] = {i};
            }
        }

        int len_k = key.size();

        for (int i = 0; i < len_k; i++)
        {
            if (_map.find(key[i]) == _map.end())
            {
                return INT_MAX;
            }
        }

        return min_step(0, 0, key, _map, len);
    }

    int get_step(int i, int j, int len)
    {
        int large = max(i, j);
        int small = min(i, j);
        int ans1 = large - small;       // 大减去小的差值
        int ans2 = small + len - large; // 小的数绕一圈回来
        return min(ans1, ans2);
    }

    /*
    pre_idx: 目前指针指到了哪里
    key_idx: 下面请搞定key[key_idx]
    key: 目标串
    _map: ring的字符统计
    rlen: ring的字符串长度
    */
    int min_step(
        int pre_idx,
        int key_idx,
        string &key,
        map<char, set<int>> &_map,
        int rlen)
    {
        if (key_idx == key.length())
        { // 没有东西需要搞定了
            return 0;
        }

        auto &pos = _map[key[key_idx]];
        int ans = INT_MAX;
        for (auto iter : pos) {
            int step = get_step(pre_idx, iter, rlen) + 1;         // 获取从pre_idx到iter的最小路径
            step += min_step(iter, key_idx + 1, key, _map, rlen); // 当前来到的位置iter即是下面的选择中指针指向的位置。
            ans = min(ans, step);
        }
        return ans;
    }

    int findRotateSteps(string ring, string key)
    {
        map<char, set<int>> _map;
        int len = ring.length();
        for (int i = 0; i < len; i++)
        {
            if (_map.find(ring[i]) != _map.end())
            {
                _map[ring[i]].insert(i);
            }
            else
            {
                _map[ring[i]] = {i};
            }
        }

        int len_k = key.size();

        for (int i = 0; i < len_k; i++)
        {
            if (_map.find(key[i]) == _map.end())
            {
                return INT_MAX;
            }
        }

        vector<vector<int>> dp(len+1, vector<int>(len_k+1, INT_MAX));

        return min_step_dp(0, 0, key, _map, len, dp);
    }


    /*
    pre_idx: 目前指针指到了哪里
    key_idx: 下面请搞定key[key_idx]
    key: 目标串
    _map: ring的字符统计
    rlen: ring的字符串长度
    */
    int min_step_dp(
        int pre_idx,
        int key_idx,
        string &key,
        map<char, set<int>> &_map,
        int rlen,
        vector<vector<int>> &dp)
    {
        if (key_idx == key.length())
        { // 没有东西需要搞定了
            dp[pre_idx][key_idx] = 0;
            return 0;
        }

        if (dp[pre_idx][key_idx] != INT_MAX) {
            return dp[pre_idx][key_idx];
        }

        auto &pos = _map[key[key_idx]];
        int ans = INT_MAX;
        for (auto iter : pos) {
            int step = get_step(pre_idx, iter, rlen) + 1;         // 获取从pre_idx到iter的最小路径
            step += min_step_dp(iter, key_idx + 1, key, _map, rlen, dp); // 当前来到的位置iter即是下面的选择中指针指向的位置。
            ans = min(ans, step);
        }
        dp[pre_idx][key_idx] = ans;
        return ans;
    }

    //因为在递归函数中preidx存在跳变，即可能变小也可能变大，所以还不能直接用整张二维表来搞
    int findRotateSteps_dp(string ring, string key) {
    }
};

int main()
{

    Solution sol;
    string ring = "godding";
    string key = "godding";
    cout << sol.findRotateSteps_baoli(ring, key) << endl;
    cout << sol.findRotateSteps(ring, key) << endl;
    return 0;
}