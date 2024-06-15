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
https://leetcode.cn/problems/symmetric-tree/description/

给你一个二叉树的根节点 root ， 检查它是否轴对称。

 

示例 1：


输入：root = [1,2,2,3,4,4,3]
输出：true
示例 2：


输入：root = [1,2,2,null,3,null,3]
输出：false
 

提示：

树中节点数目在范围 [1, 1000] 内
-100 <= Node.val <= 100
 

进阶：你可以运用递归和迭代两种方法解决这个问题吗？
*/

/*
要想二叉树轴对称，需要：
1. 每个节点的左右两个孩子相等。
2. 然后是左孩子的右孩子和右孩子的左孩子相同
3. 左孩子的左孩子和右孩子的右孩子相同

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
    //递归
    bool isSymmetric1(TreeNode* root) {
        if (!root) {
            return true;
        }
        return process(root->left, root->right);
    }   
    //比较两颗子树相同
    bool process(TreeNode* left, TreeNode* right) {
        if (left == nullptr && right == nullptr) {
            return true;
        }
        if (left == nullptr || right == nullptr) {
            return false;
        }

        if (left->val != right->val) {
            return false;
        }

        return process(left->left, right->right) && process(left->right, right->left);
    }

    //队列
    bool isSymmetric(TreeNode* root) {
        if (!root) {
            return true;
        }
        queue<TreeNode*> que;
        //每次插入需要比较的
        que.push(root->left);
        que.push(root->right);
        TreeNode* a, *b;
        while (!que.empty()) {
            a = que.front();
            que.pop();
            b = que.front();
            que.pop();

            if (a == nullptr && b == nullptr) {
                continue;
            }
            if (a == nullptr || b == nullptr) {
                return false;
            }
            if (a->val != b->val) {
                return false;
            }
            
            //下次比较左孩子的左孩子和右孩子的右孩子
            que.push(a->left);
            que.push(b->right);
            //下次比较左孩子的右孩子和右孩子的左孩子
            que.push(a->right);
            que.push(b->left);
        }
        return true;
    }   

    //中序和逆中序序列
    //在{1,2,2,2,null,2}树下，下面方法行不通
    bool isSymmetric(TreeNode* root) {
        if (!root) {
            return true;
        }
        vector<int> a,b;
        cout << "inorder:";
        inorder(root, a);
        cout << endl << " deinorder:";
        deinorder(root, b);
        cout << endl;
        for (int i=0;i<a.size();i++) {
            cout << "a:" << a[i] << " b:" << b[i] << endl; 
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }   
    void inorder(TreeNode* root, vector<int>& nums) {
        if (!root) {
            cout << "-200 ";
            nums.push_back(-200);
            return;
        }
        inorder(root->left, nums);
        cout << root->val << " ";
        nums.push_back(root->val);
        inorder(root->right, nums);
    }
    void deinorder(TreeNode* root, vector<int>& nums) {
        if (!root) {
            cout << "-200 ";
            nums.push_back(-200);
            return;
        }
        deinorder(root->right, nums);
        cout << root->val << " ";
        nums.push_back(root->val);
        deinorder(root->left, nums);
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
