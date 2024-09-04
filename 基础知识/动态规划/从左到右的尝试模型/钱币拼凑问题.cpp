/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-02-27 11:20:39
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

using namespace std;
// 一个数组arr，每个元素代表一个硬币的面值，问使用这些硬币拼凑出一个aim面值，有多少拼凑方法。
// 完整的暴力递归->记忆化搜索->经典动态规划->枚举优化
class Solution
{
public:
    int coins_ways1(vector<int> arr, int aim) {
        if (arr.size() == 0 || aim < 0) {
            return 0;
        }
        return process1(arr, 0, aim);
    }
    // index....拼凑出rest钱币有多少种方法
    int process1(vector<int>& arr, int index, int rest) {
        int len = arr.size();
        if (index == len) {
            return rest==0? 1 : 0;
        }
        
        int ways = 0;
        for (int zhang=0; (zhang * arr[index]) <= rest; zhang++) {
            ways += process1(arr, index+1, rest-(zhang*arr[index]));
        }
        return ways;
    }


    //记忆化搜索
    int coins_ways2(vector<int> arr, int aim) {
        if (arr.size() == 0 || aim < 0) {
            return 0;
        }
        vector<vector<int>> dp(arr.size()+1, vector<int>(aim+1, -1));
        return process2(arr, 0, aim, dp);
    }
    // index....拼凑出rest钱币有多少种方法
    int process2(vector<int>& arr, int index, int rest, vector<vector<int>>& dp) {
        if (dp[index][rest] != -1) {
            return dp[index][rest];
        }
        int len = arr.size();
        if (index == len) {
            dp[index][rest] = rest==0? 1 : 0;
            return dp[index][rest];
        }
        
        int ways = 0;
        for (int zhang=0; (zhang * arr[index]) <= rest; zhang++) {
            ways += process1(arr, index+1, rest-(zhang*arr[index]));
        }
        dp[index][rest] = ways;
        return ways;
    }

    // 经典动态规划
    int coins_ways3(vector<int> arr, int aim) {
        if (arr.size() == 0 || aim < 0) {
            return 0;
        }
        int len = arr.size();
        //dp[i][rest]代表i..len-1范围上的钱币拼凑出rest需有多少种可能
        vector<vector<int>> dp(len+1, vector<int>(aim+1, 0));
        dp[len][0] = 1;
        for (int index = len-1; index >= 0; index--) {
            for (int rest = 0; rest <= aim; rest++) {
                int ways = 0;
                for (int zhang=0; (zhang * arr[index]) <= rest; zhang++) {
                    ways += dp[index + 1][rest - (zhang * arr[index])];
                }
                dp[index][rest] = ways;
            }
        }
        // for (int i=0; i<=len; i++) {
        //     for (int j=0; j<=aim; j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        return dp[0][aim];
    }

    // 枚举优化版本
    int coins_ways4(vector<int> arr, int aim) {
        if (arr.size() == 0 || aim < 0) {
            return 0;
        }
        int len = arr.size();
        vector<vector<int>> dp(len+1, vector<int>(aim+1, 0));
        dp[len][0] = 1;
        for (int index = len-1; index >= 0; index--) {
            for (int rest = 0; rest <= aim; rest++) {
                // 优化枚举
                //  for (int zhang=0; (zhang * arr[index]) <= rest; zhang++) {
                //      ways += dp[index + 1][rest - (zhang * arr[index])];
                //  }
                // 因为dp[index][rest]依赖的是dp[index+1][rest-0*arr[index]]、dp[index+1][rest-1*arr[index]]、dp[index+1][rest-2*arr[index]].....
                // 而同一行的dp[index][rest-1*arr[index]]依赖的是dp[index+1][rest-2*arr[index]]、dp[index+1][rest-3*arr[index]]、dp[index+1][rest-4*arr[index]].....
                // 所以dp[index][rest] = dp[index][res-1*arr[index]] + dp[inex+1][rest-0*arr[index]]
                dp[index][rest] = dp[index + 1][rest];
                if (rest - arr[index] >= 0) {
                    dp[index][rest] += dp[index][rest - arr[index]];
                }
            }
        }
        // for (int i=0; i<=len; i++) {
        //     for (int j=0; j<=aim; j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        return dp[0][aim];
    }
    
};

int main()
{
    Solution sol;
    // vector<int> arr = {90,100,1,7};
    vector<int> arr={5,10,50,100};
    int aim=1000;
    // vector<int> arr = {5, 5};
    // int aim = 10;
    // int N = 1;
    // int testTime = 1000;
    // std::srand(std::time(0));
    // cout << "测试开始" << endl;
    // for (int i = 0; i < testTime; i++)
    // {
    //     int N = (int)((std::rand() % 10) + 2);
    //     int M = (int)((std::rand() % N) + 1);
    //     int P = (int)((std::rand() % N) + 1);
    //     int K = max((int)((std::rand() % (2*N)) + 1), abs(M - P));
    //     // cout << "N = " << N << ", M = " << M << ", P = " << P << ", K = " << K << endl;
    //     int ans0 = sol.robot_path(N, M, K, P);
    //     int ans1 = sol.robot_path_dp(N, M, K, P);

    //     if (ans0 != ans1)
    //     {
    //         cout << "ans0:" << ans0 << endl;
    //         cout << "ans1:" << ans1 << endl;
    //         cout << "Oops!" << endl;
    //         break;
    //     }
    //     // else
    //     // {
    //     //     cout << "pass, test:" << i << " ans:" << ans0 << endl;
    //     // }
    // }
    int ans0 = sol.coins_ways1(arr, aim);
    int ans1 = sol.coins_ways2(arr, aim);
    int ans2 = sol.coins_ways3(arr, aim);
    int ans3 = sol.coins_ways4(arr, aim);

    cout << "ans0:" << ans0 << " ans1:" << ans1 << " ans2:" << ans2 << " ans3:" << ans3 << endl;
    cout << "测试结束" << endl;
}