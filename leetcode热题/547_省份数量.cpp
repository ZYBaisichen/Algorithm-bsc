/*
https://leetcode.cn/problems/number-of-provinces/description/

有 n 个城市，其中一些彼此相连，另一些没有相连。如果城市 a 与城市 b 直接相连，且城市 b 与城市 c 直接相连，那么城市 a 与城市 c 间接相连。

省份 是一组直接或间接相连的城市，组内不含其他没有相连的城市。

给你一个 n x n 的矩阵 isConnected ，其中 isConnected[i][j] = 1 表示第 i 个城市和第 j 个城市直接相连，而 isConnected[i][j] = 0 表示二者不直接相连。

返回矩阵中 省份 的数量。

 

示例 1：


输入：isConnected = [[1,1,0],[1,1,0],[0,0,1]]
输出：2
示例 2：


输入：isConnected = [[1,0,0],[0,1,0],[0,0,1]]
输出：3
 

提示：

1 <= n <= 200
n == isConnected.length
n == isConnected[i].length
isConnected[i][j] 为 1 或 0
isConnected[i][i] == 1
isConnected[i][j] == isConnected[j][i]
*/


class UnionFind {
public:
    map<int,int> parents;
    map<int, int> _sizes; //每个代表集合的大小
    map<int, int> nodes;
    UnionFind() {}
    void insert(int node) {
        if (nodes.find(node) == nodes.end()) {
            parents[node] = node;
            nodes[node] = 1;
            _sizes[node] = 1;
        }
    }

    int find_parent(int x) {
        int cur = x;
        while (cur != parents[cur]) {
            cur = parents[cur];
        }

        //扁平化
        int tmp = x;
        while (parents[tmp] != cur) {
            parents[tmp] = cur;
        }
        return cur;
    }

    //合并两个集合
    void union_set(int x, int y) {
        int x_p = find_parent(x);
        int y_p = find_parent(y);
        // cout << "x:" << x << " y:" << y << " x_p:" << x_p << " y_p:" << y_p  << endl;
        if (x_p != y_p) {
            int big = _sizes[x_p] > _sizes[y_p] ? x_p : y_p;
            int little = _sizes[x_p] > _sizes[y_p] ? y_p : x_p;

            parents[little] = big;
            _sizes[big] = _sizes[big] + (_sizes[little] == -1 ? 0:_sizes[little]);
            _sizes[little] = -1;
        }
    }


};
class Solution {
public:
    /*
    并查集的典型应用，每次到来一个新的城市，先让每个城市自成一个集合。然后将两个城市所在集合进行合并。
    */
    
    int findCircleNum(vector<vector<int>>& isConnected) {
        int m = isConnected.size();
        if (m==0) {
            return 0;
        }
        int n = isConnected[0].size();
        if (m==0) {
            return 0;
        }
        UnionFind uf;
        for (int i=0;i<m;i++) {
            for (int j=0;j<n;j++) {
                uf.insert(i);
                uf.insert(j);
                if (isConnected[i][j] == 1) {
                    uf.union_set(i, j);
                }
            }
        }
        int ans = 0;
        for (auto it:uf._sizes) {
            if (it.second != -1) {
                ans += 1;
            }
        }
        return ans;
    }
};