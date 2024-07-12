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

/*
https://leetcode.cn/problems/falling-squares/description/
positions = [[1,2],[2,3],[6,1]]
输出：[2,5,5]

在二维平面上的 x 轴上，放置着一些方块。

给你一个二维整数数组 positions ，其中 positions[i] = [lefti, sideLengthi] 表示：第 i 个方块边长为 sideLengthi ，其左侧边与 x 轴上坐标点 lefti 对齐。

每个方块都从一个比目前所有的落地方块更高的高度掉落而下。方块沿 y 轴负方向下落，直到着陆到 另一个正方形的顶边 或者是 x 轴上 。一个方块仅仅是擦过另一个方块的左侧边或右侧边不算着陆。一旦着陆，它就会固定在原地，无法移动。

在每个方块掉落后，你必须记录目前所有已经落稳的 方块堆叠的最高高度 。

返回一个整数数组 ans ，其中 ans[i] 表示在第 i 块方块掉落后堆叠的最高高度。

 

示例 1：


输入：positions = [[1,2],[2,3],[6,1]]
输出：[2,5,5]
解释：
第 1 个方块掉落后，最高的堆叠由方块 1 组成，堆叠的最高高度为 2 。
第 2 个方块掉落后，最高的堆叠由方块 1 和 2 组成，堆叠的最高高度为 5 。
第 3 个方块掉落后，最高的堆叠仍然由方块 1 和 2 组成，堆叠的最高高度为 5 。
因此，返回 [2, 5, 5] 作为答案。
示例 2：

输入：positions = [[100,100],[200,100]]
输出：[100,100]
解释：
第 1 个方块掉落后，最高的堆叠由方块 1 组成，堆叠的最高高度为 100 。
第 2 个方块掉落后，最高的堆叠可以由方块 1 组成也可以由方块 2 组成，堆叠的最高高度为 100 。
因此，返回 [100, 100] 作为答案。
注意，方块 2 擦过方块 1 的右侧边，但不会算作在方块 1 上着陆。
 

提示：

1 <= positions.length <= 1000
1 <= lefti <= 108
1 <= sideLengthi <= 106

*/
class SegmentTree
{
public:
    int length;
    vector<int> height;
    vector<bool> has_update;
    vector<int> change;

    SegmentTree(int size)
    {
        length = size + 1;
        height.resize(length << 2, 0);
        has_update.resize(length << 2, false);
        change.resize(length << 2, 0);
    }
    void print_height(string name) {
        cout << name << ":" <<endl;
        for (int i=0;i<height.size();i++) {
            cout << height[i] << " ";
        }
        cout << endl;
    }
    void update(int L, int R, int C, int l, int r, int rt)
    {
        // cout << "update:" << L << "," << R << "," << C << "," << l << "," << r << "," << rt << endl;
        if (L <= l && r <= R)
        {
            height[rt] = C;
            change[rt] = C;
            has_update[rt] = true;
            return;
        }
        int mid = (l + r) >> 1;
        push_down(rt, mid - l + 1, r - mid);
        if (L <= mid)
        {
            update(L, R, C, l, mid, rt << 1);
        }
        if (R >= (mid + 1))
        {
            update(L, R, C, mid + 1, r, rt << 1 | 1);
        }
        push_up(rt);
    }
    int query(int L, int R, int l, int r, int rt)
    {
        // cout << "query:" << L << "," << R << "," << l << "," << r << "," << rt << endl;
        if (L <= l && r <= R)
        {
            return height[rt];
        }

        int mid = (l + r) >> 1;
        push_down(rt, mid - l + 1, r - mid);
        int res = 0;
        if (L <= mid)
        {
            res = max(res, query(L, R, l, mid, rt << 1));
        }
        if (R >= (mid + 1))
        {
            res = max(res, query(L, R, mid + 1, r, rt << 1 | 1));
        }
        return res;
    }

    void push_up(int rt)
    {
        height[rt] = max(height[rt << 1], height[rt << 1 | 1]);
    }
    void push_down(int rt, int lnum, int rnum)
    {
        if (has_update[rt])
        {
            has_update[rt << 1] = true;
            has_update[rt << 1 | 1] = true;
            change[rt << 1] = change[rt];
            change[rt << 1 | 1] = change[rt];
            height[rt << 1] = change[rt];
            height[rt << 1 | 1] = change[rt];
            has_update[rt] = false;
        }
    }
};

class Solution
{
public:
    void get_pos_compression(vector<vector<int>> &positions, map<int, int> &pos_id)
    {
        int n = positions.size();
        set<int> tmp_set;
        for (int i = 0; i < n; i++)
        {
            tmp_set.insert(positions[i][0]);
            tmp_set.insert(positions[i][0] + positions[i][1] - 1);
        }

        int idx_count = 1;
        for (auto iter : tmp_set)
        {
            pos_id[iter] = idx_count;
            idx_count++;
        }
    }

    vector<int> fallingSquares(vector<vector<int>> &positions)
    {
        int len = positions.size();
        vector<int> res;
        if (len == 0)
        {
            return res;
        }
        // 离散化
        map<int, int> pos_id;
        get_pos_compression(positions, pos_id);
        // for (auto iter : pos_id)
        // {
        //     cout << "key: " << iter.first << ", value:" << iter.second << endl;
        // }

        int n = pos_id.size();
        SegmentTree st(n);
        for (int i = 0; i < len; i++)
        {
            int L = pos_id[positions[i][0]];
            int R = pos_id[positions[i][0] + positions[i][1] - 1];
            // cout << "L:" << L << ", R:" << R << endl;
            int max_h = st.query(L, R, 1, n, 1);
            // cout << "max_h:" << max_h << endl;
            int cur_h = max_h + positions[i][1];
            st.update(L, R, cur_h, 1, n, 1);
            // st.print_height("after update");
            int max_h_all = st.query(1, n, 1, n, 1);
            res.push_back(max_h_all);
        }
        return res;
    }
};

int main(){
    Solution sol;
    vector<vector<int>> a = {{4, 6}, {4, 2}, {4, 3}};
    vector<int> res = sol.fallingSquares(a);
    cout << "res:";
    for (auto iter:res) {
        cout << iter << " ";
    }
    cout << endl;
}