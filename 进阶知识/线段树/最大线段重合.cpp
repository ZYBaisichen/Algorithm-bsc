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
*/
class Solution
{
    
public:
    // 自定义比较器，用于按照字符串长度排序 
    // a<b 返回true，标识a可以放在b前面 
    static bool compareByStart(const Line* a, const Line* b) {
        return a->start < b->start;
    }  

    int max_cover(vector<vector<int>>& m) {
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
};

int main(){
    Solution sol;
    vector<vector<int>> a = {{1, 3}, {2, 6}, {7, 8}, {1,5}, {2,4}};
    int res = sol.max_cover(a);
    cout << "res:" << res << endl;
}