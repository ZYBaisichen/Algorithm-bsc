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
https://leetcode.cn/problems/validate-binary-search-tree/
给你一个二叉树的根节点 root ，判断其是否是一个有效的二叉搜索树。

有效 二叉搜索树定义如下：

节点的左
子树
只包含 小于 当前节点的数。
节点的右子树只包含 大于 当前节点的数。
所有左子树和右子树自身必须也是二叉搜索树。
 

示例 1：


输入：root = [2,1,3]
输出：true
示例 2：


输入：root = [5,1,4,null,null,3,6]
输出：false
解释：根节点的值是 5 ，但是右子节点的值是 4 。
 

提示：

树中节点数目范围在[1, 104] 内
-231 <= Node.val <= 231 - 1
*/

/*
二叉搜索树的前序遍历一定是升序的，所以只需要记录一个前序节点，就可以判断是否满足条件

*/

class Solution
{
public:
    TreeNode* pre=nullptr;
    bool  ans = true;
    bool isValidBST(TreeNode* root) {
        if (!root) {
            return true;
        }
        inorder(root);
        return ans;
    }
    void inorder(TreeNode* root) {
        if (!root) {
            return;
        }
        
        inorder(root->left);
        //要遍历root时看前面的节点和自己的值哪个大
        if (pre != nullptr && pre->val >= root->val) {
            ans = false;
            return;
        }
        pre = root;
        inorder(root->right);
    }


    //二叉树递归套路
    bool isValidBST_info(TreeNode* root) {
        if(!root) {
            return true;
        }
        info res = isValidBST_core(root);
        return res.is_bst;

    }
    struct info {
        int max_num;
        int min_num;
        bool is_bst;
        bool is_null;
        info(int m, int n, bool bst, bool null) {
            max_num = m;
            min_num = n;
            is_bst = bst;
            is_null = null;
        }
    };
    info isValidBST_core(TreeNode* root) {
        if (!root) {
            return info(INT_MIN, INT_MAX, true, true);
        }
        if (!root->left && !root->right) {
            return info(root->val, root->val, true, false);
        }
        info left = isValidBST_core(root->left);
        info right = isValidBST_core(root->right);
        bool is_bst = left.is_bst && right.is_bst && ((left.max_num < root->val && right.min_num > root->val) || (right.is_null && left.max_num<root->val) || (left.is_null && right.min_num>root->val));
        int max_num = max(right.max_num, max(root->val, left.max_num));
        int min_num = min(right.min_num, min(root->val, left.min_num));
        // cout << "root:" << root->val << " max_num:" << max_num << " min_num:" << min_num << " is_bst:" << is_bst<< endl;
        return info(max_num, min_num, is_bst, false);
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
