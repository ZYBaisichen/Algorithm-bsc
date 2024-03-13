/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-13 14:41:03
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

/**
 * 给定一个正方形矩阵， 只用有限几个变量， 实现矩阵中每个位置的数顺时针转动
 * 90度， 比如如下的矩阵
 * 0 1 2 3
 * 4 5 6 7
 * 8 9 10 11
 * 12 13 14 15
 * 矩阵应该被调整为：
 * 12 8 4 0
 * 13 9 5 1
 * 14 10 6 2
 * 15 11 7 3
 */
class Solution
{
public:
    void change_print(vector<vector<int>> &matrix)
    {
        int n = matrix.size();
        if (n == 0)
        {
            return;
        }
        int m = matrix[0].size();
        // 限制外层一圈分组
        int a = 0, b = 0;
        int c = n - 1, d = m - 1;

        while (a < c)
        {
            int len = c - a + 1; // 宽度
            for (int i = 0; i < len - 1; i++)
            {
                int tmp = matrix[a][b + i];
                matrix[a][b + i] = matrix[c - i][b];
                matrix[c - i][b] = matrix[c][d - i];
                matrix[c][d - i] = matrix[a + i][d];
                matrix[a + i][d] = tmp;
            }
            a++;
            b++;
            c--;
            d--;
        }
    }
};
int main()
{
    /*
     * 1 2 3 4
     * 5 6 7 8
     * 9 10 11 12
     * 13 14 15 16
     *
     *
     * 13 9 5 1
     * 14 10 6 2
     * 15 11 7 3
     * 16 12 8 4
     */

    vector<vector<int>> matrix = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    Solution sol;
    sol.change_print(matrix);
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}