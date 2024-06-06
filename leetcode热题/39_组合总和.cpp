/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-06 17:06:35
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
https://leetcode.cn/problems/combination-sum/description/
给你一个 无重复元素 的整数数组 candidates 和一个目标整数 target ，找出 candidates 中可以使数字和为目标数 target 的 所有 不同组合 ，并以列表形式返回。你可以按 任意顺序 返回这些组合。

candidates 中的 同一个 数字可以 无限制重复被选取 。如果至少一个数字的被选数量不同，则两种组合是不同的。 

对于给定的输入，保证和为 target 的不同组合数少于 150 个。

 

示例 1：

输入：candidates = [2,3,6,7], target = 7
输出：[[2,2,3],[7]]
解释：
2 和 3 可以形成一组候选，2 + 2 + 3 = 7 。注意 2 可以使用多次。
7 也是一个候选， 7 = 7 。
仅有这两种组合。
示例 2：

输入: candidates = [2,3,5], target = 8
输出: [[2,2,2,2],[2,3,3],[3,5]]
示例 3：

输入: candidates = [2], target = 1
输出: []
 

提示：

1 <= candidates.length <= 30
2 <= candidates[i] <= 40
candidates 的所有元素 互不相同
1 <= target <= 40
*/

/*
数组累加和问题

乍一看以为是要用累加和数组求。但仔细观察发现，这不就是多重背包问题嘛。
将0..len-1个数放在容量为target的背包中，问有多少种组合。

dp[i][j]表示0..i的数放在容量为j的背包中，有多少种组合。
0. dp[...][0]，任何范围放入重量为0的背包中都有1种组合，那就是不放入任何数。
1. dp[0][arr[0]] = 1, dp[0][arr[0]*2] = 1, 即第一个数放在其倍数重量的背包里有1种放法
2. dp[i][j]普遍位置可能性：
    a. 完全不放nums[i]，可能性是dp[i-1][j]
    b. 放1个nums[i], 可能性是dp[i-1][j-nums[i]]
    c. 放2个nums[i], 可能性是dp[i-1][j-nums[i]*2]
    d. 放2个nums[i], 可能性是dp[i-1][j-nums[i]*2]
    ...
    e. 放k个nums[i], 可能性是dp[i-1][j-nums[i]*k]，需要保证num[i]*k<=j
    多种可能性累加得到dp[i][j]
3. dp[len-1][target]就是组合数
4. 因为要求输出的是组合数，所以可以根据dp的值还原出组合数

*/
class Solution {
public:

    //dfs
    vector<vector<int>> combinationSum_dfs(vector<int>& candidates, int target) {
        int len = candidates.size();
        if (len == 0) {
            return vector<vector<int>>();
        }

        vector<int> path;
        vector<vector<int>> res;

        dfs(candidates, 0, target, path, res);
        return res;

    }
    //剩余rest容量。当前在i位置，0...i-1位置都已经选择好了
    //O(2^n)
    void dfs(vector<int>& candidates, int i, int rest, vector<int>& path, vector<vector<int>>& res) {
        // cout << "dfs: i=" << i << ", rest=" << rest << endl;
        if (rest == 0) {
            for(auto it:path) {
                cout << it << " ";
            }
            cout << endl;
            res.push_back(path);
            return;
        }

        if (i == candidates.size() || rest < 0) {
            return;
        }

        //不选
        dfs(candidates, i+1, rest, path, res);

        //选
        path.push_back(candidates[i]);
        //可能会选多个i，所以i不往下走
        dfs(candidates, i, rest - candidates[i], path, res);
        path.pop_back(); //恢复
    }

    //全排列
    vector<vector<int>> combinationSum_origin(vector<int>& candidates, int target) {
        int len = candidates.size();
        if (len == 0) {
            return vector<vector<int>>();
        }
    
        vector<int> path;
        vector<vector<int>> res;

        process(candidates, 0, 0, target, path, res);
        return res;

    }
    //i...n上自由选择，当前累加和是sum，记录在path中
    void process(vector<int>& candidates, int i, int sum, int target, vector<int>& path, vector<vector<int>>& res) {
        cout << "process: i=" << i << ", sum=" << sum << endl;
        if (sum>target) {
            return;
        }
        if (sum==target) {
            for(auto it:path) {
                cout << it << " ";
            }
            cout << endl;
            res.push_back(path);
            return;
        }

        if (i >= candidates.size()) {
            return;
        }
        
        //不加当前的元素
        process(candidates, i+1, sum, target, path, res); 

        //选
        path.push_back(candidates[i]);
        //可能会选多个i，所以i不往下走
        process(candidates, i, sum + candidates[i], target, path, res);
        path.pop_back(); //恢复
    }

    // 全排列上的减枝优化
    // https://leetcode.cn/problems/combination-sum/solutions/2363929/39-zu-he-zong-he-hui-su-qing-xi-tu-jie-b-9zx7/
    /*
    全排列中会有重复的枚举，比如集合[3,4,5]和目标数9，全排列的解为{3,3,3}, {4,5}, {5,4}
    需要注意，：
        1. 集合中当元素可以被无限次重复选取。但
        2. 结果子集是不区分元素顺序的，比如[4,5]和[5,4]是同一个子集。
    为去除重复元素，一种直接的思路是对结果列表进行去重，但如果target比较大的话，产生的子集数量较多，效率很低。

    减枝遍历：观察到第一层会枚举3,4,5，当第一层枚举4时，第二层全枚举的话会出现[4,3],[4,4],[4,5]组合；然后当第一层枚举5时，第二层就没有必要再枚举4了，因为会出现[5,4]和前面的重复了
        所以我们有以下做法：
        1. 对候选数字集合排序
        2. 设置遍历起始点，本轮选择了i之后，下次选择将从i+1开始，这样保证了每个新加到子序列的元素都大于前一个元素。保证有序的枚举，就可以实现一定程度的减枝
        3. 当枚举和超过target的时候，就不需要再往下枚举了。
    */
    vector<vector<int>> combinationSum_jianzhi(vector<int>& candidates, int target) {
        int len = candidates.size();
        if (len == 0) {
            return vector<vector<int>>();
        }

        //对candiates排序, n*log(n)
        std::sort(candidates.begin(), candidates.end());

    
        vector<int> path;
        vector<vector<int>> res;

        process2(candidates, 0, 0, target, path, res);
        return res;

    }
    //i...n上自由选择，当前累加和是sum，记录在path中
    void process2(vector<int>& candidates, int i, int sum, int target, vector<int>& path, vector<vector<int>>& res) {
        // cout << "process: i=" << i << ", sum=" << sum << endl;
        if (sum>target) {
            return;
        }
        if (sum==target) {
            // for(auto it:path) {
            //     cout << it << " ";
            // }
            // cout << endl;
            res.push_back(path);
            return;
        }

        if (i >= candidates.size()) {
            return;
        }
        
        //从i往后看
        
        //不加当前的元素
        process2(candidates, i+1, sum, target, path, res); 

        //选
        path.push_back(candidates[i]);
        //可能会选多个i，所以i不往下走
        process2(candidates, i, sum + candidates[i], target, path, res);
        path.pop_back(); //恢复
    }

    // 背包问题的解
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        int len = candidates.size();
        if (len == 0) {
            return vector<vector<int>>();
        }

        vector<vector<int>> dp(len, vector<int>(target+1, 0));
        //第一列，什么都不放，这一种方法
        for (int i=0;i<len;i++) {
            dp[i][0] = 1;
        }


        //第一行
        for (int j=1;j<=target;j++) {
            if (j >= candidates[0] && (j % candidates[0] == 0)) {
                dp[0][j] = 1;
            }
        }
        

        for (int i=1;i<len;i++) {
            for (int j=1;j<=target;j++) {
                //完全不放nums[i]
                dp[i][j] = dp[i-1][j];
                //枚举放k个nums[i]的情况
                for (int k=1;k<=j/candidates[i];k++) {
                    dp[i][j] += dp[i-1][j - candidates[i]*k];
                }
            }
        }


        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<=target;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        vector<int> path;
        vector<vector<int>> res;

        //再dp上回溯求解组合数
        process4(dp, candidates, len-1, target, path, res);

        return res;

    }

    void print_arr(vector<int>&arr, string suff) {
        cout << suff;
        for (auto it:arr) {
            cout << it << " ";
        }
        cout << endl;
    }
    //通过dp来回溯搜索答案
    //但计算出来dp后，还是需要再dp上回溯答案，和减枝一样快。
    void process4(vector<vector<int>>& dp, vector<int>& candidates, int i, int j, vector<int>& path, vector<vector<int>>& res) {
        // cout << "i:" << i << " j:" << j << " dp[i][j]:" << dp[i][j] << endl;
        // cout << "path:";
        // for (int i=0;i<path.size();i++) {
        //     cout << path[i];
        // }
        // cout << endl;
        //来到了0背包处，找到了一个解
        if (j==0) {
            res.push_back(path);
            return;
        }

        //来到了最后一个元素处，如果dp值不为0，说明有解
        if (i == 0) {
            if (dp[i][j] != 0) {
                //添加j/candidates[i]个candidates[i]
                for (int k =0; k<j/candidates[i]; k++) {
                    path.push_back(candidates[i]);
                }
                // print_arr(path, "i0:");

                res.push_back(path);
                for (int k =0; k<j/candidates[i]; k++) {
                    path.pop_back();
                }
            }
            return;
        }


        //普遍位置的dp[i][j]， 代表0..i元素放入重量为j的背包中有多少种放法

        //完全不放nums[i]方向上有解
        if(dp[i-1][j] > 0) {
            process4(dp, candidates, i-1, j, path, res);
        }

        //枚举放k个nums[i]的情况
        for (int k=1;k<=j/candidates[i];k++) {
            //当放k个nums[i]时，需要看dp[i-1][j - candidates[i]*k]方向上有没有解
            if (dp[i-1][j - candidates[i]*k] > 0) {
                //放k个nums[i]
                for (int kk =0; kk<k; kk++) {
                    path.push_back(candidates[i]);
                }
                process4(dp, candidates, i-1, j - candidates[i]*k, path, res);
                for (int kk =0; kk<k; kk++) {
                    path.pop_back();
                }

            }
        }
    }
};
int main()
{

    Solution sol;
    vector<int> nums = {2,3,5};
    int target = 8;
    vector<vector<int>> res = sol.combinationSum_dp(nums, target);
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