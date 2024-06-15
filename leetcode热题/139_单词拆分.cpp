/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
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
https://leetcode.cn/problems/word-break/
给你一个字符串 s 和一个字符串列表 wordDict 作为字典。如果可以利用字典中出现的一个或多个单词拼接出 s 则返回 true。

注意：不要求字典中出现的单词全部都使用，并且字典中的单词可以重复使用。

 

示例 1：

输入: s = "leetcode", wordDict = ["leet", "code"]
输出: true
解释: 返回 true 因为 "leetcode" 可以由 "leet" 和 "code" 拼接成。
示例 2：

输入: s = "applepenapple", wordDict = ["apple", "pen"]
输出: true
解释: 返回 true 因为 "applepenapple" 可以由 "apple" "pen" "apple" 拼接成。
     注意，你可以重复使用字典中的单词。
示例 3：

输入: s = "catsandog", wordDict = ["cats", "dog", "sand", "and", "cat"]
输出: false
 

提示：

1 <= s.length <= 300
1 <= wordDict.length <= 1000
1 <= wordDict[i].length <= 20
s 和 wordDict[i] 仅由小写英文字母组成
wordDict 中的所有字符串 互不相同
*/

/*
*/

class Solution
{
public:
    bool wordBreak_1(string s, vector<string>& wordDict) {
        return process(s, wordDict, 0);
    }

    /*
    0...idx已经被解决，idx...len是否可以被字典中的词解决
    */
    bool process(string& s, vector<string>& wordDict, int idx) {
        int len = s.length();
        if (idx>=len) {
            return true;
        }
        bool ans = false;

        //因为每个词都可以用无数次，所以这里每个都要枚举
        for (int i=0;i<wordDict.size();i++) {
            string& tmp = wordDict[i];
            bool flag=true;
            int kk=idx,k=0;
            //检查是否可以解决idx开头的前缀
            for (;k<tmp.size() && kk<len;k++,kk++) {
                if (tmp[k] != s[kk]) {
                    break;
                }
            }
            // cout << "idx:" << idx << " kk:" << kk << " flag:" << flag  << " tmp:" << tmp<< endl;
            //表示可以来到结尾
            if (k == tmp.size())  {
                ans = ans || process(s, wordDict, kk);
            }
        }

        return ans;
    }


    bool wordBreak_dp(string s, vector<string>& wordDict) {
        int len = s.length();
        if (len == 0) {
            return true;
        }

        //dp[i]表示i...len是否可以用wordDict中的词拼接出来
        vector<bool> dp(len+1, false);

        dp[len] = true;

        for (int idx=len-1;idx>=0;idx--) {
            //因为每个词都可以用无数次，所以这里每个都要枚举
            for (int i=0;i<wordDict.size();i++) {
                string& tmp = wordDict[i];
                bool flag=true;
                int kk=idx,k=0;
                //检查是否可以解决idx开头的前缀
                for (;k<tmp.size() && kk<len;k++,kk++) {
                    if (tmp[k] != s[kk]) {
                        break;
                    }
                }
                cout << "idx:" << idx << " kk:" << kk << " flag:" << flag  << " tmp:" << tmp<< endl;
                //表示可以来到结尾
                if (k == tmp.size())  {
                    dp[idx] = dp[idx] || dp[kk];
                }
            }
        }
        return dp[0];
    }


    //dp优化
    bool wordBreak(string s, vector<string>& wordDict) {
        int len = s.length();
        if (len == 0) {
            return true;
        }

        //dp[i]表示i...len是否可以用wordDict中的词拼接出来
        vector<bool> dp(len+1, false);
        unordered_set<string> words_set;
        for (auto it:wordDict) {
            words_set.insert(it);
        }

        dp[len] = true;

        for (int idx=len-1;idx>=0;idx--) {
            for (int end=idx;end<len;end++) { //枚举idx...end能不能解决
                if (dp[end+1] && words_set.find(s.substr(idx, end-idx+1)) != words_set.end()) {
                    dp[idx] = true;
                    break;
                }
            }
        }
        return dp[0];
    }
};

int main()
{

    Solution sol;
    return 0;
}
