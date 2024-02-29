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
https://www.nowcoder.com/practice/2a2c00e7a88a498693568cef63a4b7bb
输入：
7
3 4 1 5 6 2 7
复制
输出：
-1 2
0 2
-1 -1
2 5
3 5
2 -1
5 -1
*/

class Solution
{
public:
    void get_nearless_norepeat(vector<int> &arr, vector<vector<int>> &res)
    {
        int len = arr.size();
        if (len == 0)
        {
            return;
        }
        stack<int> st;
        for (int i = 0; i < len; i++)
        {
            while (!st.empty() && arr[st.top()] > arr[i])
            {
                int cur_idx = st.top();
                // cout << "test:"<<cur_idx << " " << i << endl;
                st.pop();
                if (st.empty())
                {
                    res[cur_idx][0] = -1;
                }
                else
                {
                    res[cur_idx][0] = st.top();
                }
                res[cur_idx][1] = i;
            }
            st.push(i);
        }
        while (!st.empty())
        {
            int cur_idx = st.top();
            st.pop();
            if (st.empty())
            {
                res[cur_idx][0] = -1;
            }
            else
            {
                res[cur_idx][0] = st.top();
            }
            res[cur_idx][1] = -1;
        }
    }

    void get_nearless_repeat(vector<int> &arr, vector<vector<int>> &res) {
        int len = arr.size();
        if (len == 0)
        {
            return;
        }
        stack<vector<int>*> st;
        // stack<vector<int>*> st1;
        // vector<int>* tmp = new vector<int>();
        // st1.push(tmp);
        // vector<int> *tmp2 = st1.top();
        // tmp2->push_back(tmp2->size());
        // cout << (*tmp2)[0] << endl;
        for (int i = 0; i < len; i++) {
            // cout << "i:" << i << endl;
            while (!st.empty())
            {   
                vector<int>* top_vec = st.top();
                if (arr[(*top_vec)[0]] <= arr[i]) {
                    break;
                }
                //结算一批
                st.pop();
                for (int j = 0; j < top_vec->size(); j++) {
                    int cur_idx = (*top_vec)[j];
                    if (st.empty()) {
                        res[cur_idx][0] = -1;
                    } else {
                        vector<int> *_after_pop_top_vec = st.top();
                        // cout << "st.top().size():" << st.top().size() << endl;
                        res[cur_idx][0] = (*_after_pop_top_vec)[_after_pop_top_vec->size() - 1];
                        // cout << "st.top().size()222:" << st.top().size() << endl;
                    }
                    // cout << "cur_idx:" << cur_idx << endl;
                    res[cur_idx][1] = i;
                }
                delete top_vec;
            }
            if (!st.empty() && arr[(*st.top())[0]] == arr[i]) {
                st.top()->push_back(i);
            } else {
                vector<int> *tmp = new vector<int>();
                tmp->push_back(i);
                st.push(tmp);
            }
        }
        // cout << "111" << endl;
        while (!st.empty())
        {
            vector<int> *cur_vec = st.top();
            st.pop();
            for (int j = 0; j < cur_vec->size(); j++)
            {
                int cur_idx = (*cur_vec)[j];
                if (st.empty()) {
                    res[cur_idx][0] = -1;
                } else {
                    vector<int> *_after_pop_top_vec = st.top();
                    res[cur_idx][0] = (*_after_pop_top_vec)[_after_pop_top_vec->size() - 1];
                }
                res[cur_idx][1] = -1;
            }
            delete cur_vec;
        }
    }
};
int main()
{

/*
输入：
7
3 4 1 5 6 2 7
复制
输出：
-1 2
0 2
-1 -1
2 5
3 5
2 -1
5 -1
*/

    int n;
    cin >> n;
    vector<int> arr(n, 0);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    // int n=7;
    // vector<int> arr = {3, 3, 1, 5, 6, 2, 7};
    vector<vector<int>> res(n, vector<int>(2, -1));
    Solution sol;
    sol.get_nearless_repeat(arr, res);

    for (int i = 0; i < n; i++) {
        cout << res[i][0] << " " << res[i][1] << endl;
    }
}
// 64 位输出请用 printf("%lld")