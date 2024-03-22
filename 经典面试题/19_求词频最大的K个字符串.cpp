/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 11:46:15
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
class Node{
public:
    string str;
    int count;
    Node(string str, int count):str(str),count(count){}
};
class Solution
{
public:
    // 给定一个字符串类型的数组arr， 求其中出现次数最多的前K个。

    vector<string> top_k_str_num(vector<string> arr, int k)
    {
        int len = arr.size();
        map<string, int> count_map;
        vector<string> res;
        if (len == 0)
        {
            return res;
        }

        // 一种方法是统计词频后，采用bfprt求第k大问题。然后输出来k前面的数，O(N)算法
        // 但这个问题是为了引出来20题，手动改写堆，所以这里先用小根堆实现O(N*logN)

        // 统计词频
        for (int i = 0; i < len; i++)
        {
            if (count_map.find(arr[i]) != count_map.end())
            {
                count_map[arr[i]]++;
            }
            else
            {
                count_map[arr[i]] = 1;
            }
        }

        // 声明一个小根堆，其中元素类型为int
        //实际就是为了重载greater<T>
        auto cmp = [](Node *a, Node *b)
        { return a->count > b->count; }; // 按照词频组织小顶堆
        priority_queue<Node *, vector<Node *>, decltype(cmp)> min_heap(cmp);

        int heap_size=0;
        for (const auto& iter:count_map)
        {
            if (heap_size < k) { // 堆中最多10个元素，防止内存溢出 
                min_heap.push(new Node(iter.first, iter.second));
                heap_size++;
            } else if (iter.second > min_heap.top()->count) { // 当前词频大于堆顶词频，则替换堆顶
                Node* tmp = min_heap.top();
                min_heap.pop();
                min_heap.push(new Node(iter.first, iter.second));
                delete tmp;
            }
        }


        while (!min_heap.empty()) {
            Node* tmp = min_heap.top();
            res.push_back(tmp->str);
            min_heap.pop();
        }
        return res;
    }
};

int main()
{
    Solution sol;
    vector<string> arr = {"a", "b", "a", "c", "c", "c"};
    int k = 2;

    // string text1 = "a";
    // string text2 = "b";
    vector<string> res = sol.top_k_str_num(arr, k);
    for (auto iter:res) {
        cout << " " << iter;
    }
    cout << endl;
}