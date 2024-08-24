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

https://leetcode.cn/problems/merge-two-binary-trees/description/
给你两棵二叉树： root1 和 root2 。

想象一下，当你将其中一棵覆盖到另一棵之上时，两棵树上的一些节点将会重叠（而另一些不会）。你需要将这两棵树合并成一棵新二叉树。合并的规则是：如果两个节点重叠，那么将这两个节点的值相加作为合并后节点的新值；否则，不为 null 的节点将直接作为新二叉树的节点。

返回合并后的二叉树。

注意: 合并过程必须从两个树的根节点开始。

 

示例 1：


输入：root1 = [1,3,2,5], root2 = [2,1,3,null,4,null,7]
输出：[3,4,5,5,4,null,7]
示例 2：

输入：root1 = [1], root2 = [1,2]
输出：[2,2]
 

提示：

两棵树中的节点数目在范围 [0, 2000] 内
-104 <= Node.val <= 104
*/

/*


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

class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        if (!root1 && !root2) {
            return root1;
        }
        if (!root1) {
            return root2;
        }
        if (!root2) {
            return root1;
        }
        process(root1, root1, true, root2, root2);
        return root1;
    }

    /*
    前序遍历两棵树，将root2的树复制到root1上。
    pre1和pre2记录从哪个父节点遍历到此节点。
    因为需要知道从哪个地方来的，所以需要加一个left_flag，用于标记当前节点是否是父节点的左子树。（对于叶节点的情况，父节点两个指针都是空，所以需要有一个这样的标记）
    */
    void process(TreeNode* cur1, TreeNode* pre1, bool left_flag, TreeNode* cur2, TreeNode* pre2) {
        // cout << "cur1:" << (cur1? cur1->val:-1) << " pre1:" << pre1->val << " cur2:" << (cur2? cur2->val:-1)  << " pre2:" << pre2->val << endl;
        if (!cur1 && !cur2) {
            return;
        }
        //cur1为空，不需要做什么操作
        if (cur1 && !cur2) {
            return;
        }
        if (!cur1 && cur2) {
            if (left_flag) {
                pre1->left = cur2;
            } else {
                pre1->right = cur2;
            }
            return;
        }

        pre1 = cur1;
        pre2 = cur2;
        cur1->val += cur2->val;
        process(cur1->left, pre1, true, cur2->left, pre2);
        process(cur1->right, pre1, false, cur2->right, pre2);
    }

    //简单版本
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        root1 = mergeTreesCore(root1, root2);
        return root1;
    }

    //将root2往root1上合
    TreeNode* mergeTreesCore(TreeNode* root1, TreeNode* root2)  {
        if (!root1) {
            return root2;
        }
        if (!root2) {
            return root1;
        }

        //运行到这里说明root1和root2都不为空
        root1->val += root2->val;
        root1->left = mergeTreesCore(root1->left, root2->left);
        root1->right = mergeTreesCore(root1->right, root2->right);

        return root1;
    }
};

int main()
{
    Solution sol;

    return 0;
}
