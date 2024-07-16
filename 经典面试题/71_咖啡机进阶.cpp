/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 16:58:59
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

using namespace std;

/*
给定一个数组，比如arr[3,2,1,7,2]代表每个咖啡机冲好一杯咖啡的时间。每个咖啡机只能串行工作。
假设有m=100个人，都在0号时间点开始排队，让所有人都得到咖啡的时间尽可能的早。

返回每个人获得咖啡的时间点数组。
*/

/*

思路：
arr=[3,1,7] m=100
使用一个小根堆，堆中的元素是一个node{咖啡机开始时间，需要的工作时间}，两个元素的和谁小谁在前面
{
    {0,1},
    {0,3},
    {0,7},
}

1. 0号人，从堆中拿到最上面的node{0,1}，开始工作，这个咖啡机下次开始工作的时间是1，所以封装成{1,1}放到堆里。堆变成了{{1,1},{0,3},{0,7}}
2. 1号人，从堆中拿到最上面的node{1,1}，开始工作，这个咖啡机下次开始工作的时间是2，所以封装成{2,1}放在堆里，堆变成{{2,1},{0,3},{0,7}}
3. 2号人，从堆中拿到最上面的node{2,1}，开始工作，这个咖啡机下次开始工作的时间是3，所以封装成{3,1}放在堆里，堆变成{{0,3},{3,1},{0,7}}
4. 3号人，从堆中拿到最上面的node{0,3}，开始工作，这个咖啡机下次开始工作的时间3，所以封装成{3,3}放在堆里，堆变成{{3,1},{3,3},{0,7}}
5. 4号人，从堆中拿到最上面的node{3,1}，开始工作，这个咖啡机下次开始工作的时间4，所以封装成{4,1}放在堆里，堆变成{{4,1},{3,3},{0,7}}
...
...
*/

class Node {
public:
    int start;
    int work_time;
    Node(int _s, int wt) {
        start = _s;
        work_time = wt;
    }
};
class Solution
{
public:
    vector<int> get_coffee(vector<int> &arr, int m) {
        std::function<bool(Node*, Node*)> comp_by_value = [](const Node *p1, const Node *p2)
        {
			return (p1->start+p1->work_time) > (p2->start+p2->work_time);
        };
        std::priority_queue<Node *, std::vector<Node *>, std::function<bool(Node *, Node *)>> _min_priority_queue{comp_by_value};

        int len  = arr.size();
        //将每个咖啡机放进小根堆
        for (int i=0;i<len;i++) {
            _min_priority_queue.push(new Node(0, arr[i]));
        }

        vector<int> res(m,0);
        for (int i=0;i<m;i++) {
            Node* node = _min_priority_queue.top();
            _min_priority_queue.pop();
            res[i] = node->start + node->work_time;
            node->start += node->work_time; //将工作时间累加到当前节点的开始时间上
            _min_priority_queue.push(node);
        }

        return res;


    }
};

int main()
{

    Solution sol;

    // string s="12##3##";
    vector<int> nums = {3,1,7};
    int m=100;

    vector<int> res = sol.get_coffee(nums, m) ;
    for (auto iter:res) {
        cout << iter << " ";
    }
    cout << endl;

    return 0;
}