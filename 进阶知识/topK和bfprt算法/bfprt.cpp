/*
 * @Author: baisichen
 * @Date: 2024-03-02 16:10:58
 * @LastEditTime: 2024-03-04 15:45:46
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

using namespace std;

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

// 每次最少减少右侧3N/10的规模，每次选择出来的m左侧最多有7N/10
// 将荷兰国旗方法的概率O(N)复杂度，变成了严格的O(N)复杂度。

class Solution
{
public:
    //严格O(N)复杂度
    int get_k_with_bfprt(vector<int>& arr, int k)
    {
        int len = arr.size();
        if (len == 0)
        {
            return -1;
        }

        return bfprt(arr, 0, len - 1, k-1);
    }

    //笔试这样就已经足够快了
    int get_k_with_rand(vector<int> &arr, int k)
    {
        int len = arr.size();
        if (len == 0)
        {
            return -1;
        }

        return get_k_with_rand_core(arr, 0, len - 1, k - 1);
    }
    // 返回arr数组上l到r范围上，返回arr排序后应该在index位置的数
    int get_k_with_rand_core(vector<int> &arr, int l, int r, int index) {
        if (l == r) {
            return arr[l];
        }
        // cout << "l:" << l << ", r:" << r << " k:" << index << endl;
        int pivot = l+ (int)((std::rand() % (r-l)) + 1);
        // cout << "pivot:" << pivot << endl;
        vector<int> mid(2, -1);
        partition(arr, l, r, pivot, mid); // 做荷兰国旗
        // cout << "mid: " << mid[0] << ", " << mid[1] << endl;
        if (index >= mid[0] && index <= mid[1])
        {
            return arr[index];
        }
        else if (index < mid[0])
        {
            return bfprt(arr, l, mid[0] - 1, index);
        }
        else
        {
            return bfprt(arr, mid[1] + 1, r, index);
        }
    }

    // 返回arr数组上l到r范围上，返回arr排序后应该在index位置的数
    int bfprt(vector<int> &arr, int l, int r, int index)
    {   
        if (l == r) {
            return arr[l];
        }
        // cout << "l:" << l << ", r:" << r << " k:" << index << endl;
        int pivot = get_median_median(arr, l, r);
        // cout << "pivot:" << pivot << endl;
        vector<int> mid(2, -1);
        partition(arr, l, r, pivot, mid); // 做荷兰国旗
        // cout << "mid: " << mid[0] << ", " << mid[1] << endl;
        if (index >= mid[0] && index <= mid[1])
        {
            return arr[index];
        }
        else if (index < mid[0])
        {
            return bfprt(arr, l, mid[0] - 1, index);
        }
        else
        {
            return bfprt(arr, mid[1] + 1, r, index);
        }
    }

    // arr L到R上的数字，每5个一组排序，每个组的中位数挑出来组成中位数数组。
    // 再从中位数数组中挑出来中位数
    int get_median_median(vector<int> &arr, int l, int r)
    {
        int size = r - l + 1;
        int offset = size % 5 == 0 ? 0 : 1; // 如果size刚好能整除5，则不再加组，不能整除，再加1组
        vector<int> medians((size / 5) + offset, 0);
        // 构造中位数数组
        for (int i = 0; i < medians.size(); i++)
        {
            int start = l + i * 5;
            medians[i] = get_median_num(arr, start, min(start + 4, r));
            // cout << "median:" << medians[i] << " l:" << l << " r:" << r<< endl;
        }
        // print(arr, l, r, "get_median_median");
        // print(medians, 0, medians.size()-1, "medians");
        return bfprt(medians, 0, medians.size() - 1, medians.size() / 2 -1); // 求medians的中位数
    }
    void print(vector<int>& arr, int l, int r, string name) {
        cout << name <<": ";
        for (int i=l; i<=r; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    int get_median_num(vector<int> &arr, int l, int r)
    {
        select_sort(arr, l, r); // 闭区间
        return arr[(r + l) / 2];
    }

    void select_sort(vector<int> &arr, int l, int r)
    {
        for (int i = l; i <= r - 1; i++)
        {
            int min_idx = i;
            for (int j = i + 1; j <= r; j++)
            {
                min_idx = arr[min_idx] < arr[j] ? min_idx : j;
            }
            // swap
            if (min_idx != i)
            {
                swap(arr, min_idx, i);
            }
        }
    }
    void swap(vector<int> &arr, int l, int r)
    {
        int tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
    }

    // 荷兰国旗问题
    void partition(vector<int> &arr, int l, int r, int pivot, vector<int> &mid)
    {   
        // cout << "before partition arr, pivot: " << pivot << " l:" << l << " r:" << r << ":";
        // for (int i=l;i<=r;i++) {
        //     cout << arr[i] << " ";
        // }
        // cout << endl;
        // 定义一个小于pivot的区域，右边界的上一个数的下标是less
        int less = l - 1;
        // 定义一个小于pivot的区域，左边界的下一个数的下标是more
        int more = r + 1;

        // cout << "before prtition less: " << less << " more:" << more << endl;
        int index = l;
        while (index < more)
        {
            if (arr[index] == pivot)
            {
                index++;
            }
            else if (arr[index] < pivot)
            { // 将小于pivot的挪到左边，右边界右移
                // cout << "idnex: " << index << " arr[index]" << arr[index] << " povit:" << pivot << " less: " << less << endl;
                swap(arr, index++, ++less);
                
            }
            else
            { // 将大于pivot的挪到右边，左边界左移
                swap(arr, index, --more);
            }
        }
        // cout << "after prtition less: " << less << " more:" << more << endl;
        mid[0] = less + 1;
        mid[1] = more - 1;
    }
};

int main()
{
    Solution sol;
    //1 3 5 78 100
    //60 90
    vector<int> arr = {3,5,1,100,78 ,60,90, 1,5,6,1,2,3};
    int test_time = 1000;
    int len = 0;
    int k= 0;

    int ans0 = 0;
    int ans1 = 0;
    int ans2 = 0;
    for (int i=0;i<test_time;i++) {
        len = (int)((std::rand() % 100) + 1);
        arr.resize(len);
        for (int j=0;j<len;j++) {
            arr[j] = (int)((std::rand() % 1000) + 1);
        }
        k = (int)((std::rand() % len)+1);
        vector<int> arr1 = arr;
        vector<int> arr2 = arr;
        vector<int> arr3 = arr;

        sol.select_sort(arr2, 0, arr2.size() - 1);
        ans0 = arr2[k - 1];
        ans1 = sol.get_k_with_bfprt(arr1, k);
        ans2 = sol.get_k_with_rand(arr3, k);
        if (ans0!=ans1) {
            cout << " ans0:" << ans0 << " ans1:" << ans1 << endl;
        }
        if (ans0!=ans2) {
            cout << " ans0:" << ans0 << " ans2:" << ans2 << endl;
        }
    }
    // vector<int> arr1 = arr;
    // vector<int> arr2 = arr;
    // int k = 4;
    // int ans0 = 0;
    // int ans1 = 0;
    // //这个函数求的是假设将arr排序完成后，下标在k-1位置的数字
    // ans0 = sol.get_k(arr1, k);
    // sol.select_sort(arr2, 0, arr2.size() - 1);
    // ans1 = arr2[k-1];

    // cout << " ans0:" << ans0 << " ans1:" << ans1 << endl;
    cout << "测试结束" << endl;
}
