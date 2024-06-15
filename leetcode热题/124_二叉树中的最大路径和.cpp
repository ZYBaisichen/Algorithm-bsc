/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
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
和经典面试题7相同

https://leetcode.cn/problems/binary-tree-maximum-path-sum/
二叉树中的 路径 被定义为一条节点序列，序列中每对相邻节点之间都存在一条边。同一个节点在一条路径序列中 至多出现一次 。该路径 至少包含一个 节点，且不一定经过根节点。

路径和 是路径中各节点值的总和。

给你一个二叉树的根节点 root ，返回其 最大路径和 。



示例 1：


输入：root = [1,2,3]
输出：6
解释：最优路径是 2 -> 1 -> 3 ，路径和为 2 + 1 + 3 = 6
示例 2：


输入：root = [-10,9,20,null,null,15,7]
输出：42
解释：最优路径是 15 -> 20 -> 7 ，路径和为 15 + 20 + 7 = 42


提示：

树中节点数目范围是 [1, 3 * 104]
-1000 <= Node.val <= 1000
*/

/*
树形dp的典型应用：
当前在cur节点时，分为两种情况：
1. 最大路径和的路径和cur节点无关
2. 最大路径和的路径和cur节点有关，并以cur为开头

到cur时，首先获得以左右两个孩子为头的节点的信息，信息包含：
1. 从左右孩子开始往下的最大路径和
2. 以左右孩子为头的树的全局最大路径和

加工出当前节点的最大路径和的信息：
1. 不经过cur节点：左右两个子树全局最大路径和的最大值
2. 和cur有关时：
    a. 以cur节点，为头，往左或者往右延伸的路径最大和
    b. 只cur自己一个节点
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
class Solution
{
public:
    struct info
    {
        int all_max;
        int head_max;
        info() {
            all_max = INT_MIN;
            head_max = INT_MIN;
        }
        info(int all, int head) {
            all_max = all;
            head_max = head;
        }
    };
    int maxPathSum(TreeNode *root) {
        if (!root) {
            return 0;
        }
        info res = process(root);
        return res.all_max;
    }
    info process(TreeNode* cur) {
        if (cur == nullptr) {
            return info(-2000,-2000);
        }

        info l_info = process(cur->left);
        info r_info = process(cur->right);

        int head_max = INT_MIN;
        int all_max = INT_MIN;
        int p1 = max(l_info.all_max, r_info.all_max); //不经过cur节点，左右两个子树的最大路径和

        int p2= cur->val; //仅自己
        int p3 = cur->val + l_info.head_max; //以cur为头，往左延伸的最大路径和
        int p4 = cur->val + r_info.head_max;//以cur为头，往右延伸的最大路径和
        int p5 = cur->val + l_info.head_max + r_info.head_max;//以cur为头，左右两侧都有
        
        head_max = max({p2, p3, p4});
        all_max = max({p1, p5, head_max});
        // cout << "cur:" << cur->val << " head_max:" << head_max << " all_max:" << all_max << endl;
        return info(all_max,head_max);
    }
};

int main()
{

    Solution sol;
    return 0;
}
