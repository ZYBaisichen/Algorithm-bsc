/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-27 11:05:57
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
描述
给定两个有序数组arr1和arr2，再给定一个整数k，返回来自arr1和arr2的两个数相加和最大的前k个，两个数必须分别来自两个数组
按照降序输出
[要求]
时间复杂度为
�
(
�
log
⁡
�
)
O(klogk)
输入描述：
第一行三个整数N, K分别表示数组arr1, arr2的大小，以及需要询问的数
接下来一行N个整数，表示arr1内的元素
再接下来一行N个整数，表示arr2内的元素
输出描述：
输出K个整数表示答案
示例1
输入：
5 4
1 2 3 4 5
3 5 7 9 11
输出：
16 15 14 14

https://www.nowcoder.com/practice/7201cacf73e7495aa5f88b223bbbf6d1

*/
struct Node
{
    int arr1_idx;
    int arr2_idx;
    int sum;
    Node(int i1, int i2, int _sum)
    {
        arr1_idx = i1;
        arr2_idx = i2;
        sum = _sum;
    }
};
struct Compare
{ // 大跟堆
    bool operator()(const Node a, const Node b) const
    {
        return a.sum < b.sum;
    }
};
//下面的解法在acm时会超内存，后面可以看看怎么优化内存
int main()
{
    int n, k;
    cin >> n >> k;
    vector<int> arr1, arr2;

    int tmp;
    for (int i = 0; i < n; i++)
    {
        cin >> tmp;
        arr1.push_back(tmp);
    }
    for (int i = 0; i < n; i++)
    {
        cin >> tmp;
        arr2.push_back(tmp);
    }

    // int k = 4, n = 5;
    // vector<int> arr1 = {1, 2, 3, 4, 5};
    // vector<int> arr2 = {3, 5, 7, 9, 11};

    int topK = min(k, n * n);

    vector<int> res(topK, 0);
    int res_idx = 0;

    bool **visited = new bool *[n];
    for (int i = 0; i < n; i++)
    {
        visited[i] = new bool[n];
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            visited[i][j] = false;
        }
    }

    std::priority_queue<Node, std::vector<Node>, Compare> _max_priority_queue;
    _max_priority_queue.push(Node(n - 1, n - 1, arr1[n - 1] + arr2[n - 1]));
    visited[n - 1][n - 1] = true;

    /*
        arr1做行，arr2做列。最大值一定是右下角的值。
        1. 首先将右下角值放入大跟堆
        2. 每从大跟堆中弹出一个数，将其上方和左方的num加和放入入大根堆
    */

    while (!_max_priority_queue.empty() && res_idx < topK)
    {
        Node tmp = _max_priority_queue.top();
        _max_priority_queue.pop();
        res[res_idx++] = tmp.sum;

        int cur1 = tmp.arr1_idx;
        int cur2 = tmp.arr2_idx;
        // cout << "cur1:" << cur1 << " cur2:" << cur2 << endl;

        // 因为一个格子即可能从下面过来，也可能从右边过来，会重复，所以采用visited数组去重
        // 上面的格子
        if (cur1 - 1 >= 0 && !visited[cur1 - 1][cur2])
        {
            _max_priority_queue.push(Node(cur1 - 1, cur2, arr1[cur1 - 1] + arr2[cur2]));
            visited[cur1 - 1][cur2] = true;
        }
        // 左边的格子
        if (cur2 - 1 >= 0 && !visited[cur1][cur2 - 1])
        {
            _max_priority_queue.push(Node(cur1, cur2 - 1, arr1[cur1] + arr2[cur2 - 1]));
            visited[cur1][cur2 - 1] = true;
        }
    }

    for (int i = 0; i < res.size(); i++)
    {
        if (i == 0)
        {
            cout << res[i];
        }
        else
        {
            cout << " " << res[i];
        }
    }
    cout << endl;
}
// 64 位输出请用 printf("%lld")