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

// 数组arr的子数组arr_c如果达标，则max(arr_)-min(arr_c)<=num。 返回达标数组的数量
#define MAXN 100001
class Solution
{
public:
    //[3,5,2,7,1,6]
    int less_num(vector<int> &arr, int num) {
        int len = arr.size();
        deque<int> max_q; //单调递减，求窗口最大值
        deque<int> min_q; //单调递增，求窗口最小值
        int r=0;
        int ans = 0;
        // cout << "len: " << len << endl;
        // 假设找到了[0...5]的答案，完全没必要再看1..2了，只需要直接从[1..6]开始看
        for (int l=0; l< len; l++) {
            while (r<len) {
                // cout << "r:" << r << endl;
                while (!max_q.empty() && arr[max_q.back()] <= arr[r]) {
                    max_q.pop_back();
                }
                max_q.push_back(r);
                while (!min_q.empty() && arr[min_q.back()] >= arr[r]) {
                    // cout << "min_q:" << arr[min_q.back()] << " "<< arr[r]<< endl;
                    min_q.pop_back();
                }
                min_q.push_back(r);
                
                int cur_max_idx = max_q.front();
                int cur_min_idx = min_q.front();
                // cout << "cur_max_num: " << arr[cur_max_idx] << " cur_min_num: " << arr[cur_min_idx] << endl;
                if ((arr[cur_max_idx] - arr[cur_min_idx]) > num) {
                    //找到了第一个不合规的位置
                    break;
                }
                
                r++;
            }
            // cout << "ans:" << ans << " l:" << l << " r:" << r << endl;

            //收集答案，产生了r-l个达标数组
            ans += r-l;

            //l即将过期，出队
            if (!max_q.empty() && max_q.front() == l) {
                max_q.pop_front();
            }
            if (!max_q.empty() && min_q.front() == l) {
                min_q.pop_front();
            }
        }
        return ans;
    }
    
    int less_num_baoli(vector<int> &arr, int num) {
        int len  = arr.size();
        int ans = 0;
        for (int l=0;l<len;l++) {
            for (int r=l; r<len;r++) {
                int max_num=INT_MIN;
                int min_num=INT_MAX;
                for (int k=l; k<=r; k++) {
                    max_num = max(max_num, arr[k]);
                    min_num = min(min_num, arr[k]);
                }
                if (max_num-min_num <= num) {
                    ans++;
                }
            }
        }
        return ans;
    }
};

int main()
{
    Solution sol;
    //22
    vector<int> arr = {3, 5, 2, 7, 1, 6,1,4,6,7,2323,4,12};
    int num=3;
    int ans0 = 0;
    int ans1 = 0;

    ans0 = sol.less_num(arr, num);
    ans1 = sol.less_num_baoli(arr, num);

    cout << "ans0:" << ans0 << " ans1:" << ans1 << endl;
    cout << "测试结束" << endl;
}
