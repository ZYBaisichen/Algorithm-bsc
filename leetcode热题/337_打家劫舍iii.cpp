/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-20 14:17:01
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
https://leetcode.cn/problems/house-robber-iii/description/
小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为 root 。

除了 root 之外，每栋房子有且只有一个“父“房子与之相连。一番侦察之后，聪明的小偷意识到“这个地方的所有房屋的排列类似于一棵二叉树”。 如果 两个直接相连的房子在同一天晚上被打劫 ，房屋将自动报警。

给定二叉树的 root 。返回 在不触动警报的情况下 ，小偷能够盗取的最高金额 。

 

示例 1:



输入: root = [3,2,3,null,3,null,1]
输出: 7 
解释: 小偷一晚能够盗取的最高金额 3 + 3 + 1 = 7
示例 2:



输入: root = [3,4,5,1,3,null,1]
输出: 9
解释: 小偷一晚能够盗取的最高金额 4 + 5 = 9
 

提示：

树的节点数在 [1, 104] 范围内
0 <= Node.val <= 104
*/

/*
对于某个节点cur，可以打劫也可以不打劫
1. 当打劫当前节点时，就不能打劫下面的子节点，需要拿到不打劫子节点时产生的最大价值。
2. 当不打劫当前节点时，通过子节点不打劫和打劫时产生的最大价值取最大。

*/
 
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
    struct info {
        int yes;
        int no; //不打劫当前节点
        info(int y, int n) {
            yes = y;
            no = n;
        }
    };
    int rob(TreeNode* root) {

    }
};


int main()
{
    Solution sol;
    // vector<int> prices = {2,3,4,1,3};
    vector<int> prices = {1, 2, 3, 0, 2};
    cout << sol.maxProfit(prices) << endl;
    return 0;
}
