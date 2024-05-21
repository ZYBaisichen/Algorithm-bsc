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
一、问题引入
将都是正数的数组arr={a,b,c,d,e,f}，分成两部分。有多种分法：
{a},{b,c,d,e,f}
{a,b},{c,d,e,f}
{a,b,c},{d,e,f}
...
两个部分都会有累加和，即为sum_左和sum_右。怎么样的划分可以让两部分的累加和的最大值尽可能的小。即让max{sum_左，sum_右}尽可能小
利用前缀和数组，可以在O(1)上得到任何范围上的累加和。
例如：{3,5,6,2,3,2,4}， 如果枚举分割线在哪里得到不同的答案
{3},{5,6,2,3,2,4} -> max(3,22) = 22
{3,5},{6,2,3,2,4} -> max(8,17) = 17
{3,5,6},{2,3,2,4} -> max(14,11) = 14


1. 对于arr，首先生成一个数组ans, ans[i]代表[0...i]范围上的min{max(sum_左,sum_右)}值是多少？需要O(N)计算出来。

一个结论：0...i范围上最优一刀假设在k位置，那么0...i+1范围上最优一刀只需要在k及其往后的位置开始试就可以。
证明：0...i范围上的最优一刀切出来后sum_左和sum_右的关系(本质来源于范围增加sum增加的单调性)
1）假设sum_左<sum_右时，如果增加了arr[i+1]，sum_右本身就是增加的。如果这一刀往左挪动则会让sum_右变的更大，所以完全没有必要让这一刀往左挪动
2) 假设sum_左>sum_右时，当增加arr[i+1]，sum_右增加。
    a. 此时sum_右超过sum_左了，往左挪动只会让sum_右更大。所以不需要往左挪动了。
    b. sum_右增加之后还是小于sum_左的，这时的sum_左-sum_右的差值，要比sum_左(原)-sum_右(原)小。如果能往左挪动的话，原来就可以往左挪动了。
        比如3 100 16 20 5，原来分隔点在100和16之间，新加了5，右边依旧超不过左边，但往左挪动会让整体更不平衡。
进一步抽象：min{max{sum_左,sum_右}}
     F2   F1  指标s
     => 差{好{s_左,s_右}}
对于指标s和范围存在单调性，如果要求在指标上求较好情况的最差情况，都可能有这种枚举不回退的优化。进一步推广：好{差{s_左，s_右}}
*/

/*

例题：https://leetcode.cn/problems/allocate-mailboxes/description/
给一个arr，左往右是有序的。arr[i]=j代表i号居民点在x轴上的j位置。
k=3代表一共要建的邮局数量。每个邮局只能建在居民点上，建在不同位置，每个居民点到邮局的总距离就会不一样。
比如arr=[3,7,15],k=1。当建在7位置时，总距离是4+8=12；如果建在3位置，总距离就是4+12=16。

初步的思想：dp[i][j]代表0...i个居民点建j个邮局总距离是多少。假设能填出来这个表，则dp[n-1][k]就是答案。
    1. 0个邮局没有意义，第一列不用填
    2. 第一行，只有一个居民点，不管建多少邮局，总距离都是0
    3. 第二列：o..i上建一个居民点：
        a. 0..1上建在0或者1上都可以。都是arr[1]-arr[0]的距离
        b. 0..2上需要建在1上
        c. 0...3上建在1或者2上都行
        d. 总结就是建1个邮局都建在0...i的上中点或者下中点都行（初中几何）
    4. 普遍位置的dp[i][j] (一个样本做行一个样本做列，讨论末尾的情况)：
        0. 0...i上只建j-1个邮局，第j个邮局不建，依赖dp[i][j-1]
        a. 最后一个邮局负责i...i范围上的居民点，其余的0...i-1的居民点用j-1个邮局负责，即dp[i-1][j-1]
        b. 最后一个邮局负责i-1...i范围上的居民点，其余的0...i-2的居民点用j-1个邮局负责，即dp[i-2][j-1]
        ....
        求出所有的答案，得到最小值就是dp[i][j]的值
    5. 对于4中i-2..i上只建一个邮局，总距离是多少；i-3...i上只建一个邮局，总距离是多少；所以需要搞一个预处理数组record[i][j]代表i到j范围上只建1个邮局，总距离是多少。
        范围尝试模型：
        a. 对角线，record[i][i]只有一个居民点，建一个邮局，总距离是0
        b. 对于普遍位置dp[i][j]，建一个邮局在(j-i)/2上，每次都要将i..j所有的居民点和邮局求距离，最多O(N)复杂度，外层有O(N^2)，所以总复杂度是O(N^3)
        c. 对于0..0，邮局就建在0位置，record[0][0]=0;
            i) record[0][1]=[1]-[0]，当求record[0][1]时，认为邮局往后挪到了1位置。
            ii) reocord[0][2]当增加2这个居民点时，邮局不搬家，还在1位置，所以record[0][2]=record[0][1]+[2]-[1]
            iii) reocord[0][3]，新增了3，邮局在1和2位置没有区别，因为record[0][2]是基于邮局在1位置上算出来的，所以record[0][3]=record[0][2]+[3]-[1]。此时认为邮局搬家到了2位置。
            iv) 这样每个格子都是o(1)求出来。

*/

/*

四边形不等式优化

上述方法以求dp[5][2]为例，在0...5上建2个邮局，最小的距离综合是多少。主要依赖：
    record[0][5]
    record[5][5]+dp[4][1] //5..5上建一个邮局，0..4上建一个邮局
    record[4][5]+dp[3][1] //4..5上建一个邮局，0..3上建一个邮局
    record[3][5]+dp[2][1] //3..5上建一个邮局，0..2上建一个邮局
    record[2][5]+dp[1][1] //2..5上建一个邮局，0..1上建一个邮局
    record[1][5]+dp[0][1] //1..5上建一个邮局，0..0上建一个邮局
    record[0][5]          //0..5上建一个邮局
对于每个dp[i][j]将依赖前一列的所有的行，是N规模的。假设dp的格子数量是N*k的，如果不优化的话，总复杂度达到了O(N^2*k)

可以使用四边形不等式的特征：
1. 枚举行为：每个各自有枚举行为
2. 单调性：每个格子的值，对n和k有一种说不清道不明的单调关系。且是相反的。
    1) 比如0..7上有2个邮局，那么增加一个居民点，总距离肯定增加，即0...8上有2个邮局。正向
    2) 0..8上有2个邮局，增加1个邮局时，总距离肯定会变小。负向
3. 区间划分问题
4. 求一个格子时，不同时依赖本行的值和本列的值。比如邮局问题的dp[i][j]只依赖左上角值


假设，居民点个数是0...5的，邮局有3个，求dp[5][3]。
    1. 当求0...4上有3个邮局，最优划分情况下，最后一个邮局负责k..4时。
    2. 当再添加一个邮局，最优划分下最后一个邮局负责k'...5，则k<=k'。即最后的邮局的位置不会比k往左（一种猜法）
    3. dp[5][3]的上方dp[4][3], 是0..4范围上放3个邮局, 此时最后一个邮局的位置在k位置。当增加一个居民点时，最后一邮局的位置肯定在k及其右边
    4. dp[5][3]的右边dp[5][4]，是0..5范围上放4个邮局，此时最后一个邮局的位置在kk位置。当减少一个邮局是，最后一个邮局的位置肯定在kk的左边
    5. 这样上方确定了最后一个邮局位置的下限，右方确定了最后一个邮局位置的上限
    6. 所以使用一个choose数组，记录[i][j](0..i号居民点建j个邮局最后一个邮局所在位置)。
    7. 填dp[i][j]格子时，只依赖左上角dp[i-1...0][j-1]的值，所以对于每一行，从右往左。每一列从上往下，这样每一个位置都可以拿到上方和下方的上下限做优化

*/

class Solution
{
public:
    int minDistanceOrigin(vector<int>& arr, int num) { //86ms
        int n = arr.size();
        if (n==0) {
            return 0;
        }
        sort(arr.begin(), arr.end());
        vector<vector<int>> record(n, vector<int>(n,0));
        get_record(arr, record);

        vector<vector<int>> dp(n, vector<int>(num+1,0));
        //dp[...][0]0个邮局不需要填都是0
        //dp[0][...]0个居民点也不需要填都是0

        //第一列
        for (int i=0;i<n;i++) {
            dp[i][1] = record[0][i];
        }
        //i个居民点，建i个邮局时总距离也是0
        for (int i=1;i<n;i++) {
            for (int j=2;j<=min(i, num);j++) { //邮局数量到num或者i
                dp[i][j] = record[0][i];//0...i范围只分最后一个邮局
                cout << "i:" << i << " j:" << j  << " first:" << dp[i][j]<<endl;
                //枚举最后一个邮局负责的范围，i..0
                //四边形不等式可以对这个枚举进行优化
                for (int k=i;k>0;k--) {
                    //0...k-1上用j-1个邮局覆盖
                    // cout << "i:" << i << " j:" << j << " k:" << k << " record[k][i]:" << record[k][i]  << " dp[k-1][j-1]:"<< dp[k - 1][j - 1] << endl;
                    dp[i][j] = min(dp[i][j], record[k][i] + dp[k-1][j-1]);
                }
            }
        }

        // cout << "dp:" << endl;
        // for (int i=0;i<n;i++) {
        //     for (int j=0;j<=num;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return dp[n-1][num];
    }

    void get_record(vector<int>& arr, vector<vector<int>>& record) {
        int n = arr.size();
        //对角线都是0，不用填
        for (int l=0;l<n;l++) {
            for (int r=l+1;r<n;r++) {
                /*
                l r范围
                0 邮局建在了0位置
                算0 1时，邮局还建在了0位置, (0+1)/2=0
                算0 1 2时，邮局还建在1位置，(0+2)/2=1
                所以算record[l][r]时，邮局此时建在了(l+r)/2上
                */
            //    cout << "l:" << l << " r:" << r << " (l+r)/2:" << (l+r)/2 << " arr[r]:" << arr[r] <<  " arr[(l+r)>>1]:"<< arr[(l+r)>>1] << endl;
                record[l][r] = record[l][r-1] + arr[r] - arr[(l+r)>>1];
            }
        }
        // cout << "record:" << endl;
        // for (int i=0;i<n;i++) {
        //     for (int j=0;j<n;j++) {
        //         cout << record[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
    }

    //复杂度是O(N*m)，枚举范围会越来越小。
    int minDistance(vector<int>& arr, int num) { //7ms
        int n = arr.size();
        if (n==0) {
            return 0;
        }
        sort(arr.begin(), arr.end());
        vector<vector<int>> record(n, vector<int>(n,0));
        get_record(arr, record);

        //dp[...][0]0个邮局不需要填都是0
        //dp[0][...]0个居民点也不需要填都是0
        vector<vector<int>> dp(n, vector<int>(num+1,0));

        //choose[i][j]在求dp[i][j]，最优解情况下，最后一个邮局负责了k...i范围的居民点，则将这个k记录到choose[i][j]中。
        //第0行，不管有多少邮局，最后邮局位置都在0位置
        //第0列不需要填
        vector<vector<int>> choose(n, vector<int>(num+1,0));


        //第一列
        for (int i=0;i<n;i++) {
            dp[i][1] = record[0][i];
            choose[i][1] = 0; //只有一个邮局，负责0...i范围，将最右边居民点的位置记录到choose位置上
        }

        //i个居民点，建i个邮局时总距离也是0
        for (int i=1;i<n;i++) {
            for (int j=min(i, num);j>=2;j--) { //从右往左求
                int down = choose[i-1][j]; //上方
                int up = j == min(i, num) ? i : choose[i][j+1]; //k取值最大为i，当右边没有格子时，取最大值i
                dp[i][j] = record[0][i];//0...i范围只分最后一个邮局。 此时choose[i][j]应该为0，因为初始化就是0，这里就不显示写了
                // cout << "i:" << i << " j:" << j << " down:" << down << " up:" << up << endl;
                for (int k=max(down, 1); k<=up;k++) {
                    // cout << "i:" << i << " j:" << j << " down:" << down << " up:" << up << " k:"<<k <<  endl;
                    if (dp[k-1][j-1] + record[k][i]<dp[i][j]) {
                        choose[i][j] = k;
                        dp[i][j] = dp[k-1][j-1] + record[k][i];
                    }
                }
            }
        }

        // cout << "dp:" << endl;
        // for (int i=0;i<n;i++) {
        //     for (int j=0;j<=num;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        // cout << "choose:" << endl;
        // for (int i=0;i<n;i++) {
        //     for (int j=0;j<=num;j++) {
        //         cout << choose[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return dp[n-1][num];
    }


};

int main() {
    Solution sol;
    vector<int> arr = {3,18,105,877,987,1003};
    int num = 3;
    // //答案118

    // vector<int> arr = {1, 8, 12, 10, 3};
    // int num = 3;
    cout << sol.minDistanceOrigin(arr, num) << endl;
    cout << sol.minDistance(arr, num) << endl;
}