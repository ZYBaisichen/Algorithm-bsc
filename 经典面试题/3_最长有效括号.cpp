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