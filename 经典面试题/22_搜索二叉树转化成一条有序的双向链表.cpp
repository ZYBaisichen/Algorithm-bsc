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
  * 双向链表节点结构和二叉树节点结构是一样的，如果你把last认为是left， right认为是next的话。
 * 给定一个搜索二叉树的头节点head，请转化成一条有序的双向链表，并返回链 表的头节点。
 * https://leetcode.cn/problems/er-cha-sou-suo-shu-yu-shuang-xiang-lian-biao-lcof/description/
 */
/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;

    Node() {}

    Node(int _val) {
        val = _val;
        left = NULL;
        right = NULL;
    }

    Node(int _val, Node* _left, Node* _right) {
        val = _val;
        left = _left;
        right = _right;
    }
};
*/
class Solution {
public:
    Node* head = NULL, *pre=NULL;
    Node* treeToDoublyList(Node* root) {
        if (!root) return root;
        dfs(root);
        head->left = pre;
        pre->right = head;
        return head;
    }
    void dfs(Node* root) {
        if (!root) return;
        dfs(root->left); //先展开左边
        if (!pre) {
            head = root;
            pre=head;
        } else {
            pre->right = root;
        }
        root->left=pre;
        pre = root;
        dfs(root->right); //再展开右边
    }
};

struct info{
    Node* head;
    Node* last;
    info ()
};
class Solution {
public:
    Node* head = NULL, *pre=NULL;
    Node* treeToDoublyList(Node* root) {
        if (!root) return root;
        dfs(root);
        return head;
    }
    info dfs(Node* root) {
        if (root->left==nullptr && root->right==nullptr) {
            return 
        }
        dfs(root->left);
        if (!pre) {
            head = root;
            pre=head;
        } else {
            pre->right = root;
        }
        root->left=pre;
        pre = root;
        dfs(root->right);
    }

    struct info
    {
        Node *head;
        Node *last;
        info(Node* _head, Node* _last) {
            head = _head;
            last = _last;
        }
        info() {
            head = nullptr;
            last = nullptr;
        }
    };
    //使用下面的二叉树递归套路，理论上是对的，但不知道为什么力扣会报错
    Node *treeToDoublyList(Node *root)
    {
        if (!root)
            return root;
        info res = dfs2(root);
        res.head->left = nullptr;
        res.last->right = nullptr;

        cout << "res:" << res.head << endl;
        cout << res.head->val << endl;
        cout << "res_last:" << res.last << endl;
        cout << res.last->val << endl;

        Node* cur = res.head;
        cout << "zhengxiang:" << endl;
        while (cur!=nullptr) {
            cout << " " << cur->val;
            cur = cur->right;
        }
        cout << endl;

        cur = res.last;
        cout << "fanxiang:" << endl;
        while (cur!=nullptr) {
            cout << " " << cur->val;
            cur = cur->left;
        }
        cout << endl;
        return res.head;
    }
    info dfs2(Node *root)
    {
        if (root == nullptr) {
            return info(nullptr, nullptr);
        }
        info left = dfs2(root->left);
        info right = dfs2(root->right);

        // cout << "=====";
        // cout << "cur：" << root->val << endl;
        // cout << "left:" << left.last << " left_val:" << (left.last?left.last->val:-1)<< endl;
        // cout << "right:" << right.head << " right_val:" << (right.head?right.head->val:-1)<< endl;

        root->left = left.last;
        root->right = right.head;

        if (left.last) {
            left.last->right = root; 
        }
        if (right.head) {
            right.head->left = root;
        }
        Node* cur_head = left.head != nullptr? left.head : root;
        Node* cur_last = right.last != nullptr? right.last : root; 
        return info(cur_head, cur_last);
    }
};

int main()
{
    Solution sol;
    vector<string> arr= {"b#cst#1223#123","b#kl#fd","b#kl#io","d#", "a#de", "ab#c"};
    sol.print_contents(arr);
    return 0;
}