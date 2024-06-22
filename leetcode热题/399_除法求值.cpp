/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-22 21:08:21
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

https://leetcode.cn/problems/evaluate-division/description/

给你一个变量对数组 equations 和一个实数值数组 values 作为已知条件，其中 equations[i] = [Ai, Bi] 和 values[i] 共同表示等式 Ai / Bi = values[i] 。每个 Ai 或 Bi 是一个表示单个变量的字符串。

另有一些以数组 queries 表示的问题，其中 queries[j] = [Cj, Dj] 表示第 j 个问题，请你根据已知条件找出 Cj / Dj = ? 的结果作为答案。

返回 所有问题的答案 。如果存在某个无法确定的答案，则用 -1.0 替代这个答案。如果问题中出现了给定的已知条件中没有出现的字符串，也需要用 -1.0 替代这个答案。

注意：输入总是有效的。你可以假设除法运算中不会出现除数为 0 的情况，且不存在任何矛盾的结果。

注意：未在等式列表中出现的变量是未定义的，因此无法确定它们的答案。

 

示例 1：

输入：equations = [["a","b"],["b","c"]], values = [2.0,3.0], queries = [["a","c"],["b","a"],["a","e"],["a","a"],["x","x"]]
输出：[6.00000,0.50000,-1.00000,1.00000,-1.00000]
解释：
条件：a / b = 2.0, b / c = 3.0
问题：a / c = ?, b / a = ?, a / e = ?, a / a = ?, x / x = ?
结果：[6.0, 0.5, -1.0, 1.0, -1.0 ]
注意：x 是未定义的 => -1.0
示例 2：

输入：equations = [["a","b"],["b","c"],["bc","cd"]], values = [1.5,2.5,5.0], queries = [["a","c"],["c","b"],["bc","cd"],["cd","bc"]]
输出：[3.75000,0.40000,5.00000,0.20000]
示例 3：

输入：equations = [["a","b"]], values = [0.5], queries = [["a","b"],["b","a"],["a","c"],["x","y"]]
输出：[0.50000,2.00000,-1.00000,-1.00000]
 

提示：

1 <= equations.length <= 20
equations[i].length == 2
1 <= Ai.length, Bi.length <= 5
values.length == equations.length
0.0 < values[i] <= 20.0
1 <= queries.length <= 20
queries[i].length == 2
1 <= Cj.length, Dj.length <= 5
Ai, Bi, Cj, Dj 由小写英文字母与数字组成
*/

/*

*/

class Graph;
class Node;
class Edge;

// 图的大结构
class Graph
{
public:
    map<string, Node *> nodes; // 点集， 编号为0的点实例是什么
    set<Edge *> edges;         // 边集，所有的边

    Graph() {}
    void createGraph(vector<vector<string>> &matrix, vector<double> &values);
};

// 点结构
class Node
{
public:
    string value;         // value值
    int in;               // 入度
    int out;              // 出度
    vector<Node *> nexts; // 直接邻居，从当前node点出发链接的邻居
    vector<Edge *> edges; // 从当前node点出发的边都记在edges里

    Node(string value)
    {
        this->value = value;
        this->in = 0;
        this->out = 0;
    }
    Node()
    {
        this->value = "";
        this->in = 0;
        this->out = 0;
    }
};
// 边结构
class Edge
{
public:
    double value; // 权重记录
    Node *from;   // 出发点
    Node *to;     // 到达点node
    Edge(double weight, Node *from, Node *to)
    {
        this->value = weight;
        this->from = from;
        this->to = to;
    }
};

// 转换接口例子：用一个二维数组来表示图，格式为: 权值 = matrix[0][0]  from点 = matrix[0][1]  to点 = matrix[0][2]
void Graph::createGraph(vector<vector<string>> &matrix, vector<double> &values)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        // 先从数组中获得from点、to点、权值的值
        double weight = values[i];
        string from = matrix[i][0]; // 编号
        string to = matrix[i][1];

        // 将from点和to点加到图里
        if (nodes.find(from) == nodes.end())
        {
            nodes[from] = new Node(from);
        }
        if (nodes.find(to) == nodes.end())
        {
            nodes[to] = new Node(to);
        }
        // 获取from点和to点
        Node *fromNode = nodes[from];
        Node *toNode = nodes[to];

        // form点、to点和权重组成一条边
        Edge *newEdge = new Edge(weight, fromNode, toNode);
        // from点的邻接点集加入to点
        fromNode->nexts.push_back(toNode);
        // from点出度加一
        fromNode->out++;
        // to点入度加一
        toNode->in++;
        // 将这条边加入form点属于的边集里
        fromNode->edges.push_back(newEdge);
        // 将这条边加入图的边集里
        edges.insert(newEdge);

        // to到from也组成一条边
        Edge *de_newEdge = new Edge(1.0 / weight, toNode, fromNode);
        // from点的邻接点集加入to点
        toNode->nexts.push_back(fromNode);
        // from点出度加一
        toNode->out++;
        // to点入度加一
        fromNode->in++;
        // 将这条边加入form点属于的边集里
        toNode->edges.push_back(de_newEdge);
        // 将这条边加入图的边集里
        edges.insert(de_newEdge);
    }
}

class Solution
{
public:
    // 1.利用队列实现
    // 2.从源节点开始一次按照宽度进队，然后弹出
    // 3.每弹出一个点，就把该节点所有的没有进过队列的直接邻接节点放进队列
    // 4.一直弹出直到队列为空
    void bfs(Node *head)
    {
        if (head == NULL)
        {
            return;
        }
        queue<Node *> _queue;
        set<Node *> node_sets;
        _queue.push(head);
        node_sets.insert(head);
        while (!_queue.empty())
        {
            Node *cur = _queue.front();
            _queue.pop();
            // cout << cur->value << ",";
            for (auto &edge : cur->edges)
            {
                // cout << "cur->value:" << cur->value << " to:" << edge->to->value << " weight:" << edge->value << endl;
                if (node_sets.find(edge->to) == node_sets.end())
                {
                    _queue.push(edge->to);
                    node_sets.insert(edge->to);
                }
            }
        }
    }
    /*
    1. 建立有向图
    2. 对于每个问题，从开始节点开始做深度优先遍历，沿途记录下来结果。
    3. 有可能问题是a,b，但实际的变量对数组记录的是b,a，所以也需要反过来查找下
    */
    vector<double> calcEquation1(vector<vector<string>> &equations, vector<double> &values, vector<vector<string>> &queries)
    {
        Graph gf;
        gf.createGraph(equations, values);
        // bfs(gf.nodes["a"]);
        // cout << endl;
        int len = queries.size();
        vector<double> res(len, -1.0);
        // 每个答案
        Node *from_node = nullptr;
        Node *to_node = nullptr;

        for (int i = 0; i < len; i++)
        {
            from_node = nullptr;
            to_node = nullptr;
            from_node = gf.nodes.find(queries[i][0]) != gf.nodes.end() ? gf.nodes[queries[i][0]] : nullptr;
            to_node = gf.nodes.find(queries[i][1]) != gf.nodes.end() ? gf.nodes[queries[i][1]] : nullptr;
            if (!from_node || !to_node)
            {
                res[i] = -1.0;
                continue;
            }
            map<Node *, bool> visited;
            double tmp = get_res(gf, from_node, to_node, 1.0, visited);
            // cout << "res from:" << from_node->value << ",to:" << to_node->value << " tmp:" << tmp<< endl;
            if (tmp == -1.0)
            { // 反过来再找一遍
                visited.clear();
                tmp = 1.0 / get_res(gf, to_node, from_node, 1.0, visited);
                // cout << "res from:" << to_node->value << ",from:" << from_node->value << " tmp:" << tmp << endl;
            }

            res[i] = tmp; // 是不是-1都要填了
        }
        return res;
    }
    double get_res(Graph &gf, Node *cur, Node *to, double path, map<Node *, bool> &visited)
    {
        // cout << "[get_res] cur:" << cur->value << ",to:" << to->value << ",path:" << path << endl;
        if (cur == to)
        {
            return path;
        }

        double tmp = -1.0;
        for (auto it : cur->edges)
        {
            if (visited.find(it->to) == visited.end() || !visited[it->to])
            {
                path *= it->value;
                visited[it->to] = true;
                tmp = get_res(gf, it->to, to, path, visited); // 深度优先搜索
                if (tmp != -1.0)
                { // 计算出来结果后直接返回
                    return tmp;
                }
                path /= it->value;
            }
        }

        return -1.0; // 没找到的话就
    }

    /*
    flyd算法
    感觉可以不用模版.
    另外因为假设一个节点到另一个节点有边，为了保证算数的唯一性，不能存在两条不同值的路径到另一个节点
    */
    void print_m(vector<vector<double>> m, string suff) {
        cout << suff<< endl;
        for (auto& it:m) {
            for (auto it2:it) {
                cout<<it2<<" ";
            }
            cout << endl;
        }
        cout << endl;
    }
    vector<double> calcEquation(vector<vector<string>> &equations, vector<double> &values, vector<vector<string>> &queries) {
        /*
        建立邻接表
        */

        map<string, int> str_idx_map;
        map<int, string> idx_string_map;
        int len = equations.size();
        int idx = 0;
        for (int i=0;i<len;i++) {
            if (str_idx_map.find(equations[i][0]) == str_idx_map.end()) {
                str_idx_map[equations[i][0]] = idx;
                idx_string_map[idx] = equations[i][0];
                idx++;
            }
            if (str_idx_map.find(equations[i][1]) == str_idx_map.end()) {
                str_idx_map[equations[i][1]] = idx;
                idx_string_map[idx] = equations[i][1];
                idx++;
            }
        }
        cout << "str_to_idx:" << endl;
        for (auto it:str_idx_map) {
            cout << it.first << "->" << it.second << endl;
        }
        cout << "str_to_idx_end" << endl;
        vector<vector<double>> adj(idx, vector<double>(idx, -1.0));
        int from_idx=0;
        int to_idx=0;
        for (int i=0;i<len;i++) {
            from_idx = str_idx_map[equations[i][0]];
            to_idx = str_idx_map[equations[i][1]];
            adj[from_idx][from_idx] = 1.0;
            adj[from_idx][to_idx] = values[i];
            adj[to_idx][from_idx] = 1.0/values[i];
            adj[to_idx][to_idx] = 1.0;
        }
        print_m(adj, "init:");

        for (int k=0;k<idx;k++) { //挨个拿出来节点更新
            for (int i=0;i<idx;i++) {
                for (int j=0;j<idx;j++) {
                    cout << "i:" << i << " j:" << j << " k:" << k 
                        << " adj[i][j]" << adj[i][j] << " adj[i][k]: " << adj[i][k] 
                        << " adj[k][j]: "<< adj[k][j]<< endl;
                    //只有在i和j没录的时候才更新
                    if (adj[i][j]==-1.0 && adj[i][k] != -1.0 && adj[k][j] != -1.0) {
                        adj[i][j] = adj[i][k]*adj[k][j];
                    }
                }
            }
            cout<<"=-=========after k:" << k << endl;
            print_m(adj, "init:");
        }
        
        int len_q = queries.size();
        vector<double> res(len_q, 0);
        for(int i=0;i<len_q;i++) {
            if (str_idx_map.find(queries[i][0]) == str_idx_map.end() 
                || str_idx_map.find(queries[i][1]) == str_idx_map.end()) {
                res[i] = -1.0;
            } else {
                from_idx = str_idx_map[queries[i][0]];
                to_idx = str_idx_map[queries[i][1]];
                res[i] = adj[from_idx][to_idx];
            }
        }
        return res;
    }
};

int main()
{
    Solution sol;

    // vector<vector<string>> equations = {{"a","b"},{"b","c"}};
    // vector<double> values = {2.0,3.0};
    // vector<vector<string>> queries = {{"a","c"},{"b","a"},{"a","e"},{"a","a"},{"x","x"}};
    // 输出：[6.00000,0.50000,-1.00000,1.00000,-1.00000]

    vector<vector<string>> equations = {{"a","e"},{"b","e"}};
    vector<double> values = {4.0,3.0};
    vector<vector<string>> queries = {{"a","b"},{"e","e"},{"x","x"}};
    auto res = sol.calcEquation(equations, values, queries);
    for (auto it:res) {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
