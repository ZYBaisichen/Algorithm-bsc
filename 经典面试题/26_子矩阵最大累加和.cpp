/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 23:05:28
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

using namespace std;
// 子矩阵最大累加和问题
// 给定一个二维数组grid，找到其中子矩阵的最大累加和
// 返回拥有最大累加和的子矩阵左上角和右下角坐标
// 如果有多个子矩阵都有最大累加和，返回哪一个都可以
// 测试链接 : https://leetcode.cn/problems/max-submatrix-lcci/

/*
给定一个正整数、负整数和 0 组成的 N × M 矩阵，编写代码找出元素总和最大的子矩阵。

返回一个数组 [r1, c1, r2, c2]，其中 r1, c1 分别代表子矩阵左上角的行号和列号，r2, c2 分别代表右下角的行号和列号。若有多个满足条件的子矩阵，返回任意一个均可。

注意：本题相对书上原题稍作改动

示例：

输入：
[
   [-1,0],
   [0,-1]
]
输出：[0,1,0,1]
解释：输入中标粗的元素即为输出所表示的矩阵
 

说明：

1 <= matrix.length, matrix[0].length <= 200
*/

/*
对于子矩阵的某个上边界，枚举其下边界，然后压缩到一个一维数组上。在这个一维数组上求最大子数组之和。
*/
class Solution {
public:
    //dp
    vector<int> getMaxMatrix(vector<vector<int>>& matrix) {
        int n= matrix.size();
        vector<int> res(4,-1);
        if (n==0) {
            return res;
        }
        int m= matrix[0].size();
        if (m==0) {
            return res;
        }
        //枚举枚举矩阵上边界
        int max_sum = INT_MIN;
        int x1=-1,y1=-1,x2=-1,y2=-1;//记录左上角右下角
        for (int up=0;up<n;up++) {
            vector<int> tmp(matrix[up]);
            for (int down=up;down<n;down++) {
                int cur = 0, left=0;
                //下面仅求up固定，枚举到当前down时的单数组最大累加和问题
                for (int i=0;i<m;i++) {
                    if (up!=down) {
                        tmp[i]+=matrix[down][i]; //数组压缩
                    }
                    cur += tmp[i];
                    if (cur>=max_sum) { //记录一次答案
                        max_sum = cur;
                        x1=up;
                        x2=down;
                        y1=left;
                        y2=i;
                    }
                    if (cur<0) {
                        cur = 0;
                        left = i+1;
                    }
                }
            }
        }
        res[0] = x1;
        res[1] = y1;
        res[2] = x2;
        res[3] = y2;
        return res;
    }

};

int main()
{
    Solution sol;
    vector<int> arr= {-2,1,-3,4,-1,2,1,-5,4};
    //输出6
    cout << sol.maxSubArray(arr) << endl;
    return 0;
}