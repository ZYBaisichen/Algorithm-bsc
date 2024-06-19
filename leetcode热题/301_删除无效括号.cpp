/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-20 00:26:47
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

https://leetcode.cn/problems/remove-invalid-parentheses/description/

给你一个由若干括号和字母组成的字符串 s ，删除最小数量的无效括号，使得输入的字符串有效。

返回所有可能的结果。答案可以按 任意顺序 返回。

 

示例 1：

输入：s = "()())()"
输出：["(())()","()()()"]
示例 2：

输入：s = "(a)())()"
输出：["(a())()","(a)()()"]
示例 3：

输入：s = ")("
输出：[""]
 

提示：

1 <= s.length <= 25
s 由小写英文字母以及括号 '(' 和 ')' 组成
s 中至多含 20 个括号

*/

/*
和经典面试题第2题的第2问很像。那个题问的是需要补充多少个字符可以让整个字符串合法。

一些思路：
删除多少可以使整个字符串变成有效，等价于保存最大长度的子序列，让整个字符串有效。

暴力：
    枚举所有的子序列，检查子序列是否有效。每个位置可以取也可以不取，复杂度O(2^n)。判断子序列是否有效，复杂度O(n)。总复杂度O(n*2^n)

递归设计
    //0...i-1已经决定了是否保存, 且当前的左括号的数量是cnt，往下递归枚举每个位置取或者不取，这个过程始终需要cnt>=0
    //顺利到达最后的位置, 如果此时cnt为0，说明产生了一个结果。
    process(string s, int i, int& cnt, string& path, vector<string>& res) {
    }


*/

class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        int len = s.length();
        if (len == 0) {
            return {""}; 
        }
        int cnt = 0;
        string path = "";
        set<string> res_set;
        res_set.insert("");
        process(s, 0, cnt, path, res_set);
        vector<string> res;
        for (auto it:res_set) {
            res.push_back(it);
        }
        return res;
    }
    //0...i-1已经决定了是否保存, 且当前的左括号的数量是cnt，往下递归枚举每个位置取或者不取，这个过程始终需要cnt>=0
    //顺利到达最后的位置, 如果此时cnt为0，说明产生了一个结果。
    void process(string& s, int i, int cnt, string& path, set<string>& res) {
        cout << "i:" << i << " s.length():" << s.length()<< " path:" << path << " cnt:" << cnt << endl;
        if (i>s.length()) {
            return;
        }
        if (i==s.length() && path.length() != 0) {
            cout << "res path:" << path << " cnt:" << cnt << endl;
            if (cnt==0) {
                if (path.length() > res.begin()->length()) {
                    cout << "res path:" << path << " res.begin():" << *res.begin() << endl;
                    res.clear();
                    cout << "res:" << res.size() << endl;
                    res.insert(path);
                } else if (path.length() == res.begin()->length()) {
                    res.insert(path);
                }
            }
        }

        //不放
        process(s, i+1, cnt, path, res);

        //可以决定放i也可以决定不放i， 左括号可以直接放
        if (s[i] == '(') {
            path += s[i];
            process(s, i+1, cnt + 1, path, res);
            path = path.substr(0, path.length()-1);
        } else if (s[i] == ')') { //右括号有条件放
            if (cnt - 1 >= 0) {
                path += s[i];
                process(s, i+1, cnt - 1, path, res);
                path = path.substr(0, path.length()-1);
            }
        } else { //其他字符直接放
            path += s[i];
            process(s, i+1, cnt, path, res);
            path = path.substr(0, path.length()-1);
        }
    } 

};
// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
int main()
{   
    Solution sol;
    string s = "()())()";
    auto res = sol.removeInvalidParentheses(s);
    cout << "res_size:" << res.size() << " res:" << endl;
    for (auto it:res) {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
