/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-23 12:59:02
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

https://leetcode.cn/problems/partition-equal-subset-sum/description/

给定一个二叉树的根节点 root ，和一个整数 targetSum ，求该二叉树里节点值之和等于 targetSum 的 路径 的数目。

路径 不需要从根节点开始，也不需要在叶子节点结束，但是路径方向必须是向下的（只能从父节点到子节点）。

 

示例 1：



输入：root = [10,5,-3,3,2,null,11,3,-2,null,1], targetSum = 8
输出：3
解释：和等于 8 的路径有 3 条，如图所示。
示例 2：

输入：root = [5,4,8,11,null,13,4,7,2,null,null,5,1], targetSum = 22
输出：3
 

提示:

二叉树的节点个数的范围是 [0,1000]
-109 <= Node.val <= 109 
-1000 <= targetSum <= 1000 
*/

/*

*/

 
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
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
    int pathSum(TreeNode* root, int targetSum) {
        if (!root) {
            return 0;
        }
        int res = 0;
        process(root, res, targetSum);
        return res;
    }

    /*
    中序遍历，每经过一个节点时，往下寻找目标和等于targetSum的数
    */
    void process(TreeNode* cur, int& res, int targetSum) {
        if (!cur) {
            return;
        }
        
        cout << "===begin find:" << cur->val << endl;
        res += get_sum_num(cur, targetSum);
        cout << "===end find:" << cur->val << " res:"<< res << endl;
        process(cur->left, res, targetSum);
        process(cur->right, res, targetSum);
    }
    
    //当前遍历到了cur为止，还有rest需要累加
    int get_sum_num(TreeNode* cur, double rest) {
        if (!cur) { //rest没到0
            return 0;
        }
        // cout << "begin: cur:" << cur->val << " rest:" << rest << endl;
        int ans = 0;
        if (cur->val==rest) {
            ans += 1;
        }
        ans += get_sum_num(cur->left, rest - cur->val);
        ans += get_sum_num(cur->right, rest - cur->val);
        // cout << "begin: cur:" << cur->val << " rest:" << rest << " res:" << ans<< endl;
        return ans;
    }

    /*
    在数组中查找有多少个累加和为target的数组时，只需要前缀和sum，配合记录了前面有多少个sum-target的前缀数组就可以
    运用到二叉树上，只需要在前序深度遍历的路径上记录累加和，并记录在累加过程中前面出现了多少次累加和是sum-target的数组
    */

    int pathSum(TreeNode* root, int targetSum) {
        if (!root) {
            return 0;
        }
        int res = 0;
        map<double,int> _map;
        _map[0] = 1;
        res = preorder(root, 0, 0, _map, targetSum);
        return res;
    }

    //从root出发的两条路径，当前在level层
    int preorder(TreeNode* root, int level, double sum, map<double,int>& _map, int target) {
        //来到末尾了，不用计算了
        if (!root) {
            return 0;
        }

        //来到了root，将累加和累加上
        sum += root->val;
        int ans = 0;

        // cout << "cur:" << root->val << " sum:" << sum << endl;
        // cout << "map:";
        // for (auto it:_map) {
        //     cout << it.first << "," << it.second << " ";
        // }
        // cout << endl;

        if (_map.find(sum-target) != _map.end()) {
            ans+=_map[sum-target];
        }
        
        // cout << "cur:" << root->val << " sum:" << sum << " ans:" << ans<< endl;
        // cout << "map:";
        // for (auto it:_map) {
        //     cout << it.first << "," << it.second << " ";
        // }
        // cout << endl;

        if (_map.find(sum) == _map.end()) {
            _map[sum] = 0;
        }
        _map[sum]++;
        ans += preorder(root->left, level+1, sum, _map, target);
        ans += preorder(root->right, level+1, sum, _map, target);
        _map[sum]--;
        sum -=root->val;
        return ans;
    }


};

int main()
{
    Solution sol;

    // vector<vector<string>> equations = {{"a","b"},{"b","c"}};
    // vector<double> values = {2.0,3.0};
    // vector<vector<string>> queries = {{"a","c"},{"b","a"},{"a","e"},{"a","a"},{"x","x"}};
    // 输出：[6.00000,0.50000,-1.00000,1.00000,-1.00000]

    vector<vector<int>> people = {{7,0},{4,4},{7,1},{5,0},{6,1},{5,2}};
    auto res = sol.reconstructQueue(people);
    for (auto& it:res) {
        cout << it[0] << "," << it[1] << endl;
    }
    cout << endl;

    // std::list<vector<int>> myList = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}};  
  
    // for (const auto& it : myList) { // 使用const auto&可以避免不必要的拷贝，并且如果元素是只读的，这是最佳选择  
    //     std::cout << it[0] << ' ' << endl;  
    // }  
    return 0;
}
