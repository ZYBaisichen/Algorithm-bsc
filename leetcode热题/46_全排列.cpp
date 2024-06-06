/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-06 17:32:14
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

using namespace std;

/*
https://leetcode.cn/problems/permutations/

给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。 

示例 1：

输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
示例 2：

输入：nums = [0,1]
输出：[[0,1],[1,0]]
示例 3：

输入：nums = [1]
输出：[[1]]
 

提示：

1 <= nums.length <= 6
-10 <= nums[i] <= 10
nums 中的所有整数 互不相同
*/

/*
全排列，dfs

*/
class Solution {
public:

    //dfs
    vector<vector<int>> permute(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return vector<vector<int>>();
        }

        vector<int> path;
        vector<vector<int>> res;

        //以每个数开头枚举
        for (int i=0;i<len;i++) {
            vector<bool> visited(len, false);
            visited[i] = true;
            path.clear();
            path.push_back(nums[i]);
            dfs(nums, 1, path, res, visited);
        }
        return res;

    }


    void print_arr(vector<int>&arr, string suff) {
        cout << suff;
        for (auto it:arr) {
            cout << it << " ";
        }
        cout << endl;
    }
    //来到了第i层枚举， 枚举没有出现过的数字
    void dfs(vector<int>& nums, int level, vector<int>& path, vector<vector<int>>& res, vector<bool>& visited) {
        // cout << "level:" << level << endl;
        // print_arr(path, "begin:");
        int len = nums.size();
        if (level == len) {
            res.push_back(path);
            return;
        }

        for (int i=0;i<len;i++) {
            if (!visited[i]) {
                path.push_back(nums[i]);
                visited[i] = true;
                dfs(nums, level+1, path, res, visited);
                path.pop_back(); //回溯
                visited[i] = false;
            }
        }
    }
};
int main()
{

    Solution sol;
    vector<int> nums = {1,2,3};
    vector<vector<int>> res = sol.permute(nums);
    cout << "res:" << endl;
    for (auto it:res) {
        for (auto it2:it) {
            cout << it2 << " ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}