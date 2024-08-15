/*
 * @Author: baisichen
 * @Date: 2024-08-14 18:00:03
 * @LastEditTime: 2024-08-15 16:46:10
 * @LastEditors: baisichen
 * @Description: 
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

/*
https://leetcode.cn/problems/permutations-ii/solutions/2363888/47-quan-pai-lie-iihui-su-qing-xi-tu-jie-7ry7t/
给定一个可包含重复数字的序列 nums ，按任意顺序 返回所有不重复的全排列。



示例 1：

输入：nums = [1,1,2]
输出：
[[1,1,2],
 [1,2,1],
 [2,1,1]]
示例 2：

输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]


提示：

1 <= nums.length <= 8
-10 <= nums[i] <= 10
*/
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        int len = nums.size();
        sort(nums.begin(), nums.end());
        set<int> sets;
        vector<vector<int>> res;
        dfs(nums, res, 0);
        return res;
    }
    /*
    0...i-1已经决定好了，现在决定i位置，i+1...len位置的数可以选取
    */
    void dfs(vector<int>& nums, vector<vector<int>>& res, int i) {
        int len = nums.size();
        if (i==len-1) {
            res.push_back(nums);
            return;
        }

        set<int> sets; //在i位置上，不能重复放相同的数
        for (int j=i;j<len;j++) {
            if (sets.find(nums[j]) != sets.end()) {
                continue;
            }
            sets.insert(nums[i]);
            swap(nums[i], nums[j]);
            dfs(nums, res, i+1);
            swap(nums[i], nums[j]);
        }

    }
};


int main() {
    vector<int> nums = {1,1,2};
    Solution sol;
    auto res = sol.permuteUnique(nums);
    for (auto it:res) {
        for (auto it2:it) {
            cout << it2 << " ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}