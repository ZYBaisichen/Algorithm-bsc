/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 14:20:10
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

using namespace std;

class Node
{
public:
    string str;
    int count;
    Node(string str, int count) : str(str), count(count) {}
};
class TopRecord {
public:
    int used_size; // 在当前已经使用的size的下一个
    int max_size;
    vector<Node *> arr;
    map<Node*, int> heap_index;
    map<string, Node*> str_node_map; //词频统计map
    int k;
    TopRecord(int _k): k(_k) {
        max_size = 102400;
        arr.resize(max_size);
        used_size = 0;
    }

    //小顶堆，淘汰
    void add(string str) {
        int pre_index = -1;
        Node * cur;
        cout << "add:" << str <<  " used size:" << used_size <<endl;
        if (str_node_map.find(str) == str_node_map.end()) {
            cur = new Node(str, 1);
            str_node_map[str] = cur;
            heap_index[cur] = -1;//先将其下标即为-1，表示还没入堆
        } else {
            // 已经存在，词频增加
            cur = str_node_map[str];
            cur->count++;
            pre_index = heap_index[cur];
        }

        cout << "pre_idnex:" << pre_index << endl;

        if (pre_index == -1) { //表示可能是新建的，或者是原来没有资格进堆的
            if (used_size == k) { //规定一个大小为k的堆
                if (cur->count > arr[0]->count) { //大于堆顶，替换掉
                    heap_index[cur] = 0;
                    heap_index[arr[0]] = -1; //将堆顶移出去
                    arr[0] = cur;
                    heapify(0, used_size); //从堆顶开始调整
                } //否则不做任何操作
            } else {
                heap_index[cur] = used_size;
                arr[used_size] = cur;
                heap_insert(used_size++); //向上调整
            }

        } else { //原来已经在堆上，只需要向下调整
            heapify(pre_index, used_size);
        }
    }

    vector<string> top() {
        vector<string> res;
        for (int i = used_size-1; i >=0; i--) {
            res.push_back(arr[i]->str);
            cout << arr[i]->str << " " << arr[i]->count << endl;
        }
        return res;
    }

    //从idx往下做调整，总大小为size
    void heapify(int idx, int size) {
        int child = (idx<<1)+1;
        cout << "child: " << child << " size:" << size<< endl;
        while (child < size) {
            // cout << "child: " << child << " child+1:" << child+1 <<" size:" << size << " (child+1)<size:" << ((child+1)<size) <<endl;
            int minest = child; // 找到最小的孩子
            if (child+1 < size) {
                if (arr[child+1]->count < arr[child]->count) {
                    minest = child+1;
                }
            }
            minest = arr[minest]->count < arr[idx]->count ? minest: idx;
            // cout << "minest：" << minest<< " : "<< arr[minest]->str << endl;
            if (minest != idx) {
                swap(minest, idx);
                idx = minest;
                child = (idx<<1) + 1;
            } else {
                break;
            }

        }        
    }

    //从idx开始往上做调整
    void heap_insert(int idx)
    {
        int parent  = (idx - 1) >> 2;
        int cur = idx;
        while (parent >= 0 && arr[parent]->count > arr[cur]->count) {
            swap(parent, cur);
            cur = parent;
            parent = (idx - 1) >> 2;
        }
    }

    void swap(int i, int j) {
        Node* tmp  = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        heap_index[arr[i]] = i;
        heap_index[arr[j]] = j;
    }
};
class Solution
{
public:
    // 给定一个字符串类型的数组arr， 求其中出现次数最多的前K个。
    /*
    实现如下结构:
    class TopRecord{
        TopRecord(int k);// k为对象内常数
        add(string str) //加入一个字符串，可以重复加入
        vetor<string> top(); //返回加入的所有字符串中，词频最大的k个
    }

    这就需要手写堆了，维护一个可以修改词频的大小为k的堆，每次add时更新词频，并决定是否需要淘汰堆顶元素
    */

    void top_k_str_num() {
        int k = 3;
        TopRecord tr(k);
        vector<string> arr = {"a", "b", "a", "c", "c", "c","c", "d", "d"};
        for (auto iter:arr) {
            tr.add(iter);
        }
        tr.top();
    }
};

int main()
{
    Solution sol;
    sol.top_k_str_num();
    return 0;
}