/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-14 16:34:27
 * @LastEditors: baisichen
 * @Description: 指针指向有点问题，leetcode没有通过，但思想已经通了
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
https://leetcode.cn/problems/lfu-cache/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china

请你为 最不经常使用（LFU）缓存算法设计并实现数据结构。

实现 LFUCache 类：

LFUCache(int capacity) - 用数据结构的容量 capacity 初始化对象
int get(int key) - 如果键 key 存在于缓存中，则获取键的值，否则返回 -1 。
void put(int key, int value) - 如果键 key 已存在，则变更其值；如果键不存在，请插入键值对。当缓存达到其容量 capacity 时，则应该在插入新项之前，移除最不经常使用的项。在此问题中，当存在平局（即两个或更多个键具有相同使用频率）时，应该去除 最久未使用 的键。
为了确定最不常使用的键，可以为缓存中的每个键维护一个 使用计数器 。使用计数最小的键是最久未使用的键。

当一个键首次插入到缓存中时，它的使用计数器被设置为 1 (由于 put 操作)。对缓存中的键执行 get 或 put 操作，使用计数器的值将会递增。

函数 get 和 put 必须以 O(1) 的平均时间复杂度运行。



示例：

输入：
["LFUCache", "put", "put", "get", "put", "get", "get", "put", "get", "get", "get"]
[[2], [1, 1], [2, 2], [1], [3, 3], [2], [3], [4, 4], [1], [3], [4]]
输出：
[null, null, null, 1, null, -1, 3, null, -1, 3, 4]

解释：
// cnt(x) = 键 x 的使用计数
// cache=[] 将显示最后一次使用的顺序（最左边的元素是最近的）
LFUCache lfu = new LFUCache(2);
lfu.put(1, 1);   // cache=[1,_], cnt(1)=1
lfu.put(2, 2);   // cache=[2,1], cnt(2)=1, cnt(1)=1
lfu.get(1);      // 返回 1
                 // cache=[1,2], cnt(2)=1, cnt(1)=2
lfu.put(3, 3);   // 去除键 2 ，因为 cnt(2)=1 ，使用计数最小
                 // cache=[3,1], cnt(3)=1, cnt(1)=2
lfu.get(2);      // 返回 -1（未找到）
lfu.get(3);      // 返回 3
                 // cache=[3,1], cnt(3)=2, cnt(1)=2
lfu.put(4, 4);   // 去除键 1 ，1 和 3 的 cnt 相同，但 1 最久未使用
                 // cache=[4,3], cnt(4)=1, cnt(3)=2
lfu.get(1);      // 返回 -1（未找到）
lfu.get(3);      // 返回 3
                 // cache=[3,4], cnt(4)=1, cnt(3)=3
lfu.get(4);      // 返回 4
                 // cache=[3,4], cnt(4)=2, cnt(3)=3


提示：

1 <= capacity <= 104
0 <= key <= 105
0 <= value <= 109
最多调用 2 * 105 次 get 和 put 方法
*/

/*
扔掉最小频度的，如果频度相同，则扔掉最远使用的，每次get和put调用的时间复杂度是O(1)

大结构使用桶，每个桶代表一个词频，每个桶使用双向链表串到一起，桶内元素使用双向链表
使用一个map记录key的内存地址
使用一个map记录具体某个元素所在桶的地址
一开始桶和map均为空
最左侧的桶是频率最小的。同时最左边桶的第一个元素就就是最远未使用的元素，就可以在替换时删掉。

put一个数时：
1. 如果不存在该元素，桶都是空，则记录(key,value,1)到最左边的桶中
2. 如果存在该元素，则先从桶里分离出来，频率增加1，看下一个桶有没有，如果没有则新建
3. 从旧桶中挪出来了元素，看下原来的桶是否是空，如果为空，则删除桶

get一个数时：
1. 先从桶里分离出来，频率增加1，下一个桶有的话就放进去，否则就新建
2. 从旧桶中挪出来了元素，看下原来的桶是否是空，如果为空，则删除桶





总结：链表的大题
*/

struct Node {
public:
    int key;
    int value;
    int times;
    Node* prev;
    Node* next;
    Node(int k, int v) {
        key = k;
        value = v;
        times = 1;
        prev = nullptr;
        next = nullptr;
    }
};
//双向链表
class NodeList {
public:
    Node* head;
    Node* tail;
    NodeList* pre;
    NodeList* next;
    NodeList(Node* node) {
        head = node;
        tail = node;
        pre = nullptr;
        next = nullptr;
    }

    void delete_node(Node* node) {
        if (head == tail) {
            // cout << "is equal" << endl;
            head = nullptr;
            tail = nullptr;
        } else {
            if (node == head) {
                head = head->next;
                head->prev = nullptr;
            } else if (node == tail) {
                tail = tail->prev;
                tail->next = nullptr;
            } else {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
        }
        node->prev = nullptr;
        node->next = nullptr;
    }

    void add_node(Node* node) {
        if (head == nullptr) {
            head = node;
            tail = node;
        } else { //头插法
            node->next = head;
            head->prev = node;
            head = node;
            // cout << "second add:" << (head==tail) << endl;
        }
    }
};
class LFUCache
{
public:
    int _capacity; // 缓存大小限制
    int _size;     // 目前有多少个节点
    map<int, Node*> _records; // 记录key和node的映射
    map<int, NodeList*> _buckets; // 记录key和桶的映射
    NodeList* head_bucket;
    NodeList* tail_bucket;
    LFUCache(int capacity) {
        _capacity = capacity;
        head_bucket = nullptr;
        tail_bucket = nullptr;
        _size = 0;
    }

    void print_cache() {
        auto bucket = head_bucket;
        cout << "=====bucket start====" << endl;
        cout << "bucket_tail:" << tail_bucket << " bucket_head:" << head_bucket << " tail_bucket->next:" << (tail_bucket?tail_bucket->next:0) << endl;
        int count = 1;
        while (bucket) {
            auto head = bucket->head;
            cout << "bucket:" << count << ":";
            while (head) {
                cout << head->key << " " << head->value << " " << head->times << ";";
                head = head->next;
            }
            cout << "tail:" << bucket->tail << " bucket->tail->next:" << bucket->tail->next << " value:" << bucket->tail->value << endl;
            count+=1;
            bucket = bucket->next;
            cout << endl;
        }
        cout << "=====bucket end====" << endl;
        
    }

    int get(int key) {
        //先假定一些黑盒函数，然后再填
        if (_capacity == 0) {
			return -1;
		}

        int ans = -1;
        for (auto iter:_records){
            cout << endl << "======key:" <<iter.first << " value:"<< iter.second->value << endl;
        }
        if (_records.find(key) != _records.end()) { //存在
            ans = _records[key]->value;
            cout << "ans:" << ans << endl;
            auto node = _records[key];
            auto cur_bucket = _buckets[key];

            cout << endl << "************start" << endl;
            auto tmp = cur_bucket->head;
            while (tmp) {
                cout << tmp->key << " "<< tmp->value << " " << tmp->times << ";";
                tmp = tmp->next;
            }
            cout << endl;
            cout << "************end" << endl;
            node->times++;
            move_next_bucket(node, cur_bucket);//将node移动到下一个同理桶里，同时更改buckets表映射关系
        }
        return ans;
    }

    void put(int key, int value) {
        if (_capacity == 0) {
			return;
		}

        //先判断是否已经存在
        if(_records.find(key) != _records.end()) {
            //获得node地址，修改value，并更新time
            Node* node = _records[key];
            node->value = value;
            node->times++;
            //获得桶地址，看下一个桶是否为空，如果为空，新建，否则将元素扔到下一个桶里
            NodeList* bucket = _buckets[key];
            move_next_bucket(node, bucket); //将node移动到下一个桶里，同时更改buckets表映射关系
        } else {
            //如果已经达到了capacity，则淘汰词频最低的元素，即左边元素的最后一个
            if (_size == _capacity) {
                NodeList* bucket = head_bucket;
                Node* node = bucket->tail;
                cout << "bucket--------:" << endl;
                cout << "delete:" << node->key << " "<< node->value << " " << node->times << endl;
                bucket->delete_node(node);
                
                auto tmp = bucket->head;
                while (tmp) {
                    cout << tmp->key << " "<< tmp->value << " " << tmp->times << ";";
                    tmp=tmp->next;
                }
                cout << "bucket--------:" << endl;
                modify_bucket(bucket); //如果桶为空则做相应修改
                _buckets.erase(node->key);
                _records.erase(node->key);
                _size--;
            }

            //新建node
            Node* new_node = new Node(key, value);
            //找到最左侧的桶，看其词频是否等于1，如果是，则直接扔到桶里
            if (head_bucket==nullptr) {
                cout << "head_bucket nullllll" << endl;
                head_bucket = new NodeList(new_node);
                tail_bucket = head_bucket;
            } else {
                if (head_bucket->head->times == 1) {
                    cout << "head_bucket timeeeee" << endl;
                    head_bucket->add_node(new_node);
                } else {
                    cout << "delteeeeeee"<<endl;
                    auto new_bucket = new NodeList(new_node);
                    new_bucket->next = head_bucket;
                    head_bucket->pre = new_bucket;
                    head_bucket = new_bucket;
                }
            }
            
            _records[new_node->key] = new_node;
            _buckets[new_node->key] = head_bucket;
            _size++;
        }
    }

    //将node从bucket中移除
    void move_next_bucket(Node* node, NodeList* old_bucket) {
        //从前一个桶中移除
        old_bucket->delete_node(node);

        cout << endl << "************after delte node start" << endl;
        auto tmp = old_bucket->head;
        while (tmp) {
            cout << tmp->key << " "<< tmp->value << " " << tmp->times << ";";
            tmp = tmp->next;
        }
        cout << endl;
        cout << "************after delte node end" << endl;


        NodeList* nextBucket = old_bucket->next;
        NodeList* origin_pre = old_bucket->pre;
        bool empty_flag = modify_bucket(old_bucket);
        NodeList* preBucket =  empty_flag ? origin_pre : old_bucket;

        cout << endl<< "##########" << endl;
        cout << "nextBucket:" << nextBucket << " preBucket:" << preBucket << endl;
        if (nextBucket&&nextBucket->head) {
            cout << "nextBucket->vaue:" << nextBucket->head->value;
        }
        if (preBucket&&  preBucket->head) {
            cout << "preBucket->vaue:" << preBucket->head->value;
        }
        cout << endl<< "##########" << endl;

        // 如果下一个桶为空，或者词频和当前node不一样，则新建一个
        if (nextBucket == nullptr) {
            NodeList* new_bucket = new NodeList(node);
            if (preBucket != nullptr) {
                preBucket->next = new_bucket;
            }
            new_bucket->pre = preBucket;
            if (head_bucket == nullptr) {
                head_bucket = new_bucket;
                if (tail_bucket == nullptr) {
                    tail_bucket = new_bucket;
                }
            }
            tail_bucket = new_bucket; //新建的桶在最后一个，更新tail_bucket
            _buckets[node->key] = new_bucket;
        } else { //此时下一个桶不是空
            if (nextBucket->head->times == node->times) { //频率相同的放在一个桶里
                //将node扔到下一个桶里
                nextBucket->add_node(node);
                _buckets[node->key] = nextBucket;
            } else {
                NodeList *new_bucket = new NodeList(node);
                if (preBucket != nullptr) {
                    preBucket->next = new_bucket;
                }
                new_bucket->pre = preBucket;
                new_bucket->next = nextBucket;
                nextBucket->pre = new_bucket;
                if (head_bucket == nextBucket) {
                    head_bucket = new_bucket;
                }
                _buckets[node->key] = nextBucket;
            }
        }
        // if (empty_flag&&old_bucket) {
        //     delete old_bucket;
        // }
    }
    /*
    对已经删掉一个元素桶做修改
    1. 如果删掉之后桶不为空，则不做操作
    2. 如果删掉之后桶为空
    */
    bool modify_bucket(NodeList* removed_bucket) {
        if (removed_bucket->head != nullptr) {
            cout << "not kong" << endl;
            return false;
        }

        //下面就是桶空了
        //将前面的桶和后面的桶项链
        if (head_bucket == tail_bucket) {
            cout << "dengyu" << endl;
            head_bucket = nullptr;
            tail_bucket = nullptr;
        } else {
            NodeList* ans = nullptr;
            if (removed_bucket == head_bucket) {
                head_bucket = head_bucket->next;
                head_bucket->pre = nullptr;
            } else if (removed_bucket == tail_bucket) {
                tail_bucket = tail_bucket->pre;
                tail_bucket->next = nullptr;
                ans = tail_bucket;
            } else {
                cout << "removed_bucket:" << removed_bucket << " removed_bucket->pre:" << removed_bucket->pre
                     << " removed_bucket->next:" << removed_bucket->next << " tail_bucket:" << tail_bucket << " head_bucket:" << head_bucket << endl;
                removed_bucket->pre->next = removed_bucket->next;
                removed_bucket->next->pre = removed_bucket->pre;
                ans = removed_bucket->pre;
            }
        }
        return true;
    }
};

int main()
{

//     LFUCache lfu(2);
// // ["LFUCache", "put", "put", "get", "put", "get", "get", "put", "get", "get", "get"]
// // [[2], [1, 1], [2, 2], [1], [3, 3], [2], [3], [4, 4], [1], [3], [4]]
// // 输出：
// // [null, null, null, 1, null, -1, 3, null, -1, 3, 4]
//     lfu.put(1,1);
//     lfu.put(2, 2);
//     lfu.print_cache();
//     cout << "res 1:" <<  lfu.get(1) << endl;
//     lfu.print_cache();
//     cout << "res size:" << lfu._size << endl;
//     lfu.put(3, 3); 
//     lfu.print_cache();
//     cout << "res 2:" << lfu.get(2) << endl;
//     cout << "res 3:" << lfu.get(3) << endl;
//     lfu.put(4, 4);
//     cout << "res 1:" << lfu.get(1) << endl;
//     cout << "res 3:" << lfu.get(3) << endl;
//     cout << "res 4:" << lfu.get(4) << endl;
//     cout << "res 2:" << lfu.get(2) << endl;
    // int capacity = 10;
    // vector<string> list =     {"put", "put","put","get","put",  "put","get","put", "put", "get","put", "get","get","get","put", "put", "get","put", "get"};
    // vector<vector<int>> req = {{7,28},{7,1},{8,15},{6}, {10,27},{8,10},{8}, {6,29},{1,9}, {6},  {10,7},{1},  {2},  {13}, {8,30},{1,5},  {1}, {13,2},{12}};
    // vector<string> list =     {"put", "put","put","get","put",  "put","get", "put", "put", "get","put", "get","get","get","put", "put", "get","put", "get"};
    // vector<vector<int>> req = {{7,28},{7,1},{8,15},{6}, {10,27},{8,10},{8},  {6,29},{1,9}, {6},  {10,7},{1},  {2},  {13}, {8,30},{1,5},  {1}, {13,2},{12}};

    // vector<string> list = {"put",   "put", "put", "put", "put", "get", "put", "get", "get", "put", "get", "put", "put", "put", "get", "put", "get", "get", "get", "get", "put", "put", "get", "get", "get", "put", "put", "get", "put", "get", "put", "get", "get", "get", "put", "put", "put", "get", "put", "get", "get", "put", "put", "get", "put", "put", "put", "put", "get", "put", "put", "get", "put", "put", "get", "put", "put", "put", "put", "put", "get", "put", "put", "get", "put", "get", "get", "get", "put", "get", "get", "put", "put", "put", "put", "get", "put", "put", "put", "put", "get", "get", "get", "put", "put", "put", "get", "put", "put", "put", "get", "put", "put", "put", "get", "get", "get", "put", "put", "put", "put", "get", "put", "put", "put", "put", "put", "put", "put"};
    // vector<vector<int>> req = {{10},{10,13},{3,17},{6,11},{10,5},{9,10},{13},{2,19},{2},{3},{5,25},{8},{9,22},{5,5},{1,30},{11},{9,12},{7},{5},{8},{9},{4,30},{9,3},{9},{10},{10},{6,14},{3,1},{3},{10,11},{8},{2,14},{1},{5},{4},{11,4},{12,24},{5,18},{13},{7,23},{8},{12},{3,27},{2,12},{5},{2,9},{13,4},{8,18},{1,7},{6},{9,29},{8,21},{5},{6,30},{1,12},{10},{4,15},{7,22},{11,26},{8,17},{9,29},{5},{3,4},{11,30},{12},{4,29},{3},{9},{6},{3,4},{1},{10},{3,29},{10,28},{1,20},{11,13},{3},{3,12},{3,8},{10,9},{3,26},{8},{7},{5},{13,17},{2,27},{11,15},{12},{9,19},{2,15},{3,16},{1},{12,17},{9,1},{6,19},{4},{5},{5},{8,1},{11,7},{5,2},{9,28},{1},{2,2},{7,4},{4,22},{7,24},{9,26},{13,28},{11,26}};

    int capacity = 2;
    // // vector<string> list = {"get","get","put","get","get"};
    // // vector<vector<int>> req = {{6},{6},{3,50},{3},{1}};

    vector<string> list = {"put",    "put", "get","put","get","get","put","get","get","get"};
    vector<vector<int>> req = {{1,1},{2,2}, {1},  {3,3}, {2},  {3},  {4,4},{1},  {3}, {4}};


    LFUCache lfu(capacity);
    for (int i=0; i<list.size(); ++i) {
        cout << "i:" << i << endl;
        if (list[i] == "put") {
            cout << "put key:" << req[i][0] << " value:" << req[i][1] << endl;
            lfu.put(req[i][0], req[i][1]);
        } else if (list[i] == "get"){
            int ans = lfu.get(req[i][0]);
            cout << "get key:" << req[i][0] << " value:" << ans << endl;
        }
        
        lfu.print_cache();
    }

    // int testtime = 1000;
    // int max_capacity = 3;
    // for (int i=0;i<testtime;i++) {
    //     int capacity = std::rand() % max_capacity + 1;
    //     LFUCache lfu(capacity);
    //     int op_num = std::rand() % 6 + 1;
    //     //生成操作列表
    //     vector<string> list;
    //     vector<vector<int>> req;
    //     for (int i=0;i<op_num;i++) {
    //         int op = std::rand() % 2 + 1;
    //         if (op == 1) {
    //             list.push_back("put");
    //             int key = std::rand() % 10 + 1;
    //             int value = std::rand() % 100 + 1;
    //             vector<int> tmp = {key, value};
    //             req.push_back(tmp);
    //         } else if (op == 2){
    //             int key = std::rand() % 10 + 1;
    //             list.push_back("get");
    //             vector<int> tmp = {key};
    //             req.push_back(tmp);
    //         }
    //     }

    //     cout << "====!!!!!!begin====" << endl;
    //     cout << "capacity:" << capacity << endl;
    //     cout << "vector<string> list = {";
    //     for (int i=0;i<op_num;i++) {
    //         cout << "\"" << list[i] << "\"";
    //         if (i!=op_num-1) {
    //             cout << ",";
    //         }
    //     }
    //     cout << "}";
    //     cout << endl;
    //     cout << "vector<vector<int>> req = {";
    //     for (int i=0;i<op_num;i++) {
    //         cout << "{";
    //         for (int j=0;j<req[i].size();j++) {
    //             if (j==0) {
    //                 cout << req[i][j];
    //             } else {
    //                 cout << "," << req[i][j];
    //             }
    //         }
    //         cout << "}";
    //         if (i!=op_num-1) {
    //             cout << ",";
    //         }
    //     }
    //     cout << "}";
    //     cout << endl;
    //     cout << "====!!!!!!end====" << endl;

    //     //执行
    //     for (int i=0; i<list.size(); ++i) {
    //         cout << "i:" << i << endl;
    //         if (list[i] == "put") {
    //             cout << "put key:" << req[i][0] << " value:" << req[i][1] << endl;
    //             lfu.put(req[i][0], req[i][1]);
    //         } else if (list[i] == "get"){
    //             cout << "get key:" << req[i][0] << " value:" << lfu.get(req[i][0]) << endl;
    //         }
            
    //         lfu.print_cache();
    //     }

    // }
    return 0;
}