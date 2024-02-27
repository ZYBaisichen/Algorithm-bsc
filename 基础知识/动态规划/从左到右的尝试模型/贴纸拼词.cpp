// 贴纸拼词
// 我们有 n 种不同的贴纸。每个贴纸上都有一个小写的英文单词。
// 您想要拼写出给定的字符串 target ，方法是从收集的贴纸中切割单个字母并重新排列它们
// 如果你愿意，你可以多次使用每个贴纸，每个贴纸的数量是无限的。
// 返回你需要拼出 target 的最小贴纸数量。如果任务不可能，则返回 -1
// 注意：在所有的测试用例中，所有的单词都是从 1000 个最常见的美国英语单词中随机选择的
// 并且 target 被选择为两个随机单词的连接。
// 测试链接 : https://leetcode.cn/problems/stickers-to-spell-word/

/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-02-27 23:34:26
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
    int minStickers(vector<string> &stickers, string target)
    {
        int len = stickers.size();
        if (len == 0)
        {
            return 0;
        }
        vector<vector<int>> stickers_map(len, vector(26, 0));
        for (int i = 0; i < len; i++)
        {
            string s = stickers[i];
            for (int j = 0; j < s.length(); j++)
            {
                stickers_map[i][s[j] - 'a']++;
            }
        }
        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<26;j++) {
        //         cout << stickers_map[i][j] << " ";
        //     }
        //     cout << endl;
        // }

        map<string, int> dp;
        dp[""] = 0;
        //这里只能做记忆化搜索了，因为string较为复杂
        return minStickersCore(target, stickers_map, dp);
    }
    // 还剩rest需要拼接
    // map为每个纸条的转换成词频的map
    // 返回拼接rest需要的最小纸条数量
    int minStickersCore(string rest, vector<vector<int>> &stickers_map, map<string, int> &dp)
    {
        // cout << "rest:" << rest << endl;
        if (rest.size() == 0)
        {
            return 0;
        }
        if (dp.find(rest) != dp.end())
        {
            return dp[rest];
        }

        vector<int> tmp(26, 0);
        for (int i = 0; i < rest.length(); i++)
        {
            tmp[rest[i] - 'a']++;
        }
        // cout << "tmp:" ;
        // for (int i=0;i<26;i++) {
        //     cout << tmp[i] << " ";
        // }
        // cout << endl;
        int ans = INT_MAX;
        for (int i = 0; i < stickers_map.size(); i++)
        {
            // 第一个字符必须在纸条中才开始，不然这个纸条没有意义，会无限循环
            // 另外最优解一定会解决rest的第一个字符，什么时候解决都是一样的
            if (stickers_map[i][rest[0] - 'a'] == 0)
            {
                continue;
            }
            // 枚举每一个纸条
            string res_tmp = "";
            for (int j = 0; j < 26; j++)
            {
                if (tmp[j] > 0)
                {
                    int after_resolves = max(tmp[j] - stickers_map[i][j], 0);
                    // cout << "after_resolves:" << after_resolves << " i:"<<i << " j:" << j <<endl;
                    for (int k = 0; k < after_resolves; k++)
                    {
                        res_tmp += (j + 'a');
                    }
                }
            }
            // cout << "res_tmp:" << res_tmp << endl;
            int ans_tmp = minStickersCore(res_tmp, stickers_map, dp);
            if (ans_tmp != -1)
            {
                ans = min(ans, 1 + ans_tmp);
            }
        }
        dp[rest] = ans == INT_MAX ? -1 : ans;
        return dp[rest];
    }
};

int main()
{
    Solution sol;
    // vector<int> arr = {90,100,1,7};
    vector<string> arr={"with","example","science"};
    string target = "thehat";
    int aim=1000;

    int ans0 = sol.minStickers(arr, target);

    cout << "ans0:" << ans0 << endl;
    cout << "测试结束" << endl;
}