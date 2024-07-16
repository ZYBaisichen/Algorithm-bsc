/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-02 21:56:17
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
给定两个字符串s和t，返回s子序列等于t的不同子序列个数有多少个？如果得到子序列A删除的位置和得到子序列B删除的位置不同，那么认为A和B是不同的子序列
例子
s="rabbbit", t="rabbit"
返回：3
rabbbit中的b保留任意两个都可以构成t，所以有3种
*/

class Solution
{
public:
    /*
    一个样本做行一个样本做列的模型。
    s为行，t为列。dp[i][j]表示[0..i]删除到[0...j]字符串有多少种删除方法
    */
   int get_num(string s, string t) {
        int len1  = s.length();
        int len2 = t.length();
        if (len2 == 0) {
            return 1;
        }
        if  (len1==0) {
            return 0;
        }
        vector<vector<int>> dp(len1, vector<int>(len2, 0));

        if (s[0] == t[0]) {
            dp[0][0] = 1; //如果第一个字符能对上，从s[0]删到t[0]只有一种删除方法，就是不删除
        }

        //填第一行。将s第0个字符，删除到[0..j]字符串, j>=1。不可能, 所以都是0

        //填第一列
        for (int i=1;i<len1;i++) {
            dp[i][0] = dp[i-1][0];
            if (s[i] == t[0] ) {
                dp[i][0] += 1; //每增加一个和t[0]相等的字符，保留的方法数就会加1。(等于删除的方法数)
            }
        }

        /*
        普遍位置
        dp[i][j]表示[0..i]删除到[0...j]字符串有多少种删除方法
        1. 当必须不保留i号字符时，依赖前i-1个字符删成t[0..j]，即dp[i-1][j]
        2. 当想要保留i号字符时，必须满足s[i]==t[j]才能保留i号字符，此时依赖将s[0..i-1]删成t[0...j-1]，即dp[i-1][j-1]
        两个可能性相加
        */

        for (int i=1;i<len1;i++) {
            for (int j=1;j<len2;j++) {
                dp[i][j] = dp[i-1][j];
                if (s[i]==t[j]) {
                    dp[i][j] += dp[i-1][j-1];
                }
            }
        }
        
        // for (int i=0;i<len1;i++) {
        //     for (int j=0;j<len2;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout<<endl;
        return dp[len1-1][len2-1];
   } 
};

int main()
{

    Solution sol;

    string s = "rabbbit";
    string t = "rabbit";
    cout << sol.get_num(s, t) << endl;

    return 0;
}