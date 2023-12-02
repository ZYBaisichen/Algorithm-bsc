#include "graph.h"

// 1.利用队列实现
// 2.从源节点开始一次按照宽度进队，然后弹出
// 3.每弹出一个点，就把该节点所有的没有进过队列的直接邻接节点放进队列
// 4.一直弹出直到队列为空
void bfs(Node* head){
	if(head == NULL){
		return;
	}
	queue<Node*> _queue;
	set<Node*> node_sets;
    _queue.push(head);
    node_sets.insert(head);
    while(!_queue.empty()){
        Node *cur = _queue.front();
        cout << cur->value << ",";
		for(auto& next : cur->nexts){
			if(node_sets.find(next) == node_sets.end()) {
                _queue.push(next);
                node_sets.insert(next);
            }
		}
	}
}

// 1.利用栈实现
// 2.从源节点开始把节点按照深度方式放进栈中，打印，然后弹出
// 3.每弹出一个节点，把该节点下一个没有进过栈的邻接节点放进栈中，打印，弹出
// 4.重复上述过程，直到栈变空
void dfs(Node* head){
	if(head == NULL){
		return;
	}
	stack<Node*> stack;//记录的是深度优先遍历的路径，弹出就是回退到上一个节点遍历
	set<Node*> set;
	stack.push(head);
    set.insert(head);
    cout << head->value << ",";
	while(!stack.empty()){
		//弹出栈顶节点
		Node* cur = stack.top();
        stack.pop();
        //从栈顶节点的邻接节点集中寻找未遍历的next节点
		for(auto& next : cur->nexts){
			if(set.find(next) != set.end()){
				//找到后，先将刚刚弹出的节点重新压回栈中，再将next节点放进栈中
				//这样做到原因是每次一条路走到底之后，往回走时，检查是否还有没有走过的路径
				stack.push(cur);
				stack.push(next);
				//set里加入next节点，表示已遍历此节点
				set.insert(next);
				//next点加入后打印
				cout << next->value << ", ";
				//找到一个节点后就break，直接走下一个节点
				break;
			}
		}
	}
}
