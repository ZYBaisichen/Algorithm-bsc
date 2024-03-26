/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-26 22:39:17
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
给你一个字符串 s，请你将 s 分割成一些子串，使每个子串都是
回文串
。

返回符合要求的 最少分割次数 。



示例 1：

输入：s = "aab"
输出：1
解释：只需一次分割就可将 s 分割成 ["aa","b"] 这样两个回文子串。
示例 2：

输入：s = "a"
输出：0
示例 3：

输入：s = "ab"
输出：1


提示：

1 <= s.length <= 2000
s 仅由小写英文字母组成



*/

// 本题测试链接 : https://leetcode.com/problems/palindrome-partitioning-ii/

/*


*/
class Solution
{
public:
    /*
        从i开始到结束，能切出来多少回文串.[0...i-1]已经划分好了
        1. 前缀只有[i]时，递归调用f(s, i+1)
        2. 前缀只有[i,i+1]时，递归调用f(s, i+2) //前提是[i..i+1]是回文串
        3. ....
        4. 尝试每一种前缀为回文串的分隔方法
    */
    // O(N^3)
    //  int f(string s, int i) {
    //      if (i==s.length()) {
    //          return 0;
    //      }
    //      //end
    //      int ans = 0;
    //      for (int end=i;end<s.length();end++) {
    //          if (is_p(s.substr(i, end-i+1))) { //判断i...end是回文串
    //              ans = min(ans, f(s, end+1)+1);
    //          }
    //      }
    //  }
    //  //O(N)
    //  //如果能生成一个字符串中任意两个范围是否是回文串的预处理数组，就可以在O(1)拿到一个字符串是否是回文串
    //  bool is_p(string s) {
    //      int i=0, j=s.length();
    //      while (i<j) {
    //          if (s[i] != s[j]) {
    //              return false;
    //          }
    //      }
    //      return true;
    //  }

    /*
    一、根据f函数分析，生成字符串s的任意范围上是否是回文串的预处理结构，范围上尝试模型。dp[i][j]表示i..j范围上的子字符串是否是回文串
        1. 对角线dp[i][i] 都是回文串
        2. 只有当s[i]==s[j]时，依赖dp[i+1][j-1]的值

    */
    int minCut(string s)
    {
        int len = s.length();
        if (len == 0)
        {
            return 0;
        }
        if (len == 1)
        {
            return 0;
        }

        // 预处理
        vector<vector<bool>> isP(len, vector<bool>(len, false));
        for (int i = 0; i < len; i++)
        {
            isP[i][i] = true;
        }

        // isP[i][j]依赖左下角的值，所以需要从下往上，从左往右填
        // 因为[len-2][len-1]依赖[len-1][len-2]的值，但还没有填，所以再填一个斜线
        // 填[i][i+1]行，因为只有两个字符，所以相等就是回文串
        for (int i=0;i<len-1;i++) {
            isP[i][i+1] = s[i] == s[i+1];
        }
        // 因为对角线，和再往上一个对角线都填好了。
        // 所以i从len-3开始，到0；
        // 因为j>=i才有意义，因为两个对角线都填好了，所以j从i+2到len-1
        for (int i = len - 3; i >= 0; i--)
        {
            for (int j = i + 2; j < len; j++)
            {
                isP[i][j] = (s[i] == s[j]) && isP[i + 1][j - 1];
            }
        }

        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<len;j++) {
        //         cout << isP[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // return 0;

        // 根据f改动态规划
        // dp[i]表示从i...end能切出来多少回文串
        // f[i]依赖f[i+1]到f[len-1]，所以从后往前填
        // 如果i...end是回文串时，dp[i] = min(dp[i], dp[end+1])
        vector<int> dp(len + 1, 1999); // 字符串长度最长是2000，所以最少分隔成1999次
        dp[len] = 0;                   // 最后一个字符不需要分隔

        for (int i = len - 1; i >= 0; i--)
        {
            for (int end = i; end < len; end++)
            {
                if (isP[i][end])
                {
                    dp[i] = min(dp[i], dp[end + 1]+1);
                }
            }
        }

        return dp[0]-1; //切出来dp[0]段，分隔数需要减一
    }
};

int main()
{

    Solution sol;
    string s = "aab";
    cout << sol.minCut(s) << endl;

    // cout << s.mox_xor_zero2(nums) << endl;
    // cout << s.max_xor2(nums) << endl;
    // cout << s.max_xor3(nums) << endl;

    return 0;
}