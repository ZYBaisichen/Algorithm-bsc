/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 15:09:45
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

	vector<int> sortArray(vector<int>& nums) {
		heap_sort(nums);
		return nums;
	}

    void print(vector<T>& nums, string suff) {
        cout << "suff: "<< suff<<endl;
        for (auto iter:nums) {
            cout << " " << iter;
        }
        cout << endl;
    }

    void heap_sort(vector<T>& nums) {
        int len = nums.size();
        //第一次建堆，优化成O(N)
        for (int i=len-1;i>=0;i--) {
            heapify(nums, i, len); //下标不能到len，所以传len-1的下一个位置
        }
        // print(nums, "first");
        

        //挨个出堆
        int size=len-1;
        while (size>0) {
            swap(nums, 0, size);
            heapify(nums, 0, size);
            size--;
        }
    }

    //大根堆
    void heapify(vector<T>& nums, int idx, int size) {
        int child = (idx<<1)+1;
        // cout << "cur:" << nums[idx]  << " idx:" << idx<<"  child: " << child << " size:" << size<< endl;
        while (child < size) {
            // cout << "child: " << child << " child+1:" << child+1 <<" size:" << size << " (child+1)<size:" << ((child+1)<size) <<endl;
            int largest = child; // 找到最小的孩子
            if (child+1 < size) {
                if (nums[child+1] > nums[child]) {
                    largest = child + 1;
                }
            }
            largest = nums[largest] > nums[idx] ? largest : idx;
            // cout << "largest：" << largest<< " : "<< nums[largest] << endl;
            if (largest == idx) { //当前节点就是最大的，不用往下调整了
                break;
            } else {
                swap(nums, +largest, idx);
                idx = largest;
                child = (idx<<1) + 1;
            }
        }        
    }

    //从idx开始往上做调整大跟堆
    void heap_insert(vector<T> &nums, int idx)
    {
        int parent  = (idx - 1) >> 2;
        int cur = idx;
        //父亲节点比自己小，需要往上调整
        while (parent >= 0 && nums[parent] < nums[cur]) {
            swap(nums, parent, cur);
            cur = parent;
            parent = (idx - 1) >> 2;
        }
    }

    void swap(vector<T> &nums,int i, int j) {
        T tmp  = nums[i];
        nums[i] = nums[j];
        nums[j] = tmp;
    }
};

int main()
{
    Solution<int> sol;
    vector<int> nums = {1,4,5,10,9,100,56,29};
    sol.heap_sort(nums);
    for (auto iter:nums) {
        cout << " " << iter;
    }
    cout << endl;
    return 0;
}