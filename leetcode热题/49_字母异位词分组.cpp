/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-07 11:49:39
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
https://leetcode.cn/problems/group-anagrams/description/

给你一个字符串数组，请你将 字母异位词 组合在一起。可以按任意顺序返回结果列表。

字母异位词 是由重新排列源单词的所有字母得到的一个新单词。两个单词的各个字母的数量相同，只不过顺序不同



示例 1:

输入: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
输出: [["bat"],["nat","tan"],["ate","eat","tea"]]
示例 2:

输入: strs = [""]
输出: [[""]]
示例 3:

输入: strs = ["a"]
输出: [["a"]]


提示：
1 <= strs.length <= 104
0 <= strs[i].length <= 100
strs[i] 仅包含小写字母
*/

/*
hash表。
1. 异位词排序之后肯定是相同的值，所以可以用排序后的字符串做为key，统计所有的字符串作为一个value数组
2. 也可以不排序，统计每个字符串出现的字母数，然后按照字母顺序组织key，一样的效果。

*/
class Solution
{
public:
    vector<vector<string>> groupAnagrams(vector<string> &strs)
    {
        unordered_map<string, vector<string>> _map;
        for (auto it : strs)
        {
            string key = it;
            sort(key.begin(), key.end());
            _map[key].push_back(it);
        }
        vector<vector<string>> res;
        for (auto it : _map) {
            res.push_back(it.second);
        }
        return res;
    }
};
int main()
{

    Solution sol;
    vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
    auto res = sol.groupAnagrams(strs);
    cout << "res:" << endl;
    for (auto it : res) {
        for (auto it2 : it){
            cout << it2 << " ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}