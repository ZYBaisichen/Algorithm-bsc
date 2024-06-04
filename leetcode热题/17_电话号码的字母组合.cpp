/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-04 13:13:09
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
https://leetcode.cn/problems/letter-combinations-of-a-phone-number/description/
给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。

给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。



 

示例 1：

输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
示例 2：

输入：digits = ""
输出：[]
示例 3：

输入：digits = "2"
输出：["a","b","c"]
 

提示：

0 <= digits.length <= 4
digits[i] 是范围 ['2', '9'] 的一个数字。
*/

/*
先来个暴力枚举，再优化
dfs枚举digits，每个数字对应3到4种可能
base case， 枚举到最后一个数字时，结束。结果放在path里，每次往下一层回来后需要恢复现场



dfs中path和i是可变参数，但path的情况较多，使用记忆化搜索或者动规都不太可行。是否可以考虑减枝呢？
因为是顺序出现的，比如234，234的枚举路径不会出现重复，从另一个角度想，总答案的个数是3^m*4^n， m是字母数是3的个数，n是字母数是4的个数。需要遍历一遍拿到所有答案，dfs回溯就是最优的了。
考虑到当数字出现234时，
*/
class Solution
{
public:
    vector<string> letterCombinations(string digits) {
        int len = digits.length();
        if (len == 0) {
            return vector<string>();
        }
        set<string> res_set; //使用一个set，去重
        map<char, string> digit_map = {
            {'2', "abc"},
            {'3', "def"},
            {'4', "ghi"},
            {'5', "jkl"}, 
            {'6', "mno"},
            {'7', "pqrs"},
            {'8', "tuv"},
            {'9', "wxyz"}
        };
        string path = "";
        process(digits, 0, path, res_set, digit_map);
        vector<string> res;
        for (auto it:res_set) {
            res.push_back(it);
        }
        return res;
    }
    void process(string& digits, int i, string& path, set<string>& res, map<char, string>& digit_map) {
        int len = digits.length();
        if (i == len) {
            //结算path
            res.insert(path);
            return;
        }
        string cur_str = digit_map[digits[i]];
        for (int k=0;k<cur_str.length();k++) {
            path += cur_str[k];
            process(digits, i+1, path, res, digit_map);
            path = path.substr(0, path.length()-1);
        }
    }
};

int main()
{

    Solution sol;
    string s = "2";
    vector<string> res =  sol.letterCombinations(s);
    for (auto it:res) {
        cout << it << " ";
    }
    cout << endl;

    return 0;
}