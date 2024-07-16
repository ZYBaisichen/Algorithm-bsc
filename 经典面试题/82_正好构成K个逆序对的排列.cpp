/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-06 16:59:58
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
* 给定一个整数N，代表你有1-N这些数字，在给定一个整数K。你可以随意排序这些数字，但是每一种排列都有若干逆序对。返回有多少种排列，刚好有K个逆序对
例子:
input: n=3 k=0
output: 1
[1,2,3]这一个排列有0个逆序对

input: n=3, k=1
output:2
[3 2 1]有[3 2],[3 1],[2 1]三个逆序对，所以不达标
达标的只有
[1 3 2]只有[3,2]这一个逆序对
[2 1 3]只有[2 1]这一个逆序对

求的是，1-n全排列，逆序对正好是k个的排列。
*/

/*
给了两个参数，使用一个样本做行一个样本做列的尝试方法。
n做行，k做列。dp[i][j]代表1~i全排列，逆序对刚好是j个的排列数

1. 第一行，n==1。只有0个逆序对。dp[1][0]=1，其他是0
2. 第一列，有k=0个逆序对，只能是有序的才是0个, 所以都是1
3. 普遍位置dp[i][j]，假设是dp[7][3]
    a. 假设将7插入到最后一个位置，不产生任何逆序对，依赖dp[6][3]
    b. 将7插在倒数第1个位置, 产生1个逆序对。dp[7][3] = dp[6][2]
    c. 将7插在倒数第2个位置, 产生2个逆序对。dp[7][3] = dp[6][1]
    d. 将7插在倒数第3个位置, 产生3个逆序对。dp[7][3] = dp[6][0]
    e. 假设求dp[6][7],将依赖dp[5][4...2]。最后将6插在最前面将产生5个逆序对, 所以1~5的全排列至少要再产生2个逆序对才行(6-5+1)。
    所以dp[i][j]不插在最后一个位置，将依赖dp[i-1][j-1....max(j-i+1,0)]的累加和

行列模型只关心结尾；范围尝试模型关心开头和结尾
*/
typedef unsigned long long ll;
class Solution
{
public:
    int kInversePairs1(int n, int k)
    {
        if (n < 1 || k < 0)
        {
            return 0;
        }
        if (k == 0)
        {
            return 1;
        }
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        dp[1][0] = 1; //1-1，只有0个逆序对
        for (int i = 2; i <= n; i++) {
            dp[i][0] = 1;
        }

        for (int i=2;i<=n;i++) {
            for (int j=1;j<=k;j++) {
                dp[i][j] = dp[i-1][j];
                //依赖上一行
                //需要让上一行产生kk个逆序对，从最后一个数插在倒数第二个位置开始算
                for (int kk=j-1;kk>=max(0,j-i+1);kk--) {
                    dp[i][j] += dp[i-1][kk];
                }
            }
        }
        return dp[n][k];
    }

    /*
    观察到:
    dp[7][3] = sum(dp[6][2...0])+dp[6][3]=sum(dp[6][3...0])
    dp[7][4] = sum(dp[6][3...0])+dp[6][4]=sum(dp[6][4...0])=dp[7][3]+dp[6][4] (依赖dp[i][j-1]+dp[i-1][j])
    dp[7][5] = sum(dp[6][4...0])+dp[6][5]=sum(dp[6][5...0])=dp[7][4]+dp[6][5]
    dp[7][6] = sum(dp[6][5...0])+dp[6][6]=sum(dp[6][6...0])=dp[7][5]+dp[6][6]

    dp[7][7] = sum(dp[6][6...1])+dp[6][7]=sum(dp[6][7...1])=dp[7][6]+dp[6][7]-dp[6][0]（最后一位是dp[i-1][j-i]）
    dp[7][8] = sum(dp[6][7...2])+dp[6][8]=sum(dp[6][8...2])=dp[7][7]+dp[6][8]-dp[6][1]
    dp[7][9] = sum(dp[6][8...3])+dp[6][9]=sum(dp[6][9...3])=dp[7][8]+dp[6][9]-dp[6][3]
    */
    int kInversePairs(int n, int k)
    {
        if (n < 1 || k < 0)
        {
            return 0;
        }
        if (k == 0)
        {
            return 1;
        }
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        dp[1][0] = 1; //1-1，只有0个逆序对
        for (int i = 2; i <= n; i++) {
            dp[i][0] = 1;
        }

        for (int i=2;i<=n;i++) {
            for (int j=1;j<=k;j++) {
                dp[i][j] = dp[i][j-1]+dp[i-1][j]; //dp[i][j-1]是助力的。左边的数
                if (i<=j) {
                    dp[i][j] -= dp[i-1][j-i];
                }
            }
        }
        return dp[n][k];
    }

    int kInversePairs_lt(int n, int k)
    {
        if (n < 1 || k < 0)
        {
            return 0;
        }
        if (k == 0)
        {
            return 1;
        }
        vector<vector<ll>> dp(n + 1, vector<ll>(k + 1, 0));
        dp[1][0] = 1; //1-1，只有0个逆序对
        for (int i = 2; i <= n; i++) {
            dp[i][0] = 1;
        }

        for (int i=2;i<=n;i++) {
            for (int j=1;j<=k;j++) {
                dp[i][j] = (dp[i][j-1]+dp[i-1][j])%1000000007; //dp[i][j-1]是助力的。左边的数
                if (i<=j) {
                    /*
                    为了防止出现负数，需要增加一个
                    */
                    dp[i][j] = (dp[i][j]-dp[i-1][j-i]+1000000007)%1000000007;
                }
            }
        }
        return dp[n][k];
    }
};

int main()
{

    Solution sol;
    // string s = "aba";
    int n=1000;
    int k=1000;
    // cout << sol.kInversePairs1(n,k) << endl;
    // cout << sol.kInversePairs(n,k) << endl;
    cout << sol.kInversePairs_lt(n,k)<< endl;
    return 0;
}