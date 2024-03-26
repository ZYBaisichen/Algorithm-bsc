/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-26 20:27:01
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

using namespace std;

/*
给定一个布尔表达式和一个期望的布尔结果 result，布尔表达式由 0 (false)、1 (true)、& (AND)、 | (OR) 和 ^ (XOR) 符号组成。实现一个函数，算出有几种可使该表达式得出 result 值的括号方法。

示例 1:

输入: s = "1^0|0|1", result = 0

输出: 2
解释: 两种可能的括号方法是
1^(0|(0|1))
1^((0|0)|1)
示例 2:

输入: s = "0&0&0&1^1|0", result = 1

输出: 10
提示：

运算符的数量不超过 19 个

https://leetcode-cn.com/problems/boolean-evaluation-lcci/

*/

/*
范围尝试模型

过滤
1. 偶数位置必须是数字(0或者1); 奇数位置必须是二元操作符
2. 整个串的长度必须是奇数的


假设下面的串：
1 | 0 & 0 ^ 1 | 0 & 1 期待1

一、递归尝试
f(L,R,期待值d)，f函数表示在LR范围上，期待值d加括号的结果数。外层调用f(0,len,result); 下面讨论流程
 1.枚举每个逻辑符号，假设以i位置的逻辑符号作为最后结合的逻辑符号。假设此逻辑符号是&，期待是true，则需要两边的表达式都是true才行
 2.假设i位置的运算符是|。左边a种true可能性，有b种false的可能性；右边有c种true的可能性，有d种false的可能性。则以|结尾期待为true的可能性有a*c+a*d+b*c种(true|true+true|false+false|true)
 3.同理假设i位置的运算符是^。期待为true的可能性是a*d+b*c（true^false+false&true）
 4. 同理假设i位置的运算符是&。期待为true的可能性是a*c(true&true)

 5. 求出[L..R]以每个逻辑符号为结尾时的结果数，所有的方案累加就是总结果数。

通过查看f的某一种划分, 可以看出L和R只会压中数字的位置   
f(0,10) -> f(0,4) +f(6,10) 

*/
class Solution
{
public:
    bool is_valid(string s) {
        int len = s.size();
        if (len % 2 == 0) { //必须为奇数
            return false;
        }

        for (int i=0;i<len;i+=2) {
            if (s[i] != '1' && s[i] != '0') {
                return false;
            } 
        }

        for (int i=1;i<len;i+=2) {
            if (s[i] != '&' && s[i] != '|' && s[i] != '^') {
                return false;
            } 
        }
        return true;
    }
    //暴力枚举方法
    int countEval1(string s, int result) {
        int len = s.length();
        if (len==0) {
            return 0;
        }
        if (!is_valid(s)) {
            return 0;
        }

        return f(s, result, 0, len-1);
    }
    //l到r范围上，期待值为desired, 返回组合种数
    int f(string s, int desired, int l, int r) {
        if (l == r) {
            if (s[l] == '1') {
                return desired==1 ? 1 : 0;
            } else {
                return desired==1 ? 0 : 1;
            }
        }

        int res = 0;
        if (desired == 1) { //期待true
            for (int i=l+1; i<r;i+=2) {
                switch(s[i]) {
                    case '&':
                        res += f(s, 1, l, i-1) * f(s, 1, i+1, r);
                        break;
                    case '|':
                        res += f(s, 1, l, i-1) * f(s, 1, i+1, r);
                        res += f(s, 1, l, i-1) * f(s, 0, i+1, r);
                        res += f(s, 0, l, i-1) * f(s, 1, i+1, r);
                        break;
                    case '^':
                        res += f(s, 1, l, i-1) * f(s, 0, i+1, r);
                        res += f(s, 0, l, i-1) * f(s, 1, i+1, r);
                        break;
                }
            }
        } else { //期待false
            for (int i=l+1; i<r;i+=2) {
                switch(s[i]) {
                    case '&':
                        res += f(s, 0, l, i-1) * f(s, 0, i+1, r);
                        res += f(s, 0, l, i-1) * f(s, 1, i+1, r);
                        res += f(s, 1, l, i-1) * f(s, 0, i+1, r);
                        break;
                    case '|':
                        res += f(s, 0, l, i-1) * f(s, 0, i+1, r);
                        break;
                    case '^':
                        res += f(s, 1, l, i-1) * f(s, 1, i+1, r);
                        res += f(s, 0, l, i-1) * f(s, 0, i+1, r);
                        break;
                }
            }
        }
        return res;
    }

    //动态规划方法
    /*
    注意到f函数有三个可变参数，但desired只有0和1两种，所以可以用两个二维表
    dp_1[i][j]为期望为1时，i到j范围上有多少种组合数
    dp_0[i][j]为期望为0时，i到j范围上有多少种组合数

    对于dp_1[i][j]依赖
        dp_1[i][i+1...j-1], dp_1[i+1...j-1][j]
        dp_0[i][l+1...j-1], dp_0[i+1...j-1][j]
        1：左边所有的，下面所有的
        0：左边所有的，下面所有的
    对于dp_0[i][j]依赖:
        dp_0[i][i+1...j-1] dp_0[i+1..j-1][j]
        dp_1[i][j-1]  dp_1[i+1...j-1][j]
        1：左边所有的，下面所有的
        0：左边所有的，下面所有的
    
    填表顺序是从下到上，从左到右
    
    base case：当l==r时:
        dp_1[l][r] = s[l] == 1 ? 1 : 0
        dp_0[l][r] = s[l] == 1 ? 0 : 1
    */
    int countEval(string s, int result) {
        int len = s.length();
        if (len==0) {
            return 0;
        }
        if (!is_valid(s)) {
            return 0;
        }

        vector<vector<int>> dp_1(len, vector<int>(len, 0));
        vector<vector<int>> dp_0(len, vector<int>(len, 0));

        for (int i=0;i<len;i++) {
            dp_1[i][i] = s[i] == '1' ? 1 : 0;
            dp_0[i][i] = s[i] == '1' ? 0 : 1;
        }

        for (int l=len-3;l>=0;l-=2) {
            for (int r=l+2;r<len;r+=2) {
                for (int k=l+1;k<=r-1;k+=2) {
                    if (s[k] == '&') {
                        dp_1[l][r] += dp_1[l][k-1] * dp_1[k+1][r];
                        dp_0[l][r] += dp_0[l][k-1] * dp_0[k+1][r];
                        dp_0[l][r] += dp_0[l][k - 1] * dp_1[k + 1][r];
                        dp_0[l][r] += dp_1[l][k - 1] * dp_0[k + 1][r];
                    } else if (s[k] == '|') {
                        dp_1[l][r] += dp_1[l][k-1] * dp_1[k+1][r];
                        dp_1[l][r] += dp_1[l][k-1] * dp_0[k+1][r];
                        dp_1[l][r] += dp_0[l][k-1] * dp_1[k+1][r];

                        dp_0[l][r] += dp_0[l][k-1] * dp_0[k+1][r];
                    } else if (s[k] == '^') {
                        dp_1[l][r] += dp_0[l][k - 1] * dp_1[k + 1][r];
                        dp_1[l][r] += dp_1[l][k - 1] * dp_0[k + 1][r];

                        dp_0[l][r] += dp_1[l][k - 1] * dp_1[k + 1][r];
                        dp_0[l][r] += dp_0[l][k - 1] * dp_0[k + 1][r];
                    }
                }
            }
        }
        if (result == 1) {
            return dp_1[0][len-1];
        } else {
            return dp_0[0][len-1];
        }
    }
};

int main()
{

    int testTime = 100;
    int maxSize = 20;
    int maxValue = 50;
    bool succeed = true;
    // Solution s;
    // cout << "测试开始" << endl;
    // for (int i = 0; i < testTime; i++) {
    //     vector<int> arr;
    //     generate_random_array(maxSize, maxValue, arr);
    //     int ans1 = s.mox_xor_zero1(arr);
    //     int ans2 = s.mox_xor_zero2(arr);

    //     if (ans1 != ans2)
    //     {
    //         succeed = false;
    //         cout << "max_xor1: " << ans1 << " xor2:" << ans2 <<endl;
    //         break;
    //     }
    // }
    // cout << "测试结束" << endl;

   
    Solution s;
    // string str = "1^0|0|1"; //2种
    // int result = 0;

    string str = "0&0&0&1^1|0"; //10种
    int result = 1;

    cout << s.countEval1(str, result) << endl;
    cout << s.countEval(str, result) << endl;

    // cout << s.mox_xor_zero2(nums) << endl;
    // cout << s.max_xor2(nums) << endl;
    // cout << s.max_xor3(nums) << endl;

    return 0;
}