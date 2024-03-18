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

//给定一个正数L， 代表一根长度为L的绳子， 求绳子最多能覆盖其中的几个点。


class Solution {
public:
    //滑动窗口
    //因为有序，且是正数，所以具有单调性。当数组中有负数时，则需要退化成二分来做
    int get_max_dot(vector<int>& arr, int L) {
        int len = arr.size();
        if (len == 0) {
            return 0;
        }
        if (L == 0) {
            return 0;
        }
        int l = 0, r=0;
        int res = 1;
        while (l < len) {
            //每个l开头，都往右扩到不能再扩
            while (r+1 < len && (arr[r+1] - arr[l]) <= L) {
                r++;
            }
            res = max(res, r - l + 1);
            l++;
        }
        return res;
    }

    //二分
    //每到一个位置，就看最左边大于等于nums[i]-L的在哪
    int get_max_dot2(vector<int>& arr, int L) {
        int res = 1;
        int len = arr.size();
        if (len == 0) {
            return 0;
        }
        if (L == 0) {
            return 0;
        }
        for (int i=0; i<len;i++) {
            int nearest = get_nearest(arr, i-1, arr[i]-L);
            res = max(res, i-nearest+1);
        }
        return res;
    }
    int get_nearest(vector<int>& arr, int R, int value) {
        int L = 0;
        int idx = R;
        while (L<=R) {
            int mid = L + ((R-L) >> 1);
            if (arr[mid] >= value) {
                idx = mid;
                R = mid - 1;
            } else {
                L = mid + 1;
            }
        }
        return idx;
    }
};

int main() {
    Solution sol;
    vector<int> arr = {1,4,6,9,11,18};
    int L = 5;
    cout << "window:" << sol.get_max_dot(arr, L) << endl;
    cout << "window:" << sol.get_max_dot2(arr, L) << endl;
}