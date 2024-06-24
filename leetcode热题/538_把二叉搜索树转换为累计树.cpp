/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-24 15:18:04
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
#include <list>

using namespace std;

/*

https://leetcode.cn/problems/convert-bst-to-greater-tree/description/

给出二叉 搜索 树的根节点，该树的节点值各不相同，请你将其转换为累加树（Greater Sum Tree），使每个节点 node 的新值等于原树中大于或等于 node.val 的值之和。

提醒一下，二叉搜索树满足下列约束条件：

节点的左子树仅包含键 小于 节点键的节点。
节点的右子树仅包含键 大于 节点键的节点。
左右子树也必须是二叉搜索树。
注意：本题和 1038: https://leetcode-cn.com/problems/binary-search-tree-to-greater-sum-tree/ 相同

 

示例 1：



输入：[4,1,6,0,2,5,7,null,null,null,3,null,null,null,8]
输出：[30,36,21,36,35,26,15,null,null,null,33,null,null,null,8]
示例 2：

输入：root = [0,null,1]
输出：[1,null,1]
示例 3：

输入：root = [1,0,2]
输出：[3,3,2]
示例 4：

输入：root = [3,2,4,1]
输出：[7,9,4,10]
 

提示：

树中的节点数介于 0 和 104 之间。
每个节点的值介于 -104 和 104 之间。
树中的所有值 互不相同 。
给定的树为二叉搜索树。
*/

/*


*/

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

/*
初步思路：遍历递归
  1. 因为每个数要设置为大于等于它自己的数的累加和。根据二叉搜索树的性质，任何一个节点的父节点和其右兄弟树上的所有节点都比自己大
  2. 所以设置一个累加和，遍历方式是右->中->左。依次累加，就得到了遍历到的cur节点的变化后的值

反morris遍历：（morris遍历是按照右边界拆分整棵树，反morris遍历是按照左边界拆分整棵树）
当前来到了cur节点，一开始cur在root处：
1. cur无右树，则cur=cur.left
2. cur有右树，找到右树的最左节点,most_left
    a. most_left的左指针指向null时，表示是第一次来到cur，令: most_left->left=cur, cur=cur->right
    b. most_left的左指针不为空时，表示是第二次来到cur，令most_left->left=null, cur=cur.left

*/

class Solution {
public:
    TreeNode* convertBST1(TreeNode* root) {
        int sum = 0;
        process(root, sum);
        return root;
    }

    void  process(TreeNode* root, int& sum) {
        if (!root) {
            return;
        }
        process(root->right, sum);
        sum+=root->val;
        root->val = sum;
        process(root->left, sum);
    }

    TreeNode* convertBST(TreeNode* root) {
        if (!root) {
            return root;
        }

        TreeNode* cur = root, *most_left=nullptr;
        int sum = 0;
        while (cur) {
            if (!cur->right) { //无右树，第一次和第二次都是自己
                sum+=cur->val;
                cur->val=sum;
                // cout << cur->val <<" ";
                cur = cur->left;
            } else { //有右树
                most_left = cur->right;
                while (most_left->left && most_left->left != cur) {
                    most_left = most_left->left;
                }
                if (most_left->left == nullptr) {
                    most_left->left=cur;
                    cur=cur->right;
                } else { //第二次来到自己
                    sum+=cur->val;
                    cur->val=sum;
                    // cout << cur->val<<" ";
                    most_left->left = nullptr;
                    cur=cur->left;
                }
            }
        }
        return root;
    }
};

int main()
{
    Solution sol;

    return 0;
}
