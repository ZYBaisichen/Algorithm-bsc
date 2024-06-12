/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-11 13:01:44
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
#include <unordered_map>

using namespace std;

/*
https://leetcode.cn/problems/edit-distance/
给你两个单词 word1 和 word2， 请返回将 word1 转换成 word2 所使用的最少操作数  。

你可以对一个单词进行如下三种操作：

插入一个字符
删除一个字符
替换一个字符
 

示例 1：

输入：word1 = "horse", word2 = "ros"
输出：3
解释：
horse -> rorse (将 'h' 替换为 'r')
rorse -> rose (删除 'r')
rose -> ros (删除 'e')
示例 2：

输入：word1 = "intention", word2 = "execution"
输出：5
解释：
intention -> inention (删除 't')
inention -> enention (将 'i' 替换为 'e')
enention -> exention (将 'n' 替换为 'x')
exention -> exection (将 'n' 替换为 'c')
exection -> execution (插入 'u')
 

提示：

0 <= word1.length, word2.length <= 500
word1 和 word2 由小写英文字母组成
*/


/*
和经典面试题27题一样，且是其的简化版本，即插入、删除、修改的代价都是1。这里再做精简写一遍

dp[i][j]代表str1[0...i-1]范换成str2[0...j-1]所需要的次数, 可能性有：长度是i和j
1. str1[i-1]完全不使用，需要一次删除str1[i-1]的操作，然后再加上将str1[0...i-2]转换成str2[0...j-1]的次数，即dp[i-1][j]
2. str2[j-1]安全不使用， 需要一次删除Str2[j-1]的操作，然后再加上将str1[0...i-1]转换成str2[0...j-2]的次数，即dp[i][j-1]
3. 将str1[i-1]和str2[j-1]都使用，需要将str1[0...i-2]转换到str2[0...j-2]，即dp[i-1][j-1]。
    再加上str1[i-1]转换成str2[j-1]的次数，可以是修改也可以是删除之后添加。但如果是删除之后添加就转换成了str1[0...i-2]转换成str2[0...j-1]的问题，是第1种情况。
    所以这里使用的操作是修改。
*/


class Solution {
public:
    int minDistance_dp(string word1, string word2) {
        int len1 = word1.length();
        int len2 = word2.length();
        if (len1 == 0) {
            return len2;
        }
        if (len2 == 0) {
            return len1;
        }

        vector<vector<int>> dp(len1+1, vector<int>(len2+1, 0));
        //第一行, 从0个字符到j个字符，都增加j次
        for (int j=1;j<=len2;j++) {
            dp[0][j] = j;
        }

        //第一列，从i个字符到0个字符，都减少了i次
        for (int i=1;i<=len1;i++) {
            dp[i][0] = i;
        }


        //依赖dp[i-1][j], dp[i][j-1], dp[i-1][j-1]
        for (int i=1;i<=len1;i++) {
            for (int j=1;j<=len2;j++) {
                dp[i][j] = min(dp[i-1][j] + 1, dp[i][j-1] + 1);
                if (word1[i-1] == word2[j-1]) {
                    dp[i][j] = min(dp[i][j], dp[i-1][j-1]);
                } else {
                    dp[i][j] = min(dp[i][j], dp[i-1][j-1]+1);
                }
            }
        }

        // // for (int i=0;i<=len1;i++) {
        // //     for (int j=0;j<=len2;j++) {
        // //         cout << dp[i][j] << " ";
        // //     }
        // //     cout << endl;
        // // }
        // cout << endl;
        return dp[len1][len2];
    }

    //空间压缩
    int minDistance(string word1, string word2) {
        int len1 = word1.length();
        int len2 = word2.length();
        if (len1 == 0) {
            return len2;
        }
        if (len2 == 0) {
            return len1;
        }

        vector<int> dp(len2+1, 0);
        //第一行, 从0个字符到j个字符，都增加j次
        for (int j=1;j<=len2;j++) {
            dp[j] = j;
        }

        //依赖dp[i-1][j], dp[i][j-1], dp[i-1][j-1]
        int pre = 0, tmp = 0;//记录左上角的值
        for (int i=1;i<=len1;i++) {
            pre = dp[0];
            dp[0] = i;//从i个字符到0个字符，删除i次
            for (int j=1;j<=len2;j++) {
                
                tmp = min(dp[j] + 1, dp[j-1] + 1);
                if (word1[i-1] == word2[j-1]) {
                    tmp = min(tmp, pre);
                } else {
                    tmp = min(tmp, pre+1);
                }
                pre = dp[j];
                dp[j] = tmp;
            }
        }

        return dp[len2];
    }
};
int main()
{

    Solution sol;
    string str1 = "pneumonoultramicroscopicsilicovolcanoconiosis";
    string str2 = "ultramicroscopically"; 
    cout << sol.minDistance(str1, str2) << endl;
    return 0;
}