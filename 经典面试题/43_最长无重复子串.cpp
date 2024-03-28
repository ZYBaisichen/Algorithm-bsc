/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-28 18:08:28
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
给定一个字符串 s ，请你找出其中不含有重复字符的 最长
子串
 的长度。

 

示例 1:

输入: s = "abcabcbb"
输出: 3 
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
示例 2:

输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
示例 3:

输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
 

提示：

0 <= s.length <= 5 * 104
s 由英文字母、数字、符号和空格组成
*/

class Solution {
public:
    int lengthOfLongestSubstring1(string s) {
        unordered_map<char, int> map_tmp;
        int i=0,j=0;
        int res= 0;
        int len = s.length();
        while (j < len) {
            cout << "s[j]:" << s[j] << endl;
            if (map_tmp.find(s[j]) == map_tmp.end()) {
                map_tmp[s[j]] = j;
                res= max(res, j-i+1);   
                j++;
            } else {
                int old_i = i;
                i = map_tmp[s[j]]+1;
                for (int kk=old_i;kk<i;kk++) {
                    map_tmp.erase(s[kk]);
                }
            }
        }
        return res;
    }


    //dp
    /*
    dp[i]表示以i结尾时的最长无重复子串
    dp[0] = s[0]
    1. 记录上次s[i]字符出现的位置a，i位置最多推到上次出现的位置+1, 即a+1
    2. i-1字符往左推dp[i-1]长度，往左能推到i-dp[i-1]位置
    两个位置谁离i近就取谁，所以dp[i]=i-max(a+1, i-dp[i-1]])+1
    */
   int lengthOfLongestSubstring(string s) {
        int len = s.length();
        if (len==0) {
            return 0;
        }
        unordered_map<char, int> map_tmp;
        map_tmp[s[0]] = 0;
        int cur_dp = 1; //炫技一下，做个空间压缩
        int ans = 1;
        for (int i=1;i<len;i++) {
            int a = -1; //最多往左扩到开头
            if (map_tmp.find(s[i]) != map_tmp.end()) {
                a= map_tmp[s[i]];
            }

            int idx = a==-1?0:a+1;
            idx = max(idx, i-cur_dp);
            
            cur_dp = i - idx + 1; 
            ans = max(ans, cur_dp);//每填一个格子，抓一次答案
            map_tmp[s[i]] = i;
        }
        return ans;
    }
};

int main()
{

    Solution sol;
    // vector<int> arr = {2, 6, 4, 8, 10, 9, 15};
    string s = "abcabcbb";

    cout << sol.lengthOfLongestSubstring(s) << endl;
    return 0;
}