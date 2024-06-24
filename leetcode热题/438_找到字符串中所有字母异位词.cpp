/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-24 10:53:47
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
#include <list>

using namespace std;

/*

https://leetcode.cn/problems/find-all-anagrams-in-a-string/description/
给定两个字符串 s 和 p，找到 s 中所有 p 的 异位词 的子串，返回这些子串的起始索引。不考虑答案输出的顺序。

异位词 指由相同字母重排列形成的字符串（包括相同的字符串）。



示例 1:

输入: s = "cbaebabacd", p = "abc"
输出: [0,6]
解释:
起始索引等于 0 的子串是 "cba", 它是 "abc" 的异位词。
起始索引等于 6 的子串是 "bac", 它是 "abc" 的异位词。
 示例 2:

输入: s = "abab", p = "ab"
输出: [0,1,2]
解释:
起始索引等于 0 的子串是 "ab", 它是 "ab" 的异位词。
起始索引等于 1 的子串是 "ba", 它是 "ab" 的异位词。
起始索引等于 2 的子串是 "ab", 它是 "ab" 的异位词。


提示:

1 <= s.length, p.length <= 3 * 104
s 和 p 仅包含小写字母
*/

/*
方法一：ac自动机
将p的所有异位词字符串，建立ac自动机，然后遍历一遍s，可以匹配得到哪些p的异位词，就可以得到。
建立ac自动机O(k*2^k)，遍历s串O(N)。总复杂度O(N*k*2^k)
这种方法适合s很大，但p比较小的时候，p较大的时候，2^k成指数级别增长，会超时。

方法二：滑动窗口
1. p异位词的字母频率一定和p相同，记录p中出现字母数。
2. 假设p的大小是k，则在s上使用一个长度为k的滑动窗口；
3. 维护一个窗口字母频率数组，每次右滑的时候变化数组，并和p的频率数组判断是否相等
4. 往右滑动n-k次，每次窗口比较k次，总复杂度是O(N*K)

*/

class Node
{
public:
    string end;           // 以当前节点结尾，这条线路的字符串
    bool end_used;        // 标记是否已经找到过这个敏感词。如果是end节点，并且没有被收集过答案了是true。否则是false
    Node *fail;           // fail指针，，匹配失败时，往上找最佳的和当前后缀字符串相等的前缀字符串的结束节点
    vector<Node *> nexts; // 经典前缀树next数组

    Node()
    {
        end = "";
        end_used = false;
        fail = nullptr;
        nexts.resize(26, nullptr);
    }
};
class ACAutomation
{
public:
    Node *root;
    ACAutomation()
    {
        root = new Node();
    }

    void insert(string &str)
    {
        int len = str.length();
        Node *cur = root;
        for (int i = 0; i < len; i++)
        {
            int next_char = str[i] - 'a';
            if (cur->nexts[next_char] == nullptr)
            {
                cur->nexts[next_char] = new Node();
            }
            cur = cur->nexts[next_char];
        }
        cur->end = str; // 尾结点，记录这条线路的字符串
    }

    // 链接所有fail指针，使用bfs
    // 人为规定root的fail指针指向空。
    // root的下一级节点都指向自己
    void build()
    {
        queue<Node *> que;
        que.push(root);
        while (!que.empty())
        {
            Node *cur = que.front();
            que.pop();
            // 遍历nexts数组。当前节点设置自己孩子的fail指针
            for (int i = 0; i < 26; i++)
            {
                if (cur->nexts[i] != nullptr)
                {
                    cur->nexts[i]->fail = root; // 先指向root。后续如果有其他情况，再修改
                    Node *cur_fail = cur->fail;
                    while (cur_fail != nullptr)
                    {
                        // 父节点的fail链，走到的节点的下一条路有到i的，则链接上
                        // 间接判断了fail指针指向的下一个节点是否等于i(字符)
                        if (cur_fail->nexts[i] != nullptr)
                        {
                            cur->nexts[i]->fail = cur_fail->nexts[i];
                            break; // 贪心，找到最长的前缀，就跳出
                        }
                        cur_fail = cur_fail->fail; // 继续往下一个fail指针指向的节点走
                    }
                    // cout << "cur fail:" << cur->nexts[i]->fail << " root:" << root << endl;
                    que.push(cur->nexts[i]); // 当前子节点入队列
                }
            }
        }
    }

    // 查询文章中的敏感词：content
    void contain_words(string &content, vector<int> &res)
    {
        int len = content.length();
        if (len == 0)
        {
            return;
        }
        Node *cur = root; // 从root开始找
        // 遍历一遍文章
        for (int i = 0; i < len; i++)
        {
            int idx = content[i] - 'a';
            // 不能往下走，但是cur又不是根节点的情况，继续沿着fail走
            while (cur->nexts[idx] == nullptr && cur != root)
            {
                cur = cur->fail;
            }
            // 出了上面的while有两种可能
            // 现在来到的路径，是可以继续匹配的
            // 现在来到的节点已经是头节点了
            cur = cur->nexts[idx] != nullptr ? cur->nexts[idx] : root; // 能往下走继续往下走，否则来到root
            // cout << "i:" << i << " cur:" << cur << " root:" << root << endl;
            // cout << " cur_fail:" << cur->fail << endl;

            // 现在cur要么是走到了下级节点，要么就还是在root位置

            // 沿着fail过一圈答案
            Node *follow = cur;
            int len_tmp = 0;
            while (follow != root)
            {
                // if (follow->end_used)
                // {
                //     break;
                // }
                if (follow->end != "") {
                    // 当前在i位置，字符串长度是len=follow->end.length(), 所以开始位置是在i-len+1
                    len_tmp = follow->end.size();
                    res.push_back(i - len_tmp + 1);
                    follow->end_used = true;
                }
                follow = follow->fail;
            }
        }
    }
};

class Solution
{
public:
    vector<int> findAnagrams1(string s, string p)
    {
        int len_s = s.length();
        int len_p = p.length();
        if (len_p > len_s)
        {
            return vector<int>();
        }
        vector<int> s_cnt(26, 0), p_cnt(26, 0);
        for (int i = 0; i < len_p; i++)
        {
            p_cnt[p[i] - 'a']++;
        }

        for (int i = 0; i < len_p; i++)
        {
            s_cnt[s[i] - 'a']++;
        }
        vector<int> res;
        int r = 0, l = 0;
        for (r = len_p, l = 0; r < len_s; r++, l++)
        {
            if (s_cnt == p_cnt)
            {
                res.push_back(l);
            }
            s_cnt[s[r] - 'a']++;
            s_cnt[s[l] - 'a']--;
        }
        // 最后一个没有算
        if (s_cnt == p_cnt)
        {
            res.push_back(l);
        }
        return res;
    }

    vector<int> findAnagrams(string s, string p)
    {
        int len_s = s.length();
        int len_p = p.length();
        if (len_p > len_s)
        {
            return vector<int>();
        }
        ACAutomation aca;
        string path = "";
        vector<bool> visited(len_p, false);
        dfs(p, 0, path, visited, aca);
        aca.build();
        vector<int> res;
        aca.contain_words(s, res);
        return res;
    }
    // dfs建立ac自动机
    void dfs(string &p, int i, string &path, vector<bool>& visited, ACAutomation &aca)
    {
        int len = p.length();
        if (i > len)
        {
            return;
        }
        if (i == len)
        {
            cout << "insert:" << path << endl;
            aca.insert(path);
            return;
        }
        for (int k=0;k<len;k++) {
            if (!visited[k]) {
                visited[k] = true;
                path.push_back(p[k]);
                dfs(p, i + 1, path, visited, aca);
                path.pop_back();
                visited[k] = false;
            }   
        }
    }
};

int main()
{
    Solution sol;

    string s = "abab";
    string p = "ab";
    auto res = sol.findAnagrams(s, p); 
    cout << "res:";
    for (auto it:res){
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
