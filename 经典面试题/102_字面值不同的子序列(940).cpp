/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-24 16:02:30
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
#include <sstream>

using namespace std;

/*
https://leetcode.cn/problems/distinct-subsequences-ii/description/
给定一个字符串 s，计算 s 的 不同非空子序列 的个数。因为结果可能很大，所以返回答案需要对 10^9 + 7 取余 。

字符串的 子序列 是经由原字符串删除一些（也可能不删除）字符但不改变剩余字符相对位置的一个新字符串。

例如，"ace" 是 "abcde" 的一个子序列，但 "aec" 不是。
 

示例 1：

输入：s = "abc"
输出：7
解释：7 个不同的子序列分别是 "a", "b", "c", "ab", "ac", "bc", 以及 "abc"。
示例 2：

输入：s = "aba"
输出：6
解释：6 个不同的子序列分别是 "a", "b", "ab", "ba", "aa" 以及 "aba"。
示例 3：

输入：s = "aaa"
输出：3
解释：3 个不同的子序列分别是 "a", "aa" 以及 "aaa"。
 

提示：

1 <= s.length <= 2000
s 仅由小写英文字母组成

*/

/*

遍历字符串，使用map记录，map['a']=value，代表以a字符结尾的子序列有多少个。另外记录一个all为当前产生了多少不同的子序列。初始时map为空
当无重复时，以abc举例：
    1. 到达a时，将新产生{a}一个子序列。即all=all+1=1个，map[a]=1
    2. 到达b时，将新产生{b},{ab}两个子序列。即all个子序列后面多加一个b都将产生一个新的子序列，单独加1个b也算。所以多产生了all+1个子序列。此时all=all+all+1=3,map[b]=all+1=2
    3. 到达c时，将新产生{c},{ac},{bc},{abc}三个子序列，即all+1=4个。map[c]=all=4;all=all+(all+1)=7
当有重复字符时，以aba举例：
    1. 到达a时，将新产生{a}一个子序列。即all=all+1=1个，map[a]=1
    2. 到达b时，将新产生{b},{ab}两个子序列。map[b]=all+1=2, all=all+map[b]=3
    3. 到达a时，将产生{aa},{ba},{aba}三个子序列，即all+1-map[a]=3个。
        理解是：前面的a已经产生了1个序列，所以当前的a到来时会产生和之前的a一样的1个序列，所以需要去重减去之前的map[a]
        此时all和map[a]的变化：map[a]=map[a]+(all+1-map[a])=4, all=all+(all+1-map[a])=6

遍历一遍可以求出来，O(N)复杂度

*/


typedef unsigned long long ll;
class Solution {
public:
    int distinctSubseqII(string s) {
        int len = s.length();
        if (len==0) {
            return 0;
        }
        vector<ll> _map(26, 0);
        ll all = 0;
        ll mod = 1e9+7;
        for (int i=0;i<len;i++) {
            int k = s[i]-'a';
            ll add_num = (all+1)%mod-_map[k];
            _map[k] = (_map[k] + add_num + mod) % mod;
            all = (all+ add_num + mod) % mod;
            // cout << all << endl;
        }
        return all;
    }
};

int main() {
    Solution sol;
    string str = "zchmliaqdgvwncfatcfivphddpzjkgyygueikthqzyeeiebczqbqhdytkoawkehkbizdmcnilcjjlpoeoqqoqpswtqdpvszfaksn";
    cout << "%lu" <<sol.distinctSubseqII(str) << endl;
    return 0;
}