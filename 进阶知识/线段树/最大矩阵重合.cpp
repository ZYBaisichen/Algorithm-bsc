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
/*
给一系列二维数组，每个元素代表矩阵左上角和右下角。
某个矩形的底一定是某个重复区域的底，假设将所有以每个矩形为底的公共区域求出来，就可以算出来最大的重合数量
1. 矩形遍历顺序是按照底，从小到大遍历。入底小根堆
2. 当遍历到某个底时，将上顶小于当前线的矩形从堆里面去除。
2. 去除之后堆中的矩形，一定就是底在当前线下面，顶在当前线上面。
3. 然后将堆中的矩阵的左右边界作为一个线段，如果线段重合，矩阵必重合，就退化成了最大线段重合数量问题。
*/
class Rect {
public:
    int up;
    int down;
    int left;
    int right;
    Rect(int _up, int _down, int _left, int _right) {
        up = _up;
        down = _down;
        left = _left;
        right = _right;
    }
};

// 用于组织同一条底线上的矩形，按照left从小小到大
struct CompareByLeft {  
    bool operator()(const Rect* a, const Rect* b) const {  
        // 返回a的长度大于b的长度的情况，实现降序排列  
        return a->left < b->left; 
    }  
};

// 自定义比较器，用于定义小根堆，同一条底边的right入堆
struct CompareByRight {  
    bool operator()(const Rect* a, const Rect* b) const {  
        return a->right > b->right; 
    }    
};  
/*
矩阵数量是n
O(n*long(n))
+
O(N)*[O(n) + O(n*long(n))]
*/
class Solution
{
    
public:
    // 自定义比较器，根据底排序
    // a<b 返回true，标识a可以放在b前面 
    static bool compareByDown(const Rect* a, const Rect* b) {
        return a->down < b->down;
    }  

    int max_cover_rect(vector<vector<int>>& m) {
        vector<Rect*> rects;
        for (int i=0;i<m.size();i++) {
            Rect *r = new Rect(m[i][0], m[i][1], m[i][2], m[i][3]);
            rects.push_back(r);
        }
        std::sort(rects.begin(), rects.end(), compareByDown);
        // for (int i=0;i<rects.size();i++) {
        //     cout << "up:" << rects[i]->up << ", down:" << rects[i]->down << ", left:" << rects[i]->left << ", right:" << rects[i]->right << endl;
        // }

        std::multiset<Rect*, CompareByLeft> left_set; //因为有可能有左边界重合的问题，所以需要使用multiset做有序表
        int i=0;
        int res= 0;
        while (i<rects.size()) {
            int cur_down = rects[i]->down;
            int idx = i;
            while (idx < rects.size() && rects[idx]->down == cur_down) {
                left_set.insert(rects[idx]);
                cout << "cur_down:" << cur_down << " up:" << rects[idx]->up << ", down:" << rects[idx]->down << ", left:" << rects[idx]->left << ", right:" << rects[idx]->right << endl;
                idx++;
            }
            i = idx;

            //移除矩形顶部比当前底边小于等于的矩形
            while (!left_set.empty() && (*(left_set.begin()))->up <= cur_down) {
                Rect* r = *(left_set.begin());
                cout << "erase, up:" << r->up << ", down:" << r->down << ", left:" << r->left << ", right:" << r->right << endl;
                left_set.erase(r);
            }

            //线段重合问题
            std::priority_queue<Rect *, std::vector<Rect *>, CompareByRight> _min_priority_queue_right;
            for (auto iter:left_set) {
                cout << "left, up:" << iter->up << ", down:" << iter->down << ", left:" << iter->left << ", right:" << iter->right << endl;
                while (!_min_priority_queue_right.empty() 
                    && (iter->left >= (_min_priority_queue_right.top())->right)) {
                    // cout << "asdf" << endl;
                    _min_priority_queue_right.pop();
                }
                _min_priority_queue_right.push(iter);
                res = res > _min_priority_queue_right.size() ? res : _min_priority_queue_right.size();
            }
        }
        return res;
    }
};

int main(){
    Solution sol;
    vector<vector<int>> a = {{1, 0, 0, 2}, {2, 0, 3, 5}, {2, 0, 1, 3}, {3, 0, 1, 4}};
    int res = sol.max_cover_rect(a);
    cout << "res:" << res << endl;
}