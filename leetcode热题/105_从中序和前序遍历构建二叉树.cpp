/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-13 14:46:14
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
https://leetcode.cn/problems/construct-binary-tree-from-preorder-and-inorder-traversal/
给定两个整数数组 preorder 和 inorder ，其中 preorder 是二叉树的先序遍历， inorder 是同一棵树的中序遍历，请构造二叉树并返回其根节点。

 

示例 1:


输入: preorder = [3,9,20,15,7], inorder = [9,3,15,20,7]
输出: [3,9,20,null,null,15,7]
示例 2:

输入: preorder = [-1], inorder = [-1]
输出: [-1]
 

提示:

1 <= preorder.length <= 3000
inorder.length == preorder.length
-3000 <= preorder[i], inorder[i] <= 3000
preorder 和 inorder 均 无重复 元素
inorder 均出现在 preorder
preorder 保证 为二叉树的前序遍历序列
inorder 保证 为二叉树的中序遍历序列
*/

/*


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
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        int len_p = preorder.size();
        int len_i = preorder.size();
        if (len_p != len_i) {
            return nullptr;
        }
        map<int, int> _map;
        for (int i=0;i<inorder.size();i++) {
            _map[inorder[i]] = i;
        }
        TreeNode* root=nullptr;
        root = process(preorder, inorder, 0, len_p-1, 0, len_i-1, _map);
        return root;
    }
    /*
    在前序遍历的[pl,pr]和中序遍历的[il,ir]范围上建立树
    使用一个map快速找到root节点在中序序列中的位置
    */
    TreeNode* process(vector<int>& preorder, vector<int>& inorder, int pl, int pr, int il, int ir, map<int, int>& _map) {
        int len = preorder.size();
        if (pl>pr || il > ir || pl<0 || pr<0 || pl>=len || pr>=len || il<0 || ir<0 || il>=len || ir>=len) {
            return nullptr;
        }
        if (pl == pr) {
            return (new TreeNode(preorder[pl]));
        }

        int root_val = preorder[pl];
        TreeNode* root = new TreeNode(root_val);
        int t = -1, left_num = 0;

        //在中序序列中找到root的位置
        // for (int i=il; i<=ir; i++) {
        //     if (root_val == inorder[i]) {
        //         t = i;
        //         break;
        //     }
        //     left_num++;
        // }
        t = _map[root_val];
        left_num = t - il;
        
        //建立左子树
        root->left = process(preorder, inorder, pl+1, pl+left_num,  il, t-1,_map);
        root->right = process(preorder, inorder, pl+left_num+1, pr, t+1, ir,_map);

        return root;
    }
};
int main()
{

    Solution sol;
    int n=19;
    cout << sol.numTrees(n) << endl;
    cout << sol.numTrees2(n) << endl;
    return 0;
}
