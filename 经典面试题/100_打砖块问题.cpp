/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-24 13:19:46
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
#include <sstream>

using namespace std;

/*
测试链接：https://leetcode.cn/problems/bricks-falling-when-hit/description/
有一个 m x n 的二元网格 grid ，其中 1 表示砖块，0 表示空白。砖块 稳定（不会掉落）的前提是：

一块砖直接连接到网格的顶部，或者
至少有一块相邻（4 个方向之一）砖块 稳定 不会掉落时
给你一个数组 hits ，这是需要依次消除砖块的位置。每当消除 hits[i] = (rowi, coli) 位置上的砖块时，对应位置的砖块（若存在）会消失，然后其他的砖块可能因为这一消除操作而 掉落 。一旦砖块掉落，它会 立即 从网格 grid 中消失（即，它不会落在其他稳定的砖块上）。

返回一个数组 result ，其中 result[i] 表示第 i 次消除操作对应掉落的砖块数目。

注意，消除可能指向是没有砖块的空白位置，如果发生这种情况，则没有砖块掉落。

 

示例 1：

输入：grid = [[1,0,0,0],[1,1,1,0]], hits = [[1,0]]
输出：[2]
解释：网格开始为：
[[1,0,0,0]，
 [1,1,1,0]]
消除 (1,0) 处加粗的砖块，得到网格：
[[1,0,0,0]
 [0,1,1,0]]
两个加粗的砖不再稳定，因为它们不再与顶部相连，也不再与另一个稳定的砖相邻，因此它们将掉落。得到网格：
[[1,0,0,0],
 [0,0,0,0]]
因此，结果为 [2] 。
示例 2：

输入：grid = [[1,0,0,0],[1,1,0,0]], hits = [[1,1],[1,0]]
输出：[0,0]
解释：网格开始为：
[[1,0,0,0],
 [1,1,0,0]]
消除 (1,1) 处加粗的砖块，得到网格：
[[1,0,0,0],
 [1,0,0,0]]
剩下的砖都很稳定，所以不会掉落。网格保持不变：
[[1,0,0,0], 
 [1,0,0,0]]
接下来消除 (1,0) 处加粗的砖块，得到网格：
[[1,0,0,0],
 [0,0,0,0]]
剩下的砖块仍然是稳定的，所以不会有砖块掉落。
因此，结果为 [0,0] 。
 

提示：

m == grid.length
n == grid[i].length
1 <= m, n <= 200
grid[i][j] 为 0 或 1
1 <= hits.length <= 4 * 104
hits[i].length == 2
0 <= xi <= m - 1
0 <= yi <= n - 1
所有 (xi, yi) 互不相同

*/

/*

大流程：
    1. 将所有需要消除的位置从1变成2，代表将要被炮弹影响。修改过的矩阵是g
    2. 将g中所有的1，建立并查集集合，并标记各个集合是否是链接到天花板上的
    3. 按照消除顺序的逆序遍历消除位置，将其从2变成1，每次变成1时。合并各个集合，新增能挂在天花板上不掉落的砖块数量，就是消除这一砖块时掉落的砖块数量，记录在答案里


*/
void print_grid(vector<vector<int>>& grid, string suf = "grid:") {
    cout << suf << endl;
    int n = grid.size();
    if (n==0) {
        return;
    }
    int m = grid[0].size();
    for (int i=0;i<n;i++) {
        for (int j=0;j<m;j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


class UnionFind{
public:
    int* _parents; //记录每个节点的父节点
    int* _sizes; //记录当前节点的所代表集合的节点数量
    bool* is_celling; //是否是链接天花板的
    int celling_num; //链接天花板节点数量
    vector<vector<int>>&  _grid;
    int _n,_m;

    UnionFind(vector<vector<int>>& grid): _grid(grid) {
        init_space();
        init_connect();
    }
    void init_space() {
        int _n = _grid.size();
        int _m = _grid[0].size();
        // cout << "_n:" << _n << " _m:" << _m <<   endl;
        int n = _grid.size();
        int m = _grid[0].size();
        int all_num = n*m;
        
        _parents = new int[all_num];
        _sizes = new int[all_num];
        is_celling = new bool[all_num];
        celling_num = 0;

        for (int i=0;i<all_num;i++) {
            _parents[i] = -1;
            _sizes[i] = 0;
            is_celling[i] = false;
        }

        //将1的各个集合建立节点
        for (int i=0;i<n;i++) {
            for (int j=0;j<m;j++) {
                if (_grid[i][j] == 1) {
                    int idx = i*m+j;
                    _parents[idx] = idx;
                    _sizes[idx] = 1;
                    if (i==0) {
                        is_celling[idx] = true;
                        celling_num++;
                    }
                }
            }
        }

        // cout << "_parents:" << endl;
        // for (int i=0;i<all_num;i++) {
        //     cout << _parents[i] << " ";
        // }
        // cout << endl;
    }

    //初始化集合
    void init_connect() {
        int n = _grid.size();
        int m = _grid[0].size();
        int all_num = n*m;
        for (int i=0;i<n;i++) {
            for (int j=0;j<m;j++) {   
                unionSet(i, j, i+1, j);
                unionSet(i, j, i-1, j);
                unionSet(i, j, i, j+1);
                unionSet(i, j, i, j-1);
            }
        }
    }

    ~UnionFind() {
        if (_parents) {
            delete[] _parents;
        }
        if (_sizes) {
            delete[] _sizes;
        }
        if (is_celling) {
            delete[] is_celling;
        }
    }
    
    bool isSameSet(int x1, int y1, int x2, int y2) {
        int n = _grid.size();
        int m = _grid[0].size();
        int idx_a = x1 * m + y1;
        int idx_b = x2 * m + y2;
        int a_p = find_parent(idx_a);
        int b_p = find_parent(idx_b);
        // if (a==2&&b==3) {
        //     cout << "isSameSet:" <<  a_p << "," << b_p << endl;
        // }
        return a_p == b_p;
    }

    bool is_valid(int i,int j) {
        int n = _grid.size();
        int m = _grid[0].size();
        if (i<0||j<0||i>=n||j>=m) {
            return false;
        }
        if (_grid[i][j] != 1) {
            return false;
        }
        return true;
    }

    void unionSet(int x1, int y1, int x2, int y2) {
        //有一个下标无效，则不需要合并
        if (!is_valid(x1, y1) || !is_valid(x2, y2)) {
            return;
        }
        int n = _grid.size();
        int m = _grid[0].size();
        int idx_a = x1 * m + y1;
        int idx_b = x2 * m + y2;

        int a_p = find_parent(idx_a);
        int b_p = find_parent(idx_b);
        // cout << "x1:" << x1 << " y1:" << y1 << " x2:" << x2 << " y2:" << y2
        //      << " grid[x1][y1]:" << _grid[x1][y1] << " grid[x2][y2]:" << _grid[x2][y2]
        //      << " a_p:" << a_p << " b_p:" << b_p
        //      << " a_p_is_celling:" << is_celling[a_p] << " b_p_is_celling:" << is_celling[b_p]
        //      << " a_p_size:" << _sizes[a_p] << " _sizes[b_p]:" << _sizes[b_p]
        //      << " celling_num:" << celling_num << endl;
        if (a_p == -1 || b_p==-1) { //有一个是无效的，则不需要合并
            return;
        }
        //不在一个集合
        if (a_p != b_p) {
            bool status1 = is_celling[a_p];
            bool status2 = is_celling[b_p];
            int size1 = _sizes[a_p];
            int size2 = _sizes[b_p];

            int _big_p = _sizes[a_p] >= _sizes[b_p] ? a_p : b_p;
            int _little_p = _sizes[a_p] >= _sizes[b_p] ? b_p : a_p;
            _parents[_little_p] = _big_p; //小挂大
            _sizes[_big_p] = _sizes[_big_p] + _sizes[_little_p]; //大的更新size

            // _sizes[_little_p] = -1;
            //两个集合是否挂在天花板上的状态不一样时：有一个在天花板上
            if (status1 ^ status2) {
                is_celling[_big_p] = true;
                // cout << "celling_num11111:" << celling_num << " size1:" << size1 << " szie2:" << size2 << endl;
                celling_num += status1 ? size2 : size1;//加上本来不在天花板上1的数量
                // cout << "celling_num22222:" << celling_num << " size1:" << size1 << " szie2:" << size2 << endl;
            }

        }
    }

    int find_parent(int cur) {
        int res = cur;
        while(res!=-1 && _parents[res] != res) {
            res = _parents[res];
        }
        //扁平化
        int tmp = cur;
        while (res!=-1 && _parents[tmp] != res) {
            tmp = _parents[tmp];
            _parents[tmp] = res;
            // cout << "res:"  << res << " tmp:" << tmp << endl;
        }
        return res;
    }
    // map<V, Node<V>> nodes;

    //将对应消除的位置变为1，计算返回之前掉落的砖块数量
    int finger(int i,int j) {
        int n = _grid.size();
        int m = _grid[0].size();
        if (_grid[i][j] != 2) {
            return 0;
        }
        //将i,j位置的2变为1
        _grid[i][j] = 1;
        int idx = i*m+j;
        _parents[idx] = idx;
        _sizes[idx] = 1;
        if(i==0) {
            is_celling[idx] = true;
            celling_num++;
        }
        int pre = celling_num;
        // cout << "celling_num1: " << celling_num << endl;
        unionSet(i, j, i - 1, j);
        // cout << "celling_num2: " << celling_num << endl;
        unionSet(i, j, i+1, j);
        // cout << "celling_num3: "<< celling_num << endl;
        unionSet(i, j, i, j-1);
        // cout << "celling_num4: " << celling_num << endl;
        unionSet(i, j, i, j+1);
        // cout << "celling_num5: "<< celling_num << endl;
        int cur = celling_num; 
        //如果本身这个砖块是天花板了，celling_num前面已经增加了1。再合并不会把它算进去了，所以不需要再减1
        if (i==0) {
            return cur-pre;
        } else {
            return cur == pre ? 0 : cur-pre-1;
        }
    }
};



class Solution {
public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        vector<int> res;
        int n = grid.size();
        if (n==0) {
            return res;
        }
        int m = grid[0].size();
        if (m==0) {
            return res;
        }

        int len = hits.size();
        
        //将消除的地方都标记成2
        for (int i=0;i<len;i++) {
            int x= hits[i][0];
            int y= hits[i][1];
            if (grid[x][y] == 1) {
                grid[x][y] = 2;
            }
        }
        print_grid(grid);

        UnionFind uf(grid);
        // cout << "0,0,0:" << uf.isSameSet(0,0,0,0) << endl;
        // cout << "1,1,2:" << uf.isSameSet(1, 1, 1, 2) << endl;

        res.resize(len,0);
        for (int i=len-1;i>=0;i--) {
            res[i] = uf.finger(hits[i][0], hits[i][1]);
        }
        return res;
    }
};

int main() {
    Solution sol;
    // vector<vector<int>> grid = {
    //     {1, 0, 0, 0},
    //     {1, 1, 1, 0},
    // };
    // vector<vector<int>> hits = {
    //     {1, 0},
    // };
vector<vector<int>> grid = {
    {1,1,1},
    {0,1,0},
    {0,0,0}};
vector<vector<int>> hits ={
    {0,2},
    {2,0},
    {0,1},
    {1,2}};

//     grid = [[1,0,0,0],[1,1,1,0]], hits = [[1,0]]
// 输出：[2]
    vector<int> res = sol.hitBricks(grid, hits);
    cout << "res:" << endl;
    for (auto &x: res) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}