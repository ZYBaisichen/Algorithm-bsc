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
#include <algorithm>

using namespace std;

/*
 * Description: https://leetcode.cn/problems/super-egg-drop/
 * 一座大楼有0~N层，地面算作第0层，最高的一层为第N层
 * 已知棋子从第0层掉落肯定不会摔碎，从第i层掉落可能会摔碎，也可能不会摔碎(1≤i≤N)
 * 给定整数N作为楼层数，再给定整数K作为棋子数
 * 返回如果想找到棋子不会摔碎的最高层数，即使在最差的情况下扔的最少次数.
 * <p>
 * 一次只能扔一个棋子
 * N=10，K=1
 * 返回10
 * 因为只有1棵棋子，所以不得不从第1层开始一直试到第10层
 * 在最差的情况下，即第10层是不会摔坏的最高层，最少也要扔10次
 * <p>
 * N=3，K=2
 * 返回2
 * 先在2层扔1棵棋子，如果碎了试第1层，如果没碎试第3层
 * N=105，K=2
 * 返回14
 * 


力扣原题描述：
 给你 k 枚相同的鸡蛋，并可以使用一栋从第 1 层到第 n 层共有 n 层楼的建筑。

已知存在楼层 f ，满足 0 <= f <= n ，任何从 高于 f 的楼层落下的鸡蛋都会碎，从 f 楼层或比它低的楼层落下的鸡蛋都不会破。

每次操作，你可以取一枚没有碎的鸡蛋并把它从任一楼层 x 扔下（满足 1 <= x <= n）。如果鸡蛋碎了，你就不能再次使用它。如果某枚鸡蛋扔下后没有摔碎，则可以在之后的操作中 重复使用 这枚鸡蛋。

请你计算并返回要确定 f 确切的值 的 最小操作次数 是多少？

 
示例 1：

输入：k = 1, n = 2
输出：2
解释：
鸡蛋从 1 楼掉落。如果它碎了，肯定能得出 f = 0 。 
否则，鸡蛋从 2 楼掉落。如果它碎了，肯定能得出 f = 1 。 
如果它没碎，那么肯定能得出 f = 2 。 
因此，在最坏的情况下我们需要移动 2 次以确定 f 是多少。 
示例 2：

输入：k = 2, n = 6
输出：3
示例 3：

输入：k = 3, n = 14
输出：4
 

提示：

1 <= k <= 100
1 <= n <= 104
*/

/*
难点在于最差情况不知道在哪里。假设只有一个棋子，只能从第一层开始试，一直最多试n次。

大前提：如果k>log(n)，二分肯定可以试出来，棋子数量足够二分
试法：第一颗棋子扔哪里(业务限制模型)
    1. f(i,j)还剩i层楼，还有j个棋子，需要最少试几次。比如在99层楼不碎，还剩100-104个楼层需要尝试，有3个棋子。那么可以调f(5,3)
    2. 假设还剩100层楼需要试(还剩5颗棋子)，此时的最低楼层是a，a以下的楼层都是不碎的。当第一颗棋子仍在了a处。两种情况
        a. 在a层棋子碎了：则a-1层一定是最高的不碎楼层，扔了1次。还剩0层需要试，还有4个棋子, 后续需要扔的次数是f(0,4)
        b. 在a层棋子没有碎：后续需要扔的次数是f(99,5)
        c. 当前的解是1+max(f(99,5), f(0,4))。之所以要取max，是因为小的解是可能的，但不是必然的。答案是需要在必然能试出来最小不会碎的楼层的前提下的最小尝试次数。
    3. 对于f(7,2)，还剩7层楼需要试，有2个棋子
        a. 第一颗棋子扔在1位置，碎了还需要f(0,1)，没碎需要f(6,1)次。共需要1+max(f(0,1),f(6,1))
        b. 第一颗棋子扔在2位置，碎了还需要f(1,1)，没碎需要f(5,1)次。共需要1+max(f(1,1),f(5,1))
        c. 第一颗棋子扔在3位置，碎了还需要f(2,1)，没碎需要f(4,1)次。共需要1+max(f(2,1),f(4,1))
        ...

        d. 以上结果求min(a,b,c,.....)就是f(7,2)的结果
    4. base cashe: 还剩0层楼时f(0,i)=0；当棋子数是1时f(i,1)=i
*/


class Solution {
public:
    int superEggDrop_baoli(int k, int n) {
        if(n==0) {
            return 0;
        }
        if (k==1) {
            return n;
        }
        return process(n, k);
    }

    //还剩n层楼，还有k颗棋子，需要最少试几次。
    int process(int rest, int k) {
        //还剩0层，不需要试
        if (rest == 0) {
            return 0;
        }
        //1个棋子，只能挨个试rest次
        if (k==1) {
            return rest;
        }

        int ans = INT_MAX;
        for (int i=1;i<=rest;i++) {
            ans = min(ans, max(process(i-1,k-1), process(rest-i, k))); //碎了和没碎取最大值
        }
        // cout << "rest:" << rest << ", k:" << k << ", ans:" << ans+1 << endl;
        return ans+1; //加上当前层试的1次
    }


    //枚举行为会超时，复杂度O(n^2*k)
    int superEggDrop_dp(int k, int n) {
        if(n==0) {
            return 0;
        }
        if (k==1) {
            return n;
        }

        //dp[i][j]代表还剩i层楼，还有j颗棋子，需要最少试几次。
        vector<vector<int>> dp(n+1, vector<int>(k+1, 0));
        //第0行，0层楼，需要试0次

        //一个棋子，第一列
        for (int i=1;i<=n;i++) {
            dp[i][1] = i;
        }
        //依赖上方格子和左方格子，所以从上到下，从左到右枚举
        for (int i=1;i<=n;i++) {
            for (int j=2;j<=k;j++) {
                int ans = INT_MAX;
                for (int kk=1;kk<=i;kk++) {
                    // cout << "i:" << i << ", j:" << j << ", kk:" << kk << " :" << dp[i-1][j-1] <<","<< dp[i-kk][j]<< endl;
                    ans = min(ans, max(dp[kk-1][j-1], dp[i-kk][j]));
                }
                dp[i][j] = ans+1;
            }
        }

        // cout << "dp:" << endl;
        // for (int i=0;i<=n;i++) {
        //     for (int j=0;j<=k;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout<< endl;
        return dp[n][k];
    }
    /*
    四边形不等式优化
    以dp[7][2]为例, 依赖的格子为(求min)：
        1+max{dp[0][1], dp[6][2]} //扔第1层，碎了依赖dp[0][1];没碎，依赖dp[6][2]
        1+max{dp[1][1], dp[5][2]} //扔第2层
        1+max{dp[2][1], dp[4][2]} //扔第3层
        1+max{dp[3][1], dp[3][2]} //扔第4层
        1+max{dp[4][1], dp[2][2]} //扔第5层
        1+max{dp[5][1], dp[1][2]} //扔第6层
        1+max{dp[6][1], dp[0][2]} //扔第7层
    dp[7][2]需要枚举本列往上，和左上角往上所有格子。
    符合的特征：
        1. 有枚举行为
        2. 假设有100层楼，有3个棋子，当增加一个棋子时，尝试的次数会变小，减少一个棋子尝试次数会增加（极端情况时剩一颗棋子尝试次数会变得很大); 当增加楼层时，尝试次数会变多，减少楼层尝试次数会变小。
        3. 区间划分问题
        4. 不同时依赖本行和本列的值：依赖本行，但不依赖本列

    因为枚举的是第一个棋子扔的位置，对于dp[7][3]来说，dp[6][3]求出来后，当新增一层时，第一次扔的位置不会比dp[6][3]的第一个棋子扔的位置更靠下；dp[7][4]求出来后，dp[7][3]第一次扔的位置，不会比dp[7][4]扔的更靠上。
    */

   //枚举行为会超时，复杂度O(n^2*k)
    int superEggDrop_dp(int k, int n) {
        if(n==0) {
            return 0;
        }
        if (k==1) {
            return n;
        }

        //dp[i][j]代表还剩i层楼，还有j颗棋子，需要最少试几次。
        vector<vector<int>> dp(n+1, vector<int>(k+1, 0));
        //第0行，0层楼，需要试0次

        // choose[i][j]代表在计算dp[i][j]时，最优解情况下，第一个棋子从哪层楼开始扔的
        vector<vector<int>> choose(n + 1, vector<int>(k + 1, 0));

        //一个棋子，第一列
        for (int i=1;i<=n;i++) {
            dp[i][1] = i;
        }
        //依赖上方格子和左方格子，所以从上到下，从左到右枚举
        for (int i=1;i<=n;i++) {
            for (int j=2;j<=k;j++) {
                int ans = INT_MAX;
                for (int kk=1;kk<=i;kk++) {
                    // cout << "i:" << i << ", j:" << j << ", kk:" << kk << " :" << dp[i-1][j-1] <<","<< dp[i-kk][j]<< endl;
                    ans = min(ans, max(dp[kk-1][j-1], dp[i-kk][j]));
                }
                dp[i][j] = ans+1;
            }
        }

        // cout << "dp:" << endl;
        // for (int i=0;i<=n;i++) {
        //     for (int j=0;j<=k;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout<< endl;
        return dp[n][k];
    }
};

int main() {
    Solution sol;

    /*

    输入：k = 1, n = 2
    输出：2
    解释：
    鸡蛋从 1 楼掉落。如果它碎了，肯定能得出 f = 0 。
    否则，鸡蛋从 2 楼掉落。如果它碎了，肯定能得出 f = 1 。
    如果它没碎，那么肯定能得出 f = 2 。
    因此，在最坏的情况下我们需要移动 2 次以确定 f 是多少。
    示例 2：

    输入：k = 2, n = 6
    输出：3
    示例 3：

    输入：k = 3, n = 14
    输出：4
    */
    int k = 3;
    int n = 14;
    cout << sol.superEggDrop_baoli(k, n) << endl;
    cout << sol.superEggDrop(k, n) << endl;
}