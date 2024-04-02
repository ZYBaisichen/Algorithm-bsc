/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-01 21:17:35
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

using namespace std;

/*
两种不同类型的硬币，有多少种组合方式
  现有n1+n2种面值的硬币， 其中前n1种为普通币， 可以取任意枚， 后n2种为纪念币，
  每种最多只能取一枚， 每种硬币有一个面值， 问能用多少种方法拼出m的面值？
*/

/*

*/
class Solution
{
public:
    /*
    一、如果全是可以使用无限个的普通币。这就是多重背包问题了。dp[i][j] 表示[0..i]货币自由组合达到j元钱的方法数
    aim=10， coins1=[3,2,5]
    1. 第一列什么货币都不用可以拼凑出0块钱的方法数有1种
    2. 第一行，拼凑出3的整数倍块钱的方法有一种
    3. dp[i][j]：
        a. i号货币使用0张, 依赖前i-1个货币组成j元的方法数：dp[i-1][j]
        b. i号货币使用1张, 依赖前i-1个货币组成j-coins1[i]元的方法数：dp[i-1][j-coins[i]]
        ...
        n. i号货币使用k张, 依赖前i-1个货币组成j-k*coins1[i]元的方法数：dp[i-1][j-k*coins[i]]

        所有的可能性累加就是dp[i][j]的值

        为了减少枚举的行为，让dp[i][j] = dp[i-1][j] + dp[i][j-arr[i]]（dp[i][j-arr[i]]将前面的dp[i-1][j-arr[i]]+dp[i-1][j-2*arr[i]]..都已经加上了）

        从左往右，从上到下计算

    二、如果全是只能使一个的纪念币。就是01背包问题了。dp[i][j]表示[0..i]每个货币只能使用一次，组成j元钱的方法数
    1. 第一列都是1。
    2. 第一行第一个硬币只能搞定coins[0]元钱
    3. dp[i][j]:
        a. 使用i号货币，则依赖前i-1个货币组成j-coins[i]元的方法数：dp[i-1][j-coins[i]]
        b. 不使用i号货币，则依赖前i-1个货币组成j元的方法数：dp[i-1][j]
        都可以用空间压缩了

    三、假设需要搞定的总钱数是aim, 因为得到了只使用某一种货币的dp表。则:
        a. 假设使用普通货币搞定0元，使用纪念币搞定aim-0元，则方法数是：dp1[len-1][0] + dp2[len-1][aim]
        b. 假设使用普通货币搞定1元，使用纪念币搞定aim-1元，则方法数是：dp1[len-1][1] + dp2[len-1][aim-1]
        ...
        c. 假设使用普通货币搞定aim元，使用纪念币搞定0元，则方法数是：dp1[len-1][aim] + dp2[len-1][0]

        以上所有方法数累加，就是两种货币搞定aim钱数的总方法数

    */
    int money_ways(vector<int> coins1, vector<int> coins2, int aim) {
        int len1 = coins1.size();
        int len2 = coins2.size();
        vector<vector<int>> dp1(len1, vector<int>(aim+1));
        vector<vector<int>> dp2(len2, vector<int>(aim+1));

        get_dp_arb1(coins1, aim, dp1);
        get_dp_arb2(coins2, aim, dp2);

        if (len1==0) {
            return dp2[len2-1][aim];
        }
        if (len2==0) {
            return dp1[len1-1][aim];
        }

        int ans = 0;
        for (int i=0;i<=aim;i++) {
            ans += (dp1[len1-1][i] * dp2[len2-1][aim - i]);
        }
        return ans;
    }

    //arr中的货币任意选择，搞定money的方法数，生成dp表
    void get_dp_arb1(vector<int> arr, int money, vector<vector<int>>& dp) {
        int len  = arr.size();
        // dp.resize(len);
        // for (int i=0;i<len;i++) {
        //     dp[i].resize(money+1);
        // }
        //第一列
        for (int i=0;i<len;i++) {
            dp[i][0] = 1;
        }
        //第一行
        for (int j=1;j<=money;j++) {
            if (j%arr[0] == 0) {
                dp[0][j] = 1;
            } else {
                dp[0][j] = 0;
            }
        }


        //普遍位置
        for (int i=1;i<len;i++) {
            for (int j=1;j<=money;j++) {
                dp[i][j] = dp[i - 1][j]; //使用0张
                if (j>=arr[i]) {
                    dp[i][j] += dp[i][j - arr[i]];//使用1..k张
                }
            }
        }
    }

    // arr中的货币只能选择一次，搞定money的方法数。生成dp
    void get_dp_arb2(vector<int> arr, int money, vector<vector<int>>& dp) {
        int len  = arr.size();
        // dp.resize(len);
        // for (int i=0;i<len;i++) {
        //     dp[i].resize(money+1);
        // }
        //第一列
        for (int i=0;i<len;i++) {
            dp[i][0] = 1;
        }
        //第一行
        for (int j=1;j<=money;j++) {
            if (j == arr[0]) {
                dp[0][j] = 1;
            } else {
                dp[0][j] = 0;
            }
        }

        //普遍位置
        for (int i=1;i<len;i++) {
            for (int j=1;j<=money;j++) {
                dp[i][j] = dp[i - 1][j]; //不使用i号钱币
                if (j>=arr[i]) { //使用i号钱币
                    dp[i][j] += dp[i-1][j - arr[i]];
                }
            }
        }
    }
};

int main()
{

    Solution sol;
    vector<int> coins1 = {1, 2, 5, 10};
    vector<int> coins2 = {1, 5, 10, 25};

    cout << sol.money_ways(coins1, coins2, 13) << endl;

    return 0;
}