/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-30 22:49:57
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
    //选择排序
    void select_sort(vector<int>& arr) {
        int len = arr.size();
        if (len == 0) {
            return;
        }
        for (int i=0;i<len;i++) {
            //每次选最小值放在i位置
            int min = i;
            for (int j=i+1;j<len;j++) {
                min = arr[j] < arr[min] ? j : min;
            }
            swap(arr[i], arr[min]);   
        }
    }
    //插入排序
    void insert_sort(vector<int>& arr) {
        int len = arr.size();
        if (len == 0) {
            return;
        }
        //每次将Arr[i]插在0...i-1合适的位置
        for (int i=1;i<len;i++) {
            int insert_val = arr[i];
            int j=i-1;
            for (j=i-1;j>=0;j--) {
                //往后挪腾地
                if (arr[j] > insert_val) {
                    arr[j+1]=arr[j];
                } else { //遇到第一个比insert_val小的数结束
                    break;
                }
            }
            //运行到这里，j的位置一定是再第一个比insert_val小的位置
            if (j!=i-1) {
                arr[j+1] = insert_val;
            }
        }
    }

    //冒泡排序
    void bubble_sort(vector<int>& arr) {
        int len = arr.size();
        if (len == 0) {
            return;
        }
        
        for (int i=0;i<len;i++) {
            //每次将大的往上冒泡，已经有i个数
            //所以这次参与冒泡的有len-i个数，但最后一个数不需要比较，所以比较len-i-1个数就可以
            for (int j=0;j<len-1-i;j++) {
                if (arr[j] > arr[j+1]) {
                    swap(arr[j], arr[j+1]);
                }
            }
        }
    }
    //快速排序
    void quick_sort(vector<int>& arr) {
        int len = arr.size();
        if (len == 0) {
            return;
        }
        process(arr, 0, len-1);
    }
    void process(vector<int>& arr, int l, int r) {
        cout << "begin:l:" << l << " r:" << r << endl;
        if (l>=r) {
            return;
        }
        int rand_idx = l + (rand() % (r-l+1));
        int left = -1, right = -1;//锚点上下界
        // cout << "rand:" << arr[rand_idx]  << " idx:" << rand_idx << endl;
        partition(arr, l, r, left, right, arr[rand_idx]);
        // cout <<"l:" << l << " r:" << r <<" right:"<< right << " left:" << left << " rand_idx:" << rand_idx << " pivot:" << arr[rand_idx] << endl;
        process(arr, l, left-1);
        process(arr, right+1, r);
    }
    void partition(vector<int>& arr, int l, int r, int& left, int& right, int pivot) {
        // cout << "partition:" << l << " r:" << r << " pivot:" << pivot << endl;
        if (l>r) {
            return;
        }
        if (l==r) {
            left = right = l;
            return;
        }
        left = l-1;
        right = r+1;
        int i=l;
        while (i<right) {
            if (arr[i] > pivot) {
                swap(arr[--right], arr[i]);
            } else if (arr[i] < pivot) {
                swap(arr[++left], arr[i]);
                i++;
            } else {
                i++;   
            }
        }
    }
    
};

int main()
{
    Solution<int> sol;
    vector<int> nums = {100,5,13,17,21,29};
    sol.quick_sort(nums);
    for (auto iter:nums) {
        cout << " " << iter;
    }
    cout << endl;
    return 0;
}