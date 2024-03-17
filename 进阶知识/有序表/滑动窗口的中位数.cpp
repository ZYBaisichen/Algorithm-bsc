/*
 * @Author: baisichen
 * @Date: 2024-03-16 13:22:49
 * @LastEditTime: 2024-03-17 22:14:33
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

class Key
{
public:
    int idx;
    int value;
    Key(int _idx, int _value)
    {
        idx = _idx;
        value = _value;
    }

    bool is_equal(Key& other_key)
    {
        return value != other_key.value ? false : (idx != other_key.idx ? false : true);
    }

    // 比较大小
    // 当前值比other_key小，则返回<0；大于返回>0；等于返回=0
    int compare(Key& other_key)
    {
        // cout << " compare:" << idx << "," << value << " other_key:" << other_key.idx << "," << other_key.value << endl;
        if (value == other_key.value && idx == other_key.idx)
        {
            return 0;
        }
        if (value > other_key.value)
        {
            return 1;
        }
        else
        {
            if (value == other_key.value)
            {
                // cout << "=====value" << endl;
                if (idx > other_key.idx)
                {
                    // cout << "===idx" << endl;
                    return 1;
                }
            }
        }
        return -1;
    }
};

template <typename K>
class SBTNode {
public:
    K key; // key必须是可比较的。先不支持string和int以外的情况
    int size;
    SBTNode<K> *left;
    SBTNode<K> *right;
    SBTNode(int idx, int value) : key(idx, value) {
        left = nullptr;
        right = nullptr;
        size = 1; // 创建时当前节点树就一个节点，所以大小为1
    }
    SBTNode(K &_key) : key(_key.idx, _key.value){
        left = nullptr;
        right = nullptr;
        size = 1; // 创建时当前节点树就一个节点，所以大小为1
    }
};

template <typename K>
class SizeBalanceTree {
public:
        SizeBalanceTree()
        {
            root = NULL;
        }

        // （key, value） 加入到有序表，新增、修改value
        void put(int idx, int value)
        {
            K key(idx, value);
            root = add(root, key);
        }

        SBTNode<K> *add(SBTNode<K> *cur, K& key)
        {
            if (cur == nullptr)
            { // 找到了空位置，插入进去，返回当前的头部
                cur = new SBTNode<K>(key);
                return cur;
            }

            // 下面是cur不为空的情况
            cur->size++; // 从上到下滑的同时，每个节点的size++
            // cout << "cur:" << cur->key.value << ":" << cur->key.idx << ":" << cur->size << endl;
            if (key.compare(cur->key) < 0)
            { // 左滑
                cur->left = add(cur->left, key);
            }
            else if (key.compare(cur->key) > 0)
            { // 右滑
                cur->right = add(cur->right, key);
            }

            // 左右子树都更新完毕，开始调整平衡性
            // 从下往上调整平衡性。
            // 返回值是调整之后的新的头部
            return maintain(cur);
        }

        bool contains_key(K &key)
        {
            SBTNode<K> *lastNode = search(key);
            if (lastNode != nullptr && key.is_equal(lastNode->key))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void remove(K &key)
        {
            // 有这个key了，才会递归去删除
            // 同样的是从上到下删除。因为要将沿途的size更新
            // 这里只在add的时候做平衡性调整，delete的时候不调整
            if (contains_key(key))
            {
                root = delete_key(root, key);
            }
            else
            {
                // cout << "not containt key:" << key.value << endl;
            }
        }

        // 返回新的头部。虽然不做平衡性调整，但万一删除root节点，root节点就需要变换
        SBTNode<K> *delete_key(SBTNode<K> *cur, K &key)
        {
            cur->size--;

            if (key.compare(cur->key) < 0)
            {
                cur->left = delete_key(cur->left, key);
            }
            else if (key.compare(cur->key) > 0)
            {
                cur->right = delete_key(cur->right, key);
            }
            else
            {
                SBTNode<K> *cur_tmp = cur;
                if (cur->left == nullptr)
                {                     // 没有左孩子，或两个孩子都没有
                    cur = cur->right; // 右孩子接替自己返回
                }
                else if (cur->right == nullptr)
                {                    // 没有右孩子
                    cur = cur->left; // 左孩子接替自己返回
                }
                else
                { // 左右孩子都有的情况，让后继节点代替自己
                    SBTNode<K> *pre = nullptr;
                    SBTNode<K> *last_node = cur->right;
                    last_node->size--;
                    while (last_node->left != nullptr)
                    {
                        pre = last_node;
                        last_node = last_node->left;
                        last_node->size--;
                    }

                    // 如果pre为nullptr，说明后继节点就是cur的右指针
                    // 则不需要再调整后继节点本身的情况了
                    if (pre != nullptr)
                    {
                        pre->left = last_node->right;  // 让后继节点的右孩子代替它
                        last_node->right = cur->right; // 将后继节点代替自己的位置，right。 如果后继节点本身就是cur的右孩子，则不需要做任何操作
                    }

                    last_node->left = cur->left; // 将后继节点代替自己的位置，left

                    // 更新新头部的size
                    last_node->size = 1 +
                                      (last_node->left ? last_node->left->size : 0) + (last_node->right ? last_node->right->size : 0);
                    cur = last_node; // 新头部，彻底舍弃node了。
                    delete cur_tmp;
                }
            }
            // 维持平衡---可以不用维持平衡，在add的时候再维护
            // node = maintain(node);
            return cur;
        }

        // 返回等于key的，或者key的父节点。
        SBTNode<K> *search(K &key)
        {
            SBTNode<K> *cur = root, *pre = root;

            while (cur != nullptr)
            {
                pre = cur;
                if (key.compare(cur->key) == 0)
                {
                    break;
                }
                else if (key.compare(cur->key) < 0)
                { // 左滑
                    cur = cur->left;
                }
                else
                {
                    cur = cur->right;
                }
            }
            return pre; // 返回最近的不空的节点，有可能是key，也有可能是key的父节点
        }

        SBTNode<K> *maintain(SBTNode<K> *cur)
        {
            if (cur == nullptr)
            {
                return nullptr;
            }
            // 计算cur的下一级节点数和 下下一级节点数
            int left_size = cur->left != nullptr ? cur->left->size : 0;
            int right_size = cur->right != nullptr ? cur->right->size : 0;
            int left_left_size = (cur->left != nullptr && cur->left->left != nullptr)
                                     ? cur->left->left->size
                                     : 0;
            int left_right_size = (cur->left != nullptr && cur->left->right != nullptr)
                                      ? cur->left->right->size
                                      : 0;
            int right_right_size = (cur->right != nullptr && cur->right->right != nullptr)
                                       ? cur->right->right->size
                                       : 0;
            int right_left_size = (cur->right != nullptr && cur->right->left != nullptr)
                                      ? cur->right->left->size
                                      : 0;
            // cout << "=====" << endl;
            // cout << "cur->key: " << cur->key.value << " cur_size:" << cur->size << endl;
            // cout << "left_size: " << left_size << ", right_size:" << right_size << endl;
            // cout << "left_left_size:" << left_left_size << "right_right_size:" << right_right_size << endl;
            // cout << "left_right_size:" << left_right_size << "right_left_size:" << right_left_size << endl;

            // 左孙子比自己的右孩子大，LL型
            if (left_left_size > right_size)
            {
                cur = right_rotate(cur);           // 右旋
                cur->right = maintain(cur->right); // 先调整cur下级节点的平衡
                cur = maintain(cur);               // 最后调整上方节点的平衡性
            }
            else if (right_right_size > left_size)
            {                                    // 优先LL和RR
                cur = left_rotate(cur);          // 左旋
                cur->left = maintain(cur->left); // 先调整cur下级节点的平衡
                cur = maintain(cur);             // 最后调整上方节点的平衡性
            }
            else if (left_right_size > right_size)
            {                                       // LR型
                cur->left = left_rotate(cur->left); // 左子树先左旋
                cur = right_rotate(cur);            // 再右旋
                cur->left = maintain(cur->left);    // 注意这里cur已经是左右旋后的新头部了。
                cur->right = maintain(cur->right);  // 先维持node的下级节点
                cur = maintain(cur);
            }
            else if (right_left_size > left_size)
            {                                          // RL型
                // cout << "rl" << endl;
                cur->right = right_rotate(cur->right); // 右子节点先右旋
                cur = left_rotate(cur);                // 再左旋
                cur->right = maintain(cur->right);     // 这里先调右树还是左树都可以。
                cur->left = maintain(cur->left);
                cur = maintain(cur);
            }
            return cur;
        }

        // 基于cur节点做左旋，返回旋转后的原来位置的节点
        SBTNode<K> *left_rotate(SBTNode<K> *cur)
        {
            SBTNode<K> *newHead = cur->right; // 右节点代替自己
            cur->right = newHead->left;       // 右节点的左孩子变成了自己的右孩子
            newHead->left = cur;              // cur往左边沉下去
            newHead->size = cur->size;        // 新节点的大小等于老节点大小
            cur->size = (cur->left != nullptr ? cur->left->size : 0) + (cur->right != nullptr ? cur->right->size : 0) + 1;
            return newHead;
        }

        // 基于cur节点做右旋，返回旋转后的原来位置的节点
        SBTNode<K> *right_rotate(SBTNode<K> *cur)
        {
            SBTNode<K> *newHead = cur->left; // 左节点代替自己
            cur->left = newHead->right;      // 左节点的右孩子变成了自己的左孩子
            newHead->right = cur;            // cur往右边沉下去
            newHead->size = cur->size;       // 新节点的大小等于老节点大小
            cur->size = (cur->left != nullptr ? cur->left->size : 0) + (cur->right != nullptr ? cur->right->size : 0) + 1;
            return newHead; // 返回新头部
        }

        void mid_print(SBTNode<K> * cur)
        {
            if (cur == nullptr)
                return;
            mid_print(cur->left);
            cout << "key: " << cur->key.value << " idx:" << cur->key.value << endl;
            mid_print(cur->right);
        }

        SBTNode<K> *get_root()
        {
            return root;
        }

        int size()
        {
            return root ? root->size : 0;
        }

        // 返回等于key的查找的次数
        int search_count(K key)
        {
            SBTNode<K> *cur = root, *pre = root;
            int count = 0;

            while (cur != nullptr)
            {
                count++;
                pre = cur;
                if (key.compare(cur->key) == 0)
                {
                    break;
                }
                else if (key.compare(cur->key) < 0)
                { // 左滑
                    cur = cur->left;
                }
                else
                {
                    cur = cur->right;
                }
            }
            return count;
        }

        // 在root上找到第k小的数
        SBTNode<K> *get_index_node(int k)
        {
            SBTNode<K> *cur = root;
            while (cur != nullptr)
            {
                // cout << "cur_k:" << k << endl;
                // cout << "cur_key:" << cur->key.value << " cur_size:" << cur->size << endl;
                // cout << "cur_left_size:" << (cur->left ? cur->left->size : 0) << endl;
                // cout << "cur_right_size:" << (cur->right ? cur->right->size : 0) << endl;
                if (cur->left != nullptr && cur->left->size == k - 1)
                {
                    break;
                } else if (cur->left == nullptr && k == 1) {
                    break;
                } else if (cur->left != nullptr && cur->left->size >= k) {
                    // 左树上的节点比k大，则第k小的数一定在左子树上
                    // 这是还没有找到任何一个数，所以k不变
                    cur = cur->left;
                }
                else
                {
                    // 否则在右子树上。往右边寻找
                    // 注意这时左边已经找到了m个节点，在右边需要减去已经找到的节点
                    k = k - (cur->left ? cur->left->size : 0) - 1;
                    cur = cur->right;
                }
            }
            return cur;
        }

    private:
        SBTNode<K> *root;
        int all_size;
};

class Solution
{
public:
    vector<double> medianSlidingWindow(vector<int> &nums, int k)
    {
        // 滑动窗口
        // 需要这样一个结构，可以快速的添加和删除，也可以快速的返回topK（中位数）的数
        // 使用sb树
        vector<double> res;
        int len = nums.size();
        if (len == 0)
        {
            return res;
        }

        SizeBalanceTree<Key> sbt;
        int i=0;
        //先将k个元素放到sbt中
        while (i<k) {
            sbt.put(i, nums[i]);
            i++;
        }
        // sbt.mid_print(sbt.get_root());
        // cout << "here" << endl;


        int l = 0, r = k-1;
        //往右划动窗口
        while (r<len) {
            // cout << " l:" << l << " r:" << r << endl;
            int size = sbt.size();
            if (size %2 == 0) { //偶数情况
                int up_mid = size/2;
                int down_mid = up_mid+1;
                SBTNode<Key> * node1 = sbt.get_index_node(up_mid);
                SBTNode<Key> * node2 = sbt.get_index_node(down_mid);
                // cout << " node1:" << node1 << " node2:" << node2 << endl;
                // cout << " node1:" << node1->key.value << " node2:" << node2->key.value << endl;
                res.push_back(node1->key.value+(node2->key.value+0.0-node1->key.value)/2.0); //防止越界
            } else {
                SBTNode<Key> *node1 = sbt.get_index_node(size/2+1);
                // cout << " node1:" << node1->key.value << endl;
                res.push_back(node1->key.value);
            }
            Key tmp_key(l, nums[l]);
            sbt.remove(tmp_key);
            l++;
            r++;
            if (r<len) {
                sbt.put(r, nums[r]);
            }
        }
        return res;
    }
};
// https://leetcode.cn/problems/sliding-window-median/

/*
给出 nums = [1,3,-1,-3,5,3,6,7]，以及 k = 3。

窗口位置                      中位数
---------------               -----
[1  3  -1] -3  5  3  6  7       1
 1 [3  -1  -3] 5  3  6  7      -1
 1  3 [-1  -3  5] 3  6  7      -1
 1  3  -1 [-3  5  3] 6  7       3
 1  3  -1  -3 [5  3  6] 7       5
 1  3  -1  -3  5 [3  6  7]      6
 因此，返回该滑动窗口的中位数数组 [1,-1,-1,3,5,6]。


*/
int main() {
    Solution sol;
    
    // vector<int> nums = {1,3,-1,-3,5,3,6,7};
    vector<int> nums = {7, 0, 3, 9, 9, 9, 1, 7, 2, 3};
    // vector<int> nums = {9,9,9,7,1};
    int k = 6;
    vector<double> res = sol.medianSlidingWindow(nums, k);
    for (int i=0;i<res.size();i++) {
        cout << res[i] << " ";
    }
    return 0;
}