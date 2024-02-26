/*
 * @Author: baisichen
 * @Date: 2024-02-22 21:05:49
 * @LastEditTime: 2024-02-26 11:40:08
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
class Solution
{
public:
    int res;
    int win(vector<int> arr)
    {
        int len = arr.size();
        if (len == 0)
        {
            return 0;
        }

        return max(f(arr, 0, len - 1), s(arr, 0, len - 1));
    }
    int f(vector<int> &arr, int l, int r)
    {
        if (l>arr.size() || r<0) {
            return 0;
        }
        // cout << "f:: "<< "l:" << l << " r:" << r << endl;
        if (l == r)
        {
            return arr[l];
        }
        int left = arr[l] + s(arr, l + 1, r);
        int right = arr[r] + s(arr, l, r - 1);
        return max(left, right);
    }
    int s(vector<int> &arr, int l, int r)
    {
        if (l > arr.size() || r < 0)
        {
            return 0;
        }
        // cout << "s:: "<< "l:" << l << " r:" << r << endl;
        if (l == r)
        {
            return 0;
        }
        int left=f(arr, l + 1, r);
        int right=f(arr, l, r - 1);
        return min(left, right);
    }

    int winDp(vector<int> arr)
    {
        int len = arr.size();
        if (len == 0)
        {
            return 0;
        }

        vector<vector<int>> f_dp(len, vector<int>(len, 0));
        vector<vector<int>> s_dp(len, vector<int>(len, 0));
        for (int i = 0; i < len; i++) {
            for (int j=0; j<len; j++) {
                f_dp[i][j] = arr[i];
            }
        }

        for (int i = len-2; i >= 0; i--) {
            for (int j=i+1; j<len; j++) {
                int f_left = arr[i] + s_dp[i+1][j];
                int f_right = arr[j] + s_dp[i][j-1];
                f_dp[i][j] = max(f_left, f_right);

                int s_left = f_dp[i + 1][j];
                int s_right = f_dp[i][j - 1];
                s_dp[i][j] = min(s_left, s_right);
            }
        }

        return max(f_dp[0][len-1], s_dp[0][len-1]);
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
    for (int i = 0; i < testTime; i++) {
        int len = (int) ((std::rand()%10) * N);
        arr.clear();
        for (int j=0; j<len;j++) {
            arr.push_back((int)(std::rand() % 1000));
        }
        int ans0 = sol.win(arr);
        int ans1 = sol.winDp(arr);
        
        if (ans0 != ans1) {
            cout << "ans0:" << ans0 << endl;
            cout << "ans1:" << ans1 << endl;
            cout << "Oops!" << endl;
            break;
        }
        // else {
        //     cout << "pass, test:" << i << " ans:" << ans0 << endl;
        // }
    }
    // int ans0 = sol.win(arr);
    // int ans1 = sol.winDp(arr);
    // cout << "ans0:" << ans0 << ", ans1:" << ans1 <<endl;
    cout << "测试结束" << endl;
}