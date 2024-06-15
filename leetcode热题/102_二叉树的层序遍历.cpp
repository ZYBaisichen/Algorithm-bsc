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
https://leetcode.cn/problems/binary-tree-level-order-traversal/description/

给你二叉树的根节点 root ，返回其节点值的 层序遍历 。 （即逐层地，从左到右访问所有节点）。

 

示例 1：


输入：root = [3,9,20,null,null,15,7]
输出：[[3],[9,20],[15,7]]
示例 2：

输入：root = [1]
输出：[[1]]
示例 3：

输入：root = []
输出：[]
 

提示：

树中节点数目在范围 [0, 2000] 内
-1000 <= Node.val <= 1000
*/

/*
队列遍历
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
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root) {
            return vector<vector<int>>();
        }

        queue<TreeNode*> que1,que2;
        TreeNode *tmp;
        vector<vector<int>> res;
        bool flag = true;
        que1.push(root);
        while (!que1.empty() || !que2.empty()) {
            queue<TreeNode*>& cur_que = flag ? que1 : que2;
            queue<TreeNode*>& next_que = flag ? que2 : que1;
            vector<int> res_tmp;
            //每次清空队列，是一层
            while (!cur_que.empty()) {
                tmp = cur_que.front();
                res_tmp.push_back(tmp->val);
                if (tmp->left) {
                    next_que.push(tmp->left);
                }
                if (tmp->right) {
                    next_que.push(tmp->right);
                }
                cur_que.pop();
            }
            res.push_back(res_tmp);
            flag = !flag;
        }
        return res;
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
