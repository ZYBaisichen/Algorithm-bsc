#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include<map>
#include<string>
#include<queue>
#include <sstream>
#include <unordered_set>
#include <set>
using namespace std;

//图的大结构
public class Graph	{
	public HashMap<Integer,Node> nodes;   //点集
	public HashSet<Edge> edges;           //边集
	
	public Graph(){
		nodes = new HashMap<>();
		edges = new HashSet<>();
	}
}

//点结构
class Node {
public:
	int value; //value值
	int in; //入度
	int out; //出度
	Vector<Node> nexts; //直接邻居，从当前node点出发链接的邻居
	Vector<Edge> edges; //从当前node点出发的边都记在edges里
	
	Node(int value){
		this.value = value;
		this.in = 0;
		this.out = 0;
		this.nexts = new ArrayList<>();
		this.edges = new ArrayList<>();
	}
}

//边结构
public class Edges {
	public int value;
	public Node from;
	public Node to;

	public 	Edge(int weight, Node from, Node to){
		this.value = weight;
		this.from = from;
		this.to = to;
	}
}

//转换接口例子：用一个二维数组来表示图，格式为: 权值 = matrix[0][0]  from点 = matrix[0][1]  to点 = matrix[0][2]
public class Graph createGraph(Integer[][] matrix){
	Graph graph = new Graph();
	for(int i = 0; i < matrix.length; i++){
		//先从数组中获得form点、to点、权值的值
		Integer weight = matrix[i][0];
		Integer from = matrix[i][1];
		Integer to = matrix[i][2];

		//将from点和to点加到图里
		if(!graph.nodes.containskey(from)){
			graph.nodes.put(from,new Node(from));
		}
		if(!graph.nodes.containsket(from)){
			graph.nodes.put(to,new Node(to));
		}
		//获取from点和to点
		Node fromNode = graph.nodes.get(from);
		Node toNode = graph.nodes.get(to);
		//form点、to点和权重组成一条边
		Edge newEdge = new Edge(weight,fromNode,toNode);
		//from点的邻接点集加入to点
		fromNode.nexts.add(toNode);
		//from点出度加一
		formNode.out++;
		//to点出度加一
		toNode.in++;
		//将这条边加入form点属于的边集里
		formNode.edges.add(newEdge);
		//将这条边加入图的边集里
		graph.edges.add(newEdge);
	}
	return graph;
}
		
