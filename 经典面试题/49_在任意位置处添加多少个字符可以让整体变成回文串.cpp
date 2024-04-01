/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-01 17:42:00
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

    //任何dp表，都可以回溯回决策路径
    string minInsertions_string(string s) {
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
        /*
        acc dp表:
        0 1 1
        0 0 0
        0 0 0
        */

        /*
        mbadm最终填出来的dp表如下：
        0 1 2 3 2
        0 0 1 2 3
        0 0 0 1 2
        0 0 0 0 1
        0 0 0 0 0



        在[0..4]范围上需要添加2个字符才能得到字符，所以总字符长度为5+2=7个字符。
        1. [0..4]范围上，到了dp[0][4]位置。
            a.s[0]==s[4]，所以是由min(左边+1, 下边+1，左下角值)得到的，所以是由dp[1][3]得到
            b.所以可以知道结果字符的最后一个字符是s[4]，第一个是s[0]。初始假设idx=0，对应res的ss_len-idx-1和idx位置。
            c. idx+=1
        2. [1..3]范围上，到了dp[1][3]位置。
            a. s[1]!=s[3], 所以是由min(左边+1, 下边+1)得到的，因为dp[1][2]==dp[2][3]可以任选一个路径得到，假如选了dp[2][3]（往下）
            b. 所以是先处理[2..3]，然后再在后面再补充s[1]，这样res[idx]=s[1], res[ss_len-idx-1] = s[1]
            c. idx+=1
        3. [2..3]范围上，到了dp[2][3]位置。
            a. s[2]!s[3]， 所以是由min(左边+1, 下边+1)得到的，可以任选一个路径得到，假设来到了dp[2][2]（往左）
            b. 所以是先处理[2..2]，然后再在前面补充s[3]，这样res[idx]=s[3], res[ss_len-idx-1]=s[3]
            c. idx+=1
        4. [2..2]范围上，到了dp[2][2]位置。
            因为到了一个字符上，所以直接填res[idx]
        */


        int idx = 0;
        int ss_len = dp[0][len-1]+len;
        string res = "";
        for (int i=0;i<ss_len;i++) {
            res += "#"; //占位符
        }

        int i=0, j=len-1;
        while (i<j) {
            // cout << "i:" << i << " j:" << j << endl;
            int tmp = 0; // 0左边，1下边，2左下角
            int cur_dp = dp[i][j-1]+1;
            if (s[i]==s[j]) {
                if (dp[i+1][j-1] < cur_dp) { //左下角
                    tmp = 2;
                    cur_dp =dp[i+1][j-1];
                }
            }
            if ((dp[i+1][j]+1) < cur_dp) { //下边
                tmp = 1;
                cur_dp = dp[i+1][j];
            }
            // cout << "tmp:" << tmp << endl;
            if (tmp == 0) { //左边，现决定[i...j-1]，再补s[j]
                res[idx] = s[j];
                res[ss_len-idx-1] = s[j];
                // cout << "left:s[i]:" << s[i] << " s[j]:" << s[j] << " idx:" << idx << " ss_len-idx-1:" << ss_len - idx - 1 << " res:"<< res<< endl;
                idx++;
                j-=1;
            } else if (tmp == 1) { //下边，先决定[i+1...j]，再补s[i]
                res[idx] = s[i];
                res[ss_len-idx-1] = s[i];
                // cout << "down:s[i]:" << s[i] << " s[j]:" << s[j] << " idx:" << idx << " ss_len-idx-1:" << ss_len - idx - 1 << " res:" << res << endl;
                idx++;
                i+=1;
            } else if (tmp == 2) { //左下角，先解决[i+1...j-1]，两边字符不变
                res[idx] = s[i];
                res[ss_len-idx-1] = s[j];
                // cout << "left_down:s[i]:" << s[i] << " s[j]:" << s[j] << " idx:" << idx << " ss_len-idx-1:" << ss_len - idx - 1 << " res:"<< res<< endl;
                idx++;
                i+=1;
                j-=1;
            }
        }
        // cout << "idx:" << idx<<endl;
        if (idx != (ss_len+1)/2) { //偶数时，idx刚好到上分位数，不需要再填了
            res[idx] = s[j];
        }
        return res;
    }
};

int main()
{

    Solution sol;
    string str = "leetcode";
    cout << sol.minInsertions(str) << endl;
    cout << sol.minInsertions_string(str) << endl;

    return 0;
}