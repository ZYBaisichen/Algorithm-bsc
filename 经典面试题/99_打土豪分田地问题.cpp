/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-22 21:07:57
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
#include <sstream>

using namespace std;

/*
测试链接：https://www.nowcoder.com/practice/fe30a13b5fb84b339cb6cb3f70dca699?tpId=122&tqId=33655&ru=/exam/oj
//在牛客上卡了常数时间，需要优化下，将vector改成数组应该就可以过了


参考：https://www.cnblogs.com/ranranblog/p/5777306.html
题目描述
牛牛和 15 个朋友来玩打土豪分田地的游戏，牛牛决定让你来分田地，地主的田地可以看成是一个矩形，每个位置有一个价值。
分割田地的方法是横竖各切三刀，分成 16 份，作为领导干部，牛牛总是会选择其中总价值最小的一份田地， 
作为牛牛最好的朋友，你希望牛牛取得的田地的价值和尽可能大，你知道这个值最大可以是多少吗？

每个输入包含 1 个测试用例。每个测试用例的第一行包含两个整数 n 和 m（1 <= n, m <= 75），
表示田地的大小，接下来的 n 行，每行包含 m 个 0-9 之间的数字，表示每块位置的价值。
输出一行表示牛牛所能取得的最大的价值。

示例：
4 4
3332
3233
3332
2323

输出2


*/

/*

一、预处理数组
因为要求每个分块的价值，所以需要有个结构，能帮助我们O(1)复杂度求出来该区域的面积。
假设某个块，从左上角开始顺时针四个角的点依次是(a,b)(a,c)(d,c)(d,b)，定义一个help，help[i][j]表示以(0,0)为左上角,(i,j)为右下角的价值和。
则该块面积是help[d][c]-(help[a][c]+help[d][b]-help[a][b])
求解help的过程是：
    1. 第一行和第一列，挨个累加，就是前缀数组。
    2. help[i][j]=arr[i][j]+help[i-1][j]+help[i][j-1]-help[i-1][j-1]

二、算法原型
当竖着三刀固定的情况下，横着只切一刀时，怎么切能让分成的8块中最小值最大。
    1. [0..1]范围上，竖着三刀固定时，只能在0和1之间切(0作为第一部分的最后一个元素)
    2. [0...2]范围上，竖着3刀固定时，可能在0和1之间，也可能在1和2之间。假设最优一刀在k位置
    3. 当在增加一个范围时，即[0..3]上时，这一刀尝试的位置不会比k更靠上。（这里用到了邮局问题中的原型max{min{sum_上，sum_下}}，存在单调性时，不用回退尝试）
求出来up和down两个数组, up[i]表示0...i范围上固定3刀时的最优一刀下的最小值。down[i]表示i...n范围上固定3刀时的最优一刀下的最小值

三、整体大流程
    1. 首先求出help数组
    2. 暴力枚举竖着3刀，O(N^3)复杂度
    3. 每次固定竖着3刀的情况下，枚举横切一刀的位置，结合help数组，求出up和down数组。O(n)
    4. 对于横着的3刀，枚举中间一刀的位置mid；上方只需要在0..mid范围横切一刀拿到答案up[mid]；下方横切一刀拿到答案down[mid+1]。
        枚举此中间一刀的最终答案是tmp_ans = min{up[mid],down[mid]}, 最终求max(tmp_ans....)。 对于每次枚举中间一刀，可以O(1)求出答案，枚举过程是O(n)
    5. 总体复杂度是O(N^4)
*/


class Solution {
public:
    //求累加和数组，用于快速得到任意范围内矩阵的和
    void get_help(vector<vector<int>>& matrix, vector<vector<int>>& help) {
        int n = matrix.size();
        int m = matrix[0].size(); 
        help[0][0] = matrix[0][0];
        //第一行，累加和就是前缀和数组
        for (int j=1;j<m;j++) {
            help[0][j] = matrix[0][j] + help[0][j-1];
        }
        //第一列
        for(int i=1;i<n;i++) {
            help[i][0] = matrix[i][0] + help[i-1][0];
        }

        for (int i=1;i<n;i++) {
            for (int j=1;j<m;j++) {
                // cout << "i:" << i << " j:" << j << " help[i-1][j]:" << help[i - 1][j] << " help[i][j - 1]:" << help[i][j - 1]
                //     << " help[i - 1][j - 1]:" << help[i - 1][j - 1] << endl;
                help[i][j] = help[i-1][j] + help[i][j-1] - help[i-1][j-1] + matrix[i][j];
            }
        }
        // for (int i=0;i<n;i++) {
        //     for (int j=0;j<m;j++) {
        //         cout << help[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
    }
    int get_min_sum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        }
        int m = matrix[0].size();
        if (m==0) {
            return 0;
        }
        /*
            假设某个块，从左上角开始顺时针四个角的点依次是(a,b)(a,c)(d,c)(d,b)
            则该块面积是help[d][c]-(help[a][c]+help[d][b]-help[a][b])
        */
        vector<vector<int>> help(n, vector<int>(m, 0));
        get_help(matrix, help);
        
        
        int res = INT_MIN;
        //枚举竖着的3刀。每刀所在的位置是每块的最后一个元素
        for (int c1=0;c1<=m-4;c1++) {
            for (int c2=c1+1;c2<=m-3;c2++) {
                for (int c3=c2+1;c3<=m-2;c3++) {
                    int tmp = get_best_dicision(help, c1, c2, c3);
                    // cout << "c1:" << c1 << " c2:" << c2 << " c3:" << c3 << " res_tmp:" << tmp << endl;
                    res = max(res, tmp);
                }
            }
        }
        return res;
    }

    //根据固定3刀的位置，求出最优答案
    int get_best_dicision(vector<vector<int>>& help, int c1,int c2, int c3) {
        int n = help.size();
        int m = help[0].size();
        // 根据3刀的位置，求出up和down数组
        vector<int> up(n, 0); // up[i]:0...i范围切一刀八个块的最小值
        vector<int> down(n, 0); // down[i]:i...n-1范围切一刀八个块的最小值

        //当只有第0行时，不能切，所以up[0]没有意义
        up[1] = min(value(help,c1,c2,c3,0,0), value(help,c1,c2,c3,1,1));
        int split = 0; //0..1范围上切的一刀，上半部分的最后一行在0位置
        for (int i=2;i<n;i++) { //求每个up[i]
            //从split开始枚举每个位置，最多到i-1
            int ans_tmp = INT_MIN;
            for (int j=split;j<=i-1;j++) {//增加一个范围时，肯定比上一个范围时的一刀更靠后
                int tmp = min(value(help, c1, c2, c3, 0, j), value(help, c1, c2, c3, j+1, i));
                if (tmp >= ans_tmp) { //发现更优的解，上次切分的位置往后挪动
                    ans_tmp = tmp;
                    split = j;
                }
            }
            up[i] = ans_tmp;
        }
        // cout << "c1:" << c1 << " c2:" << c2 << " c3:" << c3 << " up:";
        // for (int i=0;i<n;i++) {
        //     cout << up[i] << " ";
        // }
        // cout << endl;

        //当只有第n-1行时，不能切，所以down[n-1]没有意义
        down[n-2] = min(value(help,c1,c2,c3,n-2,n-2), value(help,c1,c2,c3,n-1,n-1)); //[n-2..n-1]
        split = n-1; //[n-2..n-1]范围上切的一刀，下半部分的第一行在n-1位置
        for (int i=n-3;i>=0;i--) { //求每个down[i]。 单个邮局问题
            //从split开始枚举每个位置，最小到i+1
            int ans_tmp = INT_MIN;
            for (int j=split;j>=i+1;j--) {
                int tmp = min(value(help, c1, c2, c3, i, j-1), value(help, c1, c2, c3, j, n-1));
                if (tmp >= ans_tmp) { //最优一刀网上挪动
                    split = j;
                    ans_tmp = tmp;
                }
            }
            down[i] = ans_tmp;
        }
        
        // cout << "c1:" << c1 << " c2:" << c2 << " c3:" << c3 <<  " down:";
        // for (int i=0;i<n;i++) {
        //     cout << down[i] << " ";
        // }
        // cout << endl;

            // 枚举横着中间一刀的位置，注意需要给上一刀留出位置，所以切分后，中间部分的位置至少是1。最多到n-3
        int ans = INT_MIN;
        for (int mid=1;mid<=n-3;mid++) { //中间部分的最后一行最多到n-3，这样[n-2..n-1]才能再来一刀
            //上半部分0...mid，砍一刀的的最优解up[mid]。此时up[mid]是上半部分最优划分下的最小值
            //下半部分mid+1...n-1，砍一刀的的最优解down[mid+1]。此时down[mid+1]是下半部分最优划分下的最小值
            //两者取小的，得到当前mid的答案
            int tmp = min(up[mid], down[mid+1]);            
            // cout << "mid:" << mid << " up[mid]:" << up[mid] << " down[mid+1]:" << down[mid+1] << " tmp:" << tmp << endl;
            ans = max(ans, tmp); //让最小值尽可能的大
        }
        return ans;
    }

    //给3刀位置，和上下边界(包含边界）, 分出来的四块的最小值
    int value(vector<vector<int>>& help, int c1,int c2, int c3, int up, int down) {
        int m = help[0].size();
        int value1 = area(help, up, 0, down, c1);
        int value2 = area(help, up, c1+1, down, c2);
        int value3 = area(help, up, c2+1, down, c3);
        int value4 = area(help, up, c3+1, down, m-1);
        // cout << "c1:" << c1 << " c2:" << c2 << " c3:" << c3 << " up:" << up << " down:" << down
        //     << " value1:" << value1 << " value2:" << value2 << " value3:" << value3 << " value4:" << value4 << endl;
        return min(min(value1,value2), min(value3,value4));
    }

    //给定左上角值和右下角值，求出其构成的矩阵的累加和
    int area(vector<vector<int>> &help, int i1, int j1, int i2, int j2) {
        /*
            左上角累加和：help[i1-1][j1-1]
            上方累加和: help[i1-1][j2]
            左方累加和：help[i2][j1-1]
            从(0,0)到右下角累加和：help[i2][j2]
            所以所求矩形累加和是：help[i2][j2]-(help[i1-1][j2]+help[i2][j1-1]-help[i1-1][j1-1])
            但需要考虑边界条件
        */
    //    cout << "in area:" << " i1:" << i1 << " j1:" << j1 << " i2:" << i2 << " j2:" << j2 << endl;
       int left=0, up=0, left_up=0;
       if (i1-1>=0) {
            up = help[i1-1][j2];
       }
       if (j1-1>=0) {
            left = help[i2][j1-1];
       }
       if (j1-1>=0&&i1-1>=0) {
            left_up = help[i1-1][j1-1];
       }
       int ans = help[i2][j2]-(up+left-left_up);
       return ans;
    }
};

// int main() {
//     Solution sol;
//     vector<vector<int>> m = {
//         {3,3,3,2},
//         {3,2,3,3},
//         {3,3,3,2},
//         {2,3,2,3},
//     };
//     //{1,1,3,2,3,0,0,0,0,0}
//     cout << sol.get_min_sum(m) << endl;
//     return 0;
// }


/*
牛客
描述
牛牛和 15 个朋友来玩打土豪分田地的游戏，牛牛决定让你来分田地，地主的田地可以看成是一个矩形，每个位置有一个价值。分割田地的方法是横竖各切三刀，分成 16 份，作为领导干部，牛牛总是会选择其中总价值最小的一份田地， 作为牛牛最好的朋友，你希望牛牛取得的田地的价值和尽可能大，你知道这个值最大可以是多少吗？
输入描述：
每个输入包含 1 个测试用例。
每个测试用例的第一行包含两个整数 n 和 m（1 <= n, m <= 75），表示田地的大小，接下来的 n 行，每行包含 m 个 0-9 之间的数字，表示每块位置的价值。
输出描述：
输出一行表示牛牛所能取得的最大的价值。
示例1
输入：
4 4
3332
3233
3332
2323
复制
输出：
2
*/
int main() {
    int n, m;
    Solution sol;
    while (cin >> n >> m) { // 注意 while 处理多个 case
        vector<vector<int>> matrix(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            string s;
            cin >> s;
            for (int j=0;j<m;j++) {
                matrix[i][j] = s[j]-'0';
            }
        }
        // for (int i=0;i<n;i++) {
        //     for (int j=0;j<m;j++) {
        //         cout << matrix[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        cout << sol.get_min_sum(matrix) << endl;
    }
}
// 64 位输出请用 printf("%lld")