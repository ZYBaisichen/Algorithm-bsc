/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-07-22 13:44:49
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

            对于第1份来说，1个数是可以搞定，k-1个数是可以搞定的, 2*(k-1)也是可以搞定的.(因为(n-1)%(k-1)==0才能合并成1份)
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

    int mergeStones_jiyi(vector<int>& stones, int k) {
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

    // 注：这里依赖了[l][r][k]，但下方又依赖了[l][mid][1]，但感觉r和mid之间是有依赖关系的，一开始没想出来
    // 主要参考题解：https://leetcode.cn/problems/minimum-cost-to-merge-stones/solutions/2206339/he-bing-shi-tou-de-zui-di-cheng-ben-by-l-pnvh/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china
    
    int mergeStones_dp(vector<int>& stones, int k) {
        int n = stones.size();
        if ((n-1) % (k-1) > 0) {
            return -1;
        }
        vector<int> presum(n+1, 0);
        
        //l，r取值范围是0,,,len-1， k取值1...k
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(k+1, INT_MAX)));
        for (int i=0;i<n;i++) {
            presum[i+1] = presum[i] + stones[i];
            dp[i][i][1] = 0;
        }

        //填第一维dp[i][j][1]
        /*
        dp[3][6][4]依赖以下的最小值
        dp[3][3][1], dp[4][6][3]
        dp[3][4][1], dp[5][6][3]
        dp[3][5][1], dp[6][6][3]
        l从小到大递归，r也是从小到大，每个l和r，k都会填一遍。
        以l和r为x和y轴，t为z轴，同一层上l和r只会依赖下面所有的行和前面所有的列，所以可以按照对角线填。
        每次填一个格子的时候，都把竖着的高都填完。
        */
        for (int len=2;len<=n;len++) {
            for (int l=0;l<n&&l+len-1<n;l++) {
                int r = l + len - 1;
                for (int t = 2; t<=k; t++) {
                    for (int mid=l; mid<r; mid+=(k-1)) {
                        // cout << "l:" << l << " mid:"<<mid<<" r:" << r << " t:" << endl;
                        // cout << "dp[l][mid][1]:" << dp[l][mid][1] << " dp[mid+1][r][t-1]:" << dp[mid+1][r][t-1] << endl;
                        dp[l][r][t] = min(dp[l][r][t], dp[l][mid][1]+dp[mid+1][r][t-1]);
                    }
                }
                dp[l][r][1] = min(dp[l][r][1], dp[l][r][k] + presum[r+1]-presum[l]);
            }
        }
        return dp[0][n-1][1];
    }

     int mergeStones_dp2(vector<int>& stones, int k) {
        int n = stones.size();
        if ((n-1) % (k-1) > 0) {
            return -1;
        }
        vector<int> presum(n+1, 0);
        
        //l，r取值范围是0,,,len-1， k取值1...k
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(k+1, INT_MAX)));
        for (int i=0;i<n;i++) {
            presum[i+1] = presum[i] + stones[i];
            dp[i][i][1] = 0;
        }

        //填第一维dp[i][j][1]
        /*
        dp[3][6][4]依赖以下的最小值
        dp[3][3][1], dp[4][6][3]
        dp[3][4][1], dp[5][6][3]
        dp[3][5][1], dp[6][6][3]
        l从小到大递归，r也是从小到大，每个l和r，k都会填一遍。
        以l和r为x和y轴，t为z轴，同一层上l和r只会依赖下面所有的行和前面所有的列，所以可以按照对角线填。
        每次填一个格子的时候，都把竖着的高都填完。
        这里用的每一层从下到上，从左到右填
        */
        for (int l=n-2;l>=0;l--) {
            for (int r=l+1;r<n;r++) {
                for (int t = 2; t<=k; t++) {
                    for (int mid=l; mid<r; mid+=(k-1)) {
                        // cout << "l:" << l << " mid:"<<mid<<" r:" << r << " t:" << endl;
                        // cout << "dp[l][mid][1]:" << dp[l][mid][1] << " dp[mid+1][r][t-1]:" << dp[mid+1][r][t-1] << endl;
                        dp[l][r][t] = min(dp[l][r][t], dp[l][mid][1]+dp[mid+1][r][t-1]);
                    }
                }
                dp[l][r][1] = min(dp[l][r][1], dp[l][r][k] + presum[r+1]-presum[l]);
            }
        }
        return dp[0][n-1][1];
    }


    /*
    状态优化，在dp中，使用了dp[l][r][t]表示将区间[l,r]的石头堆合并为t堆的最小成本，t的范围是[1,k]。
    从数学上看，每次合并减少k-1个堆，如果一直合并，则可以合并成(r-l+1-1)%(k-1)+1个堆，且这个堆的数量是小于k的。
    所以在[l,r]区间最终合并到小于k堆的堆数是固定的。
    直接使用dp[l][r]表示区间[l,r]合并到不能合并为止时的最小成本，本质上是通过忽略dp方法中一定不无解的状态，加快求解

    初始时：dp[i][i]我都为0，其他状态为最大值
    dp[l][r]=min(dp[l][p]+dp[p+1][r])。其中l<=p<r
    如果(r-l)%(k-1)==0, 说明可以合成一堆，则还需要加上sum[l..r]
    答案是dp[0][n-1]

    */
    int mergeStones(vector<int>& stones, int k) {
        int n = stones.size();
        if ((n-1) % (k-1) > 0) {
            return -1;
        }
        vector<int> presum(n+1, 0);
        
        //l，r取值范围是0,,,len-1， k取值1...k
        vector<vector<int>> dp(n, vector<int>(n,  INT_MAX));
        for (int i=0;i<n;i++) {
            presum[i+1] = presum[i] + stones[i];
            dp[i][i] = 0;
        }

        for (int l=n-2;l>=0;l--) {
            for (int r=l+1;r<n;r++) {
                for (int mid=l; mid<r; mid+=(k-1)) {
                    dp[l][r] = min(dp[l][r], dp[l][mid]+dp[mid+1][r]);
                }
                if ((r-l)%(k-1) == 0) { //说明可以合并成一堆
                    dp[l][r] += presum[r+1]-presum[l];
                }
            }
        }

        return dp[0][n-1];
    }
};

int main()
{

    Solution sol;
    vector<int> arr={3,2,4,1};
    int k = 4;
    cout << sol.mergeStones_baoli(arr, k) << endl;
    cout << sol.mergeStones_jiyi(arr, k) << endl;
    cout << sol.mergeStones_dp(arr, k) << endl;
    cout << sol.mergeStones_dp2(arr, k) << endl;
    cout << sol.mergeStones(arr, k) << endl;
    return 0;
}