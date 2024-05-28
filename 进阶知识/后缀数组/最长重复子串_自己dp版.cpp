/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-28 21:21:47
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
#include <unordered_set>

using namespace std;

/*
1062题dp就可以过：https://leetcode.cn/problems/longest-repeating-substring/description/
1044题更严格一些: https://leetcode.cn/problems/longest-duplicate-substring/
给定字符串 S，找出最长重复子串的长度。如果不存在重复子串就返回 0。

 

示例 1：

输入："abcd"
输出：0
解释：没有重复子串。
示例 2：

输入："abbaba"
输出：2
解释：最长的重复子串为 "ab" 和 "ba"，每个出现 2 次。
示例 3：

输入："aabcaabdaab"
输出：3
解释：最长的重复子串为 "aab"，出现 3 次。
示例 4：

输入："aaaaa"
输出：4
解释：最长的重复子串为 "aaaa"，出现 2 次。
 

提示：

字符串 S 仅包含从 'a' 到 'z' 的小写英文字母。
1 <= S.length <= 1500

*/

/*
dp方法：
一个样本做行，一个样本做列。两个相同的字符串
dp[i][j]表示必须以s1[i]结尾，且以s2[j]结尾情况下的最长公共子串长度是多长。


当s1[i]==s2[j]的时候，依赖dp[i-1][j-1]，即往左还能扩到dp[i-1][j-1]个字符
当s1[i]!=s2[j]的时候，dp[i][j]=0

*/
class Solution {
public:
    int longestRepeatingSubstring(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        vector<vector<int>> dp(len, vector<int>(len, 0));
        int ans = 0; //至少是1
        //第一行
        for (int j=1;j<len;j++) {
            dp[0][j] = s[0]==s[j]?1:0;
        }
        for (int i=1;i<len;i++) {
            for (int j=i+1;j<len;j++) {
                if (i!=j && s[i] == s[j]) {
                    dp[i][j] = dp[i-1][j-1]+1;
                    ans = max(ans, dp[i][j]);
                }
            }
        }
        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<len;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return ans;
    }
};

int main() {
    Solution sol;
    string s = "aaaaa";
    cout << sol.longestRepeatingSubstring(s) << endl;

    return 0;
}