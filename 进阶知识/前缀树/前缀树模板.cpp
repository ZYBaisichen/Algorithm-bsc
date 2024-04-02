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


int main()
{
    Solution sol;
    // vector<string> arr= {"b#cst#1223#123","b#kl#fd","b#kl#io","d#", "a#de", "ab#c"};
    // sol.print_contents(arr);
    return 0;
}