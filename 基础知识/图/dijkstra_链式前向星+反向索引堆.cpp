// Dijkstra算法模版（Leetcode）
// 网络延迟时间
// 有 n 个网络节点，标记为 1 到 n
// 给你一个列表 times，表示信号经过 有向 边的传递时间
// times[i] = (ui, vi, wi)，表示从ui到vi传递信号的时间是wi
// 现在，从某个节点 s 发出一个信号
// 需要多久才能使所有节点都收到信号
// 如果不能使所有节点收到信号，返回 -1
// 测试链接 : https://leetcode.cn/problems/network-delay-time
/*
有 n 个网络节点，标记为 1 到 n。

给你一个列表 times，表示信号经过 有向 边的传递时间。 times[i] = (ui, vi, wi)，其中 ui 是源节点，vi 是目标节点， wi 是一个信号从源节点传递到目标节点的时间。

现在，从某个节点 K 发出一个信号。需要多久才能使所有节点都收到信号？如果不能使所有节点收到信号，返回 -1 。

 

示例 1：



输入：times = [[2,1,1],[2,3,1],[3,4,1]], n = 4, k = 2
输出：2
示例 2：

输入：times = [[1,2,1]], n = 2, k = 1
输出：1
示例 3：

输入：times = [[1,2,1]], n = 2, k = 2
输出：-1
 

提示：

1 <= k <= n <= 100
1 <= times.length <= 6000
times[i].length == 3
1 <= ui, vi <= n
ui != vi
0 <= wi <= 100
所有 (ui, vi) 对都 互不相同（即，不含重复边）
*/

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
#include <climits>
using namespace std;

/*
//纯净代码

class NodeHeap {
public:
	vector<int> nodes;
	//记录node在堆上的位置， 使用节点编号代表该节点
	map<int, int> node_index;
    map<int, int> distance_map;
	int size;


	NodeHeap(int _size) {
		size = _size;
		nodes.resize(size+1, -1);
        node_index.clear();
        distance_map.clear();
        size=0; //当前元素为0
	}
    bool is_empty() {
        return size==0;
    }

    int get_distance(int node) {
        if (distance_map.find(node) != distance_map.end()) {
            return distance_map[node];
        } else {
            return INT_MAX;
        }
    }
    
    void add_or_update_or_ignore(int node, int distance) {
        if (node_index.find(node) != node_index.end()) { //在堆上
			if (node_index[node] != -2) {
				distance_map[node] = std::min(distance_map[node], distance);
            	heapInsert(node_index[node]); //从当前节点开始做heapinsert
			}
        }  else {
			nodes[size] = node; //入堆
            node_index[node] = size++;
            distance_map[node] = distance;
            heapInsert(node_index[node]); //从当前节点开始做heapinsert
        }
    }

    void heapInsert(int cur) {
        //按照距离目标位置距离越小的越在上面
        while(distance_map[nodes[cur]] < distance_map[nodes[(cur-1)/2]]) {

            _swap(cur, (cur - 1) / 2);
            cur = (cur-1)/2;
        }
    }
    //交换a和b位置的两个数
    void _swap(int a, int b) {
        node_index[nodes[a]] = b;
        node_index[nodes[b]] = a;
        swap(nodes[a], nodes[b]);
    }

    int pop() {
        int res = nodes[0];
        _swap(0, --size); //size始终指向的是堆的最后一个元素的后面一个位置
        heapify(0);
		node_index[res] = -2; //标记下次不会再入进来
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
            _swap(lagest, cur);
            cur = lagest;
            l = cur*2 + 1;
        }
    }

};
const int maxn = 101;
const int maxm = 6001; // 边集最大6001
class dijkstra {
public:
    struct Edge {
        int to, w, next;
    }edge[maxm];
    int head[maxn];

    int cnt;//记录当前到了哪条边
    void init() {
        for (int i=0;i<maxn;++i) {
            head[i]=-1; //head初始都为-1
        }
        cnt = 0;
    }
    //新增边；u是节点编号；v是结束节点；w是边的权重
    void add_edge(int u, int v, int w) { 
        edge[cnt].to = v;
        edge[cnt].w = w;
        edge[cnt].next = head[u]; //头插法，将edge边放到开头，然后开头的节点指向自己
        head[u] = cnt++;
    }
    map<int, int> process(vector<vector<int>>& times, int n, int k) {
        map<int, int> resMap;
        //建立链式前向星结构
        init();
        for (int i=0;i<times.size();++i) {
            add_edge(times[i][0], times[i][1], times[i][2]);
        }
        NodeHeap heap(n);
        heap.add_or_update_or_ignore(k, 0); //k到k的距离是0
        while (!heap.is_empty()) {
            int u = heap.pop();
            int distance = heap.get_distance(u);
            for (int ei = head[u]; ei >= 0; ei = edge[ei].next) { //遍历u开头的每一条边
                int cal_to_node_dis = distance + edge[ei].w;
                heap.add_or_update_or_ignore(edge[ei].to, cal_to_node_dis);
            }
        }
        return heap.distance_map;
    }
};

class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        dijkstra a;
        map<int, int> dis_map = a.process(times, n, k);// n^2
        // map<Node*, int> dis_map = a.process(&graph, graph.nodes[k], n); //使用堆加速n*log(n) 
        int max_res = INT_MIN;
        if (dis_map.size() != n) {
            return -1;
        }
        for (auto& iter:dis_map) {
            max_res = std::max(iter.second, max_res);
        }
        return max_res;
    }
};
*/



class NodeHeap {
public:
	vector<int> nodes;
	//记录node在堆上的位置， 使用节点编号代表该节点
	map<int, int> node_index;
    map<int, int> distance_map;
	int size;


	NodeHeap(int _size) {
		size = _size;
		nodes.resize(size+1, -1);
        node_index.clear();
        distance_map.clear();
        size=0; //当前元素为0
	}
    bool is_empty() {
        return size==0;
    }

    int get_distance(int node) {
        if (distance_map.find(node) != distance_map.end()) {
            return distance_map[node];
        } else {
            return INT_MAX;
        }
    }
    void print_heap() {
        cout << " nodes:";
        for (int i = 0; i < nodes.size(); ++i)
        {
            cout << nodes[i] << " ";
        }
        cout << endl;
        cout << "node idx:";
        for (auto it = node_index.begin(); it != node_index.end(); ++it)
        {
            cout << it->first << ":" << it->second << " ";
        }
        cout << endl;
        cout << "dis map:";
        for (auto it = distance_map.begin(); it != distance_map.end(); ++it) {
            cout << it->first << ":" << it->second << " ";
        }
        cout << endl;
    }
    
    void add_or_update_or_ignore(int node, int distance) {
        if (node_index.find(node) != node_index.end()) { //在堆上
			cout << "in to_node: " << node << endl;
			if (node_index[node] != -2) {
				distance_map[node] = std::min(distance_map[node], distance);
                cout << "-------begin heapinsert:" << "node:" << node << " idx:"<<node_index[node]  << " size:" << size << endl;
                cout << "before_heap:";
                print_heap();
            	heapInsert(node_index[node]); //从当前节点开始做heapinsert
                cout << "after_heap:";
                print_heap();
                cout << "-------begin heapinsert end:" << endl;
			}
        }  else {
			cout << "not in to_node: " << node << endl;
			nodes[size] = node; //入堆
            node_index[node] = size++;
            distance_map[node] = distance;
            heapInsert(node_index[node]); //从当前节点开始做heapinsert
        }
    }

    void heapInsert(int cur) {
        //按照距离目标位置距离越小的越在上面
        while(distance_map[nodes[cur]] < distance_map[nodes[(cur-1)/2]]) {

            _swap(cur, (cur - 1) / 2);
            cur = (cur-1)/2;
        }
    }
    //交换a和b位置的两个数
    void _swap(int a, int b) {
        node_index[nodes[a]] = b;
        node_index[nodes[b]] = a;
        swap(nodes[a], nodes[b]);
    }

    int pop() {
        int res = nodes[0];
        _swap(0, --size); //size始终指向的是堆的最后一个元素的后面一个位置
        heapify(0);
		node_index[res] = -2; //标记下次不会再入进来
        cout << "######"<< "after pop:" << " size:" << size  << "  :";
        print_heap();
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
            _swap(lagest, cur);
            cur = lagest;
            l = cur*2 + 1;
        }
    }

};
const int maxn = 101;
const int maxm = 6001; // 边集最大6001
class dijkstra {
public:
    struct Edge {
        int to, w, next;
    }edge[maxm];
    int head[maxn];

    int cnt;//记录当前到了哪条边
    void init() {
        for (int i=0;i<maxn;++i) {
            head[i]=-1; //head初始都为-1
        }
        cnt = 0;
    }
    //新增边；u是节点编号；v是结束节点；w是边的权重
    void add_edge(int u, int v, int w) { 
        edge[cnt].to = v;
        edge[cnt].w = w;
        edge[cnt].next = head[u]; //头插法，将edge边放到开头，然后开头的节点指向自己
        head[u] = cnt++;
    }
    map<int, int> process(vector<vector<int>>& times, int n, int k) {
        map<int, int> resMap;
        //建立链式前向星结构
        init();
        for (int i=0;i<times.size();++i) {
            add_edge(times[i][0], times[i][1], times[i][2]);
        }
        for (int i=0;i<=n;i++) {
            for (int j=head[i];j!=-1;j=edge[j].next) {
                cout << "j:" << j << " start=" << i << " to=" << edge[j].to << " w=" << edge[j].w  << endl;
            }
        }
        NodeHeap heap(n);
        heap.add_or_update_or_ignore(k, 0); //k到k的距离是0
        cout << "init_heap:";
        heap.print_heap();
        while (!heap.is_empty()) {
            int u = heap.pop();
            int distance = heap.get_distance(u);
            for (int ei = head[u]; ei >= 0; ei = edge[ei].next) { //遍历u开头的每一条边
                int cal_to_node_dis = distance + edge[ei].w;
                cout << "===u:" << u  <<" to:" << edge[ei].to << " cal_to_node_dis:"<< cal_to_node_dis <<  " to_u_dis:"<< distance << " ei:"<< ei << " edge[ei]:" << edge[ei].w << endl;;
                heap.add_or_update_or_ignore(edge[ei].to, cal_to_node_dis);
            }
            cout <<  "cur u:" << u << " heap:"; 
            heap.print_heap();
        }
        return heap.distance_map;
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
        dijkstra a;
        map<int, int> dis_map = a.process(times, n, k);// n^2
        // map<Node*, int> dis_map = a.process(&graph, graph.nodes[k], n); //使用堆加速n*log(n) 
        int max_res = INT_MIN;
        cout << endl << "res:";
        for (auto& iter:dis_map) {
        cout << iter.first << ":"  << iter.second << endl;
        }
        if (dis_map.size() != n) {
            return -1;
        }
        for (auto& iter:dis_map) {
            max_res = std::max(iter.second, max_res);
        }
        return max_res;
    }
};

int main() {
    Solution sol;
    // vector<vector<int>> times = {
    //     {2,1,1},
    //     {2,3,1},
    //     {3,4,1}
    // };
    //答案69
    vector<vector<int>> times = {
        {{2,4,10},
        {5,2,38},
        {3,4,33},
        {4,2,76},
        {3,2,64},
        {1,5,54},
        {1,4,98},
        {2,3,61},
        {2,1,0},
        {3,5,77},
        {5,1,34},
        {3,1,79},
        {5,3,2},
        {1,2,59},
        {4,3,46},
        {5,4,44},
        {2,5,89},
        {4,5,21},
        {1,3,86},
        {4,1,95}}
    };
    int n = 5, k = 1;
    cout << sol.networkDelayTime(times, n, k) << endl;
}