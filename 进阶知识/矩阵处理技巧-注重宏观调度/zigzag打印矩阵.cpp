/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-03-13 12:13:00
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
 * 用zigzag的方式打印矩阵， 比如如下的矩阵
 * 0 1 2 3
 * 4 5 6 7
 * 8 9 10 11
 * 打印顺序为： 0 1 4 8 5 2 3 6 9 10 7 11
 */
class Solution
{
public:
    vector<int> zig_zag_print(vector<vector<int>>& matrix) {
        vector<int> res;
        int n = matrix.size();
        if (n == 0) {
            return res;
        }
        int m = matrix[0].size();
        //定义两个点a和b，a一直往下走，不能再往下了再往右走；b一直往右，不能再往右了再往下走。
        //a和b就是需要打印的上下界
        int a=0,b=0;
        int c=0,d=0;
        int flag = false; //从上到下的标记，初始时是从下到上打印
        while (b<m) {
            cout << "flag " << flag << " a:" << a << " b:" << b << " c:" << c << " d:" << d << endl;
            if (flag) {
                int i=c;
                int j=d;
                while (i<=a&&j>=c) {
                    res.push_back(matrix[i][j]);
                    i++;
                    j--;
                }
            } else {
                int i=a;
                int j=b;
                while (i>=c&&j<=d) {
                    res.push_back(matrix[i][j]);
                    i--;
                    j++;
                }
            }
            //挪动上下界
            //下界
            if (a<n-1) {
                a++;
            } else {
                b++;
            }
            //上界
            if (d<m-1) {
                d++;
            } else {
                c++;
            }
            flag = !flag;
        }
        return res;
    }
};
int main()
{
    vector<vector<int>> matrix = {{0,1,2,3}, {4,5,6,7}, {8,9,10,11}};
    Solution sol;
    vector<int> res = sol.zig_zag_print(matrix);
    for (int i=0;i<res.size();i++) {
        cout << res[i] << " ";
    }
    cout << endl;
    return 0;
}