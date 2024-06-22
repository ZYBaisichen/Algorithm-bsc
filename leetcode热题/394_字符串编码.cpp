/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-22 16:06:16
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

https://leetcode.cn/problems/decode-string/description/

给定一个经过编码的字符串，返回它解码后的字符串。

编码规则为: k[encoded_string]，表示其中方括号内部的 encoded_string 正好重复 k 次。注意 k 保证为正整数。

你可以认为输入字符串总是有效的；输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。

此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数 k ，例如不会出现像 3a 或 2[4] 的输入。

 

示例 1：

输入：s = "3[a]2[bc]"
输出："aaabcbc"
示例 2：

输入：s = "3[a2[c]]"
输出："accaccacc"
示例 3：

输入：s = "2[abc]3[cd]ef"
输出："abcabccdcdcdef"
示例 4：

输入：s = "abc3[cd]xyz"
输出："abccdcdcdxyz"
 

提示：

1 <= s.length <= 30
s 由小写英文字母、数字和方括号 '[]' 组成
s 保证是一个 有效 的输入。
s 中所有整数的取值范围为 [1, 300] 
*/

/*
硬模拟写试试：
遇到数字，找到中括号里的内容。
没有遇到数字则直接加到结果中
*/

class Solution {
public:
    //s = "3[a]2[bc]"
    string decodeString(string s) {
        int i=0;
        string res="";
        while (i<s.length()) {
            res += f(s, i);
        }
        return res;
    }

    //从i开始，找到它最远能够到达的位置
    //如果是字符，则可以直接返回
    //如果是数字，需要递归数字之后的括号
    string f(string& s, int& i) {
        if (i>=s.length()) {
            return "";
        }
        string res;
        string cur_str;
        int count=0;
        int len = s.length();
        cout << "=====i:" << i << "=====" << endl;
        if (s[i] >= '1' && s[i] <= '9') {
            count = 0;
            //找完整的数字
            while (s[i] >= '0' && s[i] <= '9') {
                count = count*10 + s[i] - '0';
                i++;
            }
            i++; // 跳过[
            cout << "i:" << i << " count:" << count << endl;
            cur_str = "";
            while (s[i] != ']' && i<s.length()) { //找到自己的]
                if (s[i] >= '0' && s[i]<='9') {
                    cur_str += f(s, i); //遇到数字之后解决数字之后自己的[]
                } else {
                    cur_str += s[i++];
                }
                cout << "s[i] i:" << i << " cur_str:" << cur_str << endl;
            }
            i++;//跳过]
            cout << "next i:" << i <<" cur_str:" << cur_str << endl;
            for (int k=0;k<count;k++) {
                res+=cur_str;
            }
        } else {
            res += s[i];
            i++;
        }
        cout << "i:" << i << " res:" << res << endl;
        return res;
    }
};


int main()
{
    Solution sol;
    // string s = "2[abc]3[cd]ef";
    // string s = "3[a2[c]]";
    // string s = "2[ 2[y] pq 4[ 2[jk]e 1[f] ] ]ef";
    string s = "2[2[y]pq4[2[jk]e1[f]]]ef";
    // res="yypqjkjkefjkjkefjkjkefjkjkefyypqjkjkefjkjkefjkjkefjkjkefef"
    auto res = sol.decodeString(s);
    cout << "res:" << res << endl;
    return 0;
}
