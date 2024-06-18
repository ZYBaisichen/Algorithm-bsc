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

https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/description/

给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。

百度百科中最近公共祖先的定义为：“对于有根树 T 的两个节点 p、q，最近公共祖先表示为一个节点 x，满足 x 是 p、q 的祖先且 x 的深度尽可能大（一个节点也可以是它自己的祖先）。”

 

示例 1：


输入：root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
输出：3
解释：节点 5 和节点 1 的最近公共祖先是节点 3 。
示例 2：


输入：root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4
输出：5
解释：节点 5 和节点 4 的最近公共祖先是节点 5 。因为根据定义最近公共祖先节点可以为节点本身。
示例 3：

输入：root = [1,2], p = 1, q = 2
输出：1
 

提示：

树中节点数目在范围 [2, 105] 内。
-109 <= Node.val <= 109
所有 Node.val 互不相同 。
p != q
p 和 q 均存在于给定的二叉树中。

*/

/*

*/

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:
    struct info {
        bool has_p;
        bool has_q;
        TreeNode* parent;
        info(bool p, bool q, TreeNode* par) {
            has_p = p;
            has_q = q;
            parent = par;
        }
    };
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root) {
            return nullptr;
        }
        info res = process(root, p, q);
        return res.parent;
    }
    /*
    返回以cur为节点的树上是否存在p和q。并返回结果
    */
    info process(TreeNode* cur, TreeNode* p, TreeNode* q) {
        if (!cur) {
            return info(false, false, nullptr);
        }

        info left = process(cur->left, p, q);
        info right = process(cur->right, p, q);
        if(left.parent) {
            return left;
        }
        if (right.parent) {
            return right;
        }

        bool has_p = left.has_p || right.has_p;
        TreeNode* parent = nullptr;
        if (cur == p) {
            has_p = true;
            if (left.has_q || right.has_q) {
                parent = cur;
            }
        }

        bool has_q = left.has_q || right.has_q;
        if (cur == q) {
            has_q = true;
            if (left.has_p || right.has_p) {
                parent = cur;
            }
        }
        //找到了答案则返回
        if (parent) {
            return info(has_p, has_q, parent);
        }

        //没有在子树中找到公共祖先，且cur也不是p和q的其中一个，并且两个节点都在cur树上，说明最近公共祖先是cur
        if (has_p && has_q) {
            return info(has_p, has_q, cur);
        }

        return info(has_p, has_q, nullptr);
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
