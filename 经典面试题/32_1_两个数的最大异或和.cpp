/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-26 14:25:27
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

/*
给你一个整数数组 nums ，返回 nums[i] XOR nums[j] 的最大运算结果，其中 0 ≤ i ≤ j < n 。

 

示例 1：

输入：nums = [3,10,5,25,2,8]
输出：28
解释：最大运算结果是 5 XOR 25 = 28.
示例 2：

输入：nums = [14,70,53,83,49,91,36,80,92,51,66,70]
输出：127
https://leetcode.cn/problems/maximum-xor-of-two-numbers-in-an-array/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china
*/

class num_trie
{
public:
    struct Node
    {
        Node * next[2];
        Node()
        {
            next[0] = nullptr;
            next[1] = nullptr;
        }
    };
    Node *head;
    num_trie()
    {
        head = new Node();
    }
    void add(int num)
    {
        Node *cur = head;
        for (int move = 31; move >= 0; move--)
        {
            int path = (num >> move) & 1; // 从左到右拿出来每一位
            if (cur->next[path] == nullptr)
            {
                cur->next[path] = new Node();
            }
            cur = cur->next[path];
        }
    }


    // 之前收集了很多数字(前缀和)
    // 当前的前缀和，和前面出现过的前缀和，看怎么样能异或到最大
    // 比如当前算到了[0..i]的前缀和。如果[0...j]的前缀异或和 与其相异或出来了最大值，则数组为[j...i]
    int max_xor(int cur_num)
    {
        // cout << "max_xor start" << endl;
        Node *cur = head;
        int ans = 0;
        for (int move = 31; move >= 0 && cur != nullptr; move--)
        {
            // 拿到第move位的状态
            int path = (cur_num >> move) & 1;
            // cout << "cur_path:" << path << " move:" << move << endl;
            // 最高位如果是1的话为负数，期望需要1，异或成0变成正数; 如果高位为0，期望遇到0。最高位期望遇到相同的数字
            // 其他位置期望遇到和当前位置相反的数字
            int best = move == 31 ? path : (path ^ 1);
            // cout << "best:" << best << endl;
            // 实际在前缀树上的数字
            if (cur->next[best] != nullptr || cur->next[best ^ 1] != nullptr)
            {
                best = cur->next[best] == nullptr ? (best ^ 1) : best;
                // cout << "best:" << best << endl;

                ans |= (path ^ best) << move;
            }
            cur = cur->next[best];
        }
        // cout << "max_xor end" << endl;
        return ans;
    }
};

class Solution
{
public:

    int findMaximumXOR(vector<int> &nums)
    {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        num_trie nt;
        nt.add(nums[0]);
        int res = 0;
        for (int i = 1; i < len; i++)
        {
            res = max(res, nt.max_xor(nums[i]));
            nt.add(nums[i]);
        }
        return res;
    }
};

void generate_random_array(int max_size, int max_value, vector<int> &arr)
{
    int size = (rand() % max_size) + 1;
    for (int i = 0; i < size; i++)
    {
        arr.push_back(rand() % max_value);
    }
}

void print(vector<int> &arr)
{
    for (auto iter : arr)
    {
        cout << iter << ",";
    }
    cout << endl;
}
int main(){

    /*
    1011
    0001
    1111
    0101
    1111
    */
    vector<int> nums = {3,10,5,25,2,8};
Solution s;
cout << s.findMaximumXOR(nums) << endl;

// vector<int> nums = {47,21,31,25,9,27,17,6};
// vector<int> nums = {47, 21};
// Solution s;
// cout << s.max_xor1(nums) << endl;
// cout << s.max_xor2(nums) << endl;
// cout << s.max_xor3(nums) << endl;

return 0;
}