/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-13 10:53:05
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
#include <unordered_map>

using namespace std;

/*
https://leetcode.cn/problems/word-search/description/
给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。如果 word 存在于网格中，返回 true ；否则，返回 false 。

单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。

 

示例 1：


输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
输出：true
示例 2：


输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "SEE"
输出：true
示例 3：


输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCB"
输出：false
 

提示：

m == board.length
n = board[i].length
1 <= m, n <= 6
1 <= word.length <= 15
board 和 word 仅由大小写英文字母组成
 

进阶：你可以使用搜索剪枝的技术来优化解决方案，使其在 board 更大的情况下可以更快解决问题？
*/


/*

*/

class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        int m = board.size();
        int n = board[0].size();
        bool ans = false;
        for (int i=0;i<m;i++) {
            for (int j=0;j<n;j++) {
                vector<vector<int>> visited(m, vector<int>(n, false));
                //枚举开头
                ans = ans | dfs(board, word, 0, i, j, visited);
                if (ans) {
                    break;
                }
            }
            if (ans) {
                break;
            }
        }
        return ans;
    }

    //当前来到了i,j位置， 将要寻找word[idx]，如果idx来到了末尾，则返回true
    bool dfs(vector<vector<char>>& board, string& word, int idx, int i, int j,vector<vector<int>>& visited) {
        if (idx == word.length()) {
            return true;
        }

        int m = board.size();
        int n = board[0].size();
        if (i<0 || j<0 || i>=m || j>=n) {
            return false;
        }
        //走过了这条路，不用再走了
        //减枝
        if (visited[i][j]) {
            return false;
        }

        //登录上i,j
        if (board[i][j] == word[idx]) {
            visited[i][j] = true;
            bool left = dfs(board, word, idx+1, i, j-1, visited);
            bool right = dfs(board, word, idx+1, i, j+1, visited);
            bool up = dfs(board, word, idx+1, i-1, j, visited);
            bool down = dfs(board, word, idx+1, i+1, j, visited);
            visited[i][j] = false; //回溯
            return up || down || left || right;
        }

        return false;
    } 

};
int main()
{

    Solution sol;
    vector<vector<char>> board = {{'A','B','C','E'},{'S','F','C','S'},{'A','D','E','E'}};
    string word = "ABCCED";
    cout << sol.exist(board, word) << endl;
    return 0;
}
