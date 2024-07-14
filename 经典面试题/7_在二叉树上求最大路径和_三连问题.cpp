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

using namespace std;

/*
1. 路径必须从头结点出发，到叶结点为止，返回最大路径之和
2. 路径可以从任意节点出发，只能向下到达任意节点为止，返回最大路径之和
3. 路径可以从任意节点出发，到任意节点为止，返回最大路径之和： https://leetcode.cn/problems/binary-tree-maximum-path-sum/submissions/514028323/

*/

/**
 * Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
 */
struct TreeNode
{
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
    /*
    第一个问题：
    1. 可以逐步向下传递当前加和到了哪里
    2. 也可以采用二叉树递归套路，拿到左孩子路径最大为多少，然后拿到右孩子最大路径为多少。
    */
    int maxPathSum_1(TreeNode *root)
    {
        if (!root)
        {
            return 0;
        }
        return p1(root);
    }
    int p1(TreeNode *root)
    {   
        //来到了叶节点
        if (root->left==nullptr && root->right==nullptr) {
            return root->val;
        }

        //求左子树的最大路径和
        int left = p1(root->left);
        //求右子树的最大路径和
        int right = p1(root->right);

        return max(left, right) + root->val;
    }

    /*
    第二个问题：
    当遍历到x时
    1. 当最大路径和跟自己无关，向左右两树要他们树上的最大路径和
    2. 跟x有关时
        1) 就使用一个x，不再往下走
        2) x往左走获得的最大路径和
        3) x往右走获得的最大路径和
    */ 
    struct info2 {
        int all_max; //整棵树上的最大路径和
        int head_max; //从头结点上的最大路径和
        info2() {
            all_max = INT_MIN;
            head_max = INT_MIN;
        }
        info2(int _all_max, int _head_max) {
            all_max = _all_max;
            head_max = _head_max;
        }
    };

    int maxPathSum_2(TreeNode *root) {
        if (!root) {
            return 0;
        }
        info2 info = p2(root);
        return info.all_max;
    }
    
    //返回以root开始的最大路径和，又有以root为头节点的整棵树上的最大和
    info2 p2(TreeNode *root) {
        //来到了叶节点
        if (root->left==nullptr && root->right==nullptr) {
            return info2(root->val, root->val);
        }

        //获得左树信息
        info2 left;
        info2 right;
        if (root->left) { 
            left = p2(root->left);
        }
        //获得右树信息
        if (root->right) { 
            right = p2(root->right);
        }

        //当与x无关时
        int cur_all_max = max(right.all_max, left.all_max);

        //当与x有关时
        int cur_head_max = INT_MIN;
        cur_head_max = max(cur_head_max, root->val); //只有x自己
        cur_head_max = max(cur_head_max, root->val+left.head_max); //x加左孩子做头时的最大值
        cur_head_max = max(cur_head_max, root->val+right.head_max); //x加右孩子做头时的最大值


        cur_all_max = max(cur_all_max, cur_head_max);

        return info2(cur_all_max, cur_head_max);
    }

    /*
    第三个问题：
    1. x无关的时候，
        a. 求左树最大路径
        b. 求右树最大路径
    2. x有关的时候
        a. 只有x自己
        b. x只往左延伸，不往右延伸
        c. x只往右延伸，不往左延伸
        d. x往左延伸，再往右延伸（从x的左树为起点，经过x到达右树的某个点）
    */
    struct info3 {
        int all_max; //整棵树上的最大路径和
        int head_max; //从头结点上的最大路径和
        info3() {
            all_max = INT_MIN;
            head_max = INT_MIN;
        }
        info3(int _all_max, int _head_max) {
            all_max = _all_max;
            head_max = _head_max;
        }
    };
    int maxPathSum_3(TreeNode *root) {
        if (!root) {
            return 0;
        }
        info3* info = p3(root);
        return info->all_max;
    }
    info3* p3(TreeNode *root) {
        if (!root) {
            return nullptr;
        }
        info3 * left = p3(root->left);
        info3 * right = p3(root->right);
        int p1 = INT_MIN;
        if (left != nullptr) {
            p1 = left->all_max;
        }
        int p2 = INT_MIN;
        if (right != nullptr) {
            p2 = right->all_max;
        }

        //只root自己
        int p3 = root->val;
        //只往左延伸
        int p4 = INT_MIN;
        if (left != nullptr) {
            p4 = left->head_max + root->val;
        }

        //只往右延伸
        int p5 = INT_MIN;
        if (right != nullptr) {
            p5 = right->head_max + root->val;
        }

        //往两边延伸
        int p6 = INT_MIN;
        if (left != nullptr && right != nullptr) {
            p6 = left->head_max + right->head_max + root->val;
        } 

        int cur_head_max = max(p3,max(p4,p5));
        int cur_all_max = max(max(cur_head_max, max(p1, p2)), p6);
        return new info3(cur_all_max, cur_head_max);
    }

};

int main()
{
    Solution sol;
}