/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 11:07:35
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

https://leetcode.cn/problems/invert-binary-tree/
给你一棵二叉树的根节点 root ，翻转这棵二叉树，并返回其根节点。

 

示例 1：



输入：root = [4,2,7,1,3,6,9]
输出：[4,7,2,9,6,3,1]
示例 2：



输入：root = [2,1,3]
输出：[2,3,1]
示例 3：

输入：root = []
输出：[]
*/

/*
每个节点的左右孩子交换
*/
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) {
            return root;
        }
        process(root);
        return root;
    }
    void process(TreeNode* root) {
        if (!root) {
            return;
        }
        process(root->left);
        process(root->right);
        auto* tmp = root->left;
        root->left = root->right;
        root->right = tmp;
    }
};

int main()
{

    Solution sol;
    vector<int> arr = {3,2,1,5,6,4}; 
    int k = 2;
    cout << sol.findKthLargest(arr, k) << endl;;
    return 0;
}
