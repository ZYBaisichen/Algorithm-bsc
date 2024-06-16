/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
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
参考进阶知识-前缀树

https://leetcode.cn/problems/implement-trie-prefix-tree/
Trie（发音类似 "try"）或者说 前缀树 是一种树形数据结构，用于高效地存储和检索字符串数据集中的键。这一数据结构有相当多的应用情景，例如自动补完和拼写检查。

请你实现 Trie 类：

Trie() 初始化前缀树对象。
void insert(String word) 向前缀树中插入字符串 word 。
boolean search(String word) 如果字符串 word 在前缀树中，返回 true（即，在检索之前已经插入）；否则，返回 false 。
boolean startsWith(String prefix) 如果之前已经插入的字符串 word 的前缀之一为 prefix ，返回 true ；否则，返回 false 。
 

示例：

输入
["Trie", "insert", "search", "search", "startsWith", "insert", "search"]
[[], ["apple"], ["apple"], ["app"], ["app"], ["app"], ["app"]]
输出
[null, null, true, false, true, null, true]

解释
Trie trie = new Trie();
trie.insert("apple");
trie.search("apple");   // 返回 True
trie.search("app");     // 返回 False
trie.startsWith("app"); // 返回 True
trie.insert("app");
trie.search("app");     // 返回 True
 

提示：

1 <= word.length, prefix.length <= 2000
word 和 prefix 仅由小写英文字母组成
insert、search 和 startsWith 调用次数 总计 不超过 3 * 104 次
*/

/*


*/

// struct Node {
//     map<char, Node*> next; //记录路径
//     bool is_end;
//     Node() {
//         is_end = false;
//     }
// };
// class Trie {
// public:
//     Node* root;
//     Trie() {
//         root=new Node();
//     }
    
//     void insert(string word) {
//         int len = word.size();
//         Node* cur = root;
//         for (int i=0;i<len;i++) {
//             if (cur->next.find(word[i]) == cur->next.end()) {
//                 cur->next[word[i]] = new Node();
//             }
//             cur = cur->next[word[i]];
//             if (i==len-1) {
//                 cur->is_end = true;
//             }
//         }
//     }
    
//     bool search(string word) {
//         int len = word.size();
//         if (len==0) {
//             return  true;
//         }
//         Node* cur = root;
//         for (int i=0;i<len;i++) {
//             if (cur->next.find(word[i]) == cur->next.end()) {
//                 return false;
//             } else {
//                 cur = cur->next[word[i]];
//             }
//         }
//         if (cur->is_end) {
//             return true;
//         } else {
//             return false;
//         }
//     }
    
//     bool startsWith(string prefix) {
//         int len = prefix.size();
//         if (len==0) {
//             return  true;
//         }
//         Node* cur = root;
//         for (int i=0;i<len;i++) {
//             if (cur->next.find(prefix[i]) == cur->next.end()) {
//                 return false;
//             } else {
//                 cur = cur->next[prefix[i]];
//             }
//         }
//         return true;
//     }
// };


struct Node {
    // map<char, Node*> next; //记录路径
    vector<Node*> next;
    bool is_end;
    Node() {
        is_end = false;
        next.resize(26, nullptr);
    }
};
class Trie {
public:
    Node* root;
    Trie() {
        root=new Node();
    }
    
    void insert(string word) {
        int len = word.size();
        Node* cur = root;
        int cur_word;
        for (int i=0;i<len;i++) {
            cur_word = word[i]-'a';
            if (cur->next[cur_word] == nullptr) {
                cur->next[cur_word] = new Node();
            }
            cur = cur->next[cur_word];
            if (i==len-1) {
                cur->is_end = true;
            }
        }
    }
    
    bool search(string word) {
        int len = word.size();
        if (len==0) {
            return  true;
        }
        Node* cur = root;
        int cur_word=0;
        for (int i=0;i<len;i++) {
            cur_word = word[i]-'a';
            if (cur->next[cur_word] == nullptr) {
                return false;
            } else {
                cur = cur->next[cur_word];
            }
        }
        if (cur->is_end) {
            return true;
        } else {
            return false;
        }
    }
    
    bool startsWith(string prefix) {
        int len = prefix.size();
        if (len==0) {
            return  true;
        }
        Node* cur = root;
        int cur_word=0;
        for (int i=0;i<len;i++) {
            cur_word = prefix[i]-'a';
            if (cur->next[cur_word] == nullptr) {
                return false;
            } else {
                cur = cur->next[cur_word];
            }
        }
        return true;
    }
};
int main()
{

    Solution sol;
    return 0;
}
