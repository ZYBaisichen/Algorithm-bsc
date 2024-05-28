/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-28 12:16:37
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
https://leetcode.cn/problems/strange-printer/description/
有台奇怪的打印机有以下两个特殊要求：

打印机每次只能打印由 同一个字符 组成的序列。
每次可以在从起始到结束的任意位置打印新字符，并且会覆盖掉原来已有的字符。
给你一个字符串 s ，你的任务是计算这个打印机打印它需要的最少打印次数。

 
示例 1：

输入：s = "aaabbb"
输出：2
解释：首先打印 "aaa" 然后打印 "bbb"。
示例 2：

输入：s = "aba"
输出：2
解释：首先打印 "aaa" 然后在第二个位置打印 "b" 覆盖掉原来的字符 'a'。
 

提示：

1 <= s.length <= 100
s 由小写英文字母组成

*/

/*

f(l,r)表示，需要几转可以达到s。
贪心：l..r上问题，边缘处让其第一转就转出来，以后再也不改，不会错失掉答案。因为第一个数肯定需要一转来转出来。
    比如122211222, 第一次转出来1，然后两次2，共3转。第一次转出来2，然后两次1，也是3转；答案是一样的。
可能性： 按照第一个数解决时，最多往右连带转多少元素划分
   1. [l..l+1]范围上一起转, 调用f(l,l+1)+f(l+2,r)
   2. [l..l+2]范围上一起转, 调用f(l,l+2)+f(l+3,r)
   ...
   所有可能性结果取最小值
base case：
   当l==r时，只有一个字符，答案是1

注意：
    1112221112中，假设使用使用了f(l,l+6)+f(l+7,9)，如果s[l+6]==s[l+7]位置是1的话，总结果还需要减1。
    其实就是f(l,l+6)是先解决的l位置的1，l+7先解决的l+7位置的1，因为两个位置上的字符相等，其实是可以合并到一起搞定的
*/

class Solution {
public:
    int strangePrinter_baoli(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        return f(s, 0, len-1);
    }
    int f(string& s, int l, int r) {
        if (l>r) {
            return INT_MAX;
        }
        if (l == r) {
            // cout << "l:" << l << " r:" << r << " ans:1" << endl;
            return 1;
        }
        int ans = r-l+1; //l..r之间最多转R-l+1次，即一个字符一个字符的转
        for (int k=l;k<r;k++) {
            ans = min(ans, f(s, l, k)+f(s,k+1,r) - (s[l] == s[k+1] ? 1 : 0) );
        }
        // cout << "l:" << l << " r:" << r << " ans:" << ans << endl;
        return ans;
    }

    int strangePrinter(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        vector<vector<int>> dp(len, vector<int>(len, 0));
        //先填对角线
        for (int i=0;i<len;i++) {
            dp[i][i] = 1;
            if (i<=len-2) {
                dp[i][i+1] = s[i] == s[i+1] ? 1 : 2; //相等就是1转，不等就是2转
            }
        }

        /*
        l依赖l, l+1, l+2, l+3, l+4, l+5
        r依赖r, r-1, r-2
        所以从下往上，从左往右填格子
        dp[l][r]意义：在l..r范围上，先转出来s[l]上的字符，转出来s[l..r]需要的最小转数
        */
        for (int l=len-3;l>=0;l--) {
            for (int r=l+2;r<len;r++) {
                int ans = r-l+1;
                for (int k=l;k<r;k++) {
                    ans = min(ans, dp[l][k] + dp[k+1][r] - (s[l]==s[k+1] ? 1: 0));
                }
                dp[l][r] = ans;
            }
        }
        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<len;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return dp[0][len-1];
    }
    
    
    //有点问题，在"dcdcdddabcaddcdcbdbcadadadddac"测试用例下
    int strangePrinter_youhua(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        vector<vector<int>> dp(len, vector<int>(len, 0));
        //先填对角线
        for (int i=0;i<len;i++) {
            dp[i][i] = 1;
            if (i<=len-2) {
                dp[i][i+1] = s[i] == s[i+1] ? 1 : 2; //相等就是1转，不等就是2转
            }
        }

        /*
        l依赖l, l+1, l+2, l+3, l+4, l+5
        r依赖r, r-1, r-2
        所以从下往上，从左往右填格子
        dp[l][r]意义：在l..r范围上，先转出来s[l]上的字符，转出来s[l..r]需要的最小转数
        */
        for (int l=len-3;l>=0;l--) {
            for (int r=l+2;r<len;r++) {
                int ans = r-l+1;
                if (s[l] == s[r]) {
                    dp[l][r] = dp[l+1][r-1] + 1 - ((s[l] == s[l+1]||s[r]==s[r-1]) ? 1: 0);
                } else {
                    for (int k=l;k<r;k++) {
                        ans = min(ans, dp[l][k] + dp[k+1][r] - (s[l]==s[k+1] ? 1: 0));
                    }
                    dp[l][r] = ans;
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
        return dp[0][len-1];
    }

    //将初次填表放在了主循环里，但貌似没什么优化
    int strangePrinter_youhua2(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        vector<vector<int>> dp(len, vector<int>(len, 0));
        /*
        l依赖l, l+1, l+2, l+3, l+4, l+5
        r依赖r, r-1, r-2
        所以从下往上，从左往右填格子
        dp[l][r]意义：在l..r范围上，先转出来s[l]上的字符，转出来s[l..r]需要的最小转数
        */
       int ans = -1;
        for (int l=len-1;l>=0;l--) {
            for (int r=l;r<len;r++) {
                if (l==r) {
                    dp[l][r] = 1;
                } else if (r==l+1) {
                    dp[l][r] = s[l]==s[r] ? 1 : 2;
                } else {
                    ans = r-l+1;
                    for (int k=l;k<r;k++) {
                        ans = min(ans, dp[l][k] + dp[k+1][r] - (s[l]==s[k+1] ? 1: 0));
                    }
                    dp[l][r] = ans;
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
        return dp[0][len-1];
    }
};

int main() {
    Solution sol;

    string s = "dcdcdddabcaddcdcbdbcadadadddac";
    // cout << sol.strangePrinter_baoli(s) << endl;
    cout << sol.strangePrinter(s) << endl;
    cout << sol.strangePrinter_youhua2(s) << endl;

    return 0;
}