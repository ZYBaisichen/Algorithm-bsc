/*
 * @Author: baisichen
 * @Date: 2023-12-17 23:03:16
 * @LastEditTime: 2024-05-09 15:41:06
 * @LastEditors: baisichen baisichen@baidu.com
 * @Description: 
 */
// 参考：https://en.cppreference.com/w/cpp/container/priority_queue
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

struct Program {
    int idx; //项目在数组中的位置
    int pm; //pm吧建好
    int start; //开始时间
    int rank;//优先级
    int cost; //项目花费

    Program(int _idx,int _pm,int _start, int _rank, int _cost) {
        idx = _idx;
        pm = _pm;
        start = _start;
        rank = _rank;
        cost = _cost;
    }
};

struct PmCompare
{ // 大跟堆
    bool operator()(const Program* a, const Program* b) const
    {
		//调用一次，输出a是什么，b是什么
		cout << "a:" << a->idx << " a->rank:" << a->rank << " b:" << b->idx << " b->rank:" << b->rank << endl;
		//a是堆顶元素，b是后插入的，如果返回true代表后插入的要比堆顶优先级高，越靠近堆顶
		//比如return的是a->rank <= b->rank;，如果是true代表b的rank值高时，将b放在堆顶
		//return的是a->rank>b->rank;时，如果为true代表a的rank值高，即堆顶元素的rank值大于b的rank值时，才会将b放在堆顶。
        return a->rank <= b->rank;//rank越高的元素优先级越高
    }
};


int
main()
{
	priority_queue<Program*, vector<Program*>, PmCompare> pm_heap;
	cout << "第一次插入:" << endl;
	pm_heap.push(new Program(1, 0, 0, 2, 3)); //优先级是2
	cout << "第二次插入:" << endl;
	pm_heap.push(new Program(2, 0, 0, 3, 3)); //优先级是3
	cout << "第三次插入:" << endl;
	pm_heap.push(new Program(3, 0, 0, 4, 3)); //优先级是3
	/*
	运行到这里，输出结果如下：
	第一次插入:
	第二次插入:
	a:1 a->rank:2 b:2 b->rank:3
	第三次插入:
	a:2 a->rank:3 b:3 b->rank:4

	可以看出，第一个元素插入时不会调用比较函数。
	第二个元素插入时，比较函数的a是堆顶元素(rank=2)，b是后插入的元素（rank=3），当b的rank值较大时，返回true，并叫b放在堆顶
	第三个元素插入时，比较函数的a是堆顶元素(rank=3)，b是后插入的元素（rank=4），当b的rank值较大时，返回true，并叫b放在堆顶
	
	最终出来的堆数据的rank是4,3,2
	*/
	
	//输出堆
	cout << "final heap:" << endl;
	while (!pm_heap.empty()) {
		auto tmp = pm_heap.top();
		cout << "idx:" << tmp->idx << " rank:" << tmp->rank << endl;
		delete tmp;
		pm_heap.pop();
	}
	return 0;
}