/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-06 16:01:30
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
* Description: 分裂数字。
 * 给定一个正数n，求n的裂开方法数，
 * 规定：后面的数不能比前面的数小
 * 比如4的裂开方法有：
 * 1+1+1+1、1+1+2、1+3、2+2、4
 * 5种，所以返回5
*/

class Solution
{
public:
    int ways(int num) {
        if(num == 1) {
            return 1;
        }
        return process(1, num); //最外层规定从1开始裂开
    }
    //返回num有多少种裂化方法
    /*
    pre表示之前裂开的数字。用来做去重，去掉裂开数列降序的情况。
    比如f(1,7)，第一次使用4，会调用f(4,3)，此时rest=3<4=pre了，所以方法数是0.但这个答案会在f(3,4)中找到。
    这样就可以去重了
    */
    int process(int pre, int rest) {
        if (rest==0) { //当还剩rest数需要裂开
            return 1;
        }
        if (rest<pre) { //有减枝的意思
            return 0;
        }
        int ans = 0;
        for (int i=pre;i<=rest;i++) {
            ans += process(i, rest-i);//但这里可能会重复？
        }
        return ans;
    }

    int ways_dp(int num) {
        if(num == 1) {
            return 1;
        }
        vector<vector<int>> dp(num+1, vector<int>(num+1, 0));//pre从1变化到num, rest从num变化到0
        //dp[i][j]代表在最小裂化pre的前提下，还剩rest需要裂化，裂化方法数是多少
        // 第一列
        for (int i=1;i<=num;i++) {
            dp[i][0] = 1; //当rest为0时，只有一种方法
        }
        // 第一行都是0，不填
        for (int pre=num;pre>=0;pre--) {
            for (int rest=pre;rest<=num;rest++) {
                for (int i=pre;i<=rest;i++) {
                    dp[pre][rest] += dp[i][rest-i];
                }
            }
        }
        return dp[1][num];
    }

    /*
    对枚举行为优化: O(N^2)
    观察到:
    dp[7][7]=dp[7][0]
    dp[6][6]=dp[6][0]
    dp[6][7]=dp[6][1]+dp[7][0]=dp[6][1]=dp[7][0]
    dp[5][5]=dp[5][0]
    dp[5][6]=dp[5][1]+dp[6][0] = dp[5][1]+dp[6][6]
    dp[5][7]=dp[5][1]+dp[6][1]+dp[7][0] = dp[5][1]+dp[6][7]
    所以，dp[i][j]=dp[i][j-i]+dp[i+1][j]
    当i==j时，dp[i+1][j]=0，上述公式依然成立，所以总循环不变
    */
    int ways_dp_youhua(int num) {
        if(num == 1) {
            return 1;
        }
        vector<vector<int>> dp(num+1, vector<int>(num+1, 0));//pre从1变化到num, rest从num变化到0
        //dp[i][j]代表在最小裂化pre的前提下，还剩rest需要裂化，裂化方法数是多少
        // 第一列
        for (int i=1;i<=num;i++) {
            dp[i][0] = 1; //当rest为0时，只有一种方法
        }
        // 第一行都是0，不填
        for (int pre=num;pre>=0;pre--) {
            for (int rest=pre;rest<=num;rest++) {
                // cout << pre << " " << rest << << endl;
                dp[pre][rest] = dp[pre][rest-pre];
                if (pre<num) {
                    dp[pre][rest] += dp[pre + 1][rest];
                }
            }
        }
        return dp[1][num];
    }

};

int main()
{

    Solution sol;
    // string s = "aba";
    // int num=6;
    // cout << sol.ways(num) << endl;
    // cout << sol.ways_dp(num) << endl;
    // cout << sol.ways_dp_youhua(num) << endl;
    int test_num = 10;   
    for (int num=1;num<=test_num;num++) {
        int ans1 = sol.ways(num);
        int ans2 = sol.ways_dp(num);
        int ans3 = sol.ways_dp_youhua(num);
        if (ans1!=ans2) {
            cout << "ans1:" << ans1 << " ans2:" << ans2 << endl;
        }
        if (ans1!=ans3) {
            cout << "ans1:" << ans1 << " ans3:" << ans2 << endl;
        }
    }
    return 0;
}