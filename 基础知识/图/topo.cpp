#include "graph.h"
//适用范围：要求有向无环图，且有入度为0的节点
//有向无环图中所以节点的线性序列满足下列要求:
// 1.每个节点出现且只出现一次
// 2.若存在一条A->B的边，则在排序序列中，节点A排在节点B前面
//算法步骤：
// 1.先找入度为0的点，加入result集
// 2.消除入度为0的点的影响（属于的边和点）
// 3.寻找下一个入度为0的点，重复以上过程
vector<Node*> sortedTopology(Graph graph){
	//用hashmap存节点和入度
	//key：代表某个节点  value：代表该节点剩余入度
	map<Node*,int> inMap;
	//存入度为0的节点的队列
	queue<Node*> zeroInQueue;
	//遍历图集中的点集，将点和其入度存进inMap
	for(auto& it : graph.nodes) {
        Node* cur = it.second;
        inMap[cur] = cur->in;
        //如果遇到入度为0的node，加入
		if(cur->in == 0){
            zeroInQueue.push(cur);
        }
	}
	//拓扑排序结果依次，存入result
	vector<Node*> result;
	while(!zeroInQueue.empty()){
		//弹出入度为空节点的队列的一个节点到result里
		Node* cur = zeroInQueue.front();
		result.push_back(cur);
		//遍历该节点的邻接节点集
		for(auto& next : cur->nexts){
			//消除cur节点的影响，即其next节点们的入度都要减一
			inMap[next]--;
			//如果next节点的入度更新为0，则加入队列
  			if(inMap[next] == 0){
				zeroInQueue.push(next);
			}
		}
		return result;
	}
}
