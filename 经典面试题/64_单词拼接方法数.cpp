/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 14:24:27
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
假设所有字符都是小写字母. 大字符串是str. arr是去重的单词表, 每个单词都不是空字符串且可以使用任意次.
使用arr中的单词有多少种拼接str的方式. 返回方法数。
arr={"a","aa","aaa","ab","b","c"}
str="aaabc"
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
    int ways(string str, vector<string> &arr)
    {
        set<string> _sets;
        for (auto iter : arr)
        {
            _sets.insert(iter);
        }
        return process(str, 0, _sets);
    }

    /*
    所有的候选字符串都放在了_sets中
    str[i...]能够被_sets中的单词分解的话，返回分解的方法数
    */
    int process(string str, int i, set<string> &_sets)
    {
        // i来到了末尾，只有一种方法数，就是什么单词也不用
        if (i == str.length())
        {
            return 1;
        }

        // 枚举i开始的前缀，做第一种可能性
        int ways = 0;
        for (int end = i; end < str.length(); end++)
        {
            string pre = str.substr(i, end - i + 1);
            if (_sets.find(pre) != _sets.end())
            {
                ways += process(str, end + 1, _sets);
            }
        }
        return ways;
    }

    int ways_dp(string str, vector<string> &arr)
    {
        set<string> _sets;
        for (auto iter : arr)
        {
            _sets.insert(iter);
        }

        int len = str.length();
        vector<int> dp(len+1, 0);
        dp[len] = 1;
        //dp[i]代表，拼接出i...字符串的总方法数
        for (int i=len-1; i>=0;i--) {
            int ways = 0;
            for (int end = i; end < str.length(); end++)
            {   
                //这是O(N)的，可以使用前缀树降成O(1)
                string pre = str.substr(i, end - i + 1);
                if (_sets.find(pre) != _sets.end())
                {
                    ways += dp[end + 1];
                }
            }
            dp[i] = ways;
        }
        return dp[0];
    }
    
    int ways_dp_tree(string str, vector<string> &arr)
    {
        set<string> _sets;
        TrieTree tt;
        for (auto iter : arr)
        {
            tt.insert(iter);
        }
        

        int len = str.length();
        vector<int> dp(len+1, 0);
        dp[len] = 1;
        //dp[i]代表，拼接出i...字符串的总方法数
        for (int i=len-1; i>=0;i--) {
            int ways = 0;
            Node *cur = tt.head;
            for (int end = i; end < str.length(); end++) {   
                bool  flag = false;
                //没有路了不需要再往后遍历了
                if (cur->next.find(str[end]) == cur->next.end()) {
                    flag = true;
                    break;
                }
                cur = cur->next[str[end]];
                if (cur->end>0) {
                    // cout << "i:" << i<< " end:" << end << " str[end]:" << str[end] << " " << cur->end << endl;
                    ways+=dp[end+1];
                }
                if (flag) {
                    break;
                }
            }
            dp[i] = ways;
        }
        // for (auto iter:dp) {
        //     cout << iter << " ";
        // }
        // cout << endl;
        return dp[0];
    }
};

int main()
{

    Solution sol;

    vector<string> arr = {"a", "aa", "aaa", "ab", "b", "c"};
    string str = "aaabc";

    cout << sol.ways(str, arr) << endl;
    cout << sol.ways_dp(str, arr) << endl;
    cout << sol.ways_dp_tree(str, arr) << endl;


    return 0;
}