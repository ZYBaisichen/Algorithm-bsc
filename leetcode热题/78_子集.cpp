/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-12 22:08:44
 * @LastEditors: baisichen baisichen@baidu.com
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
https://leetcode.cn/problems/subsets/description/

给你一个整数数组 nums ，数组中的元素 互不相同 。返回该数组所有可能的
子集
（幂集）。

解集 不能 包含重复的子集。你可以按 任意顺序 返回解集。

 

示例 1：

输入：nums = [1,2,3]
输出：[[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
示例 2：

输入：nums = [0]
输出：[[],[0]]
 

提示：

1 <= nums.length <= 10
-10 <= nums[i] <= 10
nums 中的所有元素 互不相同
*/


/*

*/

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return vector<vector<int>>();
        }
        sort(nums.begin(), nums.end());
        cout << "nums:";
        for (auto it:nums) {
            cout << it << " ";
        }
        cout << endl;
        vector<vector<int>> res;
        res.push_back({});
        vector<int> path;
        
        for (int i=1;i<=len;i++) {
            path.clear();
            dfs(nums, i, 0, 1, path, res);
        }
        return res;
    }
    //当前来到了第cur位置，还没有加上cur位置的数，不会往前枚举
    //需要n个数
    //枚举到了第level层，沿途路径记录在path中
    void dfs(vector<int>& nums, int n, int cur, int level, vector<int>& path, vector<vector<int>>& res) {
        cout << "n:" << n << " cur:" << cur << " level:" << level << endl;
        cout << "begin path:";
        for (auto it:path) {
            cout << it << " ";
        }
        cout << endl; 
        if (level == n+1 && path.size() == n) {
            cout << "level:"<< level<<" path:";
            for (auto it:path) {
                cout << it << " ";
            }
            cout << endl;
            res.push_back(path);
            return;
        }
        int len = nums.size();
        for (int i=cur;i<nums.size();i++) {
            path.push_back(nums[i]);
            dfs(nums, n, i+1, level+1, path, res);
            path.pop_back();
        }
    }


    vector<vector<int>> subsets2(vector<int>& nums) {
        int len = nums.size();
        vector<vector<int>> res;
        if (len == 0) {
            return res;
        }
        res.push_back({});

        for (int size=1;size<=len;size++) {
            vector<int> path;
            dfs2(res,path,nums,size,0);
        }

        return res;

    }
    void print(vector<int>& path) {
        cout << "path:" << endl;
        for (auto it:path) {
            cout << it << " ";
        }
        cout << endl;
    }
    /*

    */
    void dfs2(vector<vector<int>>& res, vector<int>& path, vector<int>& nums, int size, int cur) {
        print(path);
        int p_len = path.size();
        cout << "path_size:" << p_len << " size:" << size << endl;
        if (p_len == size) {
            res.push_back(path);
            return;
        }
        int len = nums.size();
        for (int i=cur;i<len;i++) {
            //剩余的不够size了，没必要往下枚举了。减枝
            if (len-i < size-p_len) {
                return;
            }
            path.push_back(nums[i]);
            dfs2(res,path,nums,size, i+1);
            path.pop_back();
        }
    }
};
int main()
{

    Solution sol;
    vector<int> nums = {1,2,3};
    auto res = sol.subsets(nums);
    cout << "res:"<< endl;
    for (auto it:res) {
        for(auto it2:it) {
            cout << it2<<" ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}
