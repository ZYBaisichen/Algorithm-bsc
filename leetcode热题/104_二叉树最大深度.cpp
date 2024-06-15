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
https://leetcode.cn/problems/maximum-depth-of-binary-tree/description/

给定一个二叉树 root ，返回其最大深度。

二叉树的 最大深度 是指从根节点到最远叶子节点的最长路径上的节点数。

 

示例 1：



 

输入：root = [3,9,20,null,null,15,7]
输出：3
示例 2：

输入：root = [1,null,2]
输出：2
 

提示：

树中节点的数量在 [0, 104] 区间内。
-100 <= Node.val <= 100
*/

/*
一个方法是二叉树递归套路
cur节点左右两个节点分别返回自己的最大高度，取最大值然后再加1，就是以cur为头的树的最大深度

还有个方法是，维护一个全局最大深度，每次dfs到叶结点的时候更新一遍答案


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
    int maxDepth(TreeNode* root) {
        return process(root);
    }

    int process(TreeNode* root) {
        if (!root) {
            return 0; //空节点的高度是0
        }

        int left = process(root->left);
        int right = process(root->right);
        return max(left, right) + 1;
    }

    int ans = 0;
    int maxDepth(TreeNode* root) {
        ans = 0;
        if (!root) {
            return ans;
        }
        dfs(root, 0);
        return ans;
    }
    void dfs(TreeNode* root, int depth){
        if (!root->left && !root->right) {
            // cout << "root:" << root->val << " depth:" << depth << endl;
            depth++;
            ans = max(ans, depth);
            return;
        }

        depth++;
        if (root->left) {
            dfs(root->left, depth);
        }

        if (root->right) {
            dfs(root->right, depth);
        }
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
