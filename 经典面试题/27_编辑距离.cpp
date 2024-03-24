/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 23:05:28
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
// 编辑距离
// 给你两个单词 word1 和 word2
// 请返回将 word1 转换成 word2 所使用的最少代价
// 你可以对一个单词进行如下三种操作：
// 插入一个字符，代价a
// 删除一个字符，代价b
// 替换一个字符，代价c
// 测试链接 : https://leetcode.cn/problems/edit-distance/
class Solution {
public:
    int minDistance(string word1, string word2) {
        return edit_distance1(word1, word2, 1,1,1);
    }

    /*
    原始dp版
    ic: str1插入一个字符的代价
    dc: str1删除一个字符的代价
    rc: str1替换一个字符的代价
    */
    int edit_distance1(string str1, string str2, int ic, int dc, int rc) {
        int n = str1.length();
        int m = str2.length();
        if (n==0) {
            return m*ic;
        }
        if (m==0) {
            return n*dc;
        }
        //dp[i][j]代表，从str1前缀长度为i的字符串转换为str2前缀长度为j的字符串的最小代价是多少
        vector<vector<int>> dp(n+1, vector<int>(m+1,0)); 

        dp[0][0] = 0;
        //第一行，从str1 0个前缀转换成str2的前缀长度为j的字符串
        //str2有几个字符就插入几次
        for (int j=1;j<=m;j++) {
            dp[0][j] = j*ic;
        }
        //第一列。从长度为i的字符串转化为长度为0的字符串，有多几个字符就删除几次
        for(int i=1;i<=n;i++) {
            dp[i][0] = i*dc; 
        }


        /*
        普遍位置分类。dp[i][j]，代表是将str1[0...i-1]转换为str2[0...j-1]
        1. str1[i-1]要和str2的j-1对上，即str1[0...i-1]转换为str2[0..j-1]
            a. str1[i-1]==str2[j-1]时，直接copy，自身代价是0。总代价就是dp[i-1][j-1]
            b. str1[i-1]!=str2[j-1]时，需要将i-1替换成j-1。总代价是dp[i-1][j-1]+ic。这里需要注意，也可以先删除i-1，然后再插入，但和下面将长度为i-1的str1子字符串直接转化成长度为j的str2子字符串情况重合，这里就不加了
        2. str1[i-1]不和str2[j-1]对上：
            a. str1[0...i-1]转换成str2[0...j-2]，然后插入一个str2[j-1]。代价为dp[i][j-1]+ic
            b. str1[0...i-2]转化成str2[0...j-1]，然后删除str1[i-1]。代价为dp[i-1][j]+dc

        各类情况取最小，第一大类的a和b是互斥的。第二大类一定会遇上。最后取最小代价
        */

       for (int i=1;i<=n;i++){
            for (int j=1;j<=m;j++) {
                if (str1[i-1] == str2[j-1]) {
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    dp[i][j] = dp[i-1][j-1] + rc;
                }
                dp[i][j] = min(dp[i][j], dp[i][j-1]+ic);
                dp[i][j] = min(dp[i][j], dp[i-1][j]+dc);
            }
       }
    //    for (int i=0;i<=n;i++) {
    //     for (int j=0;j<=m;j++) {
    //         cout << " " << dp[i][j];
    //     }
    //     cout << endl;
    //    }
       cout << endl;
       return dp[n][m];
    }


    /*
    空间压缩
    */
    int edit_distance2(string str1, string str2, int ic, int dc, int rc) {
        int n = str1.length();
        int m = str2.length();
        if (n==0) {
            return m*ic;
        }
        if (m==0) {
            return n*dc;
        }
        //dp[i][j]代表，从str1前缀长度为i的字符串转换为str2前缀长度为j的字符串的最小代价是多少
        vector<int> dp(m+1, 0); 


        /*
        从方法一可以看出，每个格子只会依赖左边、上边，和左上角位置，所以使用一个数组做滚动更新。
        并提前记录好左上角的值。
        */
        //第一行，从str1 0个前缀转换成str2的前缀长度为j的字符串
        //str2有几个字符就插入几次
        for (int j=0;j<=m;j++) {
            dp[j] = j*ic;
        }
        int pre = dp[0];
        for (int i=1;i<=n;i++){ //滚动n次
            for (int j=0;j<=m;j++) {
                if (j==0) { //填第一个数，从str[0...i-1]更新到0长度的字符串需要的代价
                    pre = dp[0];//每次更新数组的值前先保存，作为右边的左上角的值
                    dp[0] = i*dc;
                    continue;
                }

                int tmp = INT_MAX;
                if (str1[i-1] == str2[j-1]) {
                    tmp = pre;
                } else {
                    tmp = pre + rc;
                }
                tmp = min(tmp, dp[j-1]+ic);
                tmp = min(tmp, dp[j]+dc);

                pre = dp[j]; //更新前先保留值，作为下一轮左上角的值
                dp[j] = tmp;
            }
        }
    //    for (int i=0;i<=n;i++) {
    //     for (int j=0;j<=m;j++) {
    //         cout << " " << dp[i][j];
    //     }
    //     cout << endl;
    //    }
    //    cout << endl;
       return dp[m];
    }

};

int main()
{
    Solution sol;
    vector<int> arr= {-2,1,-3,4,-1,2,1,-5,4};
    //输出6
    cout << sol.maxSubArray(arr) << endl;
    return 0;
}