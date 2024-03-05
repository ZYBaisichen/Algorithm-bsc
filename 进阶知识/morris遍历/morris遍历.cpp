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

//https://leetcode.cn/problems/binary-tree-postorder-traversal/
//力扣94、144、145题
struct TreeNode{
public:
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

/*
当前节点cur，一开始cur来到整棵树的头(cur走到null结束)
1. cur无左树，cur=cur.right
2. cur有左树，找到左树的最右节点，mostRight
    a. mostRight的右指针指向null时，mostRight.right=cur, cur=cur.left
    b. mostRight的右指针指向cur时，将mostRight.right=null, cur=cur.right
*/
class Solution{
public:
    //经典morris遍历，自己实现的版本
    void morris(TreeNode* root) {
        if (!root) {
            return;
        }
        TreeNode* most_right = nullptr;
        TreeNode* cur = root;
        while (cur != nullptr) {
            if (cur->left) {
                most_right = cur->left;
                while (most_right->right && most_right->right != cur) {
                    most_right = most_right->right;
                }
                if (most_right->right == nullptr) {
                    most_right->right = cur;
                    cur = cur->left;
                } else {
                    most_right->right = nullptr;
                    cur = cur->right;
                }
            } else {
                cur = cur->right;
            }
        }
    }

    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> res;
        morris_pre(root, res);
        return res;
    }

    //morris前序遍历
    //走两次的节点，在第一次打印时，即为前序遍历
    //走一次的节点直接打印
    void morris_pre(TreeNode* root, vector<int>& res) {
        if (!root) {
            return;
        }
        TreeNode* most_right = nullptr;
        TreeNode* cur = root;
        while (cur != nullptr) {
            if (cur->left) {
                most_right = cur->left;
                while (most_right->right && most_right->right != cur) {
                    most_right = most_right->right;
                }
                if (most_right->right == nullptr) { //左树的最右节点的右指针指向空，说明第一次来到cur
                    res.push_back(cur->val);
                    most_right->right = cur;
                    cur = cur->left;
                } else {
                    most_right->right = nullptr;
                    cur = cur->right;
                }
            } else { //没有左树
                //没有左树，只会到达一次，直接打印
                res.push_back(cur->val);
                cur = cur->right;
            }
        }
    }

    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        morris_inorder(root, res);
        return res;
    }

    //morris中序遍历
    //走两次的节点，在第二次打印时，即为中序遍历
    //走一次的节点直接打印
    void morris_inorder(TreeNode* root, vector<int>& res) {
        if (!root) {
            return;
        }
        TreeNode* most_right = nullptr;
        TreeNode* cur = root;
        while (cur != nullptr) {
            if (cur->left) {
                most_right = cur->left;
                while (most_right->right && most_right->right != cur) {
                    most_right = most_right->right;
                }
                if (most_right->right == nullptr) { //左树的最右节点的右指针指向空，说明第一次来到cur
                    most_right->right = cur;
                    cur = cur->left;
                } else { ////左树的最右节点的右指针指向自己，说明第二次来到cur
                    res.push_back(cur->val);
                    most_right->right = nullptr;
                    cur = cur->right;
                }
            } else { //没有左树
                //没有左树，只会到达一次，直接打印
                res.push_back(cur->val);
                cur = cur->right;
            }
        }
    }


    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> res;
        morris_postorder(root, res);
        return res;
    }

    //morris后序遍历
    //每次第二次来到节点时，逆序遍历其左树的右边界
    //最后处理从头结点开始的最右边界
    void morris_postorder(TreeNode* root, vector<int>& res) {
        if (!root) {
            return;
        }
        TreeNode* most_right = nullptr;
        TreeNode* cur = root;
        while (cur != nullptr) {
            if (cur->left) {
                most_right = cur->left;
                while (most_right->right && most_right->right != cur) {
                    most_right = most_right->right;
                }
                if (most_right->right == nullptr) { //左树的最右节点的右指针指向空，说明第一次来到cur
                    most_right->right = cur;
                    cur = cur->left;
                } else { ////左树的最右节点的右指针指向自己，说明第二次来到cur
                    most_right->right = nullptr;
                    //第二次来到时，先处理左树最右节点的右指针，然后打印左树的右边界
                    print_list(cur->left, res);
                    cur = cur->right;
                }
            } else { //没有左树
                cur = cur->right;
            }
        }
        //打印以root为头的右边界
        print_list(root, res);
    }
    void print_list(TreeNode* root, vector<int> &res) {
        if (root == nullptr) {
            return;
        }
        TreeNode* tail = reverse(root);
        TreeNode* cur = tail;
        while (cur) {
            res.push_back(cur->val);
            cur = cur->right;
        }
        reverse(tail);
    }

    TreeNode* reverse(TreeNode* root) {
        TreeNode *pre = nullptr, *cur = root, *next = nullptr;
        while (cur) {
            next = cur->right;
            cur->right = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }

    //经典morris遍历，左神经典版本
    void morrisZuoshen(TreeNode* root) {
        if (!root) {
            return;
        }
        TreeNode* most_right = nullptr;
        TreeNode* cur = root;
        while (cur != nullptr) {
            if (cur->left) {
                most_right = cur->left;
                while (most_right->right && most_right->right != cur) {
                    most_right = most_right->right;
                }
                if (most_right->right == nullptr) {
                    most_right->right = cur;
                    cur = cur->left;
                    continue;
                } else {
                    most_right->right = nullptr;
                }
            }
            cur = cur->right;
        }
    }
};

int main() {

}