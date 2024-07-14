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

using namespace std;

//例题: https://leetcode.cn/problems/stream-of-characters/description/ ， 注意这个题目中字符流可能会重复出现words中的字符串，所以不需要将描黑的end节点置为不黑

/*

设计一个算法：接收一个字符流，并检查这些字符的后缀是否是字符串数组 words 中的一个字符串。

例如，words = ["abc", "xyz"] 且字符流中逐个依次加入 4 个字符 'a'、'x'、'y' 和 'z' ，你所设计的算法应当可以检测到 "axyz" 的后缀 "xyz" 与 words 中的字符串 "xyz" 匹配。

按下述要求实现 StreamChecker 类：

StreamChecker(String[] words) ：构造函数，用字符串数组 words 初始化数据结构。
boolean query(char letter)：从字符流中接收一个新字符，如果字符流中的任一非空后缀能匹配 words 中的某一字符串，返回 true ；否则，返回 false。
 

示例：

输入：
["StreamChecker", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query", "query"]
[[["cd", "f", "kl"]], ["a"], ["b"], ["c"], ["d"], ["e"], ["f"], ["g"], ["h"], ["i"], ["j"], ["k"], ["l"]]
输出：
[null, false, false, false, true, false, true, false, false, false, false, false, true]

解释：
StreamChecker streamChecker = new StreamChecker(["cd", "f", "kl"]);
streamChecker.query("a"); // 返回 False
streamChecker.query("b"); // 返回 False
streamChecker.query("c"); // 返回n False
streamChecker.query("d"); // 返回 True ，因为 'cd' 在 words 中
streamChecker.query("e"); // 返回 False
streamChecker.query("f"); // 返回 True ，因为 'f' 在 words 中
streamChecker.query("g"); // 返回 False
streamChecker.query("h"); // 返回 False
streamChecker.query("i"); // 返回 False
streamChecker.query("j"); // 返回 False
streamChecker.query("k"); // 返回 False
streamChecker.query("l"); // 返回 True ，因为 'kl' 在 words 中
 

提示：

1 <= words.length <= 2000
1 <= words[i].length <= 200
words[i] 由小写英文字母组成
letter 是一个小写英文字母
最多调用查询 4 * 104 次


每次来一个字符相当于每次从文章中读出来一个字符，是ac自动机cur指针不回退到root的查找。
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
    set<Node *> all_end;
    Node *cur;
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
        all_end.insert(cur);
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
        cur = root;
    }

    // 查询文章中的敏感词：content
    void contain_words(string& content, vector<string>& res) {
        int len = content.length();
        if (len==0) {
            return;
        }
        Node* cur_tmp = root;//从root开始找
        //遍历一遍文章
        for (int i=0;i<len;i++) {
            // cout << "contain_words:" << content[i] << endl;
            // cout << "cur_tmp:" << cur_tmp << " root:" << root << endl;
            // cout << "cur_tmp:" << cur_tmp << " root:" << root << " cur_fail:" << cur_tmp->fail << endl;
            int idx = content[i] - 'a';
            // 不能往下走，但是cur又不是根节点的情况，继续沿着fail走
            while (cur_tmp->nexts[idx] == nullptr && cur_tmp != root) {
                cur_tmp = cur_tmp->fail;
            }
            // 出了上面的while有两种可能
            // 现在来到的路径，是可以继续匹配的
            // 现在来到的节点已经是头节点了
            cur_tmp = cur_tmp->nexts[idx] != nullptr ? cur_tmp->nexts[idx] : root; // 能往下走继续往下走，否则来到root
            // cout << "i:" << i << " cur:" << cur << " root:" << root << endl;
            // cout << " cur_fail:" << cur->fail << endl;

            // 现在cur要么是走到了下级节点，要么就还是在root位置

            // 沿着fail过一圈答案
            Node *follow = cur_tmp;
            while (follow != root) {
                if (follow->end_used) {
                    break;
                }
                if (follow->end != "") {
                    res.push_back(follow->end);
                    follow->end_used = true;
                }
                follow = follow->fail;
            }
        }
        for (auto iter:all_end) {
            iter->end_used = false;
        }
    }

    bool query(char letter)
    {
        bool res = false;
        int idx = letter - 'a';
        // cout << "query:" << letter << endl;
        // cout << "cur:" << cur << " root:" << root << endl;
        // cout << "cur:" << cur << " root:" << root << " cur_fail:" << cur->fail << endl;
        // 不能往下走，但是cur又不是根节点的情况，继续沿着fail走
        while (cur->nexts[idx] == nullptr && cur != root)
        {
            cur = cur->fail;
            // cout << "cur:" << cur << " root:" << root << " cur_fail:" << cur->fail << endl;
        }
        // 出了上面的while有两种可能
        // 现在来到的路径，是可以继续匹配的
        // 现在来到的节点已经是头节点了
        cur = cur->nexts[idx] != nullptr ? cur->nexts[idx] : root; // 能往下走继续往下走，否则来到root
        // cout << "letter:" << letter << " cur:" << cur << " root:" << root << endl;
        // cout << " cur_fail:" << cur->fail << endl;

        // 现在cur要么是走到了下级节点，要么就还是在root位置

        // 沿着fail过一圈答案
        Node *follow = cur;
        while (follow != root)
        {
            if (follow->end_used)
            {
                break;
            }
            if (follow->end != "")
            {
                // follow->end_used = true;
                res = true;
                break;
            }
            follow = follow->fail;
        }
        return res;
    }
};

class StreamChecker
{
public:
    ACAutomation ac;
    StreamChecker(vector<string> &words)
    {
        for (int i = 0; i < words.size(); i++)
        {
            ac.insert(words[i]);
        }
        ac.build();
    }

    bool query(char letter)
    {
        return ac.query(letter);
    }

    void contain_words(string str) {
        vector<string> res;
        ac.contain_words(str, res);
        cout << "con_words:" << endl;
        for (int i=0;i<res.size();i++) {
            cout << res[i] << endl;
        }
    }
};
// ACAutomation ac = new ACAutomation();
// ac.insert("dhe");
// ac.insert("he");
// ac.insert("abcdheks");
// // 设置fail指针
// ac.build();

// List<String> contains = ac.containWords("abcdhekskdjfafhasldkflskdjhwqaeruv");
// for (String word : contains)
// {
//     System.out.println(word);
// }
int main() {
    ACAutomation ac;
    ac.insert("abcde");
    ac.insert("bcdf");
    ac.insert("cdtks");
    ac.build();
    vector<string> res;
    ac.contain_words("abcdtks", res);
    for (string s : res) {
        cout << s << endl;
    }
}