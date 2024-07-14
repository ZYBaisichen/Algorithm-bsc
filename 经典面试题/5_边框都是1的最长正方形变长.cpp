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

using namespace std;

/*
给一个矩阵，值均是0或1。找到边框全是1的最大正方形变长是多少。
*/

/*
矩阵如果是n*n， 则有：
    O(n^4)个长方形：任意点一个点有N^2个可能，任意两个点形成长方形，因为有重复，所以数量级达到了N^4个长方形
    O(n^3)个正方形：任意一个点有N^2可能作为左上角，只能枚举边长为1,2,3...n的，右下角的点有n种可能，所有正方形打到了O(N^3)级别
*/
class Solution
{
public:
    int largest1BorderedSquare(vector<vector<int>> &grid)
    {
        // 枚举每个正方形复杂度是O(N^3)
        // 假设再枚举每个正方形的边长4*N, 则复杂度将打到O(N^4)，所以需要优化边长遍历。
        // 弄两个数组，r[i][j]统计ij位置右侧有多少个连续的1，d[i][j]统计ij位置下方有多少个连续的1.均包含自身grid[i][j]点
        // 遍历每个正方形，假设点到了(i，j)和(a,b)两点，
        // 当前点的左边和右边连续1的数量是否超过了正方形边长；右上点下方1的数量；左下点左边1的数量；一次检查都满足正方形边长的话则返回满足条件
        int n = grid.size();
        if (n == 0)
        {
            return 0;
        }
        int m = grid[0].size();

        vector<vector<int>> r(n, vector(m, 0));
        vector<vector<int>> d(n, vector(m, 0));
        if (grid[n - 1][m - 1] == 1)
        {
            r[n - 1][m - 1] = 1;
            d[n - 1][m - 1] = 1;
        }
        // 填r数组的最后一列。和d的最后一列
        for (int i = n - 2; i >= 0; i--)
        {
            if (grid[i][m - 1] == 1)
            {
                r[i][m - 1] = 1;
                d[i][m - 1] = d[i + 1][m - 1] + 1;
            }
        }
        // cout << "here1e"<<endl;
        // 填d数组的最后一行.和r的最后一行
        for (int j = m - 2; j >= 0; j--)
        {
            if (grid[n - 1][j] == 1)
            {
                d[n - 1][j] = 1;
                r[n - 1][j] = r[n - 1][j + 1] + 1;
            }
        }
        // cout << "here2"<<endl;
        // 填普遍位置, d依赖从下到上，r依赖从右到左
        for (int i = n - 2; i >= 0; i--)
        {
            for (int j = m - 2; j >= 0; j--)
            {
                if (grid[i][j] == 1)
                {
                    r[i][j] = r[i][j + 1] + 1;
                    d[i][j] = d[i + 1][j] + 1;
                }
            }
        }
        // cout << "heree" << endl;

        int res = 0;
        for (int size = min(m, n); size > 0; size--)
        {
            for (int i = 0; i <= n - size; i++)
            { // 左上角点行最多枚举到n-size
                for (int j = 0; j <= m - size; j++)
                { // 左上角点列最多枚举到m-size
                    if (r[i][j] >= size && d[i][j] >= size && r[i + size - 1][j] >= size && d[i][j + size - 1] >= size)
                    {
                        res = max(res, size);
                    }
                }
            }
        }
        //视情况返回边长，还是正方形元素数量
        return res;
        //return res * res;
    }
};

int main()
{
    /*
    {1,1,0,0,1,0,1},
    {0,1,1,1,1,0,1},
    {0,1,0,0,1,1,0},
    {0,1,0,0,1,0,1},
    {0,1,1,1,1,1,1}
    */
    Solution sol;
    vector<vector<int>> grid = {
        { 1, 1, 1 }, 
        { 1, 0, 1 }, 
        { 1, 1, 1 }
    };
    cout << "largest1BorderedSquare:" << sol.largest1BorderedSquare(grid) << endl;
}