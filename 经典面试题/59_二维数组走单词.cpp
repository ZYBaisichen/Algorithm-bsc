/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-02 20:24:35
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

using namespace std;

/*
给定一个 m x n 二维字符网格 board 和一个单词（字符串）列表 words， 返回所有二维网格上的单词 。

单词必须按照字母顺序，通过 相邻的单元格 内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母在一个单词中不允许被重复使用。



示例 1：


输入：board = [["o","a","a","n"],["e","t","a","e"],["i","h","k","r"],["i","f","l","v"]], words = ["oath","pea","eat","rain"]
输出：["eat","oath"]
示例 2：


输入：board = [["a","b"],["c","d"]], words = ["abcb"]
输出：[]


提示：

m == board.length
n == board[i].length
1 <= m, n <= 12
board[i][j] 是一个小写英文字母
1 <= words.length <= 3 * 104
1 <= words[i].length <= 10
words[i] 由小写英文字母组成
words 中的所有字符串互不相同

https://leetcode.cn/problems/word-search-ii/

*/
struct Node{
    vector<string> end_str;
    int pass;
    int end;
    map<char, Node*> next;//map底层就是红黑树，是一个有序表
    ~Node(){
        //暂时没有考虑释放，会有些泄露
    }
    Node() {
        pass = 0; //有多少单词经过了pass
        end = 0; //有多少单词以当前节点作为结尾
    }
};


class TrieTree{ 
public:
    Node* head;
    TrieTree() {
        head = new Node();
    }
    void insert(string& str) { //建立前缀树
        Node* cur = head;
        int len=str.length();
        for (int i=0;i<len;i++) { //每个字符
            if (cur->next.find(str[i]) == cur->next.end()) {
                cur->next[str[i]] = new Node();
            }
            cur->next[str[i]]->pass++;
            if (i==len-1) {
                cur->next[str[i]]->end++;
                cur->next[str[i]]->end_str.push_back(str);
            }
            cur = cur->next[str[i]];
        }
    }
};

class Solution
{
public:
    /*
    暴力：
        对于每个单词，遍历每个开头位置看能不能走出来，上下左右走的过程中不要重复走。
        单词个数为n, 单词平均长度是m。board的范围是nn*mm，假设加了记忆化搜索总复杂度需要O(n*m*nn*mm)
        肯定会超时，需要做优化
    */
    vector<string> findWords1(vector<vector<char>> &board, vector<string> &words)
    {
        vector<string> res;
        int len = words.size();
        int n = board.size();
        int m = board[0].size();
        for (int kk = 0; kk < len; kk++)
        {
            string cur_str = words[kk];
            // cout << "kk:" << kk << " cur_str:" << cur_str << endl;
            for (int i = 0; i < n; i++)
            {
                int flag = false;
                for (int j = 0; j < m; j++)
                {
                    int cur_str_len = cur_str.length();
                    if (board[i][j] == cur_str[0])
                    { // 找到了开头
                        // cout << "find first:" << "i:" << i << " j:" << j << endl;
                        vector<vector<bool>> visited(n, vector<bool>(m, false));
                        if (process(board, cur_str, 0, i, j, visited))
                        {
                            // cout << "cur_str:" << " i:" << i << " j:" << j << endl;
                            res.push_back(cur_str);
                            flag = true;
                            break;
                        }
                    }
                }
                if (flag)
                {
                    break;
                }
            }
        }
        return res;
    }
    // 来到了idx，
    bool process(vector<vector<char>> &board, string str, int idx, int i, int j, vector<vector<bool>> &visited)
    {
        // cout << "process:" << "idx:" << idx << " i:" << i << " j:" << j <<  " visited[i][j]:" << visited[i][j] << " str[idx]:" << str[idx] << " board[i][j]:" << board[i][j]<< endl;
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size())
        {
            return false;
        }
        if (visited[i][j]) { // 来过了，不要再来了
            return false;
        }
        if (str[idx] != board[i][j])
        {
            return false;
        }
        visited[i][j] = true; //登录到了i,j上
        if (idx == str.size() - 1 && str[idx] == board[i][j])
        {
            visited[i][j] = false; //回溯
            return true;
        }

        bool next1 = false;
        bool next2 = false;
        bool next3 = false;
        bool next4 = false;

        // 上
        if (i - 1 >= 0 && idx + 1 < str.length())
        {
            next1 = process(board, str, idx + 1, i - 1, j, visited);
        }
        // 下
        if (i + 1 < board.size() && idx + 1 < str.length())
        {
            next2 = process(board, str, idx + 1, i + 1, j, visited);
        }
        // 左
        if (j - 1 >= 0 && idx + 1 < str.length())
        {
            next3 = process(board, str, idx + 1, i, j - 1, visited);
        }
        // 右
        if (j + 1 < board[0].size() && idx + 1 < str.length())
        {
            next4 = process(board, str, idx + 1, i, j + 1, visited);
        }
        visited[i][j] = false; // 回溯
        return next1 || next2 || next3 || next4;
    }

    /*
    解决多个word匹配问题，最容易想到前缀树。
    1. 将words所有单词建立前缀树
    2. 枚举board每个位置的开头，做枚举
    */
    vector<string> findWords(vector<vector<char>> &board, vector<string> &words) {
        int len = words.size();
        //建立前缀树
        TrieTree tt;
        for (int i=0;i<len;i++) {
            tt.insert(words[i]);
        }

        int n = board.size();
        int m = board[0].size();
        vector<string> res;
        for (int i = 0; i < n; i++) {
            int flag = false;
            for (int j = 0; j < m; j++) {
                string path="";
                process1(board, path, tt.head, i,j,res);
            }
        }
        return res;
    }

    /*
    来到了board的i,j位置，已经走过的路径是path
    如果找到了一个单词就将当前path加到res中
    当前来到了前缀树的cur节点
    返回从i,j出发找到了多少单词
    */
    int process1(
            vector<vector<char>> &board, string& path, Node* cur, int i,int j, vector<string>& res) {
        char ch = board[i][j];
        if (ch==0) { //已经来到过了，直接返回为0
            return 0;
        }

        if (cur->next.find(ch) == cur->next.end()) { // 没有这个字符，直接返回0
            return 0;
        }

        if (cur->next[ch]->pass == 0) { //一个比较重要的优化，如果经过ij的所有路径都已经收集到了答案，则不再处理
            return 0;
        }

        //能登录上i和j
        cur = cur->next[ch];
        path += ch;
        int fix = 0; //经过当前节点，解决的单词数
        if (cur->end > 0) { //当前节点是某个单词的结尾
            res.push_back(path);
            cur->end--;
            fix++;
        }

        board[i][j] = 0; //设置已经访问过

        //上
        if (i - 1 >= 0) {
            fix += process1(board, path, cur, i-1, j, res);
        }
        //下
        if (i + 1 < board.size()) {
            fix += process1(board, path, cur, i+1, j, res);
        }
        //左
        if (j - 1 >= 0) {
            fix += process1(board, path, cur, i, j-1, res);
        }
        //右
        if (j + 1 < board[0].size()) {
            fix += process1(board, path, cur, i, j+1, res);
        }

        board[i][j] = ch; //回溯
        cur->pass -= fix;
        path = path.substr(0, path.length()-1);
        return fix;
    }
};

void print(vector<string>& res, string suff) {
    cout << suff << endl;
    for (auto& iter:res) {
        cout << iter << " ";
    }
    cout << endl;
}

int main()
{

    Solution sol;

    vector<vector<char>> board = {{'o','a','a','n'},{'e','t','a','e'},{'i','h','k','r'},{'i','f','l','v'}};
    // vector<string> words = {"oath","pea","eat","rain"};
    vector<string> words = {"oa", "oaa"};


    // vector<vector<char>> board = {
    //     {'a','b','c','e'},
    //     {'x','x','c','d'},
    //     {'x','x','b','a'}
    // };
    // vector<string> words = {"abc","abcd"};

    // vector<vector<char>> board = {{"a","a"}};
    // vector<string> words = {"aaa"};

    vector<string> res1 =  sol.findWords1(board, words);
    vector<string> res2 =  sol.findWords(board, words);
    print(res1, "baoli");
    print(res2, "youhua");

    return 0;
}