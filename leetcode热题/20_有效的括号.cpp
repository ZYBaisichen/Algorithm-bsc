/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-04 16:50:08
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

using namespace std;

/*
https://leetcode.cn/problems/valid-parentheses/description/
进阶是经典面试题中的2、3题。

给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。

有效字符串需满足：

左括号必须用相同类型的右括号闭合。
左括号必须以正确的顺序闭合。
每个右括号都有一个对应的相同类型的左括号。
 

示例 1：

输入：s = "()"
输出：true
示例 2：

输入：s = "()[]{}"
输出：true
示例 3：

输入：s = "(]"
输出：false
 

提示：

1 <= s.length <= 104
s 仅由括号 '()[]{}' 组成

*/

/*
可以使用栈来模拟，即左括号入栈，右括号来消除栈顶左括号，如果遇到一个不匹配的，则整个串不合法。

*/

class Solution
{
public:
    bool isValid(string s) {
        int len = s.length();
        if (len == 0) {
            return true;
        }
        map<char, char> _map = {
            {')','('},
            {']','['},
            {'}','{'}
        };
        stack<char> st;
        for (int i=0;i<len;i++) {
            if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
                st.push(s[i]); 
            } else {
                char need = _map[s[i]];
                if (st.empty()) {
                    return false;
                }
                if (need != st.top()) {
                    return false;
                } else {
                    st.pop();
                }
            }
        }
        if (!st.empty()) {
            return false;
        }
        return true;
    }
};
int main()
{

    Solution sol;
    string s = "[{]}";
    cout << sol.isValid(s)<<endl;

    return 0;
}