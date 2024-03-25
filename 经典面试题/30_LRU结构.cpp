/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-24 18:52:38
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

// 实现LRU结构，put和get的操作需要O(1)的复杂度
// 测试链接 : https://leetcode.cn/problems/lru-cache/
class Node {
public:
    int key;
    int value;
    Node* last;
    Node* next;
    Node(int k, int v) {
        key =k;
        value = v;
        last=nullptr;
        next=nullptr;
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

    void add_node(Node* new_node) {
        if (new_node == nullptr) {
            return;
        }
        if (head == nullptr) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            new_node->last = tail;
            tail = new_node;
        }
    }

    //从头移除，返回头节点
    Node* remove_head() {
        if (head == nullptr) {
            return nullptr;
        }

        Node* ans = head;
        if(head == tail) {
            head = nullptr;
            tail = nullptr;
        } else {
            head = ans->next;
            ans->next = nullptr;
            head->last = nullptr;
        }
        return ans;
    }

    //将node移动到末尾，为最新操作的值
    void move_node_to_tail(Node* node) {
        if (node == tail) {
            return;
        }
        //从链表中剥离出来
        if (node == head) {
            head = head->next;
            head->last = nullptr;
        } else {
            node->next->last = node->last;
            node->last->next = node->next;
        }

        node->last = tail;
        node->next = nullptr;
        tail->next = node;
        tail = node;
    }
};

//在内存cache，cpu cache中应用广泛
class LRUCache {
public:
    DoubleList double_list;
    int capacity;
    map<int, Node*> key_node_map;
    LRUCache(int capacity) {
        this->capacity = capacity;
    }
    
    int get(int key) { //O(1)
        if (key_node_map.find(key) != key_node_map.end()) { //O(1)
            Node* ans = key_node_map[key];
            //每次有get操作，说明命中了cache，将其移动到最后。说明是最近使用过的
            double_list.move_node_to_tail(ans); //O(1)
            return ans->value;
        }
        return -1;
     }
    
    void put(int key, int value) { //O(1)
        if (key_node_map.find(key) != key_node_map.end()) {
            Node* ans = key_node_map[key]; 
            ans->value = value;
            double_list.move_node_to_tail(ans); //O(1)
        } else {
            if (key_node_map.size() == capacity) {
                Node* ans = double_list.remove_head(); //每次头都是最远没有使用过的节点 O(1)
                key_node_map.erase(ans->key);
            }
            Node* new_node = new Node(key, value);
            key_node_map[key] = new_node;
            double_list.add_node(new_node); //O(1)
        }
    }
};
/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

int main()
{
    Solution sol;

    int n = 10;
    int v = 3;
    int testTime = 100;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++) {
        int len1 = (int) (rand() % n) + 1;
        int len2 = (int) (rand() % n) + 1;
        string s1 = random_string(len1, v);
        string s2 = random_string(len2, v);
        // cout<< "s1: " << s1 << ",  s2:" << s2 << endl;
        int ans1 = sol.minDelete1(s1, s2);
        int ans2 = sol.minDelete2(s1, s2);
        int ans3 = sol.minDelete3(s1, s2);
        // cout << "ans1: " << ans1 << ",  ans2:" << ans2 << ",  ans3:" << ans3 << endl;
        if (ans1 != ans2) {
            cout << "出错了!" << endl;
        }
        if (ans1 != ans3) {
            cout << "出错了! ans3:"  << s1 <<  ", "<<s2<< endl;
        }

        if (ans2 != ans3) {
            cout << "出错了! ans3:"  << s1 <<  ", "<<s2<< endl;
        }
    }
    cout << "测试结束" << endl;


    // string s1= "bbbbc";
    // string s2 = "aa";
    // //输出6
    // cout << sol.minDelete1(s1,s2) << endl;
    // cout << sol.minDelete2(s1,s2) << endl;
    // cout << sol.minDelete3(s1, s2) << endl;
    return 0;
}