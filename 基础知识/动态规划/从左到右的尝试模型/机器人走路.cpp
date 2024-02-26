// 0-N的N个不同位置，机器人初始位置在m位置，目标走到p，必须走k步，问有多少种方法可以走
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <set>
#include <ctime>
#include <cstdlib>

using namespace std;
class Solution
{
public:
    int robot_path(int N, int M, int K, int P)
    {
        // write code here
        if (N <= 0 || M > N || K < abs(M - P))
        {
            return 0;
        }
        return walk(N, M, K, P);
    }
    // 当前在cur位置上还剩rest步
    // 从cur位置出发，走完rest步数到达p的返回可能的方法数
    int walk(int n, int cur, int rest, int p)
    {
        if (rest == 0)
        {
            if (cur == p)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        if (cur == 1)
        { // 只能往右走
            return walk(n, cur + 1, rest - 1, p);
        }

        if (cur == n)
        { // 只能往左走
            return walk(n, cur - 1, rest - 1, p);
        }

        return walk(n, cur - 1, rest - 1, p) + walk(n, cur + 1, rest - 1, p);
    }
    int robot_path_dp(int N, int M, int K, int P)
    {
        if (N <= 0 || M > N || K < abs(M - P))
        {
            return 0;
        }
        vector<vector<int>> dp(N + 1, vector<int>(K + 1, 0));
        for (int i = 1; i <= N; i++)
        {
            if (i == P)
            {
                dp[i][0] = 1;
            }
        }

        for (int rest = 1; rest <= K; rest++)
        {
            for (int cur = 1; cur <= N; cur++)
            {
                if (cur == 1)
                {
                    dp[cur][rest] = dp[cur + 1][rest - 1];
                }
                else if (cur == N)
                {
                    dp[cur][rest] = dp[cur - 1][rest - 1];
                }
                else
                {
                    dp[cur][rest] = dp[cur - 1][rest - 1] + dp[cur + 1][rest - 1];
                }
            }
        }

        // for (int i=0;i<=N;i++) {
        //     for (int j=0;j<=K;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }

        return dp[M][K];
    }
};

int main()
{
    Solution sol;
    // vector<int> arr = {90,100,1,7};
    vector<int> arr;
    int N = 1;
    int testTime = 1000;
    std::srand(std::time(0));
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
        int N = (int)((std::rand() % 10) + 2);
        int M = (int)((std::rand() % N) + 1);
        int P = (int)((std::rand() % N) + 1);
        int K = max((int)((std::rand() % (2*N)) + 1), abs(M - P));
        // cout << "N = " << N << ", M = " << M << ", P = " << P << ", K = " << K << endl;
        int ans0 = sol.robot_path(N, M, K, P);
        int ans1 = sol.robot_path_dp(N, M, K, P);

        if (ans0 != ans1)
        {
            cout << "ans0:" << ans0 << endl;
            cout << "ans1:" << ans1 << endl;
            cout << "Oops!" << endl;
            break;
        }
        // else
        // {
        //     cout << "pass, test:" << i << " ans:" << ans0 << endl;
        // }
    }
    // int ans0 = sol.robot_path(2,1,1,2);
    // int ans1 = sol.robot_path_dp(2,1,1,2);

    // cout << "ans0:" << ans0 << ", ans1:" << ans1 <<endl;
    cout << "测试结束" << endl;
}