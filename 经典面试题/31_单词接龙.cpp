/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-25 20:14:19
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

using namespace std;

// 单词接龙 II
// 按字典 wordList 完成从单词 beginWord 到单词 endWord 转化
// 一个表示此过程的 转换序列 是形式上像
// beginWord -> s1 -> s2 -> ... -> sk 这样的单词序列，并满足：
// 每对相邻的单词之间仅有单个字母不同
// 转换过程中的每个单词 si（1 <= i <= k）必须是字典 wordList 中的单词
// 注意，beginWord 不必是字典 wordList 中的单词
// sk == endWord
// 给你两个单词 beginWord 和 endWord ，以及一个字典 wordList
// 请你找出并返回所有从 beginWord 到 endWord 的 最短转换序列
// 如果不存在这样的转换序列，返回一个空列表
// 每个序列都应该以单词列表 [beginWord, s1, s2, ..., sk] 的形式返回
// 测试链接 : https://leetcode.cn/problems/word-ladder-ii/
/*
按字典 wordList 完成从单词 beginWord 到单词 endWord 转化，一个表示此过程的 转换序列 是形式上像 beginWord -> s1 -> s2 -> ... -> sk 这样的单词序列，并满足：

每对相邻的单词之间仅有单个字母不同。
转换过程中的每个单词 si（1 <= i <= k）必须是字典 wordList 中的单词。注意，beginWord 不必是字典 wordList 中的单词。
sk == endWord
给你两个单词 beginWord 和 endWord ，以及一个字典 wordList 。请你找出并返回所有从 beginWord 到 endWord 的 最短转换序列 ，如果不存在这样的转换序列，返回一个空列表。每个序列都应该以单词列表 [beginWord, s1, s2, ..., sk] 的形式返回。

 

示例 1：

输入：beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
输出：[["hit","hot","dot","dog","cog"],["hit","hot","lot","log","cog"]]
解释：存在 2 种最短的转换序列：
"hit" -> "hot" -> "dot" -> "dog" -> "cog"
"hit" -> "hot" -> "lot" -> "log" -> "cog"
示例 2：

输入：beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log"]
输出：[]
解释：endWord "cog" 不在字典 wordList 中，所以不存在符合要求的转换序列。
 

提示：

1 <= beginWord.length <= 5
endWord.length == beginWord.length
1 <= wordList.length <= 500
wordList[i].length == beginWord.length
beginWord、endWord 和 wordList[i] 由小写英文字母组成
beginWord != endWord
wordList 中的所有单词 互不相同
*/

/*
首先需要生成一个表：这个表是wordList中如果两个单词可以只变一个字符走到，就认为两个单词之间有一条路径。每两个字符串挨个比较是O(N^2)复杂度的。 下面需要优化：
    1. list先放到一个hash set中
    2. 对于一个字符串"abc"，每一个位置都枚举26个字母(除了原来的字母)，看在set中是否出现，如果出现，则这个字符串到这个变体就有一条边。
        a. 字符串长度为k的话，则候选字符串是25*k，hash表找长度为k的字符串的复杂度是O(K)的。所以找到一个字符串"abc"邻居字符串的总体复杂度是O(K^2)的
        b. 最终建立每个字符串的临接表
    3. bfs找到start字符串到每个字符串的最短距离，存到一个map中。到end字符串，最短距离其实就可以求出来了
    4. 如果需要拿到所有变换的结果，则需要dfs遍历。每次往下遍历需要严格限制逐层递增的遍历。



*/
class Solution
{
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string> &wordList)
    {
        vector<vector<string>> res;
        // 先生成邻居列表
        set<string> dict;
        for (auto iter : wordList) {
            dict.insert(iter);
        }

        vector<string> wordListTmp = wordList;
        if (dict.find(beginWord) == dict.end()) {
            wordListTmp.push_back(beginWord);
            dict.insert(beginWord);
        }

        map<string, vector<string>> nexts; // 临接表
        for (auto iter : wordListTmp)
        { // O(25*K^2)
            if (nexts.find(iter) == nexts.end())
            {
                nexts[iter] = vector<string>();
            }
            string str_modify = iter;
            // cout << "str_modify:" << str_modify << endl;
            for (int i = 0; i < str_modify.length(); i++)
            {
                char old = str_modify[i];
                for (char ch = 'a'; ch <= 'z'; ch++)
                {
                    if (ch == old)
                    {
                        continue;
                    }
                    str_modify[i] = ch;
                    if (dict.find(str_modify) != dict.end())
                    {
                        // cout << "str_modify_push_back:" << str_modify << endl;
                        nexts[iter].push_back(str_modify);
                    }
                }
                str_modify[i] = old; // 恢复
            }
        }

        // for (auto iter : nexts)
        // { // 打印临接表
        //     cout << iter.first << "->";
        //     for (auto str_next : iter.second)
        //     {
        //         cout << str_next << ", ";
        //     }
        //     cout << endl;
        // }

        // 对邻接表进行宽度优先遍历，构建各个字符串到start word的最短距离是多少
        map<string, int> from_distance_map;
        bfs(beginWord, wordList, nexts, from_distance_map);

        map<string, int> to_distance_map;
        bfs(endWord, wordList, nexts, to_distance_map);

        

        // for (auto& iter : from_distance_map)
        // { // 打印距离表
        //     cout << iter.first << "->" << iter.second << endl;
        // }
        // cout<< "=-==="<<endl;

        // for (auto &iter : to_distance_map)
        // { // 打印距离表
        //     cout << iter.first << "->" << iter.second << endl;
        // }

        // cout << "min_res:" << min_res << endl;

        // 对邻接表做深度优先遍历，拿到最短路径
        vector<string> path;
        dfs(beginWord, endWord, wordList, nexts, from_distance_map, to_distance_map, path, res);
        return res;
    }

    void dfs(string cur, string endWord, vector<string> &wordList,
             map<string, vector<string>> &nexts, map<string, int> &from_distance_map, map<string, int> &to_distance_map,
             vector<string> &path, vector<vector<string>> &res)
    {
        // cout << endl << "===========" << endl;
        // cout << "cur:" << cur  << endl;
        //避免出现环路
        /*  
            一个典型的例子是：
            graph = {
                'A': ['B', 'C'],
                'B': ['A', 'D'],
                'C': ['A', 'D'],
                'D': ['E'],
                'E': ['F'],
                'F': ['E']  # 注意这里形成了一个环
            }
        */
    //    cout << "cur:" << cur<<endl;
        path.push_back(cur);
        if (cur == endWord) {
            res.push_back(path);
        } else  { 
            int cur_distance = from_distance_map[cur];
            // cout << "next:";
            for (auto cur_next : nexts[cur])
            {
                // cout <<  cur_next << " ";
                //这里有个巧妙的地方，下一个节点到start的距离要+1，但同时到end的距离也要-1，防止出现dfs环路
                /* 
                    如果不这样做，而是使用visited的话，访问到一个节点就不能再访问一次的话，就会漏掉下面的情况: a经过b和c都能到达d的情况
                        a
                     b     c
                        d
                */
                if (from_distance_map[cur_next] == from_distance_map[cur]+1
                    && to_distance_map[cur_next] == to_distance_map[cur]-1)
                {

                    dfs(cur_next, endWord, wordList, nexts, from_distance_map, to_distance_map, path, res);
                }
            }
            
        }
        path.pop_back();
    }

    // bfs
    void bfs(string beginWord, vector<string> &wordList,
            map<string, vector<string>> &nexts, map<string, int> &distance_map)
    {        
        distance_map[beginWord] = 0;
        queue<string> que;
        que.push(beginWord);
        set<string> visited;
        visited.insert(beginWord);
        int res_min = INT_MAX;
        while (!que.empty()) {
            string cur = que.front();
            que.pop();
            for (auto next : nexts[cur]) {
                if (visited.find(next) == visited.end()) {
                    distance_map[next] = distance_map[cur] + 1;
                    que.push(next);
                    visited.insert(next);
                } 
            }
        }
    }
};


class Solution2 {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string> &wordList) {
        //构造临街表
        auto it = std::find(wordList.begin(), wordList.end(), beginWord);
        // 检查元素是否被找到  
        if (it == wordList.end()) {  
            wordList.push_back(beginWord);
        } 
        map<string, vector<string>> nexts;
        get_nexts(nexts, wordList);

        vector<vector<string>> res;
        // for (auto iter : nexts)
        // { // 打印临接表
        //     cout << iter.first << "->";
        //     for (auto str_next : iter.second)
        //     {
        //         cout << str_next << ", ";
        //     }
        //     cout << endl;
        // }
        map<string, int> from_distance_map;
        get_distance(beginWord, nexts, from_distance_map);

        // if (from_distance_map.find(endWord) == from_distance_map.end()) {
        //     return res;
        // }

        map<string, int> to_distance_map;
        get_distance(endWord, nexts, to_distance_map);

        // for (auto &iter : to_distance_map)
        // { // 打印距离表
        //     cout << iter.first << "->" << iter.second << endl;
        // }

        vector<string> path;
        get_shortest_paths(beginWord, endWord, nexts, from_distance_map, to_distance_map, path, res);

        return res;
    }

    void get_shortest_paths(string cur, string to,
            map<string, vector<string>>& nexts,  map<string, int>& from_distance_map,
            map<string, int> to_distance_map, vector<string> & path, vector<vector<string>>& res) {
        // cout<< "cur:" << cur << endl;
        path.push_back(cur);
        if (cur == to) {
            res.push_back(path);
        } else {
            for (auto next: nexts[cur]) {
                if (from_distance_map[next] == from_distance_map[cur]+1 
                    && to_distance_map[next] == to_distance_map[cur]-1) {
                    get_shortest_paths(next, to, nexts, from_distance_map, to_distance_map, path, res);
                }
            }
        }
        path.pop_back();
    }

    void get_nexts(map<string, vector<string>>& nexts, vector<string> &wordList) {
        set<string> dict;
        for (auto& iter:wordList) {
            dict.insert(iter);
        }
        for (auto word: wordList){
            vector<string> single_next;
            get_next(word, dict, single_next);
            nexts[word] = single_next;
        }
    }

    void get_next(string word, set<string>& dict, vector<string>& single_next) {
        for (char cur = 'a'; cur<='z'; cur++) {
            for (int i=0;i<word.length();i++) {
                if (word[i] != cur) {
                    char tmp = word[i];
                    word[i] = cur;
                    if (dict.find(word) != dict.end()) {
                        single_next.push_back(word);
                    }
                    word[i] = tmp;
                }
            }
        }
    }

    void get_distance(string start, map<string, vector<string>> &nexts, map<string, int>& distance_map) {
        // cout << "start: " << start << endl;
        distance_map[start] = 0;
        queue<string> que;
        que.push(start);
        set<string> visited;
        visited.insert(start);
        while (!que.empty()) {
            string cur = que.front();
            que.pop();
            for (auto next:nexts[cur]) {
                if (visited.find(next) == visited.end()) {
                    distance_map[next] = distance_map[cur]+1;
                    que.push(next);
                    visited.insert(next);
                }
            }
        }
    }
};





class dfs_circle_test {
public:
    /*
     graph = {
                'A': ['B', 'C'],
                'B': ['A', 'D'],
                'C': ['A', 'D'],
                'D': ['E'],
                'E': ['F'],
                'F': ['E']  # 注意这里形成了一个环
            }
    但没有形成环
    */
    map<string, vector<string>> graph;
    dfs_circle_test() {
        graph["a"] = {"b", "c"};
        graph["b"] = {"a", "d"};
        graph["c"] = {"a", "d"};
        graph["d"] = {"e", "b"};
        graph["e"] = {"f"};
        graph["f"] = {"e"};
    }

    vector<vector<string>> get_path(string begin, string end) {
        map<string, int> from_distance_map;
        get_distance(begin, graph, from_distance_map);
        map<string, int> to_distance_map;
        get_distance(end, graph, to_distance_map);

        
        cout << "start_distance_map" << endl;
        for (auto& iter:from_distance_map) {
            cout << iter.first << "->" << iter.second << endl;
        }
        cout << "end_distance_map" << endl;
        for (auto& iter:to_distance_map) {
            cout << iter.first << "->" << iter.second << endl;
        }


        vector<vector<string>> res;
        vector<string> path;
        dfs(begin, end, graph, from_distance_map, to_distance_map, path, res);

        return res;

    }

    void dfs(string cur, string end,
             map<string, vector<string>> &nexts, map<string, int> &from_distance_map, map<string, int> &to_distance_map,
             vector<string> &path, vector<vector<string>> &res)
    {
        cout << "cur:" << cur << endl;
        path.push_back(cur);
        if (cur == end) {
            res.push_back(path);
        } else {
            for (auto next:nexts[cur]) {
                if (from_distance_map[next] == from_distance_map[cur]+1) {
                    dfs(next, end, nexts, from_distance_map, to_distance_map, path, res);
                }
            }
        }
        path.pop_back();
    }

    void get_distance(string start,  map<string, vector<string>>& nexts, map<string, int>& distance_map) {
        //广度优先遍历求最短距离
        distance_map[start] = 0;
        queue<string> que;
        que.push(start);
        set<string> visited;
        visited.insert(start);
        while (!que.empty()) {
            string cur = que.front();
            que.pop();
            for (auto next:nexts[cur]) {
                if (visited.find(next) == visited.end()) {
                    distance_map[next] = distance_map[cur]+1;
                    que.push(next);
                    visited.insert(next);
                }
            }
        }
    }
   
};


int main()
{
    Solution sol;

    // cout << "测试开始" << endl;
    // int n = 10;
    // int testTime = 100;

    // // string beginWord = "hit";
    // // string endWord = "cog";
    // // vector<string> word_list = {"hot","dot","dog","lot","log","cog"};

    // for (int i = 0; i < testTime; i++)
    // {
    //     int len1 = (int)(rand() % n) + 1;
    //     int len2 = (int)(rand() % n) + 1;
    //     string s1 = random_string(len1, v);
    //     string s2 = random_string(len2, v);
    //     // cout<< "s1: " << s1 << ",  s2:" << s2 << endl;
    //     int ans1 = sol.minDelete1(s1, s2);
    //     int ans2 = sol.minDelete2(s1, s2);
    //     int ans3 = sol.minDelete3(s1, s2);
    //     // cout << "ans1: " << ans1 << ",  ans2:" << ans2 << ",  ans3:" << ans3 << endl;
    //     if (ans1 != ans2)
    //     {
    //         cout << "出错了!" << endl;
    //     }
    //     if (ans1 != ans3)
    //     {
    //         cout << "出错了! ans3:" << s1 << ", " << s2 << endl;
    //     }

    //     if (ans2 != ans3)
    //     {
    //         cout << "出错了! ans3:" << s1 << ", " << s2 << endl;
    //     }
    // }
    // cout << "测试结束" << endl;

    /*
    输入：beginWord = "hit", endWord = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
    输出：[["hit","hot","dot","dog","cog"],["hit","hot","lot","log","cog"]]
    解释：存在 2 种最短的转换序列：
    "hit" -> "hot" -> "dot" -> "dog" -> "cog"
    "hit" -> "hot" -> "lot" -> "log" -> "cog"
    */

    string beginWord = "hit";
    string endWord = "cog";
    vector<string> word_list = {"hot","dot","dog","lot","log","cog"};

    // string beginWord = "a";
    // string endWord = "c";
    // vector<string> word_list = {"a", "b", "c"};
    // vector<vector<string>> res_bk = {{ "aaaaa", "aaaaz", "aaawz", "aavwz", "avvwz", "vvvwz", "vvvww", "wvvww", "wwvww", "wwwww", "ywwww", "yywww", "yyyww", "yyyyw", "yyyyy", "xyyyy", "xxyyy", "xxxyy", "xxxxy", "xxxxx", "gxxxx", "ggxxx", "gggxx", "ggggx", "ggggg" }};
    // cout << "res_size: " << res_bk.size() << "," << res_bk[0].size() << endl; 1, 25
    // string beginWord = "aaaaa";
    // string endWord = "ggggg";
    // vector<string> word_list =
    //     { "aaaaa", "caaaa", "cbaaa", "daaaa", "dbaaa", "eaaaa", "ebaaa", "faaaa", "fbaaa", "gaaaa", "gbaaa", "haaaa", "hbaaa", "iaaaa", "ibaaa", "jaaaa", "jbaaa", "kaaaa", "kbaaa", "laaaa", "lbaaa", "maaaa", "mbaaa", "naaaa", "nbaaa", "oaaaa", "obaaa", "paaaa", "pbaaa", "bbaaa", "bbcaa", "bbcba", "bbdaa", "bbdba", "bbeaa", "bbeba", "bbfaa", "bbfba", "bbgaa", "bbgba", "bbhaa", "bbhba", "bbiaa", "bbiba", "bbjaa", "bbjba", "bbkaa", "bbkba", "bblaa", "bblba", "bbmaa", "bbmba", "bbnaa", "bbnba", "bboaa", "bboba", "bbpaa", "bbpba", "bbbba", "abbba", "acbba", "dbbba", "dcbba", "ebbba", "ecbba", "fbbba", "fcbba", "gbbba", "gcbba", "hbbba", "hcbba", "ibbba", "icbba", "jbbba", "jcbba", "kbbba", "kcbba", "lbbba", "lcbba", "mbbba", "mcbba", "nbbba", "ncbba", "obbba", "ocbba", "pbbba", "pcbba", "ccbba", "ccaba", "ccaca", "ccdba", "ccdca", "cceba", "cceca", "ccfba", "ccfca", "ccgba", "ccgca", "cchba", "cchca", "cciba", "ccica", "ccjba", "ccjca", "cckba", "cckca", "cclba", "cclca", "ccmba", "ccmca", "ccnba", "ccnca", "ccoba", "ccoca", "ccpba", "ccpca", "cccca", "accca", "adcca", "bccca", "bdcca", "eccca", "edcca", "fccca", "fdcca", "gccca", "gdcca", "hccca", "hdcca", "iccca", "idcca", "jccca", "jdcca", "kccca", "kdcca", "lccca", "ldcca", "mccca", "mdcca", "nccca", "ndcca", "occca", "odcca", "pccca", "pdcca", "ddcca", "ddaca", "ddada", "ddbca", "ddbda", "ddeca", "ddeda", "ddfca", "ddfda", "ddgca", "ddgda", "ddhca", "ddhda", "ddica", "ddida", "ddjca", "ddjda", "ddkca", "ddkda", "ddlca", "ddlda", "ddmca", "ddmda", "ddnca", "ddnda", "ddoca", "ddoda", "ddpca", "ddpda", "dddda", "addda", "aedda", "bddda", "bedda", "cddda", "cedda", "fddda", "fedda", "gddda", "gedda", "hddda", "hedda", "iddda", "iedda", "jddda", "jedda", "kddda", "kedda", "lddda", "ledda", "mddda", "medda", "nddda", "nedda", "oddda", "oedda", "pddda", "pedda", "eedda", "eeada", "eeaea", "eebda", "eebea", "eecda", "eecea", "eefda", "eefea", "eegda", "eegea", "eehda", "eehea", "eeida", "eeiea", "eejda", "eejea", "eekda", "eekea", "eelda", "eelea", "eemda", "eemea", "eenda", "eenea", "eeoda", "eeoea", "eepda", "eepea", "eeeea", "ggggg", "agggg", "ahggg", "bgggg", "bhggg", "cgggg", "chggg", "dgggg", "dhggg", "egggg", "ehggg", "fgggg", "fhggg", "igggg", "ihggg", "jgggg", "jhggg", "kgggg", "khggg", "lgggg", "lhggg", "mgggg", "mhggg", "ngggg", "nhggg", "ogggg", "ohggg", "pgggg", "phggg", "hhggg", "hhagg", "hhahg", "hhbgg", "hhbhg", "hhcgg", "hhchg", "hhdgg", "hhdhg", "hhegg", "hhehg", "hhfgg", "hhfhg", "hhigg", "hhihg", "hhjgg", "hhjhg", "hhkgg", "hhkhg", "hhlgg", "hhlhg", "hhmgg", "hhmhg", "hhngg", "hhnhg", "hhogg", "hhohg", "hhpgg", "hhphg", "hhhhg", "ahhhg", "aihhg", "bhhhg", "bihhg", "chhhg", "cihhg", "dhhhg", "dihhg", "ehhhg", "eihhg", "fhhhg", "fihhg", "ghhhg", "gihhg", "jhhhg", "jihhg", "khhhg", "kihhg", "lhhhg", "lihhg", "mhhhg", "mihhg", "nhhhg", "nihhg", "ohhhg", "oihhg", "phhhg", "pihhg", "iihhg", "iiahg", "iiaig", "iibhg", "iibig", "iichg", "iicig", "iidhg", "iidig", "iiehg", "iieig", "iifhg", "iifig", "iighg", "iigig", "iijhg", "iijig", "iikhg", "iikig", "iilhg", "iilig", "iimhg", "iimig", "iinhg", "iinig", "iiohg", "iioig", "iiphg", "iipig", "iiiig", "aiiig", "ajiig", "biiig", "bjiig", "ciiig", "cjiig", "diiig", "djiig", "eiiig", "ejiig", "fiiig", "fjiig", "giiig", "gjiig", "hiiig", "hjiig", "kiiig", "kjiig", "liiig", "ljiig", "miiig", "mjiig", "niiig", "njiig", "oiiig", "ojiig", "piiig", "pjiig", "jjiig", "jjaig", "jjajg", "jjbig", "jjbjg", "jjcig", "jjcjg", "jjdig", "jjdjg", "jjeig", "jjejg", "jjfig", "jjfjg", "jjgig", "jjgjg", "jjhig", "jjhjg", "jjkig", "jjkjg", "jjlig", "jjljg", "jjmig", "jjmjg", "jjnig", "jjnjg", "jjoig", "jjojg", "jjpig", "jjpjg", "jjjjg", "ajjjg", "akjjg", "bjjjg", "bkjjg", "cjjjg", "ckjjg", "djjjg", "dkjjg", "ejjjg", "ekjjg", "fjjjg", "fkjjg", "gjjjg", "gkjjg", "hjjjg", "hkjjg", "ijjjg", "ikjjg", "ljjjg", "lkjjg", "mjjjg", "mkjjg", "njjjg", "nkjjg", "ojjjg", "okjjg", "pjjjg", "pkjjg", "kkjjg", "kkajg", "kkakg", "kkbjg", "kkbkg", "kkcjg", "kkckg", "kkdjg", "kkdkg", "kkejg", "kkekg", "kkfjg", "kkfkg", "kkgjg", "kkgkg", "kkhjg", "kkhkg", "kkijg", "kkikg", "kkljg", "kklkg", "kkmjg", "kkmkg", "kknjg", "kknkg", "kkojg", "kkokg", "kkpjg", "kkpkg", "kkkkg", "ggggx", "gggxx", "ggxxx", "gxxxx", "xxxxx", "xxxxy", "xxxyy", "xxyyy", "xyyyy", "yyyyy", "yyyyw", "yyyww", "yywww", "ywwww", "wwwww", "wwvww", "wvvww", "vvvww", "vvvwz", "avvwz", "aavwz", "aaawz", "aaaaz" };
        // 输出6
    //输出：[[ "aaaaa", "aaaaz", "aaawz", "aavwz", "avvwz", "vvvwz", "vvvww", "wvvww", "wwvww", "wwwww", "ywwww", "yywww", "yyyww", "yyyyw", "yyyyy", "xyyyy", "xxyyy", "xxxyy", "xxxxy", "xxxxx", "gxxxx", "ggxxx", "gggxx", "ggggx", "ggggg" ]]

    // string beginWord = "a";
    // string endWord = "c";
    // vector<string> word_list = {"a", "b", "c", "d"};
    vector<vector<string>>
        res = sol.findLadders(beginWord, endWord, word_list);
    
    // cout << "res_len: " << res[0].size() << endl;
    for (auto &item : res)
    {
        for (auto &str : item)
        {
            cout << str << "->";
        }
        cout << endl;
    }

    return 0;
}