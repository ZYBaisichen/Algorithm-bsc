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

using namespace std;


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
//https://leetcode-cn.com/problems/minimum-depth-of-binary-tree/
class Solution {
public:
    int ans = INT_MAX;
    int minDepth(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        return morris(root);
    }

    //需要知道遍历到cur时候，cur的高度
    //需要判断哪个节点是否是叶节点：第二次到达时，判断most_right的左树是否为空，为空则表示most_right为叶节点，其高度就是要用来pk的高度
    //用一个cur_level记录当前节点所在的深度
    int morris(TreeNode* root) {
        if (!root) {
            return 0;
        }
        TreeNode* most_right = nullptr;
        TreeNode* cur = root;
        int cur_level = 0;
        int min_height = INT_MAX;

        while (cur != nullptr) {
            if (cur->left) {
                most_right = cur->left;
                int right_board_size = 1;
                while (most_right->right && most_right->right != cur) {
                    most_right = most_right->right;
                    right_board_size++;
                }
                if (most_right->right == nullptr) { //第一次到达
                    cur_level++; //更新的是当前cur的高度
                    most_right->right = cur;
                    cur = cur->left;
                } else { 
                    if (most_right->left == nullptr) {
                        //most_right是叶节点
                        min_height = min(min_height, cur_level);
                    }
                    cur_level -= right_board_size; //从左树最右节点上来的，当前层级从该节点为基准减一下
                    most_right->right = nullptr;
                    cur = cur->right;
                }
            } else { //只到达一次，不会再回来
                cur_level++;
                cur = cur->right;
            }
        }
        //处理整棵树的最右边界
        int head_right = 1;
        cur = root;
        while (cur->right) {
            head_right++;
            cur = cur->right;
        }
        if (cur->left==nullptr && cur->right==nullptr) {
            min_height = min(min_height, head_right);
        }
        return min_height;
    }
    

    int minDepthDigui(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        ans = INT_MAX;
        return minDepthCore(root);
    }
    int minDepthCore(TreeNode* root) {
        if (root->left == nullptr && root->right == nullptr) {
            return 1;
        }

        int left_h = INT_MAX;
        if (root->left) {
            left_h = minDepthCore(root->left);
        }

        int right_h = INT_MAX;
        if (root->right) {
            right_h = minDepthCore(root->right);
        }
        return min(left_h, right_h) + 1;
    }
    
};