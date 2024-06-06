/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-06 20:56:31
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
#include <functional>

using namespace std;

/*
https://leetcode.cn/problems/rotate-image/description/

给定一个 n × n 的二维矩阵 matrix 表示一个图像。请你将图像顺时针旋转 90 度。

你必须在 原地 旋转图像，这意味着你需要直接修改输入的二维矩阵。请不要 使用另一个矩阵来旋转图像。

 

示例 1：


输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
输出：[[7,4,1],[8,5,2],[9,6,3]]
示例 2：


输入：matrix = [[5,1,9,11],[2,4,8,10],[13,3,6,7],[15,14,12,16]]
输出：[[15,13,2,5],[14,3,4,1],[12,6,8,9],[16,7,10,11]]
 

提示：

n == matrix.length == matrix[i].length
1 <= n <= 20
-1000 <= matrix[i][j] <= 1000

*/

/*
原地旋转矩阵，在进阶班的矩阵处理技巧中用到了，主要是注重宏观调度
对于每一层，找到一个锚点。四个方向分别旋转。
1. 比如最开始旋转最外层，锚点位置是(0,0)， 先保存第一行作为tmp，然后逆时针处理，将第0列放在第一行；将第len行放在第0列；将第len列放在第len行；将第0行(tmp)放在第len列
2. 更新锚点位置为(1,1)，重复有运行1步骤

*/
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        if (n <= 1) {
            return;
        }
        
        int i=0,j=0, top_i, buttom_i, top_j, buttom_j;
        while (i<n/2) {
            top_i = i;
            top_j = j;
            buttom_i = n-i-1; //右下角点
            buttom_j = n-j-1;

            if (top_i >= buttom_i || top_j >= buttom_j) { //走到了最里面，可以结束了
                return;
            }


            int cur_len  = buttom_i - top_i + 1;
            vector<int> tmp(cur_len, 0);

            //保存第一行
            for (int k = top_j; k<= buttom_j; k++) {
                tmp[k] = matrix[top_i][k];
            }

            //左边的列放在上面一行，从下往上遍历
            for (int k=buttom_i,kk=top_j;k>=top_i;k--,kk++) {
                matrix[top_i][kk] = matrix[k][top_j];
            }

            // 下边的行放在左边一列
            for (int k=buttom_j,kk=buttom_i;k>=top_j;k--,kk--) {
                matrix[kk][top_j] = matrix[buttom_i][k];
            }

            // 右边的列放在下边一行
            for (int k=top_i,kk=buttom_j;k<=buttom_i;k++,kk--) {
                matrix[buttom_i][kk] = matrix[k][buttom_j];
            }

            // 上边的一行放在右边的列
            for (int k=0,kk=top_i;k<cur_len;k++,kk++) {
                matrix[kk][buttom_j] = tmp[k];
            }

            //更新锚点
            i++;
            j++;
        }
    }
};
int main() {

    Solution sol;
    vector<vector<int>> nums = {
        { 1, 2, 3 }, 
        { 4, 5, 6 }, 
        { 7, 8, 9 }};
    sol.rotate(nums);
    cout << "res:" << endl;
    for (auto it:nums) {
        for (auto it2:it) {
            cout << it2 << " ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}