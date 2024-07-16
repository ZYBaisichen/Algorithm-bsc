/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 11:14:39
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
 给定一个矩阵，先从左上角开始，每一步只能往右或者往下走，走到右下角。然后从右下角出发，每一步只能往上或者往左走，再回到左上角。任何位置的数字，只能获得一遍。返回最大路径和
 -2 -3  3
 -5 -10 1
 0  30 -5
*/

class Solution
{
public:
    /*
    */
   int max_sum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        } 
        int m = matrix[0].size();
        if (m == 0) {
            return 0;
        }
        return process(matrix,n,m,0,0,0);
   } 
   /*
   matrix没有负数

   转化成，让两个人同时从右上角出发，不能重复走
   A来到的位置是ar,ac
   B来到的位置是br,bc
   两个人迈出步数一样多，共同到达右下角，返回两个人路径的最大累加和
   来到同一个位置时，只获得一份数据

    两个人一起走，a来到一个位置，b在另外一个位置，后续b不可能再来到这个位置。

    如果改动态规划的一个原则是，可变参数的数量越少越好，所以最好能省一个参数。因为a、b走的部署一样，所以假设知道br，bc=(ar+ac)-br
   */
   int process(vector<vector<int>>& matrix, int n, int m, int ar,int ac,int br) {
        if (ar == n-1 && ac == m-1) {
            return matrix[ar][ac];
        }

        //还没到右下角
        /*
        a下 b右
        a下 b下
        a右 b右
        a右 b下
        */
        
        int bc = (ar+ac)-br;
        int a_down_b_right = INT_MIN;
        if (ar+1<n && bc+1<m) {
            a_down_b_right = process(matrix, n,m, ar+1,ac,br);
        }
        int a_down_b_down = INT_MIN;
        if (ar+1<n && br+1<n) {
            a_down_b_down = process(matrix, n,m, ar+1,ac,br+1);
        }
        int a_right_b_right = INT_MIN;
        if (ac+1<m && bc+1<m) {
            a_right_b_right = process(matrix, n, m, ar, ac+1, br);
        }
        int a_right_b_down = INT_MIN;
        if (ac+1<m && br+1<n) {
            a_down_b_down = process(matrix, n,m, ar,ac+1,br+1);
        }
        
        int cur_sum = matrix[ar][ac];
        if (ar != br) {
            cur_sum += matrix[br][bc];
        }
        return cur_sum + max(max(a_down_b_right, a_down_b_down), max(a_right_b_right, a_right_b_down));
   }


   
   int max_sum_dp(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n==0) {
            return 0;
        } 
        int m = matrix[0].size();
        if (m == 0) {
            return 0;
        }
        /*
        三个可变参数，三维动态规划
        */
       vector<vector<vector<int>>> dp(n, vector<vector<int>>(m,vector<int>(n,0)));
       dp[n-1][m-1][n-1] = matrix[n-1][m-1];
       for (int br=n-1;br>=0;br--) {
            for (int ar=n-1;ar>=0;ar--) {
                for (int ac=m-1;ac>=0;ac--) {
                    if (br==n-1&&ac==m-1&&ar==n-1) {
                        continue;
                    }
                    int bc = (ar+ac)-br;
                    int a_down_b_right = INT_MIN;
                    if (ar+1<n && bc+1<m) {
                        a_down_b_right = dp[ar+1][ac][br];
                    }
                    int a_down_b_down = INT_MIN;
                    if (ar+1<n && br+1<n) {
                        a_down_b_down = dp[ar+1][ac][br+1];
                    }
                    int a_right_b_right = INT_MIN;
                    if (ac+1<m && bc+1<m) {
                        a_right_b_right = dp[ar][ac+1][br];
                    }
                    int a_right_b_down = INT_MIN;
                    if (ac+1<m && br+1<n) {
                        a_down_b_down = dp[ar][ac+1][br+1];
                    }
                    int cur_sum = matrix[ar][ac];
                    if (ar != br) {
                        cur_sum += matrix[br][bc];
                    }
                    dp[ar][ac][br] = cur_sum + max(max(a_down_b_right, a_down_b_down), max(a_right_b_right, a_right_b_down));
                }
            }
       }

        return dp[0][0][0];
   } 
};

int main()
{

    Solution sol;

    // vector<vector<int>> m = {
    //     {-2, -3, 3},
    //     {-5, -10, 1},
    //     {0, 30, -5}};
    vector<vector<int>> m = {
        {1,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1},};
    cout << sol.max_sum(m) << endl;
    cout << sol.max_sum_dp(m) << endl;

    return 0;
}