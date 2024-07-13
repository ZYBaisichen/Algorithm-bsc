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
#include <sstream>
#include <unordered_set>
#include <functional>
#include <algorithm>

using namespace std;

class Line {
public:
    int start;
    int end;
    Line(int _start, int _end) {
        start = _start;
        end = _end;
    }
};


/*
小根堆，里面存的数是之前有哪些线段对此时的线段产生影响，入的是线段的结尾。具体流程
a. 据开始位置排序。挨个线段处理，
b. 如果当前线段是[l,r]。将小根堆中<l的数字全部弹出，即将不可能和当前线段重合的线段都出去。
c. 然后将r入到堆中，堆中的元素数量即是当前线段的可以重合的线段数量。

使用线段树的解法：
1. 每个线段在x轴上平铺开。开始位置和结束位置期间的数都可以再y轴上累加1。
2. 线段树记录累加和。最后看每个页节点上的值，取最大。
3. 每次累加是O(log(n))，每次查询叶节点log(n)，总复杂度是O(n*log(n))
*/
//只需要add的线段树, 
class SegmentTree {
public:
    int len;
    vector<int> lazy;
    vector<int> sum;
    SegmentTree(int len) {
        lazy.resize(len<<2, 0);
        sum.resize(len << 2, 0);
    }

    void print(string suff) const{
        cout << suff << endl;
        cout << "lazy:";
        for (int i=0;i<lazy.size();++i) {
            cout << lazy[i] << " ";
        }
        cout << endl;
        cout << "sum:";
        for (int i=0;i<sum.size();++i) {
            cout << sum[i] << " ";
        }
        cout << endl;
    }
    
    //在L...R范围上每个位置加1，当前在表示范围为l...r的节点上
    void add(int L, int R, int l, int r, int rt) {
        // cout << "add L:"<< L << " R:"<< R << " l:" << l << " r:"<< r <<" rt:"<< rt  << " cur_lazy:" << lazy[rt]<< endl;
        //L..R可以完全包含l..r，lazy住
        if (l>=L && r <= R) {
            sum[rt] += (r - l + 1);
            lazy[rt] += 1;
            // cout << "sum[rt]:" << sum[rt] << endl;
            return;
        }
        //没有完全包含，不能懒了，需要将当前的懒信息下发到节点上
        int mid = (l+r) >> 1;
        push_down(rt, mid-l+1, r-mid);
        // print("after_push_down:");

        if (L<=mid) {
            add(L, R, l, mid, rt<<1);
        }
        if (R>=mid+1) {
            add(L, R, mid+1, r, rt<<1|1);
        }
        push_up(rt); //向上合并自己
    }

    int query(int target, int l, int r, int rt) {
        // cout << "query target:" << target << " l:" << l << " r:" << r << " rt:" << rt << " cur_lazy:" << lazy[rt] << endl;
        if (l==r && l == target) {
            // cout << "get_res:" << sum[rt] << endl;
            return sum[rt];
        }
        int mid = (l+r) >> 1;
        push_down(rt, mid-l+1, r-mid);
        //退化成二分查找
        if (target<=mid) { //在左边
            return query(target, l, mid, rt<<1);
        } else {
            return query(target, mid+1, r, rt<<1|1);
        }
    }
    /*
    l_num: 左孩子的节点数量
    r_num: 右孩子的节点数量
    */
    void push_down(int rt, int l_num, int r_num) {
        if (lazy[rt] != 0) {
            lazy[rt << 1] += lazy[rt];
            lazy[rt << 1 | 1] += lazy[rt];
            sum[rt<<1] += lazy[rt]*l_num;
            sum[rt << 1|1] += lazy[rt] * r_num;
            lazy[rt]=0;
        }
    }


    void push_up(int rt) {
        sum[rt] = sum[rt<<1] + sum[rt<<1|1];
    }
};
class Solution
{
    
public:
    // 自定义比较器，用于按照字符串长度排序 
    // a<b 返回true，标识a可以放在b前面 
    static bool compareByStart(const Line* a, const Line* b) {
        return a->start < b->start;
    }  

    int max_cover1(vector<vector<int>>& m) {
        vector<Line*> lines;
        // 小根堆！
        // 按照结尾组织
		std::function<bool(Line*, Line*)> comp_by_value = [](const Line *p1, const Line *p2) {
			return p1->end > p2->end;
		};
        std::priority_queue<Line *, std::vector<Line *>, std::function<bool(Line *, Line *)>> _min_priority_queue{comp_by_value};
        for (int i=0;i<m.size();i++) {
            Line* l = new Line(m[i][0], m[i][1]);
            lines.push_back(l);
        }

        //这里之所以使用start为优先级排序，是因为下面线段结尾小于当前线段开头的线段都会弹出，因为越往后start越大，所以当前需要弹出的线段，后面必然也会弹出
        std::sort(lines.begin(), lines.end(), compareByStart);

        // for (auto iter:lines) {
        //     cout << "start:" << iter->start << ", end:" << iter->end << endl;
        // }

        int res = 0;
        for (int i=0;i<lines.size();i++) {
            while (!_min_priority_queue.empty() && (_min_priority_queue.top())->end <= lines[i]->start) {
                cout << "pop:" << _min_priority_queue.top()->start << ", end:" << _min_priority_queue.top()->end << endl;
                _min_priority_queue.pop();
            }
            _min_priority_queue.push(lines[i]);
            res = res > _min_priority_queue.size() ? res : _min_priority_queue.size();
        }
        return res;
    }

    int max_cover(vector<vector<int>>& m) {
        int len = m.size();
        if (len==0) {
            return 0;
        }
        //找到最左和最右边
        int min_num = m[0][0];
        int max_num = m[0][1];
        for (int i=0;i<len;i++) {
            min_num = min(min_num, m[i][0]);
            max_num = max(max_num, m[i][1]);
        }
        int st_len = max_num - min_num + 1;
        cout << "max_num:" << max_num << " min_num:" << min_num << " st_len:" << st_len<< endl;
        SegmentTree st(st_len);
        for (int i=0;i<len;i++) {
            int begin = m[i][0] - min_num + 1;
            int end = m[i][1] - min_num + 1;
            // cout << "======= begin:" << begin << " end:" << end << endl; 
            // st.print("===begin:");
            // cout << "===" <<endl;
            st.add(begin, end - 1, 1, st_len, 1); // 每次都是从头结点开始加
        }
        // st.print("init:");
        int res = 0;
        for (int i=min_num;i<=max_num;i++) {
            int target = i - min_num + 1;
            // cout << "######query#####" << target << endl;
            // st.print("query:");
            res = max(res, st.query(target, 1, st_len, 1));
        }

        return res;
    }
};

int main(){
    Solution sol;
    // vector<vector<int>> a = {
    //     {1, 3}, 
    //     {2, 6}, 
    //     {7, 8}, 
    //     {1,5}, 
    //     {2,4}};
    // vector<vector<int>> a = {
    //     {1, 4}, 
    //     {2, 5}, 
    //     {3, 4}, 
    //     {5,7}};
    vector<vector<int>> a = {
        {1, 1},
        {2, 2},
        {3, 3}};
    int res1 = sol.max_cover1(a);
    int res2 = sol.max_cover1(a);
    cout << "res1:"<<  res1 << endl;
    cout << "res2:" << res2  << endl;
}