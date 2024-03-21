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

using namespace std;
void print(vector<int>& res, string suf = "res") {
    cout << suf;
    for (auto iter:res) {
        cout << " " << iter;
    }
    cout << endl;
}

// 牛牛的背包问题 & 世界冰球锦标赛
// 牛牛准备参加学校组织的春游, 出发前牛牛准备往背包里装入一些零食, 牛牛的背包容量为w。
// 牛牛家里一共有n袋零食, 第i袋零食体积为v[i]。
// 牛牛想知道在总体积不超过背包容量的情况下,他一共有多少种零食放法(总体积为0也算一种放法)。
// 输入描述：
// 输入包括两行
// 第一行为两个正整数n和w(1 <= n <= 30, 1 <= w <= 2 * 10^9),表示零食的数量和背包的容量
// 第二行n个正整数v[i](0 <= v[i] <= 10^9),表示每袋零食的体积
// 输出描述：
// 输出一个正整数, 表示牛牛一共有多少种零食放法。
// 测试链接 : https://www.nowcoder.com/practice/d94bb2fa461d42bcb4c0f2b94f5d4281
// 测试链接 : https://www.luogu.com.cn/problem/P4799
// 请同学们务必参考如下代码中关于输入、输出的处理
// 这是输入输出处理效率很高的写法
// 提交以下所有代码，把主类名改成Main，可以直接通过
/*
5 1000
100 1500 500 500 1000
输出：8
*/

typedef long long ll;

class Solution {
public:
    //直观试法就是从左到右。看每个物品要不要放进去，走到头就找到了一种试法
    ll buy_tickets(vector<ll>& arr, ll big) {
        int len = arr.size();
        if (len==0) {return 0;}
        return f(arr, big, 0);
    }

    //当前到了idx，还剩big容量可以放入。返回idx...len-1有多少种方法
    ll f(vector<ll>& arr, ll big, int idx) {
        if (idx == arr.size()) { //来到最后一个了，找到了一种放法
            // cout << "idx: " << idx << ",big: " << big << ", value: " << 1 << endl;
            return 1;
        }
        //完全不考虑放入该物品
        ll no = f(arr, big, idx+1);
        ll yes = 0;
        if (arr[idx] <= big) { 
            yes = f(arr, big-arr[idx], idx+1);
        }
        // cout << "idx: " << idx <<  ",big: " << big << ", value: " << no + yes << endl;
        return no+yes;
    }

    //改造成动态规划
    ll buy_tickets_dp(vector<ll>& arr, ll big) {
        int len = arr.size();
        if (len==0) {return 0;}

        vector<vector<ll>> dp(len + 1, vector<ll>(big+1, 0));
        for (int i=0;i<=big;i++) {
            dp[len][i] = 1;
        }
        //依赖更小的big，更大的idx，idx从大到小，big从小到大填
        //从下面循环里可以看出，dp[i][j]代表的是将i...n个物品放入容量为j的背包中，有多少种放法
        for (int i=len-1;i>=0;i--) {
            for (int j=0;j<=big;j++) {
                dp[i][j] = dp[i+1][j];
                if (arr[i] <= j) {
                    dp[i][j] += dp[i+1][j-arr[i]];
                }
            }
        }

        // for (int i=0;i<=len;i++) {
        //     for (int j=0;j<=big;j++) {
        //         cout << " " << dp[i][j];
        //     }
        //     cout << endl;
        // }
        return dp[0][big];
    }

    //动态规划状态压缩
    //注意，如果状态的列比较多，纵向空间压缩意义就不大了。如果状态的行比较多，横向的空间压缩意义就不大了
    ll buy_tickets_dp_yasuo(vector<ll>& arr, ll bag) {
        int len = arr.size();
        if (len==0) {return 0;}
        //依赖更小的big，更大的idx，idx从大到小，big从小到大填
        //从下面循环里可以看出，dp[i][j]代表的是将i...n个物品放入容量为j的背包中，有多少种放法

        //进一步，因为第i行只会依赖第i+1行，且只依赖i+1行左侧某列的值。所以可以用一个数组，滚动更新

        vector<ll> dp(bag+1, 1); //首先最后一行都是1
        for (int i=0;i<=bag;i++) {
            dp[i] = 1;
        }

        for (int i=len-1;i>=0;i--) { //更新len轮
            for (int j=bag;j>=0;j--) {
                dp[j] = dp[j]; //首先等于前一行的值，即不放第i个物品
                if (arr[i] <= j) {
                    dp[j] += dp[j-arr[i]];
                }
            }
        }

        // for (int i=0;i<=len;i++) {
        //     for (int j=0;j<=big;j++) {
        //         cout << " " << dp[i][j];
        //     }
        //     cout << endl;
        // }
        return dp[bag];
    }
};

int main()
{
    Solution sol;
    // vector<ll> arr = {100,1500,500,500,1000};
    // ll big = 1000;

    vector<ll> arr = {1,15,5,5,10};
    ll big = 1000;
    cout << sol.buy_tickets(arr, big) << endl;
    cout << sol.buy_tickets_dp(arr, big) << endl;
    cout << sol.buy_tickets_dp_yasuo(arr, big) << endl;
    return 0;
}