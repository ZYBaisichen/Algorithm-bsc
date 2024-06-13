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
    bool ans = true;
    TreeNode* pre=nullptr;
    bool isValidBST(TreeNode* root) {
        inorder(root);
        return ans;
    }

    void inorder(TreeNode* root) {
        if (!root) {
            return;
        }
        if (pre != nullptr && pre->val >= root->val) {
            ans = false;
            return;
        }
        inorder(root->left);
        pre = cur;
        inorder(root->right);
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
