// Dijkstra算法模版（Leetcode）
// 网络延迟时间
// 有 n 个网络节点，标记为 1 到 n
// 给你一个列表 times，表示信号经过 有向 边的传递时间
// times[i] = (ui, vi, wi)，表示从ui到vi传递信号的时间是wi
// 现在，从某个节点 s 发出一个信号
// 需要多久才能使所有节点都收到信号
// 如果不能使所有节点收到信号，返回 -1
// 测试链接 : https://leetcode.cn/problems/network-delay-time

#include "graph.h"
#include<set>
#include<map>
using namespace std;
class NodeHeap {
public:
	vector<Node*> nodes;
	//记录node在堆上的位置
	map<Node*, int> node_index;
    map<Node*, int> distance_map;
	int size;


	NodeHeap(int _size) {
		size = _size;
		nodes.resize(size+1, nullptr);
        size=0; //当前元素为0
	}
    bool is_empty() {
        return size==0;
    }

    int get_distance(Node* node) {
        return distance_map[node];
    }
    
    void add_or_update_or_ignore(Node* node, int distance) {
        if (node_index.find(node) != node_index.end()) { //在堆上
			cout << "in to_node: " << node->value << endl;
			if (node_index[node] != -2) {
				distance_map[node] = std::min(distance_map[node], distance);
            	heapInsert(node_index[node]); //从当前节点开始做heapinsert
			}
        }  else {
			cout << "not in to_node: " << node->value << endl;
			nodes[size] = node; //入堆
            node_index[node] = size++;
            distance_map[node] = distance;
            heapInsert(node_index[node]); //从当前节点开始做heapinsert
        }
    }

    void heapInsert(int cur) {
        while(distance_map[nodes[cur]] < distance_map[nodes[(cur-1)/2]]) {
            swap(cur, (cur-1)/2);
            cur = (cur-1)/2;
        }
    }
    void swap(int a, int b) {
        node_index[nodes[a]] = b;
        node_index[nodes[b]] = a;

        Node* tmp = nodes[a];
        nodes[a] = nodes[b];
        nodes[b] = tmp;
    }

    Node* pop() {
        Node* res = nodes[0];
        swap(0, --size); //size始终指向的是堆的最后一个元素的后面一个位置
        heapify(0);
		node_index[res] = -2;
		return res;
	}

    void heapify(int cur) {
        int l = cur*2 + 1;
        while (l<size) {
            int lagest = l+1 < size && distance_map[nodes[l+1]] > distance_map[nodes[l]] ? l+1 : l;
            lagest = distance_map[nodes[lagest]] >  distance_map[nodes[cur]] ? lagest:cur;
            if (lagest == cur) { //不需要调整了
                break;
            }
            swap(lagest, cur);
            cur = lagest;
            l = cur*2 + 1;
        }
    }

};
class dijkstra {
public:
    //反向证明证明dijkstra的正确性：
    // 假设a到d的直接距离是100，假设存在一条路径，经过其他节点到d更短，根据dijkstra算法，每次都从最短路径开始更新，肯定不会遗漏掉
    map<Node*, int> process(Graph* graph, Node* start_node) {
        map<Node*, int> distanceMap;
        distanceMap[start_node] = 0; //距离当前节点距离为0
        set<Node*> selectedNodes;
		Node* curMinNode = getMinNode(distanceMap, selectedNodes); //每次挑选一个距离最近的节点
        while(curMinNode) {
            int edge_len = curMinNode->edges.size();
            int distance = distanceMap[curMinNode];
            for (int i=0;i<edge_len;i++) {
                Node* to_node = curMinNode->edges[i]->to;
                //不存在到to_node节点的路径，则发现了一条路径
                if (distanceMap.find(to_node) == distanceMap.end()) {
                    distanceMap[to_node] = distance + curMinNode->edges[i]->value;
                } else {
                    //经过当前curMinNode节点到达to_node，如果距离更短，则更新。
                    distanceMap[to_node] = std::min(distanceMap[to_node], distance + curMinNode->edges[i]->value);
                }
            }
            selectedNodes.insert(curMinNode);
            curMinNode = getMinNode(distanceMap, selectedNodes); //每次挑选一个距离最近的节点
        }
        return distanceMap;
    
    }
    Node* getMinNode(map<Node*, int>& disMap, set<Node*>& selectedNodes) {
        Node* minNode = NULL;
        int minId = 0;
		int min_value = INT_MAX;
		for (auto itr : disMap) {
            if (selectedNodes.find(itr.first) == selectedNodes.end() && itr.second < min_value) {
                minNode = itr.first;
                min_value = itr.second;
            }
        }
        return minNode;
    }

    map<Node*, int> processHeap(Graph* graph, Node* start_node, int n) {
        map<Node*, int> resMap;
        NodeHeap heap(n);
        heap.add_or_update_or_ignore(start_node, 0);
        while (!heap.is_empty()) {
            Node* curMinNode = heap.pop();
            int distance = heap.get_distance(curMinNode);
            int edge_len = curMinNode->edges.size();
            for (int i=0;i<edge_len;i++) {
                Node* to_node = curMinNode->edges[i]->to;
                int cal_to_node_dis = distance + curMinNode->edges[i]->value;
                heap.add_or_update_or_ignore(to_node, cal_to_node_dis);
            }
            resMap[curMinNode] = distance;
        }
        return resMap;
    
    }
};

/*
输入：times = [[2,1,1],[2,3,1],[3,4,1]], n = 4, k = 2
输出：2
求max
*/
class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        Graph graph;
        graph.createGraph(times);
        // for (auto& it:graph.nodes) {
        //     cout << it.second->value;
        // }
        dijkstra a;
        map<Node*, int> dis_map = a.process(&graph, graph.nodes[k]);// n^2
        // map<Node*, int> dis_map = a.process(&graph, graph.nodes[k], n); //使用堆加速n*log(n) 
        int max_res = INT_MIN;
        int count = 0;
        for (auto& iter:dis_map) {
            // cout << iter.first << endl;
            count++;
            max_res = std::max(iter.second, max_res);
        }
        if (count!=n) {
            return -1;
        }
        return max_res;
    }
};