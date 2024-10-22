#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <map>
#include <string>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <set>
#include <functional>
using namespace std;

class Graph;
class Node;
class Edge;

// 图的大结构
class Graph
{
public:
	map<int, Node *> nodes; // 点集， 编号为0的点实例是什么
	set<Edge *> edges;		// 边集，所有的边

	Graph() {}
	void createGraph(vector<vector<int>> matrix);
	void createGraphWithPoints(vector<vector<int>> points);	
};

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

// 转换接口例子：用一个二维数组来表示图，格式为: 权值 = matrix[0][0]  from点 = matrix[0][1]  to点 = matrix[0][2]
void Graph::createGraph(vector<vector<int>> matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		// 先从数组中获得from点、to点、权值的值
		int weight = matrix[i][0];
		int from = matrix[i][1]; // 编号
		int to = matrix[i][2];

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
	}
}


// 转换接口例子：用一个二维数组来表示全连接图，格式为: points[i][0]: x坐标；points[i][1]: y坐标
void Graph::createGraphWithPoints(vector<vector<int>> points)
{	
	int M = points.size();

	for (int i = 0; i < M; i++) {
		for (int j=0; j < M; j++) {
			if (i==j) {
				continue;
			}
			int x1 = points[i][0];
			int y1 = points[i][1];
			int x2 = points[j][0];
			int y2 = points[j][1];
			// 先从数组中获得from点、to点、权值的值
			int weight = abs(x1-x2) + abs(y1-y2);
			int from = i; // 编号
			int to = j;

			// 将from点和to点加到图里
			if (nodes.find(from) == nodes.end()) {
				nodes[from] = new Node(from);
			}
			if (nodes.find(to) == nodes.end()) {
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
		}
	}
}