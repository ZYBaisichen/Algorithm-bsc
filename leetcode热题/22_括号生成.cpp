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
https://leetcode.cn/problems/generate-parentheses/description/
数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。

 

示例 1：

输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
示例 2：

输入：n = 1
输出：["()"]
 

提示：

1 <= n <= 8

*/

/*
暴力：
    n对括号，共2*n长度，枚举所有的2^(2n)的字符串，检查每个括号串是否有效。复杂度是2^(2n)*(2*n)

减枝：
    维护一个变量cnt，代表左边左括号的数量；来到i位置时，假设0...i-1已经准备好了，在i位置可以添加左括号也可以添加右括号。
    将要添加左括号时cnt++，添加右括号时cnt--， 如果cnt小于0，则不能添加右括号。


*/
class Solution {
public:
    vector<string> generateParenthesis1(int n) {
        if (n == 0) {
            return vector<string>();
        }

        string path="";
        vector<string> res;
        process1(n*2,0,0,path,res);
        return res;
    }

    void process1(int n, int i, int cnt, string& path, vector<string>& res) {
        if (i == n) {
            if (cnt == 0) {
                res.push_back(path);
            }
        }
        path += '('; //添加左括号
        process1(n, i+1, cnt+1, path, res);
        path = path.substr(0, path.length());
        //添加右括号
        if (cnt - 1 >= 0) {
            path += ')'; //添加左括号
            process1(n, i+1, cnt-1, path, res);
            path = path.substr(0, path.length());        
        }
    }


    //这个减枝剪的更彻底一点。
    //因为process1中需要枚举左括号到2*n长度才回溯，其实没有必要枚举到最后
    vector<string> generateParenthesis(int n) {
        if (n==0) {
            return {};
        }
        string path="";
        vector<string> res;
        process(n, 0, 0, path, res);
        return res;
    }

    void process(int n,int l,int r, string& path,vector<string>& res) {
        // cout << "===begin l:" << l  << " r:" << r << " path:" << path << " n:" << n << endl;
        if (r == n) {
            if (l==r) {
                res.push_back(path);
            }
            return;
        }
        if (l<n) {
            path += "(";
            process(n, l+1, r, path, res);
            path.pop_back();
        }
        if (r<l) {
            path += ")";
            // cout << "l:" << l << " r:" << r+1 << " path:" << path << endl;
            process(n, l, r+1, path, res);
            path.pop_back(); 
        }
    }
};
int main()
{

    Solution sol;
    string s = "[{]}";
    cout << sol.isValid(s)<<endl;

    return 0;
}