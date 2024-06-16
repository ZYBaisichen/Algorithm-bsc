/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
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
参考基础知识-图-topo.cpp

https://leetcode.cn/problems/course-schedule/description/

你这个学期必须选修 numCourses 门课程，记为 0 到 numCourses - 1 。

在选修某些课程之前需要一些先修课程。 先修课程按数组 prerequisites 给出，其中 prerequisites[i] = [ai, bi] ，表示如果要学习课程 ai 则 必须 先学习课程  bi 。

例如，先修课程对 [0, 1] 表示：想要学习课程 0 ，你需要先完成课程 1 。
请你判断是否可能完成所有课程的学习？如果可以，返回 true ；否则，返回 false 。

 

示例 1：

输入：numCourses = 2, prerequisites = [[1,0]]
输出：true
解释：总共有 2 门课程。学习课程 1 之前，你需要完成课程 0 。这是可能的。
示例 2：

输入：numCourses = 2, prerequisites = [[1,0],[0,1]]
输出：false
解释：总共有 2 门课程。学习课程 1 之前，你需要先完成​课程 0 ；并且学习课程 0 之前，你还应先完成课程 1 。这是不可能的。
 

提示：

1 <= numCourses <= 2000
0 <= prerequisites.length <= 5000
prerequisites[i].length == 2
0 <= ai, bi < numCourses
prerequisites[i] 中的所有课程对 互不相同
*/

/*
经典拓扑排序算法，课程依赖关系构成图。
如果是无环图，则说明可以修完所有的课程。否则就修不完

*/

class Graph;
class Node;
class Edge;

// 点结构
class Node
{
public:
	int value;			  // value值
	int in;				  // 入度
	int out;			  // 出度
	vector<Node *> nexts; // 直接邻居，从当前node点出发链接的邻居
	vector<Edge *> edges; // 从当前node点出发的边都记在edges里

	Node(int value) {
		this->value = value;
		this->in = 0;
		this->out = 0;
	}
	Node() {
		this->value = -1;
		this->in = 0;
		this->out = 0;
	}
};

// 边结构
class Edge
{
public:
	int value;	// 权重记录
	Node *from; // 出发点
	Node *to;	// 到达点node
	Edge(int weight, Node *from, Node *to)
	{
		this->value = weight;
		this->from = from;
		this->to = to;
	}
};

// 图的大结构
class Graph
{
public:
	map<int, Node *> nodes; // 点集， 编号为0的点实例是什么
	set<Edge *> edges;		// 边集，所有的边

	Graph() {}
	void createGraph(vector<vector<int>> matrix) {
        //prerequisites = [[1,0]]。0是1的前向节点
        int weight = 1;
        int from=0,to=0;
        Node* from_node = nullptr, *to_node = nullptr;
        Edge* new_edge=nullptr;
        for (int i=0;i<matrix.size();i++) {
            from = matrix[i][1];
            to = matrix[i][0];
            if (nodes.find(from) == nodes.end()) {
                nodes[from] = new Node(from);
            }
            if (nodes.find(to) == nodes.end()) {
                nodes[to] = new Node(to);
            }
            from_node = nodes[from];
            to_node = nodes[to];
            //将to放在from的下一个节点上
            from_node->nexts.push_back(to_node);
            from_node->out++;
            to_node->in++;
            //将from到to的一条边放在from_node上
            new_edge = new Edge(1, from_node, to_node);
            from_node->edges.push_back(new_edge);
            edges.insert(new_edge);
        }
    }
};


class Solution {
public:
    //prerequisites = [[1,0]]。0是1的前向节点
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        Graph graph;
        graph.createGraph(prerequisites);
        //找到所有入度为0的点
        queue<Node*> _set_zero;
        map<Node*, int> in_map;
        for (auto it:graph.nodes) {
            in_map[it.second] = it.second->in;
            if (it.second->in == 0) {
                _set_zero.push(it.second); 
            }
        } 

        while (!_set_zero.empty()) {
            auto* cur = _set_zero.front();
            in_map.erase(cur);
            _set_zero.pop();
            for (auto it:cur->nexts) {
                in_map[it]--; //对应节点入度减一，删除当前节点
                if (in_map[it] == 0) {
                    _set_zero.push(it);
                }
            }
        }

        if (in_map.size() != 0) {
            return false;
        } else {
            return true;
        }

    }

    //观察到，课程安排之和入度有关，所以可以简化下代码
    /*
    1. 生成每个节点的入度表
    2. 将入度为0的点入队列
    3. 从队列中弹出一个节点。将这个节点的入度值置为-1；
    4. 将弹出的节点下面的节点的入度都减1，减到0入队列。重复3，4过程，直到队列为空
    5. 最后检查入度表是否有不是-1的节点，如果有则表示不能修完所有课程
    */
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        map<int,int>  _in_map;
        map<int, set<int>> adjacency; //邻接表
        int len = prerequisites.size();
        int from=0,to=0;
        for (int i=0;i<len;i++) {
            from = prerequisites[i][1];
            to = prerequisites[i][0];
            if (adjacency.find(from) != adjacency.end()) {
                adjacency[from].insert(to);
            } else {
                adjacency[from] = {to};
            }

            if (_in_map.find(to) != _in_map.end()) {
                _in_map[to]++;
            } else {
                _in_map[to] = 1;
            }
            if (_in_map.find(from) == _in_map.end()) {
                _in_map[from] = 0;
            }
        }

        queue<int> que;
        for (auto it:_in_map) {
            if (it.second == 0) {
                que.push(it.first);
            }
        }
        // cout << "_in_map:";
        // for(auto it:_in_map) {
        //     cout << it.first << "," << it.second << " ";
        // }
        // cout << endl;
        // cout << "que.size():" << que.size() << " top:" << que.front() << endl;

        int cur = 0;
        while (!que.empty()) {
            cur = que.front();
            _in_map.erase(cur);
            que.pop();
            for (auto it:adjacency[cur]) {
                _in_map[it]--; //对应节点入度减一，删除当前节点
                if (_in_map[it] == 0) {
                    que.push(it);
                }
            }
        }

        if (_in_map.size() == 0) {
            return true;
        } else {
            return false;
        }
    }

    /*
    还有一个比较有趣的解法是，dfs找环路，每个节点如果被其他节点发起的dfs找到了，则记录为-1，如果被当前dfs访问过记录为1。
    每次dfs开始搜索时，如果碰到了个状态为1的节点，说明本次dfs搜索已经找到过了，说明有环路，直接返回true。
    */
};
int main()
{

    Solution sol;
    return 0;
}
