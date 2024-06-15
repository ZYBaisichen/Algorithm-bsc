/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 17:51:57
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
https://leetcode.cn/problems/flatten-binary-tree-to-linked-list/description/
给你二叉树的根结点 root ，请你将它展开为一个单链表：

展开后的单链表应该同样使用 TreeNode ，其中 right 子指针指向链表中下一个结点，而左子指针始终为 null 。
展开后的单链表应该与二叉树 先序遍历 顺序相同。
 

示例 1：


输入：root = [1,2,5,3,4,null,6]
输出：[1,null,2,null,3,null,4,null,5,null,6]
示例 2：

输入：root = []
输出：[]
示例 3：

输入：root = [0]
输出：[0]
 

提示：

树中结点数在范围 [0, 2000] 内
-100 <= Node.val <= 100
 

进阶：你可以使用原地算法（O(1) 额外空间）展开这棵树吗？
*/

/*
一、立刻想到的的是使用前序遍历，等到一个序列，然后再构建链表

二、原地展开，可以使用二叉树递归套路试试

*/
struct TreeNode{
public:
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Solution
{
public:
    void flatten1(TreeNode* root) {
        if (!root) {
            return;
        }
        vector<TreeNode*> res;
        dfs(root,res);

        int len = res.size();
        for (int i=0;i<len-1;i++) {
            res[i]->left = nullptr;
            res[i]->right = res[i+1];
        }
        res[len-1]->left = nullptr;
        res[len-1]->right = nullptr;
    }
    void dfs(TreeNode* root, vector<TreeNode*>& res) {
        if (!root) {
            return;
        }
        res.push_back(root);
        dfs(root->left, res);
        dfs(root->right, res);
    }

    struct info {
        TreeNode* s;
        TreeNode* e;
        info() {
            s= nullptr;
            e=nullptr;
        }
        info(TreeNode* _s, TreeNode* _e) {
            s = _s;
            e = _e;
        }
    };

    void flatten(TreeNode* root) {
        if (!root) {
            return;
        }
        info res = flatten_core(root);
        // TreeNode* cur = res.s;
        // cout << "res:";
        // while (cur) {
        //     cout << cur->val << " ";
        //     cur = cur->right;
        // }
        // cout << endl;
    }

    info flatten_core(TreeNode* cur) {
        if (!cur) {
            return info();
        }
        //叶子节点
        if (!cur->left && !cur->right) {
            cur->right=nullptr;
            cur->left=nullptr;
            return info(cur, cur);
        }

        info left = flatten_core(cur->left);
        info right = flatten_core(cur->right);
        // cout << "root:" << cur->val << endl;
        // if (left.s) {
        //     cout << "left.start:" << left.s->val << endl;
        // }
        // if (left.e) {
        //     cout << "left.end:" << left.e->val << endl;
        // }
        // if (right.s) {
        //     cout << "right.start:" << right.s->val << endl;
        // }
        // if (right.e) {
        //     cout << "right.end:" << right.e->val << endl;
        // }
        TreeNode *start = cur;
        TreeNode *end = cur;
        cur->left = nullptr;
        cur->right = left.s;
        if (left.s) {
            left.s->left = nullptr;
        }

        if (left.e) {
            end = left.e;
        }

        end->right = right.s;
        if (right.s) {
            right.s->left = nullptr;
        }
        if (right.e) {
            end = right.e;
        }

        // cout << "root:" << cur->val << ",stat:" << (!start ? -1 : start->val) << ",end:" << (!end ? -1 : end->val) << endl;
        return info(start, end);
    }
    
    //很久之前的做法。两个子节点一定是左右树的开头。
    //左边的结尾一定可以通过拉平的链表找到。
    //不容易理解的写法
     void flatten2(TreeNode* root) {
        if (!root) return;
        
        //拉平左子树
        flatten(root->left);
        //拉平右子树
        flatten(root->right);
        
        //拉平的左子树放在右边
        TreeNode* ori_right = root->right;
        root->right=root->left; 
        root->left=NULL;

        //将拉平的右子树的放在左子树的最后
        TreeNode* tmp = root->right ? root->right : root;
        while(tmp && tmp->right) {
            tmp=tmp->right; 
        }
        tmp->right = ori_right;
    }
};

//前序遍历建树
static int idx = 0;
TreeNode* build_tree(string str) {
    if (idx==str.length()){
        return nullptr;
    }
    if (str[idx] == '#') {
        idx++;
        return nullptr;
    }
    // cout << "i: "<< idx << " str[i]:"<< str[idx] << endl;

    TreeNode *node = new TreeNode(str[idx] - '0');
    idx++;
    node->left = build_tree(str);
    node->right = build_tree(str);
    return node;    
}

void inorder(TreeNode* root) {
    if (!root) {
        return;
    }
    inorder(root->left);
    cout << root->val <<  " ";
    inorder(root->right);
}
int main() {

    Solution sol;
    string str = "123##4##5#6##";
    auto root = build_tree(str);
    cout << endl;
    inorder(root);
    cout << endl;
    sol.flatten(root);
    return 0;
}
