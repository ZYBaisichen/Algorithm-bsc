/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-26 14:59:27
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
给你一个由非负整数组成的数组 nums 。另有一个查询数组 queries ，其中 queries[i] = [xi, mi] 。

第 i 个查询的答案是 xi 和任何 nums 数组中不超过 mi 的元素按位异或（XOR）得到的最大值。换句话说，答案是 max(nums[j] XOR xi) ，其中所有 j 均满足 nums[j] <= mi 。如果 nums 中的所有元素都大于 mi，最终答案就是 -1 。

返回一个整数数组 answer 作为查询的答案，其中 answer.length == queries.length 且 answer[i] 是第 i 个查询的答案。



示例 1：

输入：nums = [0,1,2,3,4], queries = [[3,1],[1,3],[5,6]]
输出：[3,3,7]
解释：
1) 0 和 1 是仅有的两个不超过 1 的整数。0 XOR 3 = 3 而 1 XOR 3 = 2 。二者中的更大值是 3 。
2) 1 XOR 2 = 3.
3) 5 XOR 2 = 7.
示例 2：

输入：nums = [5,2,4,6,6,3], queries = [[12,4],[8,1],[6,3]]
输出：[15,-1,5]

1 <= nums.length, queries.length <= 105
queries[i].length == 2
0 <= nums[j], xi, mi <= 109


测试链接 :中文的运行测试不好使，英文的测试可以
英文：https://leetcode.com/problems/maximum-xor-with-an-element-from-array/submissions/1214185423/
中文：https://leetcode.cn/problems/maximum-xor-with-an-element-from-array/submissions/516779137/
*/

class num_trie
{
public:
    struct Node
    {
        Node *next[2];
        int min; //记录能够到达当前节点的最小值
        Node()
        {
            next[0] = nullptr;
            next[1] = nullptr;
            min = INT_MAX;
        }
    };
    Node *head;
    num_trie()
    {
        head = new Node();
    }

    ~num_trie() {
        //不好释放
    }
    void add(int num)
    {
        Node *cur = head;
        head->min = min(head->min, num);
        for (int move = 31; move >= 0; move--)
        {
            int path = (num >> move) & 1; // 从左到右拿出来每一位
            if (cur->next[path] == nullptr)
            {
                cur->next[path] = new Node();
            }
            cur->next[path]->min = min(cur->next[path]->min, num);
            cur = cur->next[path];
        }
    }

    // 之前收集了很多数字(前缀和)
    // 当前的前缀和，和前面出现过的前缀和，看怎么样能异或到最大
    // 比如当前算到了[0..i]的前缀和。如果[0...j]的前缀异或和 与其相异或出来了最大值，则数组为[j...i]
    int max_xor(int cur_num, int aim)
    {
        if (head->min > aim) {
            return -1;
        }
        // cout << "max_xor start" << endl;
        Node *cur = head;
        int ans = 0;
        for (int move = 31; move >= 0 && cur != nullptr; move--)
        {
            // 去除当前sum的高位状态
            int path = (cur_num >> move) & 1;
            // cout << "cur_path:" << path << " move:" << move << endl;
            // 最高位如果是1的话为负数，期望需要1，异或成0变成正数; 如果高位为0，期望遇到0。最高位期望遇到相同的数字
            // 其他位置期望遇到和当前位置相反的数字
            int best = move == 31 ? path : (path ^ 1);
            // cout << "best:" << best << endl;
            // 实际在前缀树上的数字
            if (cur->next[best] != nullptr || cur->next[best ^ 1] != nullptr) {
                // 如果cur->next[best]->min > aim也不可能走到bes路径
                best = (cur->next[best] == nullptr||cur->next[best]->min > aim) ? (best ^ 1) : best;
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
    // // 初始方法
    // vector<int> maximizeXor(vector<int> &nums, vector<vector<int>> &queries)
    // {
    //     vector<int> res;
    //     int len = queries.size();
    //     if (len == 0)
    //     {
    //         return res;
    //     }

    //     for (int i = 0; i < len; i++)
    //     {
    //         vector<int> &cur_que = queries[i];
    //         num_trie nt;
    //         // 每次将aim建立前缀树
    //         int flag = false;
    //         for (int j = 0; j < nums.size(); j++)
    //         {
    //             if (nums[j] <= cur_que[1])
    //             {
    //                 flag=true;
    //                 nt.add(nums[j]);
    //             }
    //         }

    //         // 根据前缀树中输出最大的异或和
    //         if(flag) {
    //             res.push_back(nt.max_xor(cur_que[0]));
    //         } else {
    //             res.push_back(-1);
    //         }
    //     }
    //     return res;
    // }


    // 优化下前缀树
    vector<int> maximizeXor(vector<int> &nums, vector<vector<int>> &queries)
    {
        vector<int> res;
        int len = queries.size();
        if (len == 0)
        {
            return res;
        }
        num_trie nt;
        for (int j = 0; j < nums.size(); j++) {
            nt.add(nums[j]);
        }

        for (int i = 0; i < len; i++)
        {
            vector<int> &cur_que = queries[i];
            int tmp = nt.max_xor(cur_que[0], cur_que[1]);
            res.push_back(tmp);
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
int main()
{

    /*
    1011
    0001
    1111
    0101
    1111
    */
    vector<int> nums = {5, 2, 4, 6, 6, 3};
    vector<vector<int>> queries = { {12, 4},
                                    {8, 1},
                                    {6, 3} };
    // vector<int> nums = {0,1,2,3,4};
    // vector<vector<int>> queries = {{3,1},{1,3},{5,6}};
    Solution s;
    vector<int> res =  s.maximizeXor(nums, queries);

    for (auto iter : res)
    {
        cout << iter << ",";
    }
    cout << endl;

    // vector<int> nums = {47,21,31,25,9,27,17,6};
    // vector<int> nums = {47, 21};
    // Solution s;
    // cout << s.max_xor1(nums) << endl;
    // cout << s.max_xor2(nums) << endl;
    // cout << s.max_xor3(nums) << endl;

    return 0;
}