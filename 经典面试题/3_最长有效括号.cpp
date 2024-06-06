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

using namespace std;


// 最长有效括号
// 给你一个只包含 '(' 和 ')' 的字符串
// 找出最长有效（格式正确且连续）括号子串的长度。
// 测试链接 : https://leetcode.cn/problems/longest-valid-parentheses/
/*
给你一个只包含 '(' 和 ')' 的字符串，找出最长有效（格式正确且连续）括号
子串
的长度。

 

示例 1：

输入：s = "(()"
输出：2
解释：最长有效括号子串是 "()"
示例 2：

输入：s = ")()())"
输出：4
解释：最长有效括号子串是 "()()"
示例 3：

输入：s = ""
输出：0
 

提示：

0 <= s.length <= 3 * 104
s[i] 为 '(' 或 ')'
*/

/*
dp[i]表示必须以[0...i]子串中，最长有效括号长度
1. 当s[0]=='(', s[1]==')'时，dp[1]=2
2. 有效括号末尾只可能是)， 所以只用填s[i]==')'的dp[i]就可以。
   a. 假设dp[i-1]==0, 代表以s[i-1]为结尾的情况下，找不到合法的括号。要想从i位置往左扩的话，只有一种可能是s[i-1]=='('
   b. 假设dp[i-1]>0时，往左可以扩到i-1-dp[i-1]+1位置，只需要判断left=i-1-dp[i-1]位置是'('时，就可以和s[i]配对，总长度变成dp[i-1]+2
   c. 有个坑是，left再往前一个如果也是合法的，就需要也加上。这样就连到了一起。dp[i]+=dp[left-1]。
*/
class Solution {
public:
    int longestValidParentheses(string s) {
        int len = s.length();
        if (len == 0 || len == 1) {
            return 0;
        }
        //dp[0, len-1]
        // dp[i]: 必须以i为结尾的情况下，最长有效括号子串的长度
        vector<int> dp(len, 0);
        if (s[1] ==')' && s[0] == '(') {
            dp[1] = 2;
        }
        int res = dp[1];
        for (int i=2; i<len; i++) {
            if (s[i] == ')') {
                int left = i - dp[i-1] - 1;
                if (left >= 0 && s[left] == '(') {
                    dp[i] = dp[i-1] + 2;
                    //再往前一个位置看。因为再往前一个也做了同样的操作，所以长度会累加传递
                    if (left - 1 >= 0) {
                        dp[i] += dp[left-1];
                    }
                }
                res = max(res, dp[i]);
            }
        }
        return res;
    }
};

int main() {
    Solution sol;
    string str= "()))((()";
    cout << "str1:" << sol.check_str_1(str) << endl;
    cout << "str2:" << sol.check_str_2(str) << endl;
}