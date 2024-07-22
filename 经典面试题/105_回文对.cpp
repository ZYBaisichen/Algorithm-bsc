/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-25 12:37:45
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
#include <unordered_set>

using namespace std;

/*
https://leetcode.cn/problems/palindrome-pairs/description/
// 有关manacher的解释，看这个帖子 : https://www.mashibing.com/question/detail/56727

给定一个由唯一字符串构成的 0 索引 数组 words 。

回文对 是一对整数 (i, j) ，满足以下条件：

0 <= i, j < words.length，
i != j ，并且
words[i] + words[j]（两个字符串的连接）是一个
回文串
。
返回一个数组，它包含 words 中所有满足 回文对 条件的字符串。

你必须设计一个时间复杂度为 O(sum of words[i].length) 的算法。



示例 1：

输入：words = ["abcd","dcba","lls","s","sssll"]
输出：[[0,1],[1,0],[3,2],[2,4]]
解释：可拼接成的回文串为 ["dcbaabcd","abcddcba","slls","llssssll"]
示例 2：

输入：words = ["bat","tab","cat"]
输出：[[0,1],[1,0]]
解释：可拼接成的回文串为 ["battab","tabbat"]
示例 3：

输入：words = ["a",""]
输出：[[0,1],[1,0]]

提示：

1 <= words.length <= 5000
0 <= words[i].length <= 300
words[i] 由小写英文字母组成
*/

/*
暴力思想：
枚举每两对字符串，拼接到一起，看是否是回文串。枚举过程是O(N^2)，判断是否是回文串是O(k)。总复杂度O(N^2*k)


另一种角度: 将所有字符串放在map中，对每个字符串:
    1. 枚举其前缀字符串到i位置时，当前缀串[0..i]是回文串时。看map中是否有[i+1...len-1]的逆序，如果有，则将其补在前面整体就构成了回文串。得到一个答案
    2. 枚举其后缀串到i位置，当后缀串[i...len-1]是回文串时。看map中是否有[0...i-1]的逆序，如果有，则补齐到当前字符串后面，整体构成了回文串。得到一个答案
    3. 复杂度：枚举字符串是O(N)。其前缀串枚举是O(K)，每次需要判断前缀串是否是回文O(K)+逆序O(K)+在map中找O(K)。总复杂度是O(N*K*(3K))即O(N*k^2)
    4. 通过manacher算法求出来回文半径数组，可以将判断是否是回文优化成O(1), 总复杂度O(N*(K+K*(2*k)))=O(N*k^2)

判断前缀串是否是回文串的两种方法：
    1. 从前缀串的中点向两边遍历，复杂度O(K)
    2. 使用manacher算法求出回文半径数组，每次需要判断某个前后缀字符串是否是回文时，只需要找到该串的中点，判断是否能到边界即可。
*/


class Solution {
public:
    vector<vector<int>> palindromePairs(vector<string>& words) {
        int len = words.size();
        if (len == 0) {
            return vector<vector<int>>();
        }
        map<string, int> _map;
        for (int i=0;i<len;i++) {
            _map[words[i]] = i;
        }

        vector<vector<int>> res;
        std::set<std::pair<int, int>> unique_res_set;

        //针对每个字符串做判断
        for (int i=0;i<len;i++) {
            string word = words[i];
            string word_reverse = word;
            std::reverse(word_reverse.begin(), word_reverse.end());
            if (_map.find(word_reverse) != _map.end()) {//整体逆序
                if (i != _map[word_reverse]) {
                    unique_res_set.insert({i, _map[word_reverse]});
                    unique_res_set.insert({_map[word_reverse], i});
                }
            }
            //word本身是回文串
            if (_map.find("") != _map.end() && word_reverse == word && i != _map[""])
            {
                unique_res_set.insert({i, _map[""]});
                unique_res_set.insert({_map[""], i});
            }

            int w_len = word.length();
            vector<int> mc = manacher(word);
            int mc_len = mc.size();
            int mid = mc_len >> 1;
            cout << word << ":";
            for (auto iter:mc) {
                cout << iter << " ";
            }
            cout << endl;
            
            //枚举前缀串.
            //对于word[i]最多枚举到中点位置，才有可能包含边界。
            //不能包含mid，因为mid如果包含边界，就是整个字符串了，前面已经求过了
            //下面的j其实是在ss上枚举
            for (int j=1;j<mid;j++) { //[0...len-1]已经在上面求过了
                if (j - mc[j] == -1) { //左边刚好到边界
                    //从逆序串中截取出来需要补充的
                    /*
                    此时ss中可以产生的回文串长度是(mc[j]-1)*2， 回归到原字符串的长度就是mc[j]-1。
                    所以需要从逆序串中截取的长度为w_len-(mc[j]-1)]....(1)
                    而j-mc[j]==-1，所以j=mc[j]-1。化简(1)式的到需要截取的长度是w_len-j。注意这里的w_len和mid是相等的
                    */
                    string tmp = word_reverse.substr(0, w_len-j);
                    cout << "j:" << j  << " mc[j]:" << mc[j] << " tmp:" << tmp << endl;
                    if (_map.find(tmp) != _map.end()) {
                        unique_res_set.insert({_map[tmp], i});
                    } 
                }
            }

            //枚举后缀串   
            //在ss上只可能中mid+1位置才能扩展到最后，同样不包含mid
            for (int j=mid+1;j<mc_len-1;j++) { //[0...len-1]已经在上面求过了
                if (j+mc[j] == mc_len) { //右边刚好到边界
                    //从逆序串中截取出来需要补充的
                    /*
                    此时ss中可以产生的回文串长度是(mc[j]-1)*2+1， 回归到原字符串的长度就是mc[j]-1
                    所以需要从逆序串中截取的长度为w_len-(mc[j]-1)]....(2)

                    当前产生回文串的开头下标在ss中是，(j-mc[j])+1， 还原到s中的下标就是((j-mc[j])+1)/2。
                    在s的逆序字符串word_reverse中的下标就是w_len-1-((j-mc[j])+1)/2
                    所以从逆序字符串word_reverse中截取的位置就是w_len-1-((j-mc[j])+1)/2+1


                    */
                    int start = w_len-1-((j-mc[j])+1)/2+1;
                    string tmp = word_reverse.substr(start, w_len-(mc[j]-1));
                    cout << "houzhui j:" << j << " mc[j]:" << mc[j] << " word_reverse:" << word_reverse
                         << " start:" << start << " len:" << w_len - (mc[j] - 1) << " w_len:" << w_len << " tmp:" << tmp << endl;
                    if (_map.find(tmp) != _map.end()) {
                        unique_res_set.insert({i, _map[tmp]});//当前的字符串在前面
                    } 
                }
            }
            for (auto iter:unique_res_set) {
                cout << iter.first << ", " << iter.second<<endl;
            }
            // break;
        }
        for (auto iter:unique_res_set) {
            res.push_back({iter.first, iter.second});
            // cout << iter.first << ", " << iter.second<<endl;
        }
        return res;
    }

    //扩充字符串
    string expand(string& s) {
        string res = "#";
        int len = s.length();
        for (int i=0; i<len;i++) {
            res += s[i];
            res += '#';
        }
        return res;
    }

    //求出回文半径数组
    vector<int> manacher(string s) {
        string ss = expand(s);
        
        int ss_len = ss.length();
        vector<int> p(ss_len, 0);//以每个字符为中心的回文半径, 包括自身
        int r = -1; //最右回文串的右边界
        int c = -1; //最右回文串的中心点
        cout << "ss:" << ss << endl;

        for (int cur=0; cur<ss_len; cur++) {
            if (cur==0) {
                p[0] = 1;
                r = 0;
                c = 0;
            } else {
                if (cur > r) {
                    //暴力从cur往两边扩
                    int count = 1;
                    int l_tmp = cur-1;
                    int r_tmp = cur+1;
                    while (l_tmp >= 0 && r_tmp < ss_len && ss[l_tmp] == ss[r_tmp]) {
                        l_tmp--;
                        r_tmp++;
                        count++;
                    }
                    r = r_tmp-1;
                    c = cur;
                    p[cur] = count;
                } else {
                    int cur_tmp = c - (cur - c); //相对于c对称的位置
                    int l = c - (r-c); //c为中心点的回文串的左边界
                    int l_cur_tmp = cur_tmp-(p[cur_tmp]-1);//cur_tmp位置的左边界
                    if (l_cur_tmp > l) {
                        p[cur] = p[cur_tmp];
                    } else if (l_cur_tmp < l) { //最多扩到右边界
                        p[cur] = r - cur + 1;
                    } else {
                        int count = r - cur +1;
                        int l_tmp = cur - (count-1) -1;
                        int r_tmp = r+1;
                        while (l_tmp >= 0 && r_tmp < ss_len && ss[l_tmp] == ss[r_tmp]) {
                            l_tmp--;
                            r_tmp++;
                            count++;
                        }
                        r = r_tmp - 1;
                        c = cur;
                        p[cur] = count;
                    }

                }
            }
        }

        return p;
    }
};

int main() {
    Solution sol;
    /*
    输入：words = ["abcd","dcba","lls","s","sssll"]
    输出：[[0,1],[1,0],[3,2],[2,4]]
    解释：可拼接成的回文串为 ["dcbaabcd","abcddcba","slls","llssssll"]
    */
//    vector<string> words = {"abcd","dcba","lls","s","sssll"};
   vector<string> words = {"aa",""};
    vector<vector<int>> res = sol.palindromePairs(words);
    for (auto& iter:res) {
        cout << iter[0] << " " << iter[1] << endl;
    }
    cout << endl;
    return 0;
}