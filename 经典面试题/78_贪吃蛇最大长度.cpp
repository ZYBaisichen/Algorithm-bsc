/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-05 19:32:52
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
给定一个二维数组matrix，每个单元都是一个整数，有正有负。
最开始一条长度为0的蛇，从矩阵最左侧任选一个单元格进入地图，蛇每次只能往右上、右、右下走。
蛇到达一个单元格后，自身长度瞬间加上该单元格的数值，任何时候长度为负则游戏结束。
小Q有个能力，可以在游戏开始的时候将地图的某个节点的值变成其相反数(只能改变一个节点)。问在游戏过程中，他的蛇最长长度可以到多少

例如：
1 -4 10
3 -2 -1
2 -1 0
0 5 -2
最优路径为从左侧的3开始, 3->-4(利用能力变成4)->10。所以返回17
*/
/*
业务限制模型

暴力尝试：返回到达(i,j)的旅程中。
0) 完全没有使用过能力的情况下，返回路径和，没有可能到达的话，返回负数
2) 在使用过能力的情况下，返回路径最大和，没有可能到达的话，返回负数
vector<int> f(int x,  int i,int j)
process中的所有可能性都是业务带来的，所以叫业务限制模型
主函数枚举到每个位置的长度，最大值就是答案

*/

class Solution
{
public:
    int get_snake_max_len(vector<vector<int>>& m) {
        int ans = -1;
        for (int i=0;i<m.size();i++) {
            for (int j=0;j<m[0].size();j++) {
                // cout << "=============" << endl;
                vector<int> tmp = process(m, i,j);
                ans = max(ans, max(tmp[0], tmp[1]));
            }
        }
        return ans;
    }

    vector<int> process(vector<vector<int>>& m, int i, int j) {
        if (j==0) { 
            //使用和不使用能力
            vector<int> res = {m[i][j], -m[i][j]};
            // cout << "i:" << i << ",j:" << j << " res:" << res[0] << "," << res[1] << endl;
            return res;
        }

        //j>0
        //左侧的答案
        vector<int> left = process(m, i,j-1);
        int pre_not_used = left[0]; //之前不使用能力的最大长度 
        int pre_used = left[1]; //之前使用能力的最大长度
        
        //左上过来的答案
        if (i-1>=0) {
            vector<int> left_up = process(m, i-1,j-1);
            pre_not_used = max(pre_not_used, left_up[0]);
            pre_used = max(pre_used, left_up[1]);
        }
        //左下过来的答案
        if (i+1<m.size()) {
            vector<int> left_down = process(m, i+1,j-1);
            pre_not_used = max(pre_not_used, left_down[0]);
            pre_used = max(pre_used, left_down[1]);
        }
        vector<int> res(2,0);
        res[0] = pre_not_used + m[i][j]; //完全不使用能力

        res[1] = max(pre_used + m[i][j], pre_not_used - m[i][j]); //使用能力分为两种，1. 前面的已经使用了。2. 前面没有使用，在这一次使用了
        // cout << "i:" << i << ",j:" << j << " res:"<< res[0] << ","<<res[1]<<endl;
        return res;
    }


    int get_snake_max_len_dp(vector<vector<int>>& m) {
        int ans = -1;
        int nn = m.size();
        int mm = m[0].size();
        //两张表，一张不使用能力，一张使用能力
        vector<vector<int>> not_used(nn, vector<int>(mm, -1));
        vector<vector<int>> used(nn, vector<int>(mm, -1));

        //第一列
        for (int i=0;i<m.size();i++) {
            not_used[i][0] = m[i][0];
            used[i][0] = -m[i][0];
        }

        //每个位置依赖，前一列上中下三个位置。从左到右，从上到下填
        for (int j=1;j<m[0].size();j++) {
            for (int i=0;i<m.size();i++) {
                //左侧
                int pre_not_used = not_used[i][j-1];
                int pre_used = used[i][j - 1];
                //如果有左上
                if(i-1>=0) {
                    pre_not_used = max(pre_not_used, not_used[i - 1][j-1]);
                    pre_used = max(pre_used, used[i - 1][j-1]);
                }
                //如果有左下
                if (i+1<m.size()) {
                    // cout << " i:" << i << " j:" << j << " not_used[i + 1][j-1]:" << not_used[i + 1][j-1]<< endl;
                    pre_not_used = max(pre_not_used, not_used[i + 1][j-1]);
                    pre_used = max(pre_used, used[i + 1][j-1]);
                }
                

                not_used[i][j] = pre_not_used+m[i][j];
                used[i][j] = max(pre_not_used-m[i][j], pre_used+m[i][j]);
                ans = max(ans, max(not_used[i][j], used[i][j]));
                // cout << "i:" << i << " j:" << j << " pre_not_used:" << pre_not_used << " pre_used:" << pre_used << " m[i][j]:" << m[i][j] 
                //     << " not_used[i][j]:" << not_used[i][j] << " used[i][j]:" << used[i][j]<< endl;
            }
        }

        // cout << "not_used=====" << endl;
        // for (int i=0;i<m.size();i++) {
        //     for (int j=0;j<m[0].size();j++) {
        //         cout << not_used[i][j]<<" ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        // cout << "used=====" << endl;
        // for (int i=0;i<m.size();i++) {
        //     for (int j=0;j<m[0].size();j++) {
        //         cout << used[i][j]<<" ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return ans;
    }
};

int main()
{

    Solution sol;
    vector<vector<int>> m = {{1,-4,1000},
                            {3,-2,-1},
                            {2,-1,100},
                            {0,5,-2}};
    cout << sol.get_snake_max_len(m) << endl;
    cout << sol.get_snake_max_len_dp(m) << endl;
    return 0;
}