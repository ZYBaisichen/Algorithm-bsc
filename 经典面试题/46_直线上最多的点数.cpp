/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-01 13:09:28
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
给你一个数组 points ，其中 points[i] = [xi, yi] 表示 X-Y 平面上的一个点。求最多有多少个点在同一条直线上。



示例 1：


输入：points = [[1,1],[2,2],[3,3]]
输出：3
示例 2：


输入：points = [[1,1],[3,2],[5,3],[4,1],[2,3],[1,4]]
输出：4


提示：

1 <= points.length <= 300
points[i].length == 2
-104 <= xi, yi <= 104


测试链接：https://leetcode.cn/problems/max-points-on-a-line/description/
*/

/*

*/

class Solution
{
public:
    int maxPoints(vector<vector<int>> &points)
    {
        map<int, map<int, int>> mp; // 第一个key代表斜率的分子，value中的key代表斜率的分母，value代表在这个斜率下有多少点
        /*
        1. 遍历每个点作为起点。
            a. 然后再遍历每个点作为其搭配点，得到一个斜率，在这个斜率下统计有多少点。
            b. 每个起点统计能穿过的最多点。
            c. 注意斜率还需要分情况：共x、共y、相同点、普通斜率
        */
        int len = points.size();
        if (len == 0)
        {
            return 0;
        }
        int ans = 0;
        for (int i = 0; i < len; i++)
        {
            mp.clear();
            int same_point = 0; // 相同点
            int same_x = 0;     // 共x
            int same_y = 0;     // 共y
            int line = 0;       // 普通斜率
            // 前面的一定也可以将经过i后面时穿过最多的点得到。所以只需要往后看就可以
            for (int j = i + 1; j < len; j++)
            {
                cout << "=====" << endl;
                int delta_x = points[j][0] - points[i][0];
                int delta_y = points[j][1] - points[i][1];
                if (delta_x == 0 && delta_y == 0)
                { // 相同点
                    same_point++;
                }
                else if (delta_x == 0)
                {
                    same_x++;
                }
                else if (delta_y == 0)
                {
                    same_y++;
                }
                else
                {
                    int _gcd = gcd(delta_x, delta_y);
                    delta_x /= _gcd;
                    delta_y /= _gcd;
                    cout << "x1:" << points[i][0]  << " y1:" << points[i][1] << " x1:" << points[j][0] << " y2:" << points[j][1] << endl;
                    cout << " delta_x = " << delta_x << ", delta_y = " << delta_y << endl;
                    if (mp.find(delta_x) == mp.end())
                    {
                        mp[delta_x] = map<int, int>();
                        mp[delta_x][delta_y] = 0;
                    }
                    else
                    {
                        if (mp[delta_x].find(delta_y) == mp[delta_x].end())
                        {
                            mp[delta_x][delta_y] = 0;
                        }
                    }
                    mp[delta_x][delta_y]++;
                    cout << "mapmp[delta_x][delta_y]:" << mp[delta_x][delta_y] << endl;
                    line = max(line, mp[delta_x][delta_y]); // 每次抓一个最多的普通斜率数
                }
            }
            // 以i为起点的能穿过最多点的直线
            ans = max(ans, max(max(line, same_point), max(same_x, same_y)));
            cout << "line:" << line << " same_point:" << same_point << " same_x:" << same_x << " same_y:" << same_y << " ans:" << ans << endl;
        }
        return ans+1; //需要加上节点本身
    }

    int gcd(int a, int b)
    {
        return b == 0 ? a : gcd(b, a % b);
    }
};

int main()
{

    Solution sol;
    vector<vector<int>> arr = {{1,1},{3,2},{5,3},{4,1},{2,3},{1,4}};
    // vector<vector<int>> arr = {{1,1},{2,2},{3,3}};
    // string s = "abacb";
    // string s = "abcabcbb";

    cout << sol.maxPoints(arr) << endl;
    return 0;
}