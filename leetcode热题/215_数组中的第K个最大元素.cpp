/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 11:07:35
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
#include <queue>
#include <algorithm>
#include <cmath>
#include <functional>
#include <unordered_map>

using namespace std;

/*
稳定最优解是bfprt；概率最优解是荷兰国旗问题。

https://leetcode.cn/problems/kth-largest-element-in-an-array/description/

给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。

请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。

你必须设计并实现时间复杂度为 O(n) 的算法解决此问题。



示例 1:

输入: [3,2,1,5,6,4], k = 2
输出: 5
示例 2:

输入: [3,2,3,1,2,4,5,5,6], k = 4
输出: 4


提示：

1 <= k <= nums.length <= 105
-104 <= nums[i] <= 104
*/

/*
经典拓扑排序算法，课程依赖关系构成图。
如果是无环图，则说明可以修完所有的课程。否则就修不完

*/
class Solution
{
public:
    struct cmp
    {
        bool operator()(const int a, const int b) const
        {
            return a > b; // 新来b，val小时放在堆顶
        }
    };
    // O(n*log(n))的解法
    int findKthLargest_dui(vector<int> &nums, int k)
    {
        int len = nums.size();
        if (len < k)
        {
            return -1;
        }
        priority_queue<int, vector<int>, cmp> pq; // 小根堆
        for (int i = 0; i < k; i++)
        {
            pq.push(nums[i]);
        }

        for (int i = k; i < len; i++)
        {
            if (nums[i] > pq.top())
            {
                if (pq.size() == k)
                {
                    pq.pop();
                }
                pq.push(nums[i]);
            }
        }

        return pq.top();
    }

    // O(N)的荷兰国旗问题，随机选择锚点
    int findKthLargest_random(vector<int> &nums, int k)
    {
        int len = nums.size();
        if (len < k)
        {
            return -1;
        }
        int l = 0, r = len - 1;
        // 第k大的数在排序后数组的len-k下标位置
        return find_k_lagest_core(nums, l, r, len - k);
    }
    // 在l..r上得到，排序之后应该在数组的k下标上的数
    int find_k_lagest_core(vector<int> &nums, int l, int r, int k)
    {
        if (l == r)
        {
            return nums[l];
        }
        int pivot = l + (int)(std::rand() % (r - l + 1));
        // cout << "l:" << l << " r:" << r << " pivot:" << pivot << " k:" << k<< endl;
        vector<int> mid = {-1, -1};
        partition(nums, l, r, nums[pivot], mid);
        // cout << "mid:" << mid[0] <<  "," << mid[1] << endl;
        // print_arr(nums, "partition:");
        if (k >= mid[0] && k <= mid[1])
        {
            return nums[k];
        }
        else if (k < mid[0])
        {
            return find_k_lagest_core(nums, l, mid[0] - 1, k);
        }
        else
        {
            return find_k_lagest_core(nums, mid[1] + 1, r, k);
        }
    }

    // O(N)的荷兰国旗问题，bfprt选择锚点
    int findKthLargest(vector<int> &nums, int k)
    {
        int len = nums.size();
        if (len < k)
        {
            return -1;
        }
        int l = 0, r = len - 1;
        // 第k大的数在排序后数组的len-k下标位置
        return bfprt(nums, l, r, len - k);
    }
    void print_arr(vector<int> &nums, string suff)
    {
        cout << suff;
        for (auto it : nums)
        {
            cout << it << " ";
        }
        cout << endl;
    }
    // 在l..r上得到，排序之后应该在数组的k下标上的数
    int bfprt(vector<int> &nums, int l, int r, int k)
    {
        if (l == r)
        {
            return nums[l];
        }
        cout << "l:" << l << " r:" << r << " k:" << k << endl;
        print_arr(nums, "nums:");
        int pivot = get_median_median(nums, l, r); // 注意，中位数的中位数返回的是数字，不是下标
        cout << "l:" << l << " r:" << r << " pivot:" << pivot << " k:" << k << endl;
        vector<int> mid = {-1, -1};
        partition(nums, l, r, pivot, mid);
        // cout << "mid:" << mid[0] <<  "," << mid[1] << endl;
        print_arr(nums, "partition:");
        if (k >= mid[0] && k <= mid[1])
        {
            return nums[k];
        }
        else if (k < mid[0])
        {
            return bfprt(nums, l, mid[0] - 1, k);
        }
        else
        {
            return bfprt(nums, mid[1] + 1, r, k);
        }
    }
    // 5友算法
    int get_median_median(vector<int> &arr, int l, int r)
    {
        int size = r - l + 1;
        int offset = size % 5 == 0 ? 0 : 1; // 如果size刚好整除5，就不再加组
        vector<int> medians((size / 5) + offset, 0);
        // 构造中位数数组
        for (int i = 0; i < medians.size(); i++)
        {
            int start = l + i * 5;
            medians[i] = get_median_num(arr, start, min(start + 4, r));
        }
        print_arr(medians, "medians:");

        // 求medians的中位数
        return bfprt(medians, 0, medians.size() - 1, medians.size() / 2 - 1);
    }

    //[l..r]范围上求中位数
    int get_median_num(vector<int> &arr, int l, int r)
    {
        // 选择排序，将l和r排成有序
        for (int i = l; i <= r - 1; i++)
        {
            int min_idx = i;
            for (int j = i + 1; j <= r; j++)
            {
                min_idx = arr[min_idx] < arr[j] ? min_idx : j;
            }
            // swap，将这次最小的放在i位置
            if (min_idx != i)
            {
                swap(arr[min_idx], arr[i]);
            }
        }
        return arr[l + ((r - l) >> 1)];
    }
    // 荷兰国旗问题
    // 范围是l..r，锚点是pivot
    // 结果放在mid中，存储pivot的左右边界
    void partition(vector<int> &arr, int l, int r, int pivot, vector<int> &mid)
    {
        int less = l - 1;
        int more = r + 1;
        int idx = l;
        while (idx < more)
        {
            if (arr[idx] == pivot)
            {
                idx++;
            }
            else if (arr[idx] < pivot)
            {
                swap(arr[idx], arr[++less]);
                idx++;
            }
            else
            {
                swap(arr[idx], arr[--more]);
            }
        }
        mid[0] = less + 1;
        mid[1] = more - 1;
    }
};

int main()
{

    Solution sol;
    vector<int> arr = {3,2,1,5,6,4}; 
    int k = 2;
    cout << sol.findKthLargest(arr, k) << endl;;
    return 0;
}
