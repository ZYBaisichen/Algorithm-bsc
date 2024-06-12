/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-12 21:43:18
 * @LastEditors: baisichen baisichen@baidu.com
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
https://leetcode.cn/problems/minimum-window-substring/description/

给你一个字符串 s 、一个字符串 t 。返回 s 中涵盖 t 所有字符的最小子串。如果 s 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。

 

注意：

对于 t 中重复字符，我们寻找的子字符串中该字符数量必须不少于 t 中该字符数量。
如果 s 中存在这样的子串，我们保证它是唯一的答案。
 

示例 1：

输入：s = "ADOBECODEBANC", t = "ABC"
输出："BANC"
解释：最小覆盖子串 "BANC" 包含来自字符串 t 的 'A'、'B' 和 'C'。
示例 2：

输入：s = "a", t = "a"
输出："a"
解释：整个字符串 s 是最小覆盖子串。
示例 3:

输入: s = "a", t = "aa"
输出: ""
解释: t 中两个字符 'a' 均应包含在 s 的子串中，
因此没有符合条件的子字符串，返回空字符串。
 

提示：

m == s.length
n == t.length
1 <= m, n <= 105
s 和 t 由英文字母组成
 

进阶：你能设计一个在 o(m+n) 时间内解决此问题的算法吗？
*/


/*
先用滑动窗口和hash表来解：(非常坑的官方解给出的答案) （应该不是最优解，但也通过了）
1. 使用两个指针，分别指向滑动窗口的开始和结束位置，如果窗口内的字符串不能完全覆盖t字符串的所有字符，则右扩。
2. 右扩发现能覆盖t字符串的所有字符，则记录一次答案，长度和开始位置。
3. 找到一个答案后，左边指针往右缩，指导不能满足条件时，再往右扩，重复2步骤


*/


class Solution {
public:
    map<char, int> ori, cnt;

    bool check() {
        for (auto it:ori) { //挨个检查t的每个字符
            // cout << endl<<it.first << ", " << it.second << " cnt:" << cnt[it.first] << endl;
            if (cnt.find(it.first) != cnt.end()) {
                if (it.second > cnt[it.first]) {
                    // cout << "1123123" << endl;
                    return false;
                }
                
            } else {
                // cout << "333333" << endl;
                return false;
            }
        }
        return true;
    }
    string minWindow(string s, string t) {
        int lens = s.length();
        int lent = t.length();
        if (lent == 0) {
            return s;
        }
        
        //将t的字符放到ori中
        for (int i = 0;i<lent;i++) {
            if (ori.find(t[i]) != ori.end()) {
                ori[t[i]]++;
            } else {
                ori[t[i]] = 1;
            }
        }

        for (auto it:ori) {
            cout << "key:" << it.first << " val:" << it.second << endl;
        }

        int begin=0, len=INT_MAX;
        int l=0,r=0;
        cnt[s[0]] = 1;
        cout << "s[0]" << s[0] << endl;
        while (r<lens) {
            //窗口内的元素可以覆盖t
            bool check_ans = check();
            cout << "l:" << l << " r:" << r << " check_ans:" << check_ans << endl;
            if (check_ans && l<=r) {
                cnt[s[l]]--;
                if (len > r-l+1) {
                    begin = l;
                    len = r-l+1;
                }
                l++;
            } else {
                r++;
                if (cnt.find(s[r]) != cnt.end()) {
                    cnt[s[r]]++;
                } else {
                    cnt[s[r]] = 1;
                }
            }
        }
        cout << "begin:" << begin << " len:" << len << endl;
        if (len == INT_MAX) {
            return "";
        } else {
            return s.substr(begin, len);
        }
    }
};
int main()
{

    Solution sol;
    string s = "cabwefgewcwaefgcf";
    string t = "cae";
    cout << sol.minWindow(s, t) << endl;
    return 0;
}
