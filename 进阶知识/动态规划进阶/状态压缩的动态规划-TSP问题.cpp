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

 * TSP问题
 * 有N个城市，任何两个城市之间的都有距离，任何一座城市到自己的距离都为0
 * 所有点到点的距离都存在一个N*N的二维数组matrix里，也就是整张图由邻接矩阵表示
 * 现要求一旅行商从k城市出发必须经过每一个城市且只在一个城市逗留一次，最后回到出发的k城
 * 参数给定一个matrix，给定k。返回总距离最短的路的距离。

TSP经常被作为超级计算机的测试题目


测试链接 : https://www.luogu.com.cn/problem/P1171

*/

/*
暴力尝试：
定义一个函数process(set<int> sets, int start)。sets数组表示还有多少个城市没有走，当前从start开始走。
因为需要从k城市出发，再回到k城市，走的是一个环，从任何城市出发求出来的最短距离都是一样的。假设从0城市出发，主函数调用从0城市开始走。
process意义是还有sets中所有的城市没有经过，从start开始，最后到达0位置的最短距离。
整理流程：假设有0,1,2,3,4五座城市, 主函数调用process({0,1,2,3,4}, 0)
1. 从0城市出发。假设下一个城市去到1，调用process({1,2,3,4}, 1);假设下一个城市去2，调用process({1,2,3,4},2);;假设下一个城市去3，调用process({1,2,3,4},3);假设下一个城市去4，调用process({1,2,3,4},4)
    所有往下枚举的路径加上o到1的路径和，取最小值。
2. base：假设最后还是还剩一个城市，则直接返回该城市到0的距离。即process({4},0)的值为matrix[4][0]


状态压缩的动态规划：
因为sets中每个城市要么存在或者不存在，所以其状态可以用0和1表示。使用一个32位整型，0~n位表示每个城市的状态。
还没有经过的城市，对应的位置是1。否则是0

基础
判断是否只有一个城市了：取num的最后一个1，然后判断和num是否相等，相等则就剩一个城市了。 num == (num & (~num+1))
将num第5位的数变成0：就是1往左移动5位，然后取反，和num相与就可以求得到了。num & (~(num<<5))
将num第5位的数变成1：就是1往左移动5位，和num相或就可以求得到了。num | (~(num<<5))

*/

class Solution
{
public:
    int dsp1(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        }
        int m = matrix[0].size();
        if (m==0) {
            return 0;
        }

        vector<int> sets(n, 1); //一开始，n座城市都在
        return process(matrix, sets, 0);

    }
    int process(vector<vector<int>>& matrix, vector<int>& sets, int start) {
        int city_num = 0;
        int single_city = -1;
        for (int i=0;i<sets.size();i++) {
            if (sets[i] == 1) {
                city_num++;
                single_city = i;
            }
        }
        if (city_num == 1) {
            return matrix[single_city][0];
        }


        sets[start] = 0; //表示已经去过该城市
        int ans = INT_MAX;
        for (int i=0;i<sets.size();i++) {
            if (sets[i] == 1) {
                ans = min(ans, matrix[start][i]+process(matrix, sets, i));
            }
        }
        sets[start] = 1; //恢复现场
        return ans;
    }

    //状态压缩后的递归
    int dsp2(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        }
        int m = matrix[0].size();
        if (m==0) {
            return 0;
        }
        unsigned int sets = (1 << n) - 1; //初始时所有城市都在
        return process2(matrix, sets, 0);

    }
    void print_sets(string suf, unsigned int sets) {
        cout << suf<<endl;
        stack<int> st;
        while (sets != 0) {
            st.push((sets & 1));
            sets = (sets >> 1);
        }
        while (!st.empty()) {
            cout << st.top();
            st.pop();
        }
        cout << endl;
    }
    int process2(vector<vector<int>>& matrix, unsigned int sets, int start) {
        unsigned int tmp = 1;
        // cout << "start: " << start << endl;
        // print_sets("sets: ", sets);
        //只剩一个城市了
        if ((sets & (~sets + 1)) == sets) {
            return matrix[start][0];
        }

        // print_sets("before move start sets: ", sets);
        
        // print_sets("move start 1: ", ~(tmp << start));
        sets &= ~(tmp << start); //去掉start城市，表示已经去过了
        // print_sets("after move start sets: ", sets);

        int ans = INT_MAX;
        int n = matrix.size();
        //挨个枚举每个城市
        for (int move=1;move<n;move++) {//0城市肯定不在了。
            if((sets&(tmp<<move)) != 0) { //如果当前城市还在集合中
                int ans_tmp = matrix[start][move]+process2(matrix, sets, move);
                // cout << "start:" << start << " move:" << move << " ans_tmp:" << ans_tmp << endl;
                ans = min(ans, ans_tmp);
            }
        }
        sets |= (tmp<<start); //恢复现场
        return ans;
    }

    //状态压缩后的记忆化搜索
    int dsp3(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        }
        int m = matrix[0].size();
        if (m==0) {
            return 0;
        }
        unsigned int sets = (1 << n) - 1; //初始时所有城市都在
        //sets的取值会是0~(1<<n)-1，所以长度是1<<n
        vector<vector<int>> dp(1<<n, vector<int>(n+1, -1));
        return process3(matrix, sets, 0, dp);

    }
    int process3(vector<vector<int>>& matrix, unsigned int sets, int start, vector<vector<int>>& dp) {
        unsigned int tmp = 1;
        // cout << "start: " << start << endl;
        // print_sets("sets: ", sets);
        if (dp[sets][start] != -1) {
            return dp[sets][start];
        }

        //只剩一个城市了
        if ((sets & (~sets + 1)) == sets) {
            dp[sets][start] = matrix[start][0];
            return dp[sets][start];
        }

        // print_sets("before move start sets: ", sets);
        
        // print_sets("move start 1: ", ~(tmp << start));
        sets &= ~(tmp << start); //去掉start城市，表示已经去过了
        // print_sets("after move start sets: ", sets);

        int ans = INT_MAX;
        int n = matrix.size();
        //挨个枚举每个城市
        for (int move=1;move<n;move++) {//0城市肯定不在了。
            if((sets&(tmp<<move)) != 0) { //如果当前城市还在集合中
                int ans_tmp = matrix[start][move]+process3(matrix, sets, move,dp);
                // cout << "start:" << start << " move:" << move << " ans_tmp:" << ans_tmp << endl;
                ans = min(ans, ans_tmp);
            }
        }
        sets |= (tmp<<start); //恢复现场
        dp[sets][start] = ans;
        return dp[sets][start];
    }


    //dp循环
    int dsp4(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        }
        int m = matrix[0].size();
        if (m==0) {
            return 0;
        }
        unsigned int tmp_1 = 1;
        unsigned int max_status = (tmp_1 << n);
        //sets的取值会是0~(1<<n)-1，所以长度是1<<n
        vector<vector<int>> dp(max_status, vector<int>(n+1, -1));

        /*
        状态依赖关系：
        sets取值是0~(1<<n)-1
        当sets=0时，没有城市可以走。
        当sets=1时，只有0号城市可以走, 所以dp[1][0]=0，其他城市无意义
        当sets=2时，只有1号城市可以走, 所以dp[2][1]=matrix[1][0]，其他城市无意义
        当sets=3时，有0号和1号城市可以走, 当下一步从0号城市开始走，sets变成2(00010)，还剩1号城市可以走，依赖dp[2][1]+matrix[start][0];
            当下一步从1号城市开始走，sets变成1(00001)， 还剩0号城市可以走，依赖dp[1][0]+matrix[start][1]
            最后两个依赖答案取最小值。
        
        可以看到，从上到下，当求下面的行时，上面所依赖的dp都已经求出来了。所以从上到下求解。
        sets取值为2^n, 列为n个城市，每次求解一个格子时，会枚举下一步所有的城市，最后去最小值，也是n的。
        所以总复杂度是O(2^n*n*n)=O(2^n*n^2)
        */

       
       for (unsigned int sets=0;sets<max_status;sets++) {
            for (int start=0;start<n;start++) {
                //只剩一个城市了
                if ((sets & (~sets + 1)) == sets) {
                    dp[sets][start] = matrix[start][0];
                } else {
                    unsigned int sets_tmp = sets;
                    sets_tmp &= ~(tmp_1 << start);
                    int ans = INT_MAX;
                    //挨个枚举每个城市
                    for (int move=0;move<n;move++) {//0城市肯定不在了。
                        if((sets_tmp&(tmp_1<<move)) != 0) { //如果当前城市还在集合中
                            int ans_tmp = matrix[start][move]+dp[sets_tmp][move];
                            // cout << "start:" << start << " move:" << move << " ans_tmp:" << ans_tmp << endl;
                            ans = min(ans, ans_tmp);
                        }
                    }
                    dp[sets][start] = ans;
                }
            }
       }
       return dp[max_status-1][0];
    }
};


vector<vector<int>> generate_matrix(int n) {
    int max_num = 20;
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i=0;i<n;i++) {
        for (int j=i;j<n;j++) {
            if (i==j) {
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = (std::rand() % max_num) +1;
                matrix[j][i] = matrix[i][j];
            }
        }
    }

    // for (int i=0;i<n;i++) {
    //     for (int j=0;j<n;j++) {
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    return matrix;
}

int main() {
    Solution sol;

//     //答案是3
//     // vector<vector<int>> matrix = {
//     //     {0,1,2},
//     //     {1,0,2},
//     //     {2,1,0},
//     // };
//     /*
//     0,0 0
//     0,1 1
//     0,2 2
//     1,0 1
//     1,1 0
//     1,2 2
//     2,0 2
//     2,1 1
//     2,2 0
//     */

//     //路径是0,1,4,2,3,0  答案：23
//    vector<vector<int>> matrix = {
//         {0,3,100,8,9},
//         {3,0,3,10,5},
//         {100,30,0,4,3},
//         {8,10,4,0,20},
//         {9,5,3,20,0},
//     };
//     cout << sol.dsp1(matrix) << endl;
//     cout << sol.dsp2(matrix) << endl;
//     cout << sol.dsp3(matrix) << endl;
//     cout << sol.dsp4(matrix) << endl;
    int test_time=100;
    int max_n = 10;
    for (int i=0;i<test_time;i++) {
        int n = (std::rand()%max_n) +1;
        vector<vector<int>> matrix = generate_matrix(n);
        int ans1 = sol.dsp1(matrix);
        int ans2 = sol.dsp2(matrix);
        int ans3 = sol.dsp3(matrix);
        int ans4 = sol.dsp4(matrix);
        if (ans1!=ans2) {
            cout << "ans2 error: ans1:" << ans1 << " ans2:" << ans2 << " ans3:" << ans3 << " ans4:" << ans4 << endl;
            break;
        }
        if (ans1!=ans3) {
            cout << "ans3 error: ans1:" << ans1 << " ans2:" << ans2 << " ans3:" << ans3 << " ans4:" << ans4 << endl;
            break;
        }
        if (ans1!=ans4) {
            cout << "ans3 error: ans1:" << ans1 << " ans2:" << ans2 << " ans3:" << ans3 << " ans4:" << ans4 << endl;
            break;
        }
    }
   
}