/*
 * @Author: baisichen
 * @Date: 2024-03-16 13:22:49
 * @LastEditTime: 2024-03-16 19:20:20
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
class SBTNode {
public:
    K key; //key必须是可比较的。先不支持string和int以外的情况
    V value;
    int size;
    SBTNode<K, V>* left;
    SBTNode<K, V>* right;
    SBTNode(K _key, V _value) {
        key = _key;
        value = _value;
        left = nullptr;
        right = nullptr;
        size = 1; //创建时当前节点树就一个节点，所以大小为1
    }
};

template <typename K, typename V> 
class SizeBalanceTree {
public:
    SizeBalanceTree() {
        root = NULL;
    }

    //（key, value） 加入到有序表，新增、修改value
    void put(K key, V val) {
        SBTNode<K, V>* lastNode = search(key);// 找到key或其父节点
        if (lastNode != nullptr && key == lastNode->key) { // 更新值的情况
            lastNode->value = val;
        } else { // 没找到，则新插入值。因为下面是递归行为，所以从上到下开始加并更新各个节点的size，最后从下到上逐个调整平衡性。
            root = add(root, key, val);
        }
    }

    SBTNode<K, V>* add(SBTNode<K, V>* cur, K key, V value) {
        if (cur == nullptr) { //找到了空位置，插入进去，返回当前的头部
            cur = new SBTNode<K, V>(key, value);
            return cur;
        }
        
        //下面是cur不为空的情况
        cur->size++;//从上到下滑的同时，每个节点的size++
        cout << "cur:" << cur->key << ":" << cur->value << ":" <<cur->size << endl;
        if (key < cur->key) { //左滑
            cur->left = add(cur->left, key, value);
        } else if (key > cur->key) { //右滑
            cur->right = add(cur->right, key, value);
        }

        // 左右子树都更新完毕，开始调整平衡性
        // 从下往上调整平衡性。
        //返回值是调整之后的新的头部
        return maintain(cur);
    }

    bool contains_key(K key) {
        SBTNode<K, V>* lastNode = search(key);
        if (lastNode != nullptr && key == lastNode->key) {
            return true;
        } else {
            return false;
        }
    }

    void remove(K key) {
        // 有这个key了，才会递归去删除
        // 同样的是从上到下删除。因为要将沿途的size更新
        // 这里只在add的时候做平衡性调整，delete的时候不调整
        if (contains_key(key)) {
            root = delete_key(root, key);
        } else {
            cout << "not containt key:" << key << endl;
        }
    }

    //返回新的头部。虽然不做平衡性调整，但万一删除root节点，root节点就需要变换
    SBTNode<K, V> *delete_key(SBTNode<K, V> *cur, K key)
    {
        cur->size--;
        
        if (key < cur->key) {
            cur->left = delete_key(cur->left, key);
        } else if (key > cur->key) {
            cur->right = delete_key(cur->right, key);
        } else {
            SBTNode<K, V> *cur_tmp = cur;
            if (cur->left == nullptr) { //没有左孩子，或两个孩子都没有
                cur = cur->right; //右孩子接替自己返回
            } else if (cur->right == nullptr) { //没有右孩子
                cur = cur->left; //左孩子接替自己返回
            } else { //左右孩子都有的情况，让后继节点代替自己
                SBTNode<K, V>* pre = nullptr;
                SBTNode<K, V>* last_node = cur->right;
                last_node->size--;
                while (last_node->left != nullptr) {
                    pre = last_node;
                    last_node = last_node->left;
                    last_node->size--;
                }

                //如果pre为nullptr，说明后继节点就是cur的右指针
                //则不需要再调整后继节点本身的情况了
                if (pre != nullptr) {
                    pre->left = last_node->right; // 让后继节点的右孩子代替它
                    last_node->right = cur->right; // 将后继节点代替自己的位置，right。 如果后继节点本身就是cur的右孩子，则不需要做任何操作
                }

                last_node->left = cur->left; // 将后继节点代替自己的位置，left

                // 更新新头部的size
                last_node->size = 1 + 
                    (last_node->left ? last_node->left->size : 0 ) 
                    + (last_node->right ? last_node->right->size : 0);
                cur = last_node; // 新头部，彻底舍弃node了。
                delete cur_tmp;
            }
        }
        // 维持平衡---可以不用维持平衡，在add的时候再维护
        // node = maintain(node);
        return cur;
    }

    // 返回等于key的，或者key的父节点。
    SBTNode<K, V>* search(K key) {
        SBTNode<K, V> *cur = root, *pre = root;

        while (cur != nullptr) {
            pre = cur;
            if (key == cur->key) {
                break;
            } else if (key < cur->key) { //左滑
                cur = cur->left;
            } else { 
                cur = cur->right;
            }
        }
        return pre; // 返回最近的不空的节点，有可能是key，也有可能是key的父节点
    }

    SBTNode<K, V>* maintain(SBTNode<K, V>* cur) {
        if (cur == nullptr) {
            return nullptr;
        }
        // 计算cur的下一级节点数和 下下一级节点数
        int left_size = cur->left != nullptr ? cur->left->size : 0;
        int right_size = cur->right != nullptr ? cur->right->size : 0;
        int left_left_size = (cur->left != nullptr && cur->left->left != nullptr) 
            ? cur->left->left->size : 0;
        int left_right_size = (cur->left != nullptr && cur->left->right != nullptr) 
            ? cur->left->right->size : 0;
        int right_right_size = (cur->right != nullptr && cur->right->right != nullptr) 
            ? cur->right->right->size : 0;
        int right_left_size = (cur->right != nullptr && cur->right->left != nullptr) 
            ? cur->right->left->size : 0;
        cout << "=====" << endl;
        cout << "cur->key: " << cur->key << endl;
        cout << "left_size: " << left_size << ", right_size:" << right_size << endl;
        cout << "left_left_size:" << left_left_size << "right_right_size:" << right_right_size << endl;
        cout << "left_right_size:" << left_right_size << "right_left_size:" << right_left_size << endl;

        //左孙子比自己的右孩子大，LL型
        if (left_left_size > right_size) {
            cur = right_rotate(cur); // 右旋
            cur->right = maintain(cur->right); // 先调整cur下级节点的平衡
            cur = maintain(cur); //最后调整上方节点的平衡性
        } else if (right_right_size > left_size) { //优先LL和RR
            cur = left_rotate(cur); // 左旋
            cur->left = maintain(cur->left); // 先调整cur下级节点的平衡
            cur = maintain(cur); //最后调整上方节点的平衡性
        } else if (left_right_size > right_size) { //LR型
            cur->left = left_rotate(cur->left); //左子树先左旋 
            cur = right_rotate(cur); //再右旋
            cur->left = maintain(cur->left);   // 注意这里cur已经是左右旋后的新头部了。
            cur->right = maintain(cur->right); // 先维持node的下级节点
            cur = maintain(cur);
        } else if (right_left_size > left_size) { //RL型
            cur->right = right_rotate(cur->right); //右子节点先右旋
            cur = left_rotate(cur); //再左旋
            cur->right = maintain(cur->right);//这里先调右树还是左树都可以。
            cur->left = maintain(cur->left);
            cur = maintain(cur);
        }
        return cur;
    }

    // 基于cur节点做左旋，返回旋转后的原来位置的节点
    SBTNode<K, V>* left_rotate(SBTNode<K, V>* cur) {
        SBTNode<K, V> *newHead = cur->right; // 右节点代替自己
        cur->right = newHead->left;            // 右节点的左孩子变成了自己的右孩子
        newHead->left = cur;                  // cur往左边沉下去
        newHead->size = cur->size;             // 新节点的大小等于老节点大小
        cur->size = (cur->left != nullptr ? cur->left->size : 0)
                + (cur->right != nullptr ? cur->right->size : 0) + 1;
        return newHead;
    }

    //基于cur节点做右旋，返回旋转后的原来位置的节点
    SBTNode<K, V>* right_rotate(SBTNode<K, V>* cur) {
        SBTNode<K, V> *newHead = cur->left; // 左节点代替自己
        cur->left = newHead->right;            // 左节点的右孩子变成了自己的左孩子
        newHead->right = cur;                  // cur往右边沉下去
        newHead->size = cur->size;             // 新节点的大小等于老节点大小
        cur->size = (cur->left != nullptr ? cur->left->size : 0)
                + (cur->right != nullptr ? cur->right->size : 0) + 1;
        return newHead; // 返回新头部
    }

    void mid_print(SBTNode<K, V>* cur) {
        if (cur == nullptr) return;
        mid_print(cur->left);
        cout << "key: " << cur->key << ", value: " << cur->value << endl;
        mid_print(cur->right);

    }

    SBTNode<K, V>* get_root() {
        return root;
    }

    // 返回等于key的查找的次数
    int search_count(K key) {
        SBTNode<K, V> *cur = root, *pre = root;
        int count = 0;

        while (cur != nullptr) {
            count++;
            pre = cur;
            if (key == cur->key) {
                break;
            } else if (key < cur->key) { //左滑
                cur = cur->left;
            } else { 
                cur = cur->right;
            }
        }
        return count; 
    }

private:
    SBTNode<K, V>* root;

};


int main() {
    SizeBalanceTree<int,int> sbt;
    // LL : 4,3,2,1,0
    // RR: 0,1,2,3,4
    // LR: 10,4,6
    // RL: 2,10,8
    sbt.put(2, 2);
    sbt.put(10, 10);
    sbt.put(8, 8);
    sbt.put(9, 9);
    sbt.put(11, 11);
    sbt.remove(10);
    cout << "root:" << sbt.get_root()->key << endl;
    // sbt.mid_print(sbt.get_root());
    cout << "count:" << sbt.search_count(9) << endl;
    return 0;
}