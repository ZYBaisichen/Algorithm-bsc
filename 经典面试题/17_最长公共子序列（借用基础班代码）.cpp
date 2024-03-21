/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-21 20:58:54
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
using namespace std;
// 测试链接 : https://leetcode.cn/problems/longest-common-subsequence/
class Solution
{
public:
    // 一个样本做行一个样本做列。现将两个样本对应到二维数组中，再找他们的有意义
    // dp[i][j]代表text1的0...i和text2的0...j子串的最长公共子序列
    // 第一行text1[0]在text2中遇到相等时为1，后续也为1
    // 第一列text2[0]在text1中遇到相等时为1，后续也为1
    int longestCommonSubsequence(string text1, string text2)
    {
        int len1 = text1.length();
        int len2 = text2.length();
        vector<vector<int>> dp(len1, vector(len2, 0));
        for (int i = 0; i < len1; i++)
        {
            if (text2[0] == text1[i] || (i > 0 && dp[i - 1][0] == 1))
            {
                dp[i][0] = 1;
            }
        }
        for (int j = 0; j < len2; j++)
        {
            if (text1[0] == text2[j] || (j > 0 && dp[0][j - 1] == 1))
            {
                dp[0][j] = 1;
            }
        }

        //     for (int i=1; i<len1; i++) {
        //         for (int j=1; j<len2; j++) {
        //             cout << dp[i][j] << " ";
        //         }
        //         cout << endl;
        //    }
        // 普遍位置的可能性
        /*
            1. 不以text1[i]和text2[j]为结尾的公共子序列长度为dp[i-1][j-1]
            2. 以text1[i]结尾，但不以text2[j]结尾，公共子序列长度为dp[i][j-1]
            3. 不以text1[i]结尾，但以text2[j]结尾，公共子序列长度为dp[i-1][j]
            4. 子串以两个为结尾时(text1[i]==text2[j])，子序列长度为dp[i-1][j-1]
            依赖上方，左方，左上的值，但左方也依赖左上的值，所以第一种情况可以不用考虑。从上到下，从左到右填值
        */

        for (int i = 1; i < len1; i++)
        {
            for (int j = 1; j < len2; j++)
            {
                dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
                if (text1[i] == text2[j])
                {
                    dp[i][j] = max(dp[i][j], dp[i - 1][j - 1] + 1);
                }
            }
        }
        return dp[len1 - 1][len2 - 1];
    }

    // 空间压缩
    int longestCommonSubsequence2(string text1, string text2)
    {
        int len1 = text1.length();
        int len2 = text2.length();

        //     for (int i=1; i<len1; i++) {
        //         for (int j=1; j<len2; j++) {
        //             cout << dp[i][j] << " ";
        //         }
        //         cout << endl;
        //    }
        // 普遍位置的可能性
        // 分析发现，每个各自只会依赖左上角、左边、右边的值，所以可以使用一个数组做滚动更新，这里需要注意左边值更新会覆盖左上角的值，所以需要有个临时变量存储左上角的值
        /*
            1. 不以text1[i]和text2[j]为结尾的公共子序列长度为dp[i-1][j-1]
            2. 以text1[i]结尾，但不以text2[j]结尾，公共子序列长度为dp[i][j-1]
            3. 不以text1[i]结尾，但以text2[j]结尾，公共子序列长度为dp[i-1][j]
            4. 子串以两个为结尾时(text1[i]==text2[j])，子序列长度为dp[i-1][j-1]
            依赖上方，左方，左上的值，但左方也依赖左上的值，所以第一种情况可以不用考虑。从上到下，从左到右填值
        */
        vector<int> dp(len2, 0);

        //更新第0行
        for (int j=0;j<len2;j++) {
            if (text1[0] == text2[j] || (j>0 && dp[j-1] == 1)) {
                dp[j] = 1;
            }
        }

        for (int i = 1; i < len1; i++) { //循环滚动i次
            int pre = dp[0]; //记录左上角的值
            //更新第一列的元素
            if (text2[0] == text1[i] || dp[0] ==1) {
                dp[0] = 1;
            }
            for (int j = 1; j < len2; j++) {
                
                int value = max(dp[j-1], dp[j]); // 左边和上边
                if (text1[i] == text2[j]) //滚到底i行了对应的text1元素为text1[i]
                {
                    value = max(value, pre + 1); //左上角+1
                }
                pre = dp[j]; //更新dp[j]之前，先保存pre，作为下一个元素的左上角位置
                dp[j] = value;
            }
        }
        return dp[len2-1];
    }
};

int main()
{
    Solution sol;
    string text1 = "abcde";
    string text2 = "ace";

    int ans0 = sol.longestCommonSubsequence(text1, text2);

    cout << "ans0:" << ans0 << endl;
    cout << sol.longestCommonSubsequence2(text1, text2) << endl;
    cout << "测试结束" << endl;
}