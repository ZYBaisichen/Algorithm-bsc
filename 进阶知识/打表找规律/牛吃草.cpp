/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-13 11:16:54
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

using namespace std;

// 草一共有n的重量，两只牛轮流吃草，A牛先吃，B牛后吃
// 每只牛在自己的回合，吃草的重量必须是4的幂，1、4、16、64....
// 谁在自己的回合正好把草吃完谁赢，根据输入的n，返回谁赢
class Solution {
public:
    string win1(int n) {
        return f(n, "A");
    }
    // rest表示当前还剩多少草
    // cur标识从cur的牛先手开始吃
    // 在剩余rest草，当前cur开始吃的情况下，返回胜出的牛
    string f(int rest, string cur)  {
        string houshou = cur == "A" ? "B" : "A";
        if (rest < 5) {
            //为0时表示前一个牛已经吃完了
            return (rest==2 || rest ==0) ? houshou:cur;
        }

        int pick = 1;
        while (pick <= rest) {
            //以后手开始赢的人如果是它的后手的话(为cur)，才是当前的牛赢
            if (f(rest - pick, houshou) == cur) {
                return cur;
            }
            pick *= 4;
        }
        return houshou;
    }

    string win2(int n) {
        if (n < 5) {
            //为0时表示前一个牛已经吃完了
            return (n == 2 || n == 0) ? "B" : "A";
        }
        if (n %5 == 0 || n%5 == 2) {
            return "B";
        } else {
            return "A";
        }
    }
};
int main() {
    int n=60;
    Solution sol;
    for (int i=0;i<n;i++) {
        string ans0 = sol.win1(i);
        // cout << i << ","<< ans0 << endl;
        string ans1 = sol.win2(i);
        if (ans0 != ans1) {
            cout << i << ","<< ans0 << "," << ans1 << endl;
        }
    }
    return 0;
}