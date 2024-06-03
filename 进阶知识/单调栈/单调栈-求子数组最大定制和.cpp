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

    int get_max_dingzhi_sum(vector<int> &arr)
    {
        int len = arr.size();
        vector<vector<int>> nearless_res(len, vector<int>(2, -1));
        get_nearless_repeat(arr, nearless_res);

        for (int i = 0; i < len; i++)
        {
            cout << nearless_res[i][0] << " " << nearless_res[i][1] << endl;
        }

        vector<int> pre_sum(len, 0);
        pre_sum[0] = arr[0];
        cout << "pre_sum:" << pre_sum[0] << " ";
        for (int i = 1; i < len; i++)
        {
            pre_sum[i] = pre_sum[i - 1] + arr[i];
            cout << pre_sum[i] << " ";
        }
        cout << endl;

        int max_res = INT_MIN;
        int r = nearless_res[0][1];
        if (r==-1) {
            max_res = pre_sum[len-1]*arr[0];
        } else {
            max_res = (pre_sum[r]-arr[r]) * arr[0];
        }
        cout << "0, max_res:" << max_res << endl;

        int l = nearless_res[len-1][0];
        if (l==-1) {
            max_res = max(max_res, pre_sum[len-1] * arr[len-1]);
        } else {
            max_res = max(max_res, (pre_sum[len - 1]-pre_sum[l]) * arr[len - 1]);
        }
        cout << "1, max_res:" << max_res << endl;

        for (int i = 1; i < len-1; i++)
        {
            int l = nearless_res[i][0]; 
            int r = nearless_res[i][1];
            int sum=0;
            if (r==-1) {
                sum = pre_sum[len-1];
            } else {
                sum = pre_sum[r] - arr[r];
            }
            if (l!=-1) {
                sum = sum - pre_sum[l];
            }
            cout << "cur_sum:" << sum << " arr[i]:"<< arr[i] << " l:" << l << " r:" << r << endl;
            max_res = max(sum * arr[i], max_res);
        }
        return max_res;
    }

    void get_nearless_repeat(vector<int> &arr, vector<vector<int>> &res)
    {
        int len = arr.size();
        if (len == 0)
        {
            return;
        }
        stack<vector<int> *> st;
        // stack<vector<int>*> st1;
        // vector<int>* tmp = new vector<int>();
        // st1.push(tmp);
        // vector<int> *tmp2 = st1.top();
        // tmp2->push_back(tmp2->size());
        // cout << (*tmp2)[0] << endl;
        for (int i = 0; i < len; i++)
        {
            // cout << "i:" << i << endl;
            while (!st.empty())
            {
                vector<int> *top_vec = st.top();
                if (arr[(*top_vec)[0]] <= arr[i])
                {
                    break;
                }
                // 结算一批
                st.pop();
                for (int j = 0; j < top_vec->size(); j++)
                {
                    int cur_idx = (*top_vec)[j];
                    if (st.empty())
                    {
                        res[cur_idx][0] = -1;
                    }
                    else
                    {
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
            if (!st.empty() && arr[(*st.top())[0]] == arr[i])
            {
                st.top()->push_back(i);
            }
            else
            {
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
                if (st.empty())
                {
                    res[cur_idx][0] = -1;
                }
                else
                {
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

    int n = 7;
    vector<int> arr = {3, 3, 1, 5, 6, 2, 7};

    Solution sol;
    cout << sol.get_max_dingzhi_sum(arr) << endl;
}
// 64 位输出请用 printf("%lld")