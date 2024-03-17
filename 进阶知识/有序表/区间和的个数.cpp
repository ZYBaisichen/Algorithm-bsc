/*
 * @Author: baisichen
 * @Date: 2024-03-16 13:22:49
 * @LastEditTime: 2024-03-17 17:33:33
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

class SBTNode
{
public:
    long long key;
    int size;
    int all;
    SBTNode *left;
    SBTNode *right;
    SBTNode(long long _key)
    {
        key = _key;
        left = nullptr;
        right = nullptr;
        size = 1; // 创建时当前节点树就一个节点，所以大小为1
        all = 1;
    }
};

class SizeBalanceTree
{
public:
    SizeBalanceTree()
    {
        root = NULL;
    }

    // （key, value） 加入到有序表，新增、修改value
    void put(long long key)
    {
        SBTNode *lastNode = search(key); // 找到key或其父节点
        bool have_add = false;
        if (lastNode != nullptr && key == lastNode->key)
        { // 之前已经加过
            have_add = true;
        }
        root = add(root, key, have_add);
    }

    SBTNode *add(SBTNode *cur, long long key, bool have_add)
    {
        if (cur == nullptr)
        { // 找到了空位置，插入进去，返回当前的头部
            cur = new SBTNode(key);
            return cur;
        }
        // 沿途的all++
        cur->all++;
        // 滑到了需要找的key，直接返回
        if (cur->key == key)
        {
            return cur;
        }

        // 只有在没加过这个节点的时候才会让key的size++
        if (!have_add)
        {
            cur->size++;
        }
        // cout << "cur:" << cur->key  << ":" << cur->size << endl;
        if (key < cur->key)
        { // 左滑
            cur->left = add(cur->left, key, have_add);
        }
        else if (key > cur->key)
        { // 右滑
            cur->right = add(cur->right, key, have_add);
        }

        // 左右子树都更新完毕，开始调整平衡性
        // 从下往上调整平衡性。
        // 返回值是调整之后的新的头部
        return maintain(cur);
    }

    bool contains_key(long long key)
    {
        SBTNode *lastNode = search(key);
        if (lastNode != nullptr && key == lastNode->key)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // 返回小于key的所有数
    int less_key_size(long long key)
    {
        SBTNode *cur = root;
        int ans = 0;
        while (cur != nullptr)
        {
            if (key == cur->key)
            {
                if (cur->left)
                {
                    ans += cur->left->all;
                }
                break;
            }
            else if (cur->key < key)
            {
                ans += (cur->all - (cur->right ? cur->right->all : 0));
                cur = cur->right;
            }
            else {
                cur = cur->left;
            }
        }
        return ans;
    }

    // 返回等于key的，或者key的父节点。
    SBTNode *search(long long key)
    {
        SBTNode *cur = root, *pre = root;

        while (cur != nullptr)
        {
            pre = cur;
            if (key == cur->key)
            {
                break;
            }
            else if (key < cur->key)
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

    SBTNode *maintain(SBTNode *cur)
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
        // cout << "cur->key: " << cur->key << endl;
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
            cur->right = right_rotate(cur->right); // 右子节点先右旋
            cur = left_rotate(cur);                // 再左旋
            cur->right = maintain(cur->right);     // 这里先调右树还是左树都可以。
            cur->left = maintain(cur->left);
            cur = maintain(cur);
        }
        return cur;
    }

    // 基于cur节点做左旋，返回旋转后的原来位置的节点
    SBTNode *left_rotate(SBTNode *cur)
    {
        int cur_node_num = cur->all - (cur->left != nullptr ? cur->left->all : 0) - (cur->right != nullptr ? cur->right->all : 0);
        SBTNode *newHead = cur->right; // 右节点代替自己
        cur->right = newHead->left;    // 右节点的左孩子变成了自己的右孩子
        newHead->left = cur;           // cur往左边沉下去
        newHead->size = cur->size;     // 新节点的大小等于老节点大小
        newHead->all = cur->all;
        cur->size = (cur->left != nullptr ? cur->left->size : 0) + (cur->right != nullptr ? cur->right->size : 0) + 1;
        cur->all = (cur->left != nullptr ? cur->left->all : 0) + (cur->right != nullptr ? cur->right->all : 0) + cur_node_num;

        return newHead;
    }

    // 基于cur节点做右旋，返回旋转后的原来位置的节点
    SBTNode *right_rotate(SBTNode *cur)
    {
        int cur_node_num = cur->all - (cur->left != nullptr ? cur->left->all : 0) - (cur->right != nullptr ? cur->right->all : 0);
        SBTNode *newHead = cur->left; // 左节点代替自己
        cur->left = newHead->right;   // 左节点的右孩子变成了自己的左孩子
        newHead->right = cur;         // cur往右边沉下去
        newHead->size = cur->size;    // 新节点的大小等于老节点大小
        newHead->all = cur->all;
        cur->size = (cur->left != nullptr ? cur->left->size : 0) + (cur->right != nullptr ? cur->right->size : 0) + 1;
        cur->all = (cur->left != nullptr ? cur->left->all : 0) + (cur->right != nullptr ? cur->right->all : 0) + cur_node_num;

        return newHead; // 返回新头部
    }

    void mid_print(SBTNode *cur)
    {
        if (cur == nullptr)
            return;
        mid_print(cur->left);
        // cout << "key: " << cur->key << endl;
        mid_print(cur->right);
    }

    SBTNode *get_root()
    {
        return root;
    }

    // 返回等于key的查找的次数
    int search_count(long long key)
    {
        SBTNode *cur = root, *pre = root;
        int count = 0;

        while (cur != nullptr)
        {
            count++;
            pre = cur;
            if (key == cur->key)
            {
                break;
            }
            else if (key < cur->key)
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

private:
    SBTNode *root;
};

// https://leetcode.cn/problems/count-of-range-sum/
/*
// 假设需要计算子数组累加和在[10,60]范围内，在以 i位置 作为子数组的末尾时，有多少子数组满足条件？依次以
     * 0位置、1位置、2位置...最后位置 结尾。依次枚举
//假设遍历到i时累加和为sum=100，要想找[10,30]区间内的数，其实就是找i前面的区间在[100-30, 100-10]区间内的累加和有多少个。
    利用有序表，每表里到一个位置，就将累加和加入到有序表中。然后查看小于sum-lower + 1的数b，还有累加和小于sum-upper的数a
    计算b-a即可求出来在[sum-upper, sum-lower]中的子数组数量
*/
class Solution
{
public:
    int countRangeSum(vector<int> &nums, int lower, int upper)
    {
        int len = nums.size();
        long long sum = 0;
        int ans = 0;
        // 根据分析，需要有个结构，能够快速查找到小于某个sum的子数组累加和数量
        // 所需要的功能有：添加数据、查询小于sum的个数，还需要支持重复值的插入
        SizeBalanceTree sbt;
        sbt.put(sum); // 先将累加和为0放进去
        for (int i=0; i < len; i++)
        {
            sum += nums[i];
            long long left = sum - upper;
            long long right = sum - lower + 1;
            // cout << "left:" << left << " right:" << right << endl;
            long long a = sbt.less_key_size(left);
            long long b = sbt.less_key_size(right);
            // cout << "a:" << a << " b:" << b << endl;
            ans += (int)(b - a);
            sbt.put(sum);
        }
        return ans;
    }
};

int main()
{
    Solution sol;
    // vector<int> nums = {-2,5,-1};
    // int lower = -2;
    // int upper = 2;
    vector<int> nums = {0};
    int lower = 0;
    int upper = 0;

    cout << sol.countRangeSum(nums, lower, upper) << endl;
    return 0;
}