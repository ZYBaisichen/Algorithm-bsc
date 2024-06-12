/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-12 22:08:44
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

*/

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return vector<vector<int>>();
        }
        sort(nums.begin(), nums.end());
        cout << "nums:";
        for (auto it:nums) {
            cout << it << " ";
        }
        cout << endl;
        vector<vector<int>> res;
        res.push_back({});
        vector<int> path;
        
        for (int i=1;i<=len;i++) {
            path.clear();
            dfs(nums, i, 0, 1, path, res);
        }
        return res;
    }
    //当前来到了第cur位置，还没有加上cur位置的数，不会往前枚举
    //需要n个数
    //枚举到了第level层，沿途路径记录在path中
    void dfs(vector<int>& nums, int n, int cur, int level, vector<int>& path, vector<vector<int>>& res) {
        cout << "n:" << n << " cur:" << cur << " level:" << level << endl;
        cout << "begin path:";
        for (auto it:path) {
            cout << it << " ";
        }
        cout << endl; 
        if (level == n+1 && path.size() == n) {
            cout << "level:"<< level<<" path:";
            for (auto it:path) {
                cout << it << " ";
            }
            cout << endl;
            res.push_back(path);
            return;
        }
        int len = nums.size();
        for (int i=cur;i<nums.size();i++) {
            path.push_back(nums[i]);
            dfs(nums, n, i+1, level+1, path, res);
            path.pop_back();
        }
    }
};
int main()
{

    Solution sol;
    vector<int> nums = {1,2,3};
    auto res = sol.subsets(nums);
    cout << "res:"<< endl;
    for (auto it:res) {
        for(auto it2:it) {
            cout << it2<<" ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}
