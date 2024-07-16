/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 23:05:28
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
/**
 * 给你一个字符串类型的数组arr，譬如： String[] arr = { "b\\cst", "d\\", "a\\d\\e", "a\\b\\c" };
 * 你把这些路径中蕴含的目录结构给画出来，子目录直接列在父目录下面，并比父目录 向右进两格，就像这样:
 * 
 * 同一级的需要按字母顺序排列，不能乱。
 * 
 */

struct Node{
    string str;
    map<string, Node*> next;//map底层就是红黑树，是一个有序表
    Node() {
        str = "";
    }
    Node(string& str){
        this->str = str;
    }
};
class TrieTree{ 
public:
    Node* root;
    TrieTree() {
        root = new Node();
    }
    void insert(string& str) { //建立前缀树
        size_t start = 0, end = 0;  
        Node* cur = root;
        while ((end = str.find('#', start)) != std::string::npos) {
            string cur_str = str.substr(start, end - start);
            cout << "str:" << str << " end:" << end << " start:" << start << " cur_str:"<< cur_str << endl;
            Node* next_node = new Node(cur_str); //将这条路的str也整体存到node中一份
            if (cur->next.find(cur_str) == cur->next.end()) {
                cur->next[cur_str] = next_node;
            }
            cur = cur->next[cur_str];
            start = end + 1;
        }

        string cur_str = str.substr(start, str.length() - start);
        if (cur_str.length() != 0) {
            Node *next_node = new Node(cur_str);
            if (cur->next.find(cur_str) == cur->next.end()) {
                cout << " last_str:" << cur_str << " start:" << start << endl;
                cur->next[cur_str] = next_node;
            }
        }
    }

    void print() {
        for (auto& iter:root->next) {
            cout << "[print] iter:" <<  iter.second->str << endl;
            dfs_print(iter.second, 0);
        }
    }

    //深度优先遍历当前树
    //level标记当前的层数
    void dfs_print(Node* cur, int level) {
        if (cur == NULL) {
            return;
        }
        print_kongge(level);
        cout << cur->str << endl;
        for (auto& iter:cur->next) {
            dfs_print(iter.second, level+1);
        }
    }

    void print_kongge(int level) {
        int _num = level*2;
        for (int i = 0; i < _num; ++i) {
            cout << " ";
        }
    }
};

class Solution
{
public:

    //按照前缀树组织目录结构。注意前缀树的子节点需要按照有序表组织。
    //然后深度优先遍历
    void print_contents(vector<string>& arr) {
        TrieTree tt;
        for(int i = 0; i < arr.size(); ++i) {
            tt.insert(arr[i]);
        }

        tt.print();
    }
};

int main()
{
    Solution sol;
    vector<string> arr= {"b#cst#1223#123","b#kl#fd","b#kl#io","d#", "a#de", "ab#c"};
    sol.print_contents(arr);
    return 0;
}