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
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return morris(root);
    }

    //需要知道遍历到cur时候，cur的高度
    //需要判断哪个节点是否是叶节点：第二次到达时，判断most_right的左树是否为空，为空则表示most_right为叶节点，其高度就是要用来pk的高度
    bool morris(TreeNode* root) {
        if (!root) {
            return true;
        }
        TreeNode* most_right = nullptr;
        TreeNode* cur = root;
        TreeNode* pre = nullptr;
        bool ans = true;

        while (cur != nullptr) {
            if (cur->left) {
                most_right = cur->left;
                while (most_right->right && most_right->right != cur) {
                    most_right = most_right->right;
                }
                if (most_right->right == nullptr) { //第一次到达
                    most_right->right = cur;
                    cur = cur->left;
                } else { 
                    if (pre!=nullptr && cur->val <= pre->val) {
                        ans = false;
                    } else {
                        pre = cur;
                    }
                    most_right->right = nullptr;
                    cur = cur->right;
                }
            } else { //只到达一次，不会再回来
                if (pre!=nullptr && cur->val <= pre->val) {
                    ans = false;
                } else {
                    pre = cur;
                }
                cur = cur->right;
            }
        }
        return ans;
    }
};