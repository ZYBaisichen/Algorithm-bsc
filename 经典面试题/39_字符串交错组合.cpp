/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-27 12:15:05
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

using namespace std;

/*
给定三个字符串 s1、s2、s3，请你帮忙验证 s3 是否是由 s1 和 s2 交错 组成的。

两个字符串 s 和 t 交错 的定义与过程如下，其中每个字符串都会被分割成若干 非空
子字符串
：

s = s1 + s2 + ... + sn
t = t1 + t2 + ... + tm
|n - m| <= 1
交错 是 s1 + t1 + s2 + t2 + s3 + t3 + ... 或者 t1 + s1 + t2 + s2 + t3 + s3 + ...
注意：a + b 意味着字符串 a 和 b 连接。



示例 1：


输入：s1 = "aabcc", s2 = "dbbca", s3 = "aadbbcbcac"
输出：true
示例 2：

输入：s1 = "aabcc", s2 = "dbbca", s3 = "aadbbbaccc"
输出：false
示例 3：

输入：s1 = "", s2 = "", s3 = ""
输出：true


提示：

0 <= s1.length, s2.length <= 100
0 <= s3.length <= 200
s1、s2、和 s3 都由小写英文字母组成


进阶：您能否仅使用 O(s2.length) 额外的内存空间来解决它?

https://leetcode.cn/problems/interleaving-string/description/
*/

/*
例子：s1="aabcb" s2="aacbb" s3="aaabaccbbb"

    0    a   a   c   b   b
0   1    1   1   0   0   0

a   1

a   1

b   0

c   0

b   0




因为有重复字符，所以s3走到一个字符时，如果是a，但s1和s2都有，那么就不知道该走s1还是s2了。

0. s3的长度不是s1+s2的长度，那么肯定不可能

s1做行(0...N)， s2做列(0...M)。dp[i][j]:s1拿出i前缀长度(s1[0...i-1])，s2拿出j前缀长度(s2[0...j-1])，能否交错组成s3长度为i+j的字符串[0...i+j-1]
dp[N][M]就是需要求的结果。下面列出可能性：
1. 第一行，s1拿0个，s2拿j个, 所以只要s2[0..j]==s3[0..j]就是true，否则是false；第一列同理
2. dp[i][j]对应s1[0..i-1]和s2[0..j-1] s3[0..i+j-1]
    a. s3[i+j-1] == s1[i-1]时，选择s1的最后一个字符作为我的最后一个字符，前面怎么搞定是dp[i-1][j]
    b. s3[i+j-1] == s2[j-1]时，选择s2的最后一个字符作为我的最后一个字符，前面怎么搞定是dp[i][j-1]
    c. a和b两种有一个为true时，dp[i][j] = true
    d. s3[i+j-1] != s1[i-1] && s3[i+j-1] != s2[j-1]时，dp[i][j] = false
 */
class Solution
{
public:
    bool isInterleave(string s1, string s2, string s3) {
        int len1 = s1.length();
        int len2 = s2.length();
        int len3 = s3.length();
        if (len3 != len1+len2) {
            return false;
        }


        vector<vector<bool>> dp(len1+1, vector<bool>(len2+1, false));
        dp[0][0] = true;

        //第一列
        for (int i = 1; i <= len1; ++i) {
            dp[i][0] = dp[i-1][0] && s3[i-1] == s1[i-1]; //前面有一个false，后面都是false
        }

        //第一行
        for (int j = 1; j <= len2; ++j) {
            dp[0][j] = dp[0][j-1] && s3[j-1] == s2[j-1]; //前面有一个false，后面都是false
        }

        //依赖前一行和前一列，所以从上到下，从左到右
        for (int i = 1; i <= len1; ++i) {
            for (int j = 1; j <= len2; ++j) {
                // cout << "i:" << i << " j:" << j << endl;
                // cout << "s1:" << s1[i-1] << " s2:" << s2[j-1] << " s3:" << s3[i+1-1] <<endl;
                int a=false, b=false;
                if (s3[i+j-1] == s1[i-1]) {
                    a = dp[i-1][j];
                }
                if (s3[i+j-1] == s2[j-1]) {
                    b = dp[i][j-1];
                }
                dp[i][j] = a || b;
            }
        }


        // for (int i=0;i<=len1;i++) {
        //     for (int j=0;j<=len2;j++) {
        //         cout << dp[i][j]<<" ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        return dp[len1][len2];
    }
};
int main()
{

    Solution sol;
    string s1 = "aabcc", s2 = "dbbca", s3 = "aadbbcbcac";
    cout << sol.isInterleave(s1, s2, s3) << endl;
    return 0;
}