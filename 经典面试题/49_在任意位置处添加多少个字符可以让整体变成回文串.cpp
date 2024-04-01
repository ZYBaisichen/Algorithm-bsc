/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-01 16:38:53
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

 给定一个字符串，问在任意处添加多少个字符可以让整体变成回文串
 http://poj.org/problem?id=1159
https://leetcode.cn/problems/minimum-insertion-steps-to-make-a-string-palindrome/
给你一个字符串 s ，每一次操作你都可以在字符串的任意位置插入任意字符。

请你返回让 s 成为回文串的 最少操作次数 。

「回文串」是正读和反读都相同的字符串。



示例 1：

输入：s = "zzazz"
输出：0
解释：字符串 "zzazz" 已经是回文串了，所以不需要做任何插入操作。
示例 2：

输入：s = "mbadm"
输出：2
解释：字符串可变为 "mbdadbm" 或者 "mdbabdm" 。
示例 3：

输入：s = "leetcode"
输出：5
解释：插入 5 个字符后字符串变为 "leetcodocteel" 。


提示：

1 <= s.length <= 500
s 中所有字符都是小写字母。


*/
class Solution
{
public:
    /*
    f(str,l,r)
    l到r范围上至少添加几个可以让整体变成回文串。外层调用f(str,0,len-1)
    */

    /*
     dp[i][i]对角线是回文串，补充0个字符
     dp[i][i+1]两个字符，如果相等就是回文，补充0个；如果不相等则不是回文，补充1个
     dp[i][j]：表示str[i..j]整体添加几个字符可以让i..j变成回文串
         a. j位置字符最后处理，之前的字符变成回文串，最后前面添加一个str[j]变成回文串，此时可能性为dp[i][j-1]+1
         b. 最后解决i位置字符，后面的整体变成回文串，最后在最后添加一个str[i]就变成了回文串，dp[i+1][j]+1
         c. j位置和i位置相等，则为dp[i+1][j-1]
         d. 各个可能性取最小值
    */
    int minInsertions(string s)
    {
        int len = s.length();
        vector<vector<int>> dp(len, vector<int>(len, 0));
        // 第二条对角线
        for (int i = 0; i <= len - 2; i++)
        {
            dp[i][i+1] = s[i] == s[i + 1] ? 0 : 1;
        }

        // dp[i][j]依赖左边、下边、左下角的值，所以从下到上，从左到右填格子
        for (int i = len - 3; i >= 0; i--)
        {
            for (int j = i + 2; j < len; j++)
            {
                dp[i][j] = min(dp[i][j - 1] + 1, dp[i + 1][j] + 1);
                if (s[i] == s[j])
                {
                    dp[i][j] = min(dp[i][j], dp[i + 1][j - 1]);
                }
            }
        }

        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<len;j++) {
        //         cout << dp[i][j]<<" ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return dp[0][len - 1];
    }
};

int main()
{

    Solution sol;
    string str = "xmgogqsxhuchcimdkqstluqpykkjjsxvgshkugoiicauapganwcibrjakbghpvefjwciieuwcrgqifzerxulmylzkuavxsstluwechevssbnjgvonrnbmbnamztvtyqisqfikkvdnttuzzcfbzsjtgxyfgddaqgspkhbxqnvzlvuxtksrdnggannvmioblfkhdmfozndgdxygyqmrjamgzjcjnmadlgiqcgmdjvjokmddtyulhexbbbuoldpfiouthgtectrvouzoduiskajrxiyzwdlsnlimazsbunutxzsafcfvwulcmnnwxbraslraiyanzdcbhiecxoeek";

    cout << sol.minInsertions(str) << endl;

    return 0;
}