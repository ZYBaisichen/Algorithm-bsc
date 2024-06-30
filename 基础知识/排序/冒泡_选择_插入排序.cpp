/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-30 16:14:12
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
};

int main()
{
    Solution<int> sol;
    vector<int> nums = {100,5,13,17,21,29};
    sol.select_sort(nums);
    for (auto iter:nums) {
        cout << " " << iter;
    }
    cout << endl;
    vector<int> nums2 = {100,5,13,17,21,29};
    sol.insert_sort(nums);
    for (auto iter:nums) {
        cout << " " << iter;
    }
    cout << endl;
    vector<int> nums3 = {100,5,13,17,21,29};
    sol.bubble_sort(nums);
    for (auto iter:nums) {
        cout << " " << iter;
    }
    cout << endl;
    return 0;
}