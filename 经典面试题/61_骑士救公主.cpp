/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 10:41:19
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
 * Description: 龙与地下城游戏
 * 1）骑士从左上角出发，每次只能向右或向下走，最后到达右下角见到公主。
 * 2）地图中每个位置的值代表骑士要遭遇的事情。如果是负数，说明此处有怪兽，要让骑士损失血量。如果是非负数，代表此处有血瓶，能让骑士回血。
 * 3）骑士从左上角到右下角的过程中，走到任何一个位置时，血量都不能少于1。为了保证骑土能见到公主，初始血量至少是多少?
 * 根据map,输出初始血量。
 * 
 * 
 -2 -3  3
 -5 -10 1
 0  30 -5
*/

class Solution
{
public:
    /*
    */
   int min_hp(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 1;
        } 
        int m = matrix[0].size();
        if (m == 0) {
            return 1;
        }
        return process(matrix,n,m,0,0);
   } 
   /*
   准备登上i,j
   返回到右下角需要的最少血量
   */
   int process(vector<vector<int>>& matrix, int n, int m, int i,int j) {
        if (i==n-1 && j==m-1) { //准备登录右下角。如果需要掉血，则需要当前-matrix[i][j]+1的血量
            return matrix[i][j] < 0 ? -matrix[i][j]+1 : 1;
        }

        if (i==n-1) { //来到了最后一行，只能往右走
            int right_need = process(matrix, n, m, i, j+1);
            if (matrix[i][j] < 0) {
                return (-matrix[i][j])+right_need;
            } else if (matrix[i][j]>=right_need) { //右侧需要3，当前要登录的已经满足3了，只需要1活着就行
                return 1;
            } else {
                return right_need - matrix[i][j];
            }
        }

        if (j==m-1) { //来到了最后一列，只能往下走
            int down_need = process(matrix, n, m, i+1, j);
            if (matrix[i][j] < 0) {
                return (-matrix[i][j])+down_need;
            } else if (matrix[i][j]>=down_need) { //右侧需要3，当前要登录的已经满足3了，只需要1活着就行
                return 1;
            } else {
                return down_need - matrix[i][j];
            }
        }

        int min_need = min(
            process(matrix, n, m, i+1, j), //往下
            process(matrix, n, m, i, j+1) //往右
        );
        if (matrix[i][j] < 0) {
            return (-matrix[i][j]) + min_need;
        } else if (matrix[i][j]>=min_need) { //右侧需要3，当前要登录的已经满足3了，只需要1活着就行
            return 1;
        } else {
            return min_need - matrix[i][j];
        }
   }

   int min_hp_dp(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 1;
        } 
        int m = matrix[0].size();
        if (m == 0) {
            return 1;
        }
        /*
        i从0到n-1,j从0到m-1
        dp[i][j]代表准备登录i,j，并存储从当前位置到达右下角位置，需要的最低血量
        依赖i+1,j+1，所以从下往上，从右往左填格子
        */

       vector<vector<int>> dp(n, vector<int>(m,0));
       dp[n-1][m-1] = matrix[n-1][m-1]<0? -matrix[n-1][m-1]+1 : 1;
        
        //最后一行
       for (int j=m-2;j>=0;j--) {
            // if (matrix[n-1][j]<0) {
            //     dp[n-1][j] = dp[n-1][j+1] - matrix[n-1][j];
            // } else if (matrix[n-1][j]>=dp[n-1][j+1]) {
            //     dp[n - 1][j] = 1;
            // } else {
            //     dp[n-1][j] = dp[n-1][j+1] - matrix[n-1][j];
            // }

            if (matrix[n-1][j]>=dp[n-1][j+1]) {
                dp[n - 1][j] = 1;
            } else { //包括了matrix[n-1][j]<0、dp[n-1][j+1]>matrix[n-1][j]两种情况
                dp[n-1][j] = dp[n-1][j+1] - matrix[n-1][j];
            }
       }
       //最后一列
        for (int i=n-2;i>=0;i--) {
            if (matrix[i][m-1]>=dp[i+1][m-1]) {
                dp[i][m-1] = 1;
            } else { //包括了matrix[i][m-1]<0、dp[i+1][m-1]>matrix[i][m-1]两种情况
                dp[i][m-1] = dp[i+1][m-1] - matrix[i][m-1];
            }
        }

        for (int i=n-2;i>=0;i--) {
            for(int j=m-2;j>=0;j--) {
                
                int min_need = min(dp[i + 1][j], dp[i][j+1]);
                // cout << "i:" << i << "," << j << " val:" << matrix[i][j] << " min_need:" << min_need<< endl;
                if (matrix[i][j]>=min_need) {
                    dp[i][j] = 1;
                } else {
                    dp[i][j] = min_need - matrix[i][j];
                }

            }
        }

        // for (int i=0;i<n;i++) {
        //     for (int j=0;j<m;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        return dp[0][0];
   } 

    //注意到dp[i][j]只会依赖下一行和右边的一列的数字，所以可以进行空间压缩
   int min_hp_dp_yasuo(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 1;
        } 
        int m = matrix[0].size();
        if (m == 0) {
            return 1;
        }

        vector<int> dp(m, 0);
        dp[m-1] = matrix[n-1][m-1]<0? -matrix[n-1][m-1]+1 : 1;
        for (int i=n-1;i>=0;i--) { //滚动填写n次
            if (i==n-1) {
                dp[m-1] = matrix[n-1][m-1]<0? -matrix[n-1][m-1]+1 : 1;
            } else {
                int down_need = dp[m - 1];
                if (matrix[i][m-1]>=down_need) {
                    dp[m-1] = 1;
                } else { //包括了matrix[i][m-1]<0、dp[i+1][m-1]>matrix[i][m-1]两种情况
                    dp[m-1] = dp[m-1] - matrix[i][m-1];
                }
            }
            for (int j=m-2;j>=0;j--) {
                int min_need = min(dp[j], dp[j+1]); //下方和右方
                // cout << "i:" << i << "," << j << " val:" << matrix[i][j] << " min_need:" << min_need<< endl;
                if (matrix[i][j]>=min_need) {
                    dp[j] = 1;
                } else {
                    dp[j] = min_need - matrix[i][j];
                }
            }
        }
        return dp[0];
   }
};

int main()
{

    Solution sol;

    vector<vector<int>> m = {
        {-2, -3, 3},
        {-5, -10, 1},
        {0, 30, -5}};
    cout << sol.min_hp(m) << endl;
    cout << sol.min_hp_dp(m) << endl;
    cout << sol.min_hp_dp_yasuo(m) << endl;

    return 0;
}