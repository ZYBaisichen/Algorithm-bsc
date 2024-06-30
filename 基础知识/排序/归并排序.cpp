/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-30 13:57:23
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

using namespace std;

template <typename T>
class Solution {
public:
    //mergeSort
    //递归
    void merge_sort1(vector<int>& arr) {
        int len = arr.size();
        if (len == 0 || len < 2) {
            return;
        }
        //在0~len-1上做归并排序
        process(arr, 0, len-1);
    }

    void process(vector<int>& arr, int l, int r) {
        if (l==r) {
            return;
        }
        int mid = l+((r-l)>>1);
        process(arr,l, mid);
        process(arr,mid+1,r);
        merge(arr,l,mid,r);
    }
    void print_arr(vector<int>& arr, string suff) {
        cout << suff;
        for (auto it:arr) {
            cout << it << " ";
        }
        cout << endl;
    }
     //非递归
    void merge_sort2(vector<int>& arr) {
        int len = arr.size();
        if (len == 0 || len < 2) {
            return;
        }
        int merge_size = 1; //需要合并的数组左边的长度
        while (merge_size < len) {
            int l = 0;
            while (l<len) {
                int mid = l+merge_size-1; //中间值
                if (mid>=len) {
                    break;
                }
                int r = min(mid+merge_size, len-1);
                // cout << "l:" << l << " mid:" << mid << " r:" << r << endl;
                merge(arr, l, mid, r);
                // print_arr(arr, "one merge:");
                l=r+1;
            }
            // print_arr(arr, to_string(merge_size) + " merge====:");
            if (merge_size > len/2) {
                break;
            }
            merge_size <<= 1;
            
        }
    }
    void merge(vector<int>& arr, int l, int mid, int r) {
        int len = r-l+1;

        vector<int> help(len, 0);
        int i=0;
        int p1 = l;
        int p2=mid+1;
        while (p1<=mid && p2<=r) {
            help[i++] = arr[p1] > arr[p2] ? arr[p2++] : arr[p1++];
        }

        while (p1<=mid) {
            help[i++] = arr[p1++];
        }
        while (p2<=r) {
            help[i++] = arr[p2++];
        }
        for (i=0;i<len;i++) {
            arr[l+i] = help[i];
        }
    }


};

int main()
{
    Solution<int> sol;
    vector<int> nums = {100,5,13,17,21,29};
    sol.merge_sort1(nums);
    for (auto iter:nums) {
        cout << " " << iter;
    }
    cout << endl;
    vector<int> nums2 = {100,5,13,17,21,29};
    sol.merge_sort2(nums);
    for (auto iter:nums) {
        cout << " " << iter;
    }
    cout << endl;
    return 0;
}