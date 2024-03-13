/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-13 10:56:58
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

// 有装下8个苹果的袋子、装下6个苹果的袋子，一定要保证买苹果时所有使用的袋子都装满
// 对于无法装满所有袋子的方案不予考虑，给定n个苹果，返回至少要多少个袋子
// 如果不存在每个袋子都装满的方案返回-1

class Solution {
public:
    int bags1(int apple) {
        int res = -1;
        if (apple == 0) {
            return 0;
        }
        int max_8 = apple / 8;
        // cout << "max_8:" << max_8 << endl;
        for (int i=max_8; i>=0; i--) {
            int rest = apple - i*8;
            // cout << "rest:" << rest  << " i:" << i << " apple:" << apple << endl;
            if (rest == 0 || rest % 6 == 0) {
                res = i + rest / 6;
                break;
            }
        }
        return res;
    }
    int bags2(int apple) {
        if (apple == 0) {
            return 0;
        }
        if (apple % 2 == 1) {
            return -1;
        }
        if (apple < 18) {
            if (apple == 6 || apple == 8) {
                return 1;
            } else if (apple == 12 || apple == 14 || apple == 16) {
                return 2;
            } else {
                return -1;
            }
        } else {
            return (apple-18)/8 + 3;
        }
    }
};
int main() {
    int n=1000;
    Solution sol;
    for (int i=0;i<n;i++) {
        int ans0 = sol.bags1(i);
        // cout << i << ","<< ans0 << endl;
        int ans1 = sol.bags2(i);
        if (ans0 != ans1) {
            cout << i << ","<< ans0 << "," << ans1 << endl;
        }
    }
    return 0;
}