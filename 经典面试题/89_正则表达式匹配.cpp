/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-10 19:51:24
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
https://leetcode.cn/problems/regular-expression-matching/description/

给你一个字符串 s 和一个字符规律 p，请你来实现一个支持 '.' 和 '*' 的正则表达式匹配。

'.' 匹配任意单个字符
'*' 匹配零个或多个前面的那一个元素
所谓匹配，是要涵盖 整个 字符串 s的，而不是部分字符串。


示例 1：

输入：s = "aa", p = "a"
输出：false
解释："a" 无法匹配 "aa" 整个字符串。
示例 2:

输入：s = "aa", p = "a*"
输出：true
解释：因为 '*' 代表可以匹配零个或多个前面的那一个元素, 在这里前面的元素就是 'a'。因此，字符串 "aa" 可被视为 'a' 重复了一次。
示例 3：

输入：s = "ab", p = ".*"
输出：true
解释：".*" 表示可匹配零个或多个（'*'）任意字符（'.'）。


提示：

1 <= s.length <= 20
1 <= p.length <= 20
s 只包含从 a-z 的小写字母。
p 只包含从 a-z 的小写字母，以及字符 . 和 *。
保证每次出现字符 * 时，前面都匹配到有效的字符

*/

class Solution
{
public:
    // 正则表达式的
    bool is_valid(string s, string p)
    {
        int lens = s.length();
        int lenp = p.length();
        // if (lens == 0) {
        //     if (lenp==0) {
        //         return true;
        //     } else {
        //         for (int i=0;i<=lenp-2.;i+=2) { //必须是*号间隔
        //             if (!(p[i]!='*' && p[i+1]=='*')) {
        //                 return false;
        //             }
        //         }
        //     }
        // }
        // lens不为0，但lenp为0
        // if (lenp==0) {
        //     return false;
        // }

        for (int i = 0; i < lens; i++)
        {
            if (s[i] == '.' || s[i] == '*')
            {
                return false;
            }
        }

        // p中两个*不能相邻
        // if (p[0] == '*')
        // {
        //     return false;
        // }
        // for (int i = 0; i < lenp - 1; i++)
        // {
        //     if (p[i] == '*' && p[i + 1] == '*')
        //     {
        //         return false;
        //     }
        // }
        return true;
    }

    bool isMatch_baoli(string s, string p)
    {
        if (!is_valid(s, p))
        {
            // cout << "not valid" << endl;
            return false;
        }

        // cout << "is valid" << endl;
        vector<vector<bool>> dp(s.length() + 1, vector<bool>(p.length() + 1, false));
        bool ans = process1(s, p, 0, 0, dp);
        int lens = s.length();
        int lenp = p.length();

        cout << "baoli:" << endl;
        for (int i = 0; i <= lens; i++)
        {
            for (int j = 0; j <= lenp; j++)
            {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        return ans;
    }
    /*
    s[i....]能不能被p[j...]完全匹配出来
    典型的一个样本做行一个样本做列的尝试模型
    */
    bool process1(string s, string p, int i, int j, vector<vector<bool>> &dp)
    {
        int lens = s.length();
        int lenp = p.length();
        cout << "begin i:" << i << " j:" << j << " lens:" << lens << " lenp:" << lenp << endl;
        if (j == lenp)
        { // p来到了结尾，必须s也来到结尾才能匹配出来
            if (i == lens)
            {
                dp[i][j] = true;
                return true;
            }
            else
            {
                return false;
            }
        }

        // p没有来到结尾，但s来到了结尾
        if (i == lens)
        {
            int ans = j + 1 < lenp && p[j + 1] == '*' && process1(s, p, i, j + 2, dp);
            dp[i][j] = ans;
            return ans;
        }

        // s和p都没来到结尾
        // j+1不是*的情况，必须匹配上才能进入下面
        if (j == lenp - 1 || p[j + 1] != '*') {
            bool ans = (p[j] == '.' || s[i] == p[j]) && process1(s, p, i + 1, j + 1, dp);
            cout << "not* i:" << i << " j:" << j << " ans:" << ans << endl;
            dp[i][j] = ans;
            return ans;
        } else if (p[j] == '*') {
            int ans = process1(s, p, i, j + 2, dp);
            dp[i][j] = ans;
            return ans;
        } else {
            // p[j+1]是*的情况
            /*
            枚举行为:
            dp[i][j]依赖了:dp[i][j+2],dp[i+1][j+2],dp[i+2][j+2],dp[i+3][j+2]
            dp[i-1][j]依赖了dp[i-1][j+2],dp[i][j+2],dp[i+1][j+2]
            dp[i-1][j] = dp[i-1][j+2] || dp[i][j]
            dp[i][j] = dp[i][j+2]||dp[i+1][j]
            */
            bool p1 = false;
            int idx = i;
            while (idx != lens && (s[idx] == p[j] || p[j] == '.'))
            { // 逐个枚举，搞定几个s[i],0个，1个，2个。。。
                if (process1(s, p, idx, j + 2, dp))
                { // 搞定0个、1个、2个、3个。。。
                    dp[i][j] = true;
                    cout << "is* i:" << i << " j:" << j << " idx:" << idx << " true"<< endl;
                    return true;
                }
                idx++;
            }
            dp[i][j] = process1(s, p, idx, j + 2, dp);
            return dp[i][j];//最后一个, a*匹配，aaaa，idx=3时，while里调用的是使用了3个a时的情况，所以需要再补一个4个a的情况
        }
    }

    /*
    s[i....]能不能被p[j...]完全匹配出来
    典型的一个样本做行一个样本做列的尝试模型
    */
    bool process2(string s, string p, int i, int j, vector<vector<bool>> &dp)
    {
        int lens = s.length();
        int lenp = p.length();
        cout << "begin i:" << i << " j:" << j << " lens:" << lens << " lenp:" << lenp << endl;
        if (j == lenp)
        { // p来到了结尾，必须s也来到结尾才能匹配出来
            if (i == lens)
            {
                dp[i][j] = true;
                return true;
            }
            else
            {
                return false;
            }
        }

        // p没有来到结尾，但s来到了结尾
        if (i == lens)
        {
            int ans = j + 1 < lenp && p[j + 1] == '*' && process2(s, p, i, j + 2, dp);
            dp[i][j] = ans;
            return ans;
        }

        // s和p都没来到结尾
        // j+1不是*的情况，必须匹配上才能进入下面
        if (j == lenp - 1 || p[j + 1] != '*')
        {
            bool ans = (p[j] == '.' || s[i] == p[j]) && process2(s, p, i + 1, j + 1, dp);
            cout << "not* i:" << i << " j:" << j << " ans:" << ans << endl;
            dp[i][j] = ans;
            return ans;
        }
        else
        {
            // p[j+1]是*的情况
            bool p1 = process2(s, p, i, j + 2, dp); // 匹配0个字符
            cout << "is* i:" << i << " j:" << j << " p1:" << p1 << endl;
            bool p2 = (s[i] == p[j] || p[j] == '.') && process2(s, p, i + 1, j, dp);
            cout << "is* i:" << i << " j:" << j << " p2:" << p2 << endl;
            dp[i][j] = p1 || p2;
            return p1 || p2;
        }
    }

    bool isMatch_xielvyouhua(string s, string p)
    {
        if (!is_valid(s, p))
        {
            // cout << "not valid" << endl;
            return false;
        }

        // cout << "is valid" << endl;
        vector<vector<bool>> dp(s.length() + 1, vector<bool>(p.length() + 1, false));
        bool ans = process2(s, p, 0, 0, dp);
        int lens = s.length();
        int lenp = p.length();

        cout << "baoli:" << endl;
        for (int i = 0; i <= lens; i++)
        {
            for (int j = 0; j <= lenp; j++)
            {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        return ans;
    }

    /*
    s[i....]能不能被p[j...]完全匹配出来
    典型的一个样本做行一个样本做列的尝试模型
    */
    bool isMatch(string s, string p)
    {
        int lens = s.length();
        int lenp = p.length();
        if (!is_valid(s, p))
        {
            // cout << "not valid" << endl;
            return false;
        }

        vector<vector<bool>> dp(s.length() + 1, vector<bool>(p.length() + 1, false));

        // 最后一列，只有同时到达时才可能匹配上
        dp[lens][lenp] = true;
        // 最后一行
        for (int j = lenp - 1; j >= 1; j -= 2)
        {
            if (p[j] == '*' && p[j - 1] != '*')
            {
                dp[lens][j - 1] = true;
            }
            else
            {
                break;
            }
        }

        // i和j均依赖下一行和右一列
        for (int i = lens - 1; i >= 0; i--)
        {
            for (int j = lenp - 1; j >= 0; j--)
            {
                cout << "i:" << i << " j:" << j << endl;
                // 后面一个字符不是*的情况
                if (j == lenp - 1 || p[j + 1] != '*')
                {
                    dp[i][j] = (p[j] == '.' || s[i] == p[j]) && dp[i + 1][j + 1];
                }
                else
                {
                    // p[j+1]是*的情况
                    dp[i][j] = dp[i][j + 2];
                    dp[i][j] = dp[i][j] || ((s[i] == p[j] || p[j] == '.') && dp[i + 1][j]);
                }
            }
        }

        cout << "not baoli:" << endl;
        for (int i = 0; i <= lens; i++)
        {
            for (int j = 0; j <= lenp; j++)
            {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        return dp[0][0];
    }
};

int main()
{

    Solution sol;
    string s = "aaa";
    string p = "a*";
    cout << sol.isMatch_baoli(s, p) << endl;
    cout << sol.isMatch_xielvyouhua(s, p) << endl;
    cout << sol.isMatch(s, p) << endl;
    return 0;
}