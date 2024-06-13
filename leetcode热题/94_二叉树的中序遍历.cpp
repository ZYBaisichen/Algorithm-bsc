/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-13 11:43:06
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
https://leetcode.cn/problems/binary-tree-inorder-traversal/description/
给定一个二叉树的根节点 root ，返回 它的 中序 遍历 。



示例 1：


输入：root = [1,null,2,3]
输出：[1,3,2]
示例 2：

输入：root = []
输出：[]
示例 3：

输入：root = [1]
输出：[1]


提示：

树中节点数目在范围 [0, 100] 内
-100 <= Node.val <= 100


进阶: 递归算法很简单，你可以通过迭代算法完成吗？
*/

/*
复习下morris遍历
morris遍历核心思想是使用树的右边界将整棵树拆分遍历。
当前来到了cur节点，一开始cur在root处：
1. cur无左树，则cur=cur.right
2. cur有左树，找到左树的最优节点,most_right
    a. most_right的右指针指向null时，表示是第一次来到cur，令: most_right->right=cur, cur=cur->left
    b. most_right的右指针不为空时，表示是第二次来到cur，令most_right->right=null, cur=cur.right
*/
struct TreeNode
{
public:
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution
{
public:
    vector<int> inorderTraversal(TreeNode *root)
    {
        vector<int> res;
        morris_inorder(root, res);
        return res;
    }

    void morris_inorder(TreeNode *cur, vector<int> &res)
    {
        while (cur != nullptr)
        {
            if (cur->left == nullptr)
            { // 没有左树
                res.push_back(cur->val);
                cur = cur->right;
            }
            else
            {
                TreeNode *most_right = cur->left;
                while (most_right->right && most_right->right != cur)
                {
                    most_right = most_right->right;
                }
                if (most_right->right == nullptr)
                {
                    most_right->right = cur;
                    cur = cur->left;
                }
                else
                { // 第二次来到cur，中序遍历结果
                    most_right->right = nullptr;
                    res.push_back(cur->val);
                    cur = cur->right;
                }
            }
        }
    }
};
int main()
{

    Solution sol;
    
    return 0;
}
