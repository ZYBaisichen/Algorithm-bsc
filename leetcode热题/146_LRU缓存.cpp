/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
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
#include <unordered_map>

using namespace std;

/*
https://leetcode.cn/problems/linked-list-cycle-ii/

和经典面试题30题一样

请你设计并实现一个满足  LRU (最近最少使用) 缓存 约束的数据结构。
实现 LRUCache 类：
LRUCache(int capacity) 以 正整数 作为容量 capacity 初始化 LRU 缓存
int get(int key) 如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1 。
void put(int key, int value) 如果关键字 key 已经存在，则变更其数据值 value ；如果不存在，则向缓存中插入该组 key-value 。如果插入操作导致关键字数量超过 capacity ，则应该 逐出 最久未使用的关键字。
函数 get 和 put 必须以 O(1) 的平均时间复杂度运行。

示例：

输入
["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
[[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
输出
[null, null, null, 1, null, -1, null, -1, 3, 4]

解释
LRUCache lRUCache = new LRUCache(2);
lRUCache.put(1, 1); // 缓存是 {1=1}
lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}
lRUCache.get(1);    // 返回 1
lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
lRUCache.get(2);    // 返回 -1 (未找到)
lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
lRUCache.get(1);    // 返回 -1 (未找到)
lRUCache.get(3);    // 返回 3
lRUCache.get(4);    // 返回 4
 

提示：

1 <= capacity <= 3000
0 <= key <= 10000
0 <= value <= 105
最多调用 2 * 105 次 get 和 put

*/

/*
双向链表+hashmap结构
*/

struct Node{
    Node* next;
    Node* last;
    int key;
    int value;
    Node() {
        next = nullptr;
        last = nullptr;
        key = 0;
        value = 0; 
    }
     Node(int k, int v) {
        key = k;
        value = v;
        next = nullptr;
        last = nullptr;
     }
};
class DoubleList {
public:
    Node* head;
    Node* tail;
    DoubleList() {
        head = nullptr;
        tail = nullptr;
    }

    void add_node(Node* node) {
        if (node == nullptr) {
            return;
        }

        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->last = tail;
            tail = node;
        }
    }

    
    //从链表中移除头结点，返回头结点的指针
    Node* remove_head() {
        Node* ans = head;
        //head头是最远未使用的
        if (head == tail) {
            head = nullptr;
            tail = nullptr;
        } else {
            head = ans->next;
            ans->next = nullptr;
            head->last = nullptr;
        }
        return ans;
    }


    //在get命中或者put命中的时候，将某个节点移动到最后
    void move_node_to_tail(Node* node) {
        if (head == tail) {
            return;
        }
        if (node == tail) {
            return;
        }

        //从链表中拿出来
        if (node == head) {
            head = head->next;
            head->last=nullptr;
        } else {
            node->last->next = node->next;
            node->next->last = node->last;
        }

        node->last = tail;
        tail->next = node;
        node->next = nullptr;
        tail = node;
    }

    void print() {
        cout << "cur_list:";
        Node* cur = head;
        while (cur) {
            cout << cur->value << " ";
            cur = cur->next;
        }
        cout << endl;
    }
};

class LRUCache {
public:
    int _capacity;
    map<int,Node*> _key_node_map;
    DoubleList dl;
    LRUCache(int capacity) {
        _capacity = capacity;
        _key_node_map.clear();
    }
    
    int get(int key) {
        // cout << "get:" << key << endl;
        if (_key_node_map.find(key) != _key_node_map.end()) {
            Node* ans = _key_node_map[key];
            dl.move_node_to_tail(ans);
            // cout << "after get:";
            // dl.print();
            return ans->value;
        }
        return -1;
    }
    
    void put(int key, int value) {
        // cout << "put:" << key <<"," << value << " cap:" << _capacity << endl;
        if (_key_node_map.find(key) != _key_node_map.end()) {
            Node* tmp = _key_node_map[key];
            tmp->value = value;
            dl.move_node_to_tail(tmp);
        } else {
            if (_key_node_map.size() == _capacity) {
                auto* head = dl.remove_head();
                _key_node_map.erase(head->key);
                delete head;
            }
            _key_node_map[key] = new Node(key, value);
            dl.add_node(_key_node_map[key]);
        }
        // for (auto it:_key_node_map) {
        //     cout << "key:"  << it.first;
        //     cout << " value:" << (it.second?it.second->value:-1) << endl;
        // }
        // dl.print();
    }
};

int main()
{

    Solution sol;
    return 0;
}
