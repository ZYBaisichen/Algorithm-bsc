/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-01 15:42:56
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
 * int[] d，d[i]：i号怪兽的能力
 * int[] p，p[i]：i号怪兽要求的钱
 * 开始时你的能力是0，你的目标是从0号怪兽开始，通过所有的怪兽。
 * 如果你当前的能力，小于i号怪兽的能力，你必须付出p[i]的钱，贿赂这个怪兽，然后怪兽就会加入你
 * 他的能力直接累加到你的能力上；如果你当前的能力，大于等于i号怪兽的能力
 * 你可以选择直接通过，你的能力并不会下降，你也可以选择贿赂这个怪兽，然后怪兽就会加入你
 * 他的能力直接累加到你的能力上
 * 返回通过所有的怪兽，需要花的最小钱数
 * （课上会给出不同的数据量描述）
 *
 * 总结：这道题以money或者ability两个变量作为列。根据题目所给的数据量，推测应该使用哪个变量作为列。
 * 三个变量钱数、能力值、能到达的怪兽号。三个变量可以用二维数组表示。如果能力值累计数太大就用钱数做列。
 */

/*

*/
typedef long long ll;
class Solution
{
public:
    //d为怪兽的能力值。p是贿赂每只怪兽的价格
    int min_money(vector<int>& d, vector<int>& p) {
        return process1(d,p, 0, 1);
    }
    //来到了idx，[0...idx-1]都已经决定好要不要贿赂了，并得到了ability能力
    //返回[idx...len]通关之后需要的最小钱数
    int process1(vector<int>& d, vector<int>& p, int idx, int ability) {
        int len = d.size();
        if (idx == len) { //没有怪兽了
            return 0;
        }

        //如果当前小于怪兽的能力，则必须贿赂才能通过
        if (ability < d[idx]) {
            return process1(d, p, idx + 1, ability + d[idx]) + p[idx];
        } else { //否则要么可以贿赂，或者不贿赂。两者取最小
            return min(
                process1(d, p, idx + 1, ability + d[idx]) + p[idx], 
                process1(d, p, idx + 1, ability));
        }
    }

    int min_money_dp(vector<int>& d, vector<int>& p) {
        return process1_dp(d, p);
    }
    //使用ability做列有个问题是，怪兽的能力可能会很大，内存会超限。
    //所以在怪兽能力很大的时候可以换一种思维
    int process1_dp(vector<int>& d, vector<int>& p) {
        int len = d.size();
        if (len == 0) {
            return 0;
        }
        ll sum_ability = 0;
        for (int i=0;i<len;i++) {
            sum_ability += d[i]; 
        }
        //dp[i][j]代表0...i-1已经决定好，[i...len-1]上通关怪兽在当前能力是j的情况下，所花费的最小钱数
        vector<vector<int>> dp(len + 1, vector<int>(sum_ability+1, INT_MAX));
        //填最后一行
        for (int j = 0; j <= sum_ability;j++) {
            dp[len-1][j] = 0;
        }
        //dp[i][j]表示通过[i...len-1]个怪兽在当前能力为j的情况下，花费的最小钱数
        //因为依赖idx+1，和ability+d[idx]， 所以右到左，从下到上依次填表
        for (int i=len-2;i>=0;i--) {
            for (int j=sum_ability;j>=0;j--) {
                if (j < d[i]) { //当前能力小于怪兽的能力，必须贿赂
                    dp[i][j] = dp[i+1][j+d[i]] + p[i];
                } else {
                    dp[i][j] = min(dp[i+1][j], dp[i+1][j + d[i]] + p[i]);
                }
            }
        }
        return dp[0][0];
    }

    //d为怪兽的能力值。p是贿赂每只怪兽的价格
    int min_money2(vector<int>& d, vector<int>& p) {
        int len = d.size();
        ll sum_money = 0;
        for (int i=0;i<len;i++) {
            sum_money += p[i];
        }
        for (int j=0;j<=sum_money;j++) {
            // cout << "=======" << endl;
            ll ability = process2(d,p, len-1, j); //表示在通过idx号怪兽，在花费为j时能否通过
            // cout << "cur_money:" << j << " ability:" << ability << endl;
            if (ability != -1) {
                return j; //第一个可以通过的钱数就是答案
            }
        }
        return sum_money;
    }

    // 表示在通过idx号怪兽，刚好花费为j时，能达到的最大能力值
    ll process2(vector<int>& d, vector<int>& p, int idx, int money) {
        // cout << "======" << endl;
        // cout << "idx:" << idx << " money:" << money << endl;
        if (money < 0)
        {
            return -1;
        }
        if (idx == 0) { //通过第一头怪兽，需要正好花费p[i]时才能通过
            if (money == p[idx]) {
                return d[idx];
            } else {
                return -1; //不能通过
            }
        }
        //1. 要想通过当前怪兽，上一个怪兽如果花费money-p[i]，则当前怪兽刚好花费p[i]就可以通过
        ll cur_ability = -1;
        int ability1 = -1;
        if (money-p[idx]>=0) {
            ability1 = process2(d, p, idx-1, money-p[idx]);
        }
        if (ability1!=-1) {
            cur_ability = ability1 + d[idx];
            // cout << "1111222idx:" << idx << " money:" << money << " ability1:" << ability1 << " cur_ability:" << cur_ability << " cur_d:" << d[idx]<< endl;
        }
        // cout << "1111idx:" << idx << " money:" << money << " ability1:" << ability1 << " cur_ability:" << cur_ability  << endl;

        //2. 上一个怪兽花费了j元，并且获得的能力超过了当前怪兽的能力，则当前怪兽可以不花钱，也是正好花费了j元通过了idx号怪兽
        ll ability2 = process2(d, p, idx-1, money);
        // cout << "idx:" << idx << " money:" << money << " ability111: " << cur_ability << " ability2:" << ability2 << endl;
        // cout << "ability222: " << ability2 << endl;
        if (ability2 >= d[idx]) {
            cur_ability = max(cur_ability, ability2);
        }
        // cout << "idx:" << idx << " money:" << money << " cur_ability: " << cur_ability << endl;
        // cout << "====" << endl;

        return cur_ability;
    }


    //d为怪兽的能力值。p是贿赂每只怪兽的价格
    int min_money2_dp(vector<int>& d, vector<int>& p) {
        int len = d.size();
        ll sum_money = 0;
        /*
        dp[i][j]: 表示到达第i只怪兽，花费必须为j时，能达到的最大能力值
        */
        for (int i=0;i<len;i++) {
            sum_money += p[i];
        }
        vector<vector<int>> dp(len, vector<int>(sum_money+1,-1));
        dp[0][p[0]] = d[0]; //第一个怪兽，必须花费p[0]可以获得能力值

        for (int i=1;i<len;i++) {
            for (int j=0;j<=sum_money;j++) {
                // 1、不管当前能力值够不够，都直接买。切必须是前一个怪兽花费j-p[i]
                if (j-p[i]>=0 && dp[i-1][j-p[i]] != -1) {
                    dp[i][j] = dp[i-1][j-p[i]] + d[i]; //前一个花费j-p[i]当前花费p[i]获得的最大能力
                }
                // 2. 前一个怪兽花了j元，能力够了，则本次不用再买了
                if (dp[i-1][j] >= d[i]) {
                    dp[i][j] = max(dp[i][j], dp[i-1][j]);
                } 
            }
        }
        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<=sum_money;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        cout << endl;
        for (int j=0;j<=sum_money;j++) {
            if (dp[len-1][j] != -1) {
                return j; //第一个可以通过的钱数就是答案
            }
        }
        return -1;
    }
    
};

int main()
{

    Solution sol;
    vector<int> d = {10, 20, 5,100,110};
    vector<int> p = {5, 15, 5,1,5};
    // vector<vector<int>> arr = {{1,1},{2,2},{3,3}};
    // string s = "abacb";
    // string s = "abcabcbb";

    cout << sol.min_money(d,p) << endl;
    cout << sol.min_money_dp(d, p) << endl;
    cout << sol.min_money2(d, p) << endl;
    cout << sol.min_money2_dp(d, p) << endl;

    return 0;
}