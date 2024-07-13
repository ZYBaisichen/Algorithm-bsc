/*
 * @Author: baisichen
 * @Date: 2024-03-16 13:22:49
 * @LastEditTime: 2024-03-17 11:30:41
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

using namespace std;

template <typename K, typename V>
class SkipListNode {
public:
    K key;
    V value;
    bool is_first;
    map<int, SkipListNode*> next;
    SkipListNode(K _key, V _value) {
        key = _key;
        value = _value;
        next[0] = nullptr; //必有0层
        is_first = false;
    }

    //判断当前节点是否小于otherkey
    bool is_key_less(K other_key) {
        //当前是第一个节点，肯定小于other_key
        if (is_first) {
            return true;
        }
        if (key < other_key) {
            return true;
        }
        //key是null时也是大于other_key的
        return false;
    }

    bool is_equal(K other_key) {
        if (is_first) {
            return false;
        }
        return key == other_key;
    }

};
template <typename K, typename V>
class SkipList {
public:
    double probability;
    double leve_up;
    SkipListNode<K, V>* head;
    int max_level;
    int size;
    SkipList() {
        head = new SkipListNode<K, V>(INT_MIN, INT_MAX);
        head->is_first = true;
        probability = 0.5;
        max_level = 0; //最少有有个0层
        size = 1; //初始只有head一个节点
        leve_up = 10;
    }
    //level_num可以手动设置有几层，便于调试
    void put(K key, V value, int level_num=-1) {
        //找到最右的比key小的节点
        SkipListNode<K, V>* pre = most_right_less_node_in_tree(key);
        //取出该节点0层再下面一个节点
        SkipListNode<K, V> *find = pre->next[0];
        // cout << "pre:" << pre << " pre->key:"  << pre->key << " find:" << find << endl;
        if (find && find->is_equal(key)) {
            find->value = value;
        } else { // find == null || find->key != key
            size++; //总节点数加1
            int new_node_level = 0;//新节点数，最少有一个0层
            std::srand(std::time(0));
            double randomValue = static_cast<double>(std::rand()) / (static_cast<double>(RAND_MAX) + 1.0);
            if (level_num == -1) {
                //摇骰子，一直摇到出现1为止
                while (randomValue < probability && new_node_level < leve_up) {
                    new_node_level++;
                    std::srand(std::time(0));
                    randomValue = static_cast<double>(std::rand()) / (static_cast<double>(RAND_MAX) + 1.0);
                    // cout << "randomValue: " << randomValue << endl;
                }
            } else {
                new_node_level = level_num;
            }
            // cout << "new_node:" << key <<" new_node_level:" << new_node_level << endl;
            // newNodeLevel 有可能超过最大高度的，需要更新
            // 最左节点要和当前新节点一样高
            // 这个while是有可能走不到的，如果随机得到的层数小于最左节点的高度就不会进入
            while (max_level < new_node_level) {
                max_level++;
                head->next[max_level] = nullptr; // 增加一层
            }

            SkipListNode<K, V>* new_node = new SkipListNode<K, V>(key, value);
            for (int i = 0; i <= new_node_level; ++i) {
                new_node->next[i] = nullptr;
            }
            // cout << "here" <<endl;

            // 从head的最高层开始，往右寻找每一层 < key的最右节点
            int level = max_level;
            SkipListNode<K, V>* pre = head;
            while (level >= 0) {
                //在同一层中找到最右边的小于key的节点
                pre  = most_right_less_node_in_level(pre, key, level);
                // cout << " add_pre:" << pre << endl;
                //当前来到的层，在新节点中存在
                if (level <= new_node_level) {
                    new_node->next[level] = pre->next[level];
                    pre->next[level] = new_node;
                }
                --level;
            }
        }
    }


    //仅测试正整数
    V get(K key) {
        SkipListNode<K, V>* pre = most_right_less_node_in_tree(key);
        SkipListNode<K, V> *find = pre->next[0];
        return find != nullptr && find->is_equal(key) ? find->value : -1;
    }

    void remove(K key) {
        if (contains_key(key)) { //有这个key的时候才删除
            size--;
            SkipListNode<K, V>* pre = head;
            int level = max_level; // 从最高层开始
            while (level >= 0) {
                pre = most_right_less_node_in_level(pre, key, level);
                SkipListNode<K, V>* next = pre->next[level];
                // 分为两种情况
                // 1、pre的下一个节点就=被删除的key
                // 2、pre的下一个节点的key 是 > 被删除的key。（只有需要删除的节点高度不够时才会有这种情况 ）
                if (next && next->is_equal(key)) {
                    pre->next[level] = next->next[level];
                }
                // 还需判断在head节点处，最上层的指针是null的情况、除了第0层
                // 也就是删除整个跳表最上面那指向null的层数
                // 不能缩0层
                if (level !=0 && pre == head && head->next[level] == nullptr) {
                    head->next.erase(level);
                    max_level--;
                }
                level--;
            }
        }
    }
    K first_key() {
        return head->next[0] != nullptr ? head->next[0]->key : -1;
    }

    K last_key() {
        int level = max_level;
        SkipListNode<K, V>* cur = head;
        // 从高层开始找到最后一个
        while (level >= 0) {
            SkipListNode<K, V>* next = cur->next[level];
            //挪到最后一个节点再往下挪动
            while (next!=nullptr) {
                cur = next;
                next = next->next[level];
            }
            --level;
        }
        return cur->key;
    }

    // 返回 >=key的，最左的节点
    K ceiling_key(K key) {
        //小于key的最右的节点的下一个节点，就是大于等于key最左的节点
        SkipListNode<K, V>* pre = most_right_less_node_in_tree(key);
        SkipListNode<K, V> *find = pre->next[0];
        // cout << "celling key, pre: " << pre << " pre_key:" << pre->key << endl;
        return find != nullptr ? find->key : -1;
    }

    //返回<=key的最右节点
    K floor_key(K key) {
        SkipListNode<K, V>* pre = most_right_less_node_in_tree(key);
        SkipListNode<K, V> *find = pre->next[0];
        return find != nullptr && find->is_equal(key) ? find->key : pre->key;
    }

    bool contains_key(K key) {
        SkipListNode<K, V>* pre = most_right_less_node_in_tree(key);
        SkipListNode<K, V> *find = pre->next[0];
        return find != nullptr && find->is_equal(key);
    }
    

    SkipListNode<K, V> *most_right_less_node_in_level(SkipListNode<K, V> * cur_node, K key, int level) {
        SkipListNode<K, V> *pre = cur_node, *cur = cur_node;
        //在同一层中找到最右边的小于key的节点
        while (cur && cur->is_key_less(key)) {
            pre = cur;
            // cout << "most_right_less_node_in_level: key: " << cur->key << " level:" << level << " pre:" << pre->key << endl;
            cur = cur->next[level];
        }
        return pre;
    }
    SkipListNode<K, V> * most_right_less_node_in_tree(K key) {
        SkipListNode<K, V> *pre = head;
        int level = max_level;
        while (level >= 0) {
            pre = most_right_less_node_in_level(pre, key, level);
            // cout << "most_right_less_node_in_tree, pre:" << pre->key << endl;
            --level;
        }
        return pre;
    }
    void print_list() {
        int level = max_level;
        //打印每一层
        while (level >= 0) {
            cout << "cur_level:" << level << ": ";
            SkipListNode<K, V> *next = head->next[level];
            while (next) {
                cout << next->key << " ";
                next = next->next[level];
            }
            cout << endl;
            --level;
        }
    }
};

int main() {
    SkipList<int, int> sl;
    // LL : 4,3,2,1,0
    // RR: 0,1,2,3,4
    // LR: 10,4,6
    // RL: 2,10,8
    sl.put(1, 1, 0);
    sl.put(2, 2, 2);
    sl.put(3, 10, 1);
    sl.put(4, 9, 3);
    sl.put(5, 5, 2);
    // sl.put(6, 6, 1);
    sl.put(7, 7, 4);
    sl.print_list();

    cout << "celling key:" << sl.ceiling_key(6) << endl;
    cout << "floor key:" << sl.floor_key(6) << endl;
    cout << "first key:" << sl.first_key() << endl;
    cout << "last key:" << sl.last_key() << endl;

    sl.remove(4);
    cout << "after remove==="<<endl;
    sl.print_list();

    cout << "get key:" << sl.get(3) << endl;

    return 0;
}