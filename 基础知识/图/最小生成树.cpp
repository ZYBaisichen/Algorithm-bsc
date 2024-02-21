#include "graph.h"
/* java
public class Kruskal {
	public static class EdgeComparator implements Comparator<Edge>{
		@Override
		public int compare(Edge o1, Edge o2) {
			// TODO Auto-generated method stub
			return o1.weight-o2.weight;
		}
	}
	
	// Union-Find Set，并查集
	public static class UnionFind {
		// key 某一个节点， value key节点往上的节点
		private HashMap<Node, Node> fatherMap;
		// key 某一个集合的代表节点, value key所在集合的节点个数
		private HashMap<Node, Integer> sizeMap;

		public UnionFind() {
			fatherMap = new HashMap<Node, Node>();
			sizeMap = new HashMap<Node, Integer>();
		}
		
		public void makeSets(Collection<Node> nodes) {
			fatherMap.clear();
			sizeMap.clear();
			for (Node node : nodes) {
				fatherMap.put(node, node);
				sizeMap.put(node, 1);
			}
		}
		//找到这个集合的代表
		private Node findFather(Node n) {
			Stack<Node> path = new Stack<>();
			while(n != fatherMap.get(n)) {
				path.add(n);
				n = fatherMap.get(n);
			}
			while(!path.isEmpty()) {
				fatherMap.put(path.pop(), n);
			}
			return n;
		}
		
		//判断两个点是否在同一个集合中
		public boolean isSameSet(Node a, Node b) {
			return findFather(a) == findFather(b);
		}
		
		//将两个点并入同一个集合
		public void union(Node a, Node b) {
			if (a == null || b == null) {
				return;
			}
			Node aDai = findFather(a);
			Node bDai = findFather(b);
			if (aDai != bDai) {
				int aSetSize = sizeMap.get(aDai);
				int bSetSize = sizeMap.get(bDai);
				if (aSetSize <= bSetSize) {
					fatherMap.put(aDai, bDai);
					sizeMap.put(bDai, aSetSize + bSetSize);
					sizeMap.remove(aDai);
				} else {
					fatherMap.put(bDai, aDai);
					sizeMap.put(aDai, aSetSize + bSetSize);
					sizeMap.remove(bDai);
				}
			}
		}
	}
	
	
	//用并查集实现kruskal算法 
	public static Set<Edge> kruskalMST(Graph graph){
		UnionFind unionFind = new UnionFind();
		unionFind.makeSets(graph.nodes.values());
		// 从小的边到大的边，依次弹出，小根堆！
		PriorityQueue<Edge> priorityQueue = new PriorityQueue<>(new EdgeComparator());
		for (Edge edge : graph.edges) { // M 条边
			priorityQueue.add(edge);  // O(logM)
		}
		Set<Edge> result = new HashSet<>();
		while (!priorityQueue.isEmpty()) { // M 条边
			Edge edge = priorityQueue.poll(); // O(logM)
			if (!unionFind.isSameSet(edge.from, edge.to)) { // O(1)
				result.add(edge);
				unionFind.union(edge.from, edge.to);
			}
		}
		return result;
	}
}


public class Prim {
	
	//定义一个堆比较器，将边的权值从小到大排序
	public static class EdgeComparator implements Comparator<Edge>{
		@Override
		public int compare(Edge o1, Edge o2) {
			// TODO Auto-generated method stub
			return o1.weight-o2.weight;
		}
	}
	
	public static Set<Edge> primSet(Graph graph){
		PriorityQueue<Edge> priorityQueue=new PriorityQueue<>(new EdgeComparator());
		//解锁出来的Node
		HashSet<Node> nodeSet=new HashSet<>();
		//解锁出来的edge
		HashSet<Edge> edgeSet =new HashSet<>();
		
		ArrayList<Integer> list=new ArrayList<>();
		Set<Edge> result=new HashSet<>();	//依次挑选的边放在result
		for(Node node:graph.nodes.values()) {	//随便挑了一个点,此for循环防止出现森林
			//node是开始点
			if(!nodeSet.contains(node)) {
				nodeSet.add(node);
				for(Edge edge:node.edges) {	//由该点解锁所有相关的边
					if(!edgeSet.contains(edge)) {
						priorityQueue.add(edge);
						edgeSet.add(edge);
					}
				}
				while(!priorityQueue.isEmpty()) {
					Edge edge=priorityQueue.poll();	//弹出已解锁边中的最小边
					Node toNode=edge.to;	//可能的新一个节点
					if(!nodeSet.contains(toNode)) {
						nodeSet.add(toNode);
						result.add(edge);
					}
					for(Edge nextEdge:toNode.edges) {
						priorityQueue.add(nextEdge);
					}
				}
			}
			break;
		}
		return result;
	}
}
*/

//C++

class Kruskal {
public:
	// Union-Find Set，并查集
	class UnionFind {
		// key 某一个节点， value key节点往上的节点
		map<Node*, Node*> fatherMap;
		// key 某一个集合的代表节点, value key所在集合的节点个数
		map<Node*, int> sizeMap;

		public UnionFind() {
			fatherMap = new HashMap<Node, Node>();
			sizeMap = new HashMap<Node, Integer>();
		}
		
		public void makeSets(Collection<Node> nodes) {
			fatherMap.clear();
			sizeMap.clear();
			for (Node node : nodes) {
				fatherMap.put(node, node);
				sizeMap.put(node, 1);
			}
		}
		//找到这个集合的代表
		private Node findFather(Node n) {
			Stack<Node> path = new Stack<>();
			while(n != fatherMap.get(n)) {
				path.add(n);
				n = fatherMap.get(n);
			}
			while(!path.isEmpty()) {
				fatherMap.put(path.pop(), n);
			}
			return n;
		}
		
		//判断两个点是否在同一个集合中
		public boolean isSameSet(Node a, Node b) {
			return findFather(a) == findFather(b);
		}
		
		//将两个点并入同一个集合
		public void union(Node a, Node b) {
			if (a == null || b == null) {
				return;
			}
			Node aDai = findFather(a);
			Node bDai = findFather(b);
			if (aDai != bDai) {
				int aSetSize = sizeMap.get(aDai);
				int bSetSize = sizeMap.get(bDai);
				if (aSetSize <= bSetSize) {
					fatherMap.put(aDai, bDai);
					sizeMap.put(bDai, aSetSize + bSetSize);
					sizeMap.remove(aDai);
				} else {
					fatherMap.put(bDai, aDai);
					sizeMap.put(aDai, aSetSize + bSetSize);
					sizeMap.remove(bDai);
				}
			}
		}
	}
	
	
	//用并查集实现kruskal算法 
	public static Set<Edge> kruskalMST(Graph graph){
		UnionFind unionFind = new UnionFind();
		unionFind.makeSets(graph.nodes.values());
		// 从小的边到大的边，依次弹出，小根堆！
		PriorityQueue<Edge> priorityQueue = new PriorityQueue<>(new EdgeComparator());
		for (Edge edge : graph.edges) { // M 条边
			priorityQueue.add(edge);  // O(logM)
		}
		Set<Edge> result = new HashSet<>();
		while (!priorityQueue.isEmpty()) { // M 条边
			Edge edge = priorityQueue.poll(); // O(logM)
			if (!unionFind.isSameSet(edge.from, edge.to)) { // O(1)
				result.add(edge);
				unionFind.union(edge.from, edge.to);
			}
		}
		return result;
	}
}


class Prim
{
public:
	// 自定义仿函数, 按照边的权值从小到达排序
	struct CompareByValue
	{
		bool operator()(const Edge &p1, const Edge &p2) const
		{
			return p1.value > p2.value;
		}
	};

	static set<Edge *> primSet(Graph* graph)
	{
		cout << "asdf" << endl;
		std::function<bool(Edge*, Edge*)> comp_by_value = [](const Edge *p1, const Edge *p2)
		{
			return p1->value > p2->value;
		};
		std::priority_queue<Edge *, std::vector<Edge *>, std::function<bool(Edge *, Edge *)>> _min_priority_queue{comp_by_value};

		// 解锁出来的Node
		set<Node *> nodeSet;
		// 解锁出来的edge
		set<Edge *> edgeSet;

		set<Edge *> edgeQueueSet;

		set<Edge*> result; // 依次挑选的边放在result
		for (auto it = graph->nodes.begin(); it != graph->nodes.end(); ++it) {// 随便挑了一个点,此for循环防止出现森林
			auto node = it->second;
			// cout<< "node: " << node->value << endl;
			// node是开始点
			if (nodeSet.find(node) == nodeSet.end())
			{
				nodeSet.insert(node);
				for (auto edge : node->edges)
				{ // 由该点解锁所有相关的边
					if (edgeQueueSet.find(edge) == edgeQueueSet.end())
					{
						_min_priority_queue.push(edge);
						edgeQueueSet.insert(edge);
					}
				}
				while (!_min_priority_queue.empty())
				{
					Edge *edge = _min_priority_queue.top(); // 弹出已解锁边中的最小边
					_min_priority_queue.pop();
					// cout << "edge: " << edge->from->value << " -> " << edge->to->value << ", weight: " << edge->value << endl;
					Node *toNode = edge->to;				// 可能的新一个节点
					if (nodeSet.find(toNode) == nodeSet.end())
					{
						nodeSet.insert(toNode);
						result.insert(edge);
						// cout << "push_res" << endl << "=======" << endl;
					}
					for (auto nextEdge : toNode->edges)
					{
						if (edgeQueueSet.find(nextEdge) == edgeQueueSet.end()) {
							// cout << "push queue, edge: " << nextEdge->from->value << " -> " << nextEdge->to->value << ", weight: " << nextEdge->value << endl;
							_min_priority_queue.push(nextEdge);
							edgeQueueSet.insert(nextEdge);
						}
					}
				}
			}
			// break;
		}
		return result;
	}
};


class Kruskal
{
public:
	// Union-Find Set，并查集
	class UnionFind
	{
	public:
		// key 某一个节点， value key节点往上的节点
		map<Node *, Node *> fatherMap;
		// key 某一个集合的代表节点, value key所在集合的节点个数
		map<Node *, int> sizeMap;

		UnionFind() {
		}
		void makeSets(map<int, Node *>& nodes)
		{
			fatherMap.clear();
			sizeMap.clear();
			for (auto it = nodes.begin(); it != nodes.end(); ++it) {
				auto node = it->second;
				fatherMap[node] = node;
				sizeMap[node] = 1;
			}
		}
		// 找到这个集合的代表
		Node* findFather(Node* n)
		{
			stack<Node *> path;
			while (n != fatherMap[n])
			{
				path.push(n);
				n = fatherMap[n];
			}
			while (!path.empty())
			{
				fatherMap[path.top()] = n;
				path.pop();
			}
			return n;
		}

		// 判断两个点是否在同一个集合中
		bool isSameSet(Node* a, Node* b) {
			return findFather(a) == findFather(b);
		}

		// 将两个点并入同一个集合
		void unionSet(Node* a, Node* b) {
			if (a == NULL || b == NULL) {
				return;
			}
			Node* aDai = findFather(a);
			Node* bDai = findFather(b);
			if (aDai != bDai) { //不在一个集和中
				int aSetSize = sizeMap[aDai];
				int bSetSize = sizeMap[bDai];
				if (aSetSize <= bSetSize)
				{
					fatherMap[aDai] = bDai;
					sizeMap[bDai] = aSetSize + bSetSize;
					sizeMap.erase(aDai);
				}
				else
				{
					fatherMap[bDai] = aDai;
					sizeMap[aDai] = aSetSize + bSetSize;
					sizeMap.erase(bDai);
				}
			}
		}
	};

	// 用并查集实现kruskal算法
	static set<Edge*> kruskalMST(Graph* graph) {
		UnionFind* unionFind = new UnionFind();
		unionFind->makeSets(graph->nodes);
		// 从小的边到大的边，依次弹出，小根堆！
		std::function<bool(Edge*, Edge*)> comp_by_value = [](const Edge *p1, const Edge *p2) {
			return p1->value > p2->value;
		};
		std::priority_queue<Edge *, std::vector<Edge *>, std::function<bool(Edge *, Edge *)>> _min_priority_queue{comp_by_value};

		for (auto edge : graph->edges)
		{							 // M 条边
			_min_priority_queue.push(edge); // O(logM)
		}
		set<Edge*> result;
		while (!_min_priority_queue.empty()) {									  // M 条边
			Edge* edge = _min_priority_queue.top(); // O(logM)
			// cout << "min_queue:" << edge->from->value << " -> " << edge->to->value << ", weight: " << edge->value << endl;
			_min_priority_queue.pop();
			if (!unionFind->isSameSet(edge->from, edge->to))
			{ // O(1)
				result.insert(edge);
				unionFind->unionSet(edge->from, edge->to);
			}
		}
		return result;
	}
};

//https://leetcode.cn/problems/min-cost-to-connect-all-points/
/*
输入：points = [[3,12],[-2,5],[-4,1]]
输出：18
*/
class Solution {
public:
	//无论K方法还是P方法，都需要将容器改为C风格数组。才能AC
	//K算法
    int minCostConnectPoints(vector<vector<int>>& points) {
		int M = points.size();
		if (M == 0) {
			return 0;
		}
		int N = points[0].size();
		if (N==0) {
			return 0;
		}
		Graph graph;
		graph.createGraphWithPoints(points);
		const set<Edge*>& min_tree = Kruskal::kruskalMST(&graph);
		int res = 0;
		for (auto edge : min_tree) {
			res += edge->value;
		}
		return res;

    }

	//P算法
    int minCostConnectPoints(vector<vector<int>>& points) {
		int M = points.size();
		if (M == 0) {
			return 0;
		}
		int N = points[0].size();
		if (N==0) {
			return 0;
		}
		Graph graph;
		graph.createGraphWithPoints(points);
		const set<Edge*>& min_tree = Prim::primSet(&graph);
		int res = 0;
		for (auto edge : min_tree) {
			res += edge->value;
		}
		return res;

    }
};

int main()
{
	
	Graph graph;
	vector<vector<int>> matrix = {
		{8, 0, 1},
		{8, 1, 0},
		{1, 0, 2},
		{1, 2, 0},
		{15, 0, 3},
		{15, 3, 0},
		{2, 1, 2},
		{2, 2, 1},
		{10, 1, 4},
		{10, 4, 1},
		{7, 2, 3},
		{7, 3, 2},
		{12, 2, 5},
		{12, 5, 2},
		{3, 2, 4},
		{3, 4, 2},
		{3, 3, 5},
		{3, 5, 3},
		{4, 4, 5},
		{4, 5, 4}
	};
	graph.createGraph(matrix);
	const set<Edge *>& result_P = Prim::primSet(&graph);
	for (auto edge : result_P)
	{
		cout << edge->from->value << " -> " << edge->to->value << ", weight: " << edge->value << endl;
	}

    cout << "=============" << endl;
	const set<Edge *> &result_K = Kruskal::kruskalMST(&graph);
	for (auto edge : result_K)
	{
		cout << edge->from->value << " -> " << edge->to->value << ", weight: " << edge->value << endl;
	}
	return 0;
}