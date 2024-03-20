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

using namespace std;

/*
 // 二维接雨水
// 给你一个 m * n 的矩阵，其中的值均为非负整数，代表二维高度图每个单元的高度
// 请计算图中形状最多能接多少体积的雨水。
// 测试链接 : https://leetcode.cn/problems/trapping-rain-water-ii/
*/

class Node {
public:
    int num;
    int row;
    int col;
    Node(int _num, int _row, int _col) {
        num = _num;
        row = _row;
        _col = col;
    }
};
class Solution
{
public:
    /*
    整体流程：
    1. 组织有一个小根堆，node为num,row,col三个元素，按照num排序
    2. 将所有边缘处的点都入到小根堆
    3. 从小根堆弹出一个数，num，维护一个max代表所有弹出的最大值。
    4. 将弹出的数上下左右都入小根堆，前提是维护一个enter数组，入过小根堆的不会重复进入。
    5. 上下左右入小根堆之前，如果其值比max小，则更新总水量。
    6. 循环往复处理

    具体含义：
    1. 边缘点入小根堆后，弹出的一定是边缘最薄弱的地方，后面如果连着一片湖，都会以该数做瓶颈。
    2. 如果弹出的值没有更新max，则当前弹出的数连在了之前max弹出的这片湖里。
    3. 当max被更新了，说明转了一片湖。


    每个点都需要出和入小根堆一次O(N*M)， 假设小根堆最大能达到K(最大N*M), 所以总复杂度是O(N*M*long(k))
    */
    int trapRainWater(vector<vector<int>>& heightMap) {
        int n = heightMap.size();
        if (n==0) {
            return 0;
        }
        int m = heightMap[0].size();
        if (m==0) {
            return 0;
        }

        auto cmp = [](Node* a, Node* b) { return a->num > b->num; }; //按照num组织小顶堆
        // auto cmp_big = [](project* a, project* b) { return a->_profit < b->_profit; }; //按照利润组织大顶堆
        priority_queue<Node*, vector<Node*>, decltype(cmp)> min_heap(cmp);
        // priority_queue<project*, vector<project*>, decltype(cmp_big)> max_heap(cmp_big);

        vector<vector<bool>> visited(n, vector<bool>(m, false));
        //上边缘
        for (int j=0;j<=m-2;j++) {
            min_heap.push(new Node(heightMap[0][j], 0, j));
            visited[0][j] = true;
        }
        //右边缘
        for (int i=0;i<=n-2;i++) {
            min_heap.push(new Node(heightMap[i][m-1], i, m-1));
            visited[i][m-1] = true;
        }
        //下边缘
        for (int j=m-1;j>=1;j--) {
            min_heap.push(new Node(heightMap[n-1][j], n-1, j));
            visited[n-1][j] = true;
        }
        //左边缘
        for (int i=n-1;i>=1;i--) {
            min_heap.push(new Node(heightMap[i][0], i, 0));
            visited[i][0] = true;
        }

        

        int cur_max = 0;
        int water = 0;
        while (!min_heap.empty()) {
            Node* cur = min_heap.top();
            min_heap.pop();
            cur_max = max(cur_max, cur->num);

            //上下左右入堆
            int row = cur->row;
            int col = cur->col;
            //上
            if (row - 1 >= 0) {
                water += max(cur_max-heightMap[row-1][col], 0);//如果要入堆的高度比max小，则更新water
                if (!visited[row-1][col]) {
                    min_heap.push(new Node(heightMap[row-1][col], row-1, col));
                    visited[row-1][col] = true;
                }
            }
            //下
            if (row + 1 < n) {
                water += max(cur_max-heightMap[row+1][col], 0);//如果要入堆的高度比max小，则更新water
                if (!visited[row+1][col]) {
                    min_heap.push(new Node(heightMap[row+1][col], row+1, col));
                    visited[row+1][col] = true;
                }
            }
            //左
            if (col - 1 >= 0) {
                water += max(cur_max-heightMap[row][col - 1], 0);//如果要入堆的高度比max小，则更新water
                if (!visited[row][col - 1]) {
                    min_heap.push(new Node(heightMap[row][col - 1], row, col - 1));
                    visited[row][col - 1] = true;
                }
            }
            //右
            if (col + 1 < m) {
                water += max(cur_max-heightMap[row][col + 1], 0);//如果要入堆的高度比max小，则更新water
                if (!visited[row][col + 1]) {
                    min_heap.push(new Node(heightMap[row][col + 1], row, col + 1));
                    visited[row][col + 1] = true;
                }
            }
            
        }
        return water;
    }
};

int main()
{
    Solution sol;
    // vector<int> arr = {3,1,2,5,2,4};
    vector<int> arr = {4,5,1,3,2};
    cout << "res:" << sol.calcVigorousWater2(arr) << endl;
    return 0;
}