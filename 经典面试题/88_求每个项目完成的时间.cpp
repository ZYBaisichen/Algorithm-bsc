/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-10 14:30:15
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
#include <sstream>

using namespace std;

/*
// 项目有四个信息:
// 1)哪个项目经理提的
// 2)被项目经理润色出来的时间点
// 3)项目优先级
// 4)项目花费的时间
// 项目经理们可以提交项目给程序员们，程序员可以做这些项目。
// 比如长度为4的数组[1, 3, 2, 2]，表示1号项目经理提的，被项目经理润色出来的时间点是3，优先级2，花费程序员2个时间。
// 所以给一个N*4的矩阵，就可以代表N个项目。 给定一个正数pm，表示项目经理的数量，每个项目经理只负责自己的那些项目，
// 并且一次只能提交一个项目 给程序员们，这个提交的项目做完了，才能再次提交。
// 经理对项目越喜欢，就会越早提交。一个项目优先级越高越被喜欢;如果优先级一样，花费时间越少越喜欢;

// 如果还一样，被项目经理润色出来的时间点越早越喜欢。给定一个正数sde，表示程序员的数量，所有经理提交了的项目，
// 程序员会选自己喜欢的项目做，每个人做 完了一个项目，然后才会再来挑选。当程序员在挑选项目时，有自己的喜欢标准。
// 一个项目花费时间越少越被喜欢;如果花费时间一样，该项目的负责人编号越小越被喜欢。
// 返回一个长度为N的数组，表示N个项目的结束时间。

// 比如:
// int pms=2;
// int sde=2;
// int[][]programs={{1,1,1,2},{1,2,1,1},{1,3,2,2},{2,1,1,2},{2,3,5,5}};
// 返回:{3, 4, 5, 3, 9}

*/

/*
维护两类堆
多个项目经理堆，每个堆维护每个项目经理的项目，堆顶是这个项目经理最喜欢的项目。
程序员堆，堆顶表示程序员最喜欢的堆

两个方法，一个add方法，一个get方法。add将项目加到项目经理堆中，get方法将项目从程序员堆中取出来。
每次add项目时，可能会导致程序员堆中的结构发生变化，所以需要手动改程序员堆。
*/

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
        if (a->rank != b->rank) { //优先级越高pm越喜欢
            return a->rank < b->rank;
        } else if (a->cost != b->cost) { //花费越少越喜欢
            return a->cost > b->cost; //新插入b，cost越小越将其放在堆顶
        } else {
            return a->start > b->start; //越早提交越喜欢
        }
    }
};

struct SdeCompare
{ // 大跟堆
    bool operator()(const Program* a, const Program* b) const
    {
        if (a->cost != b->cost) { //花费越少越喜欢
            return a->cost > b->cost; //新插入b，cost越小越将其放在堆顶
        } else {
            return a->pm > b->pm; //pm编号越小越喜欢
        }
    }
};

class BigQueues {
    vector<priority_queue<Program*, vector<Program*>, PmCompare>> pm_heap;
    vector<Program*> sde_heap; //程序员堆
    vector<int> indexes; //程序员堆中编号为i的pm在数组中的位置，program_idx:idx
    int heap_size;//
    PmCompare pm_compare;
public:
    BigQueues(int pms, int sdes) {
        for (int i=0;i<=pms;i++) {
            pm_heap.push_back(priority_queue<Program*, vector<Program*>, PmCompare>(pm_compare));
        }
        sde_heap.resize(pms);//每个pms最多提一个最喜欢的
        indexes.resize(pms+1);
        for (int i=0;i<=pms;i++) {
            indexes[i] = -1;
        }
        heap_size = 0;
    }
    bool is_empty() {
        return heap_size == 0;
    }

    void print_heap() {
        cout << "pm_heap:" << endl;
        for (int i =0 ;i<pm_heap.size(); i++) {
            cout << "pm_heap idx:" << i << ":" ;
            auto tmp_que = pm_heap[i];//复制出来一个队列输出
            while (!tmp_que.empty()) {
                auto tmp = tmp_que.top();
                cout << tmp->pm << " " << tmp->start << " " << tmp->rank << " " << tmp->cost;
                tmp_que.pop();
            }
            cout << endl;
        }
        cout<< endl;

        cout << "sde_heap:" << endl;
        for (int i=0;i<heap_size;i++) {
            cout << "idx:" << i << ":" << sde_heap[i]->pm << " " << sde_heap[i]->start << " " << sde_heap[i]->rank << " " << sde_heap[i]->cost << endl;
        }
        cout << endl;

        cout << "pm indexes:" << endl;
        for (int i=0;i<indexes.size();i++) {
            cout << i << ":" << indexes[i] << endl;
        }
        cout << endl;
    }

    void add(Program* program) { //添加一个项目
        auto& pm_queue = pm_heap[program->pm];
        pm_queue.push(program);
        // cout << "[add]:" << program->pm <<endl;

        //下面将当前最喜欢的项目入程序员堆
        Program* cur_pm_top = pm_queue.top();
        int heap_idx = indexes[cur_pm_top->pm];
        // cout << "[add] pm:" << program->pm << " cur_pm_top->pm:" << cur_pm_top->pm << " heap_idx:" << heap_idx << endl;
        // cout << "[add] indexes:";
        // for (int i=0;i<indexes.size();i++) {
        //     cout << i << "," << indexes[i] << " ";
        // }
        // cout << endl;
        if (heap_idx == -1) { //还没有入过堆
            sde_heap[heap_size] = cur_pm_top;
            indexes[cur_pm_top->pm] = heap_size;
            heap_insert(heap_size++); //从heap_size开始做heap_insert，完事加1
        } else { //当前pm已经入过堆了
            sde_heap[heap_idx] = cur_pm_top;
            heap_insert(heap_idx);
            heapify(heap_idx);
        }
    }

    Program* pop() { //取出一个项目。注意弹出后要delete
        Program* head = sde_heap[0]; //取出在程序员看来优先级最高的项目来做
        auto& pm_queue = pm_heap[head->pm];
        // while (!pm_queue.empty()) {
        //     auto tmp = pm_queue.top();
        //     cout <<"pm:" <<  tmp->pm << endl;
        //     pm_queue.pop();
        // }
        // cout << "pop:" << sde_heap.size()  << " pm_heap_size:" << pm_heap.size() << " pm:" << head->pm << " pm_queue:" << pm_queue.size()<< endl;
        
        pm_queue.pop();
        if (pm_queue.empty()) { //如果当前pm的队列为空，则将这个pm的项目从程序员堆中删除
            swap(0, heap_size-1);
            Program* tmp = sde_heap[heap_size-1];
            sde_heap[--heap_size] = nullptr;
            indexes[tmp->pm] = -1;
        } else {
            // cout << "pop:" << sde_heap.size()  << " pm_heap_size:" << pm_heap.size() << " pm:" << head->pm << " pm_queue:" << pm_queue.size()<< endl;
            // cout << " pm_queue.top():" << pm_queue.top() << endl;
            sde_heap[0] = pm_queue.top(); //这个pm新的项目进来了
        }
        heapify(0); //新项目进来调整优先级
        return head;
    }

    void heap_insert(int index) {
        SdeCompare sde_cmp;
        while (index!=0) {
            int parent_idx = (index-1)/2;
            if (sde_cmp(sde_heap[parent_idx], sde_heap[index])) { //如果sde_heap[index]比父节点的优先级高，则滚上去
                swap(index, parent_idx);
                index = parent_idx;
            } else {
                break;
            }
        }
    }

    void heapify(int index) {
        SdeCompare sde_cmp;
        int left = index*2+1;
        int right = index*2+2;
        int best = index; //每次调整找到优先级最高的
        while (left < heap_size) {
            if (sde_cmp(sde_heap[best], sde_heap[left])) { //如果左孩子比当前节点的优先级高
                best = left;
            }
            if (right < heap_size && sde_cmp(sde_heap[best], sde_heap[right])) { //如果右孩子比当前节点的优先级高
                best = right;
            }

            if (best == index) {
                break;
            }
            swap(index, best);
            index= best;
            left = index*2+1;
            right = index*2+2;
        }
    }

    void swap(int a,int b) {
        Program* tmpa = sde_heap[a];
        Program* tmpb = sde_heap[b];
        sde_heap[a] = tmpb;
        sde_heap[b] = tmpa;
        //交换两个pm提的项目在堆中的位置
        indexes[tmpa->pm] = b;
        indexes[tmpb->pm] = a;
    }
};

struct StartCompare { // 大跟堆
    bool operator()(const Program* a, const Program* b) const{
        //润色时间越小，优先级越高
        return a->start > b->start;
    }
};

class Solution
{
public:
    //项目经理数、程序员数量、项目信息
    vector<int> work_finish(int pms, int sdes, vector<vector<int>>& programs) {
        StartCompare cmp_tmp;
        priority_queue<Program*, vector<Program*>, StartCompare> start_queue(cmp_tmp);
        int len = programs.size();
        //项目经理编号、润色出来的时间点、优先级、花费程序员时间
        for (int i=0;i<len;i++) {
            // cout << i << ": "<< programs[i][0] << ", " << programs[i][1] << ", " << programs[i][2] << ", " << programs[i][3] << endl;
            start_queue.push(new Program(i, programs[i][0], programs[i][1], programs[i][2], programs[i][3]));
        }
        // while (!start_queue.empty()) {
        //     Program* tmp = start_queue.top();
        //     start_queue.pop();
        //     cout << tmp->idx << ": "<< tmp->pm << ", " << tmp->start << ", " << tmp->rank << ", " << tmp->cost << endl;
        // }
        // return vector<int>();
        //所有的项目在最开始时都在programsQueue中被锁住

        // //程序员唤醒堆
        priority_queue<int, vector<int>, std::greater<int>> sde_que;
        for (int i=0;i<sdes;i++) { //初始时每个程序员的唤醒时间是1
            sde_que.push(1);
        }

        BigQueues big_que(pms, sdes);
        int finish=0;//任何一个项目做完finish都会加1，达到项目数的时候结束
        vector<int> ans(len);
        while (finish != len) {
            int sde_wake_time = sde_que.top();//世界时间线到达第一个员工醒来的时间
            sde_que.pop();
            while (!start_queue.empty()) {//如果在这个时间之前润色好的项目都加到bigqueue项目中去
                if (start_queue.top()->start > sde_wake_time) {
                    break;
                }
                auto tmp = start_queue.top();
                cout << "big_queue add:" << " sde_wake_time:" << sde_wake_time << " :"<< tmp->pm << ","<< tmp->start << "," << tmp->rank << "," << tmp->cost << endl;
                big_que.add(start_queue.top());
                start_queue.pop();
            }
            // big_que.print_heap();
            // break;

            if (big_que.is_empty()) {//员工醒的时候，没有项目被润色出来。即没有项目可以做。则将这个程序员的唤醒时间放置到第一个项目开始的时候
                auto tmp = start_queue.top();
                cout << "big_queue get_next start:" << " sde_wake_time:" << sde_wake_time << " :" << tmp->pm << "," << tmp->start << "," << tmp->rank << "," << tmp->cost << endl;
                sde_que.push(start_queue.top()->start);
            } else {
                Program* program = big_que.pop(); //拿出来一个项目做
                cout << "big_queue process program:" << " sde_wake_time:" << sde_wake_time << " :" << program->idx << ", " << program->pm << ", " << program->start << ", " << program->rank << ", " << program->cost << endl;
                ans[program->idx] = sde_wake_time + program->cost; //程序员醒来开始做的时间，加上花费的时间，就是该项目结束的时间
                sde_que.push(ans[program->idx]); //这个程序员醒来的时间将是结束时间
                finish++;
            }
        }
        return ans;
    }
};

void print_array(vector<int> arr){
    for (auto i : arr) {
        cout << i << " ";
    }
    cout << endl;
}



int main()
{
    Solution sol;
    int pms = 2;
    int sde = 2;
    // 项目经理编号、润色出来的时间点、优先级、花费程序员时间
    vector<vector<int>> programs = { 
        { 1, 1, 1, 3 }, 
        { 1, 2, 1, 1 }, 
        { 1, 3, 2, 2 }, 
        // { 2, 1, 1, 2 }, 
        { 2, 3, 5, 5 } 
        };
    vector<int> ans = sol.work_finish(pms, sde, programs);
    print_array(ans);
    return 0;
}