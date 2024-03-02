/*
 * @Author: baisichen
 * @Date: 2024-03-02 16:10:58
 * @LastEditTime: 2024-03-02 16:23:19
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

class Solution
{
public:
    int f1(int n) {
        if (n < 1)
        {
            return 0;
        }
        if (n == 1 || n == 2)
        {
            return 1;
        }
        int a = 1, b = 1;
        int res = 0;
        for (int i=3;i<=n;i++) {
            res = a+b;
            a=b;
            b=res;
        }
        return res;
    }
    //  快速幂版本
    int f2(int n)
    {
        if (n < 1)
        {
            return 0;
        }
        if (n == 1 || n == 2)
        {
            return 1;
        }
        vector<vector<int>> base = {{1, 1}, {1, 0}};
        const vector<vector<int>> &res = matrix_power(base, n-2);
        return 1 * res[0][0] + 1 * res[1][0];
    }
    vector<vector<int>> matrix_power(vector<vector<int>> &base, int n)
    {
        vector<vector<int>> t = base; // 初始为矩阵一次方
        vector<vector<int>> res = {{1, 0}, {0, 1}};
        while (n != 0)
        {
            cout << "n:" << n << endl;  
            if (n & 1)
            {
                res = multy(res, t);
            }
            t = multy(t, t);
            n = n >> 1;
        }
        return res;
    }
    void print(vector<vector<int>> &m, string name) {
        cout << "name:" << name << endl;
        for (int i = 0; i < m.size(); ++i) {
            for (int j = 0; j < m[i].size(); ++j) {
                cout << m[i][j] << " ";
            }
            cout << endl;
        }
    }
    // 目前只能处理n*n的矩阵相乘
    vector<vector<int>> multy(vector<vector<int>> &a, vector<vector<int>> &b)
    {
        int n = a.size();
        vector<vector<int>> res(n, vector(n, 0));
        if (n != b.size())
        {
            return res;
        }
        // print(a, "multy_begin,a");
        // print(b, "multy_begin,b");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < b.size(); j++)
            {
                for (int k = 0; k < n; k++)
                {
                    // cout << "i:" << i << ",j:" << j << ",k:" << k << " a[i,k]" << a[i][k] << " b[k,j]" << b[k][j] << endl;
                    res[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        // print(res, "multy_begin,res");
        return res;
    }
};

int main()
{
    Solution sol;
    int ans0 = 0;
    int ans1 = 0;
    int ans2 = 0;
    int ans3 = 0;
    //1 1 2 3 5 8 13 21 34 55
    int n = 10;
    ans0 = sol.f1(n);
    ans1 = sol.f2(n);

    cout << "ans0:" << ans0 << " ans1:" << ans1 << " ans2:" << ans2 << " ans3:" << ans3 << endl;
    cout << "测试结束" << endl;
    return 0;
}
