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

//https://leetcode.cn/problems/stream-of-characters/solutions/2187670/python3javacgo-yi-ti-yi-jie-qian-zhui-sh-79kg/
class Node {
public:
    string end;// 以当前节点结尾，这条线路的字符串
    bool end_used; // 标记是否已经找到过这个敏感词。如果是end节点，并且没有被收集过答案了是true。否则是false
    Node* fail; //fail指针，，匹配失败时，往上找最佳的和当前后缀字符串相等的前缀字符串的结束节点
    vector<Node*> nexts; //经典前缀树next数组

    Node() {
        end = "";
        end_used = false;
        fail = nullptr;
        nexts.resize(26, nullptr);
    }

};
class ACAutomation{
public:
    Node* root;
    ACAutomation() {
        root = new Node();
    }

    void insert(string& str) {
        int len  = str.length();
        Node* cur = root;
        for (int i=0;i<len;i++) {
            int next_char = str[i] - 'a';
            if (cur->nexts[next_char] == nullptr) {
                cur->nexts[next_char] = new Node();
            }
            cur = cur->nexts[next_char];
        }
        cur->end=str;// 尾结点，记录这条线路的字符串
    }

    //链接所有fail指针，使用bfs
    //人为规定root的fail指针指向空。
    //root的下一级节点都指向自己
    void build() {
        queue<Node*> que;
        que.push(root);
        while (!que.empty()) {
            Node* cur = que.front();
            que.pop();
            //遍历nexts数组。当前节点设置自己孩子的fail指针
            for (int i=0; i<26; i++) {
                if (cur->nexts[i] != nullptr) {
                    cur->nexts[i]->fail = root;// 先指向root。后续如果有其他情况，再修改
                    Node* cur_fail = cur->fail;
                    while (cur_fail != nullptr) {
                        // 父节点的fail链，走到的节点的下一条路有到i的，则链接上
                        if (cur_fail->nexts[i] != nullptr) {
                            cur->nexts[i]->fail = cur_fail->nexts[i];
                            break; //贪心，找到最长的前缀，就跳出
                        }
                        cur_fail = cur_fail->fail; //继续往下一个fail指针指向的节点走
                    }
                    // cout << "cur fail:" << cur->nexts[i]->fail << " root:" << root << endl;
                    que.push(cur->nexts[i]); //当前子节点入队列
                }
            }
        }
    }

    // 查询文章中的敏感词：content
    void contain_words(string& content, vector<string>& res) {
        int len = content.length();
        if (len==0) {
            return;
        }
        Node* cur = root;//从root开始找
        //遍历一遍文章
        for (int i=0;i<len;i++) {
            int idx = content[i] - 'a';
            // 不能往下走，但是cur又不是根节点的情况，继续沿着fail走
            while (cur->nexts[idx] == nullptr && cur != root) {
                cur = cur->fail;
            }
            // 出了上面的while有两种可能
            // 现在来到的路径，是可以继续匹配的
            // 现在来到的节点已经是头节点了
            cur = cur->nexts[idx] != nullptr ? cur->nexts[idx] : root; //能往下走继续往下走，否则来到root
            // cout << "i:" << i << " cur:" << cur << " root:" << root << endl;
            // cout << " cur_fail:" << cur->fail << endl;

            // 现在cur要么是走到了下级节点，要么就还是在root位置

            // 沿着fail过一圈答案
            Node* follow = cur;
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