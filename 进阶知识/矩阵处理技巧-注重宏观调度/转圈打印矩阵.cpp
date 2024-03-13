/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-13 13:35:58
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
 * 给定一个正方形或者长方形矩阵matrix，实现转圈打印
 * 输入：
 * 1 2 3
 * 4 5 6
 * 7 8 9
 *
 * 输出：1 2 3 6 9 8 7 4 5
 */
class Solution
{
public:
    vector<int> q_print(vector<vector<int>> &matrix)
    {
        vector<int> res;
        int n = matrix.size();
        if (n == 0)
        {
            return res;
        }
        int m = matrix[0].size();
        int a = 0, b = 0;
        int c = n - 1, d = m-1;

        while (a <= c && b <= d) {
            // 上边， 从左到右
            //防止只有一行或者一列的情况，错过数字
            cout << "a: " << a << " b:" << b << " c:" << c<< " d:" << d<< endl;
            for (int i = b; i <= d; ++i) {
                res.push_back(matrix[a][i]);
            }
            // 右边，从上到下
            for (int i = a+1; i < c; ++i) {
                res.push_back(matrix[i][d]);
            }
            if (a != c) {
                // 下边，从右到左
                for (int i = d; i > b; --i) {
                    res.push_back(matrix[c][i]);
                }
            }

            // 左边，从下到上
            for (int i = c; i > a; --i) {
                res.push_back(matrix[i][b]);
            }
            
            a++;
            b++;
            c--;
            d--;
        }
        return res;
    }
};
int main()
{   
    /*
    1 2 3 4
    5 6 7 8
    9 10 11 12
    */
    vector<vector<int>> matrix = {{1, 2, 3, 4}, {5, 6,7,8}, {9, 10, 11, 12}};
    /*
    1 2 3
    4 5 6
    7 8 9
    10 11 12
    */
    // vector<vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7,8,9}, {10, 11, 12}};
    Solution sol;
    vector<int> res = sol.q_print(matrix);
    for (int i = 0; i < res.size(); i++)
    {
        cout << res[i] << " ";
    }
    cout << endl;
    return 0;
}