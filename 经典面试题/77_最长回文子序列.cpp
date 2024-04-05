/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-05 18:33:56
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
给定一个字符串，求最长回文子序列

解法一: 将字符串逆序，两个字符串求最长公共子序列就是它的最长回文子序列
解法二: 范围尝试模型， dp[l][r]代表str[l...r]的最长回文子序列。
1. 对角线上只有一个字符都是1
2. 往上一条对角线，只有两个字符，如果相等回文串就是2，否则就是0
3. 普遍为止dp[l][r], 可能性：
    a. 不包含l和r位置字符，依赖dp[l+1][r-1]
    b. 包含l字符，不包含r位置字符，依赖dp[l][r-1]
    c. 不包含l字符，包含r字符，依赖dp[l+1][r]
    d. 两个位置字符都包含，必须满足str[l]==str[r]， 然后是dp[l+1][r-1]+2

https://leetcode.cn/problems/longest-palindromic-subsequence/description/

*/

class Solution
{
public:

    int longestPalindromeSubseq(string s) {
        int len = s.length();
        if (len==0) {
            return 0;
        }
        vector<vector<int>> dp(len,vector<int>(len,0));
        //对角线
        for (int i=0; i<len; ++i) {
            dp[i][i] = 1;
        }
        //往上一条对角线
        for (int i=0;i<len-1;i++) {
            if (s[i]==s[i+1]) {
                dp[i][i+1] = 2;
            } else {
                dp[i][i+1] = 1; //最少是一个字符
            }
        }

        for (int i=len-2;i>=0;i--) {
            for (int j=i+2;j<len;j++) {
                int p1 = dp[i+1][j-1];
                int p2 = dp[i][j-1];
                int p3 = dp[i+1][j];
                int p4 = INT_MIN;
                if (s[i]==s[j]) {
                    p4 = dp[i+1][j-1]+2;
                }
                dp[i][j] = max(max(p1, p2), max(p3, p4));
            }
        }
        // for(int i=0;i<len;i++) {
        //     for (int j=0;j<len;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return dp[0][len-1];
    }
};

int main()
{

    Solution sol;
    string str = "abcabc";
    cout << sol.longestPalindromeSubseq(str) << endl;
    return 0;
}