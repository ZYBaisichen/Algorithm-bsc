/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-24 18:52:38
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

using namespace std;

// 求完全二叉树的节点个数
// 测试链接 : https://leetcode.cn/problems/count-complete-tree-nodes/

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
public:
    //O(N)
    //使用二叉树递归套路
    int countNodes(TreeNode* root) {
        return process(root);
    }
    int process(TreeNode* root) {
        if (root != nullptr) {
            return 0;
        }

        int left = process(root->left);
        int right = process(root->right);

        return left+right+1;
    }

    //O(log(n))解法
    int countNodes(TreeNode* root) {
        if (!root) {
            return 0;
        }
        return process1(root, 1, most_left_hight(root, 1));
    }

    //从level层开始，往左看能走到第几层
    int most_left_hight(TreeNode* cur, int level) { //log(n)
        while (cur) {
            level++;
            cur=cur->left;
        } 
        return level-1;
    }
    /*
    根据完全二叉树定义，要么是全满，要么是最后一层从左到右填满。
    1. 对于遍历到的x节点，先找到最左节点，为这个树的深度h。  
    2. 然后找到右子树的最左节点，可以得到右子树的高度
        a. 右子树的高度如果为h，说明左子树满了，左子树节点数为2^h-1。右子树还需要继续递归求
        b. 如果右子树高度如果为h-1， 说明右子树是满的，左子树不满。右子树节点数是2^(h-1)。左子树还需要递归求
    
    process1含义：当前遍历到了root，在第level层，总高度是h，返回root节点下的总节点数
    */
    int process1(TreeNode* root, int level, int h) {
        /*
            
            每次找右树的最左节点走的次数：h，h-1, h-2...，所以需要O(h^2)的复杂度
            而二叉树的高度h=log(n)，所以总复杂度是O((log(n))^2)


        */
        
        if (level == h) { //到了最后一层
            return 1;
        }
        
        int right_h = most_left_hight(root->right, level+1);
        if (right_h == h) { //左树满了，往右树上找
            int left_num = (1<<(h-level)) - 1;
            return (left_num + 1) + process1(root->right, level+1, h);
        } else { //否则右树是满的
            int right_num = (1<<(h-level-1)) -1;
            return (right_num+1) + process1(root->left, level+1, h);
        }
    }
};

//生成长度为len，有v种字符的字符串
string random_string(int len, int v) {
    string res;
    for (int i = 0; i < len; ++i) {
        res += rand() % v + 'a';
    }
    return res;
}
int main()
{
    Solution sol;

    int n = 10;
    int v = 3;
    int testTime = 100;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++) {
        int len1 = (int) (rand() % n) + 1;
        int len2 = (int) (rand() % n) + 1;
        string s1 = random_string(len1, v);
        string s2 = random_string(len2, v);
        // cout<< "s1: " << s1 << ",  s2:" << s2 << endl;
        int ans1 = sol.minDelete1(s1, s2);
        int ans2 = sol.minDelete2(s1, s2);
        int ans3 = sol.minDelete3(s1, s2);
        // cout << "ans1: " << ans1 << ",  ans2:" << ans2 << ",  ans3:" << ans3 << endl;
        if (ans1 != ans2) {
            cout << "出错了!" << endl;
        }
        if (ans1 != ans3) {
            cout << "出错了! ans3:"  << s1 <<  ", "<<s2<< endl;
        }

        if (ans2 != ans3) {
            cout << "出错了! ans3:"  << s1 <<  ", "<<s2<< endl;
        }
    }
    cout << "测试结束" << endl;


    // string s1= "bbbbc";
    // string s2 = "aa";
    // //输出6
    // cout << sol.minDelete1(s1,s2) << endl;
    // cout << sol.minDelete2(s1,s2) << endl;
    // cout << sol.minDelete3(s1, s2) << endl;
    return 0;
}