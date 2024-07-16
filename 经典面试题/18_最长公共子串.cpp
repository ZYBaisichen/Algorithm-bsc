/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-21 22:35:01
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
class Solution
{
public:
    // 给定两个子串，输出最长公共子串。如果子串为空，则返回-1

    // 方法1: 空间复杂度O(n*m)
    string lcst1(string text1, string text2)
    {
        int len1 = text1.length();
        int len2 = text2.length();
        if (len1 == 0 || len2 == 0)
        {
            return 0;
        }

        // 一个样本做行一个样本做列
        // dp[i][j]的含义是必须以text1的i字符结尾，并且必须以text2的j字符结尾，的最长公共子串是多长
        /*
        1. 当text1[i] != text2时，dp[i][j]就是0
        2. 当text1[i] == text2时，dp[i][j]依赖dp[i-1][j-1]+1
        */
        vector<vector<int>> dp(len1, vector(len2, 0));
        int len = 0;
        int end = -1; // 下标值 ， 如果有多个长度一样的公共子串，可能就会随机返回一个
        for (int i = 0; i < len1; i++)
        { // 填第一列
            if (text2[0] == text1[i])
            {
                dp[i][0] = 1;
                end = i;
                len = max(len, 1);
            }
        }
        for (int j = 0; j < len2; j++)
        { // 填第一行
            if (text1[0] == text2[j])
            {
                dp[0][j] = 1;
                end = 0;
                len = max(len, 1);
            }
        }

        // 从上到下，从左到右
        for (int i = 1; i < len1; i++)
        {
            for (int j = 1; j < len2; j++)
            {
                if (text1[i] == text2[j])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    if (dp[i][j] > len)
                    {
                        len = dp[i][j];
                        end = i;
                    }
                }
            }
        }
        if (end == -1) {
            return "-1";
        } else {
            return text1.substr(end - len + 1, len);
        }
    }

    // 方法2: 空间压缩
    // 最优方法是后缀数组，求出来lcp之后，max(lcp[i]), st. 0<=i<len
    string lcst2(string text1, string text2) {
        //从方法1中观察到dp中每个格子只会依赖左上角格子，和自身值
        //如果可以从右上角开始，挨个斜线更新dp数组，那也能达到最终想要的最长值
        int len1 = text1.length();
        int len2 = text2.length();
        if (len1 == 0 || len2 == 0) {
            return "";
        }
        int len = 0;
        int end = -1;

        //row col初始在右上角位置
        int row = 0;
        int col = len2-1;

        while (row < len1) {
            int i = row;
            int j = col;
            int cur_dp_val = 0;
            while (i<len1 && j<len2) { //一条斜线往右下走
                if (text1[i] == text2[j]) {
                    cur_dp_val++; //使用左上角的值更新当前的值
                } else {
                    cur_dp_val = 0; //重新从0开始
                }
                if (cur_dp_val > len) {
                    len = cur_dp_val;
                    end = i;
                }
                i++;
                j++;
            }
            
            //枚举斜线起始点，往左到不能再往左了，往下
            if (col>0) {
                col--;
            } else {
                row++;
            }
        }

        if (end == -1) {
            return "-1";
        } else {
            return text1.substr(end - len + 1, len);
        }
    }
};

int main()
{
    Solution sol;
    string text1 = "abcde11112342323";
    string text2 = "abce11111112323";

    // string text1 = "a";
    // string text2 = "b";

    cout << sol.lcst1(text1, text2) << endl;
    cout << sol.lcst2(text1, text2) << endl;
    cout << "测试结束" << endl;
}