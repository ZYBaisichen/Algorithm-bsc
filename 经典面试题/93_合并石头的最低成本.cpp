/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-14 10:30:29
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
#include <sstream>

using namespace std;

/*

https://leetcode.cn/problems/minimum-cost-to-merge-stones/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china

有 n 堆石头排成一排，第 i 堆中有 stones[i] 块石头。

每次 移动 需要将 连续的 k 堆石头合并为一堆，而这次移动的成本为这 k 堆中石头的总数。

返回把所有石头合并成一堆的最低成本。如果无法合并成一堆，返回 -1 。

 

示例 1：

输入：stones = [3,2,4,1], K = 2
输出：20
解释：
从 [3, 2, 4, 1] 开始。
合并 [3, 2]，成本为 5，剩下 [5, 4, 1]。
合并 [4, 1]，成本为 5，剩下 [5, 5]。
合并 [5, 5]，成本为 10，剩下 [10]。
总成本 20，这是可能的最小值。
示例 2：

输入：stones = [3,2,4,1], K = 3
输出：-1
解释：任何合并操作后，都会剩下 2 堆，我们无法再进行合并。所以这项任务是不可能完成的。.
示例 3：

输入：stones = [3,5,1,2,6], K = 3
输出：25
解释：
从 [3, 5, 1, 2, 6] 开始。
合并 [5, 1, 2]，成本为 8，剩下 [3, 8, 6]。
合并 [3, 8, 6]，成本为 17，剩下 [17]。
总成本 25，这是可能的最小值。
 

提示：

n == stones.length
1 <= n <= 30
1 <= stones[i] <= 100
2 <= k <= 30

*/

/*
1. 石头长度n和k有一定的数学关系，可以直接计算出最终能不能合
当k=2时，永远都可以合并
当k=3时
    1：不可以
    2：不可以
    3：可以
    4：不可以
    5：可以
    6： 不可以
总结下来是，当(n-1)%(k-1)==0时可以合并，否则不可以合并

2. int f(l,r,part) l到r范围上一定要凑成part个数的最小代价
主函数调用f(0,n-1,1)

3. 
*/

class Solution
{
public:
    int mergeStones_baoli(vector<int>& stones, int k) {
        int len = stones.size();
        if ((len-1) % (k-1) > 0) {
            return -1;
        }
        vector<int> presum(len+1, 0);
        for (int i=0;i<len;i++) {
            presum[i+1] = presum[i] + stones[i];
        }
        cout << "presum:";
        for (int i=0;i<=len;i++) {
            cout << presum[i] << " ";
        }
        cout << endl;
        return process(0, len - 1, 1, stones, k, presum);
    }
    //l到r范围上一定要凑成part个数的最小代价
    //如果合不出来则返回-1
    int process(int l, int r, int part, vector<int>& arr, int k, vector<int>& presum) {
        cout << "l:" << l << " r:" << r << " part:" << part << endl;
        if (l==r) {
            //只有一个数，当part也等于1时代价是0，否则代价是-1
            cout << "l:" << l << " r:" << r << " part:" << part  << " res:"<< (part == 1 ? 0 : -1) <<endl;
            return part == 1 ? 0 : -1;
        }

        //l到r上不止一个数
        if (part == 1) {
            //先合出来k份，然后当前再合一份
            int next = process(l,r,k,arr,k,presum);
            cout << "l:" << l << " r:" << r << " part:" << part << " next:" << next << " res:" << next + presum[r+1] - presum[l] << endl;
            if (next == -1) { //l到r上都合不出来k份，则一定也合不出来1份
                return -1;
            } else {
                //等于l到r上的累加和，加上合k份的代价
                return next + presum[r+1] - presum[l];
            }
        } else { //part > 1
            int ans = INT_MAX;
            /*
            例如数组中第7,8,9,10,11,12位置上的数，合并成3份。
            枚举第一份在哪里，比如：
            1. 7作为第一份，8到12位置搞出来2份
            2. 7 8作为第一份，后面的搞出2份
            3. 7 8 9作为第一份，后面的搞出2份
            4. 7 8 9 10作为第一份，后面的搞出2份

            对于第1份来说，1个数是可以搞定，k-1个数是可以搞定的, 2*(k-1)也是可以搞定的
            */
            for (int mid=l;mid<r;mid+=k-1) {
                int next1 = process(l, mid, 1, arr, k, presum);
                int next2 = process(mid+1, r, part-1, arr, k, presum);
                cout << "l:" << l << " r:" << r << " mid:" << mid << " part:" << part << " next1:" << next1 << " next2:" << next2 << " res:" << min(ans, next1+next2) << endl;
                if (next1 != -1 && next2 != -1) {
                    ans = min(ans, next1+next2);
                }
            }
            return ans;
        }
    }

    int mergeStones(vector<int>& stones, int k) {
        int len = stones.size();
        if ((len-1) % (k-1) > 0) {
            return -1;
        }
        vector<int> presum(len+1, 0);
        for (int i=0;i<len;i++) {
            presum[i+1] = presum[i] + stones[i];
        }
        //l，r取值范围是0,,,len-1， k取值1...k
        vector<vector<vector<int>>> dp(len, vector<vector<int>>(len, vector<int>(k+1, INT_MAX)));
        return process_dp(0, len - 1, 1, stones, k, presum, dp);
    }
    
    //l到r范围上一定要凑成part个数的最小代价
    //如果合不出来则返回-1
    int process_dp(int l, int r, int part, vector<int>& arr, int k, vector<int>& presum, vector<vector<vector<int>>>& dp) {
        if (dp[l][r][part] != INT_MAX) {
            return dp[l][r][part];
        }
        if (l==r) {
            //只有一个数，当part也等于1时代价是0，否则代价是-1
            dp[l][r][part] = (part == 1 ? 0 : -1);
            return dp[l][r][part];
        }

        //l到r上不止一个数
        if (part == 1) {
            // dp part==1时依赖的是l,r,k
            //往下分解时part又会减少
            
            //先合出来k份，然后当前再合一份
            int next = process_dp(l, r, k, arr, k, presum, dp);
            if (next == -1) { //l到r上都合不出来k份，则一定也合不出来1份
                dp[l][r][part] = -1;
                return -1;
            } else {
                //等于l到r上的累加和，加上合k份的代价
                dp[l][r][part] = next + presum[r+1] - presum[l];
                // cout << "dp_ l:" << l << " r:" << r << " part:" << part << " next:" << next << " res:" << next + presum[r + 1] - presum[l] << endl;
                return dp[l][r][part];
            }
        } else { //part > 1
            int ans = INT_MAX;
            /*
            例如数组中第7,8,9,10,11,12位置上的数，合并成3份。
            枚举第一份在哪里，比如：
            1. 7作为第一份，8到12位置搞出来2份
            2. 7 8作为第一份，后面的搞出2份
            3. 7 8 9作为第一份，后面的搞出2份
            4. 7 8 9 10作为第一份，后面的搞出2份

            对于第1份来说，1个数是可以搞定，k-1个数是可以搞定的, 2*(k-1)也是可以搞定的
            */
            for (int mid=l;mid<r;mid+=k-1) {
                int next1 = process_dp(l, mid, 1, arr, k, presum, dp);
                int next2 = process_dp(mid + 1, r, part - 1, arr, k, presum, dp);
                if (next1 != -1 && next2 != -1) {
                    ans = min(ans, next1+next2);
                }
            }
            dp[l][r][part] = ans;
            return ans;
        }
    }
};

int main()
{

    Solution sol;
    vector<int> arr={3,2,4,1};
    int k = 4;
    cout << sol.mergeStones_baoli(arr, k) << endl;
    cout << sol.mergeStones(arr, k) << endl;
    return 0;
}