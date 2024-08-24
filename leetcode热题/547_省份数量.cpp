/*
https://leetcode.cn/problems/number-of-provinces/description/

�� n �����У�����һЩ�˴���������һЩû��������������� a ����� b ֱ���������ҳ��� b ����� c ֱ����������ô���� a ����� c ���������

ʡ�� ��һ��ֱ�ӻ��������ĳ��У����ڲ�������û�������ĳ��С�

����һ�� n x n �ľ��� isConnected ������ isConnected[i][j] = 1 ��ʾ�� i �����к͵� j ������ֱ���������� isConnected[i][j] = 0 ��ʾ���߲�ֱ��������

���ؾ����� ʡ�� ��������

 

ʾ�� 1��


���룺isConnected = [[1,1,0],[1,1,0],[0,0,1]]
�����2
ʾ�� 2��


���룺isConnected = [[1,0,0],[0,1,0],[0,0,1]]
�����3
 

��ʾ��

1 <= n <= 200
n == isConnected.length
n == isConnected[i].length
isConnected[i][j] Ϊ 1 �� 0
isConnected[i][i] == 1
isConnected[i][j] == isConnected[j][i]
*/


class UnionFind {
public:
    map<int,int> parents;
    map<int, int> _sizes; //ÿ�������ϵĴ�С
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

        //��ƽ��
        int tmp = x;
        while (parents[tmp] != cur) {
            parents[tmp] = cur;
        }
        return cur;
    }

    //�ϲ���������
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
    ���鼯�ĵ���Ӧ�ã�ÿ�ε���һ���µĳ��У�����ÿ�������Գ�һ�����ϡ�Ȼ�������������ڼ��Ͻ��кϲ���
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