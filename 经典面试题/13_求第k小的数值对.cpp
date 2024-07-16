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
void print(vector<int>& res, string suf = "res") {
    cout << suf;
    for (auto iter:res) {
        cout << " " << iter;
    }
    cout << endl;
}

/*
长度为N的数组arr，一定可以组成N^2个数值对
例如：arr = [3, 1, 2]，
数值对有(3, 3)(3, 1)(3, 2)(1, 3)(1, 1)(1, 2)(2, 3)(2, 1)(2, 2)，也就是任意两个数都有数值对，而且自己和自己也算数值对
数值对的排序规则，第一维数据从小到大，第一维数据一样的，第二维数据也从小到大
上面的数值对排序结果为：(1, 1)(1, 2)(1, 3)(2, 1)(2, 2)(2, 3)(3, 1)(3, 2)(3, 3)
给定一个数组arr，和整数k，返回第k小的数值对


bfprt的应用


*/

struct num_pair {
    int first;
    int second;
    num_pair(int f, int s) : first(f), second(s) {}
};
class Solution
{
public:
    //首先想法是，暴力枚举所有的数值对
    //然后对数值对排序，排序完成后找到第k小（下标为k-1）的数值对
    vector<int> kthMinPair1(vector<int> &nums, int k) {
        int len = nums.size();
        vector<int> res;
        if (len == 0)
        {
            return res;
        }

        vector<num_pair*> res_tmp;
        for (int i=0;i<len;i++) {
            for (int j=0;j<len;j++) {
                num_pair* tmp = new num_pair(nums[i], nums[j]);
                res_tmp.push_back(tmp);
            }
        }      
  
        // 使用比较函数对vector进行排序  
        std::sort(res_tmp.begin(), res_tmp.end(), [](const num_pair* a, const num_pair* b) {  
            if (a->first != b->first) {  
                return a->first < b->first;  
            } else {  
                return a->second < b->second;  
            }  
        });

        // cout << "res_tmp:" << endl;
        // for (int i=0; i<res_tmp.size(); i++) {
        //     cout << "(" << res_tmp[i]->first << ", " << res_tmp[i]->second << ") ";
        // }
        // cout << endl;

        res.push_back(res_tmp[k-1]->first);
        res.push_back(res_tmp[k-1]->second);
        return res;
    }
    /*    
    1. 首先让nums有序
    2. 先定位第一个数。如果需要第56小的数值对，总数N=10，那么第一个数肯定是第5个数(因为第一个数搞定[1,10)，第二个数搞定[20,30), 第5个数搞定[50,60))
        a. 第一个数位于的位置是：a=(K-1)/10
    3. 然后定位第二个数。假设和第一个数arr[a]相等的数有b个，比第一个数arr[a]小的数有c个。
        a. 比第一个数小的数，将其作为第一个数，能搞定的数值对有：c*N个，则还剩rest=k-c*N个数对需要搞定。这些数对必然都是以arr[a]作为开头的
        b. 在rest里，从头0位置开始，每个数作为第二个数可以搞定b个数值对(比如: 1,2,2,2,3, 3, 3，中arr[a]=3时，1可以与3个3组成(3,1),(3,1),(3,1))， 
            所以第rest个数值对在nums的(rest-1)/b位置
    
    例子：
    arr = [1,1, 2,3], 数值对排序后是(1,1),(1,1), (1,1),(1,1),(1,2),(1,2),(1,3),(1,3),(2,1),(2,1),(2,2),(2,3),(3,1),(3,1),(3,2),(3,3)
    n=4，找第5大的数值对，第一个数在(5-1)/4=1位置。
    第二个数：小于arr[1]=1的数有c=0个，可以搞定c*4=0个，还有rest=5-0=0个需要搞定。
        相等数的有1个的第二个数在(5-1)/2=2位置
    //
    */
    vector<int> kthMinPair2(vector<int> &nums, int k) {
        int len = nums.size();
        vector<int> res;
        if (len == 0)
        {
            return res;
        }
        // O(N*log(N))
        sort(nums.begin(), nums.end());
        // print(nums, "sorted");
        
        // 第一个数位于的位置是：a=(K-1)/10
        int a = (k - 1) / len;
        int first_num = nums[a];
        int less_num_size = 0;
        int first_num_size = 0;

        //O(N)
        for (int i=0; i< len; i++) {
            if (nums[i] < first_num) {
                less_num_size++;
            } else if (nums[i] == first_num) {
                first_num_size++;
            }
        }
        int rest = k - less_num_size * len;
        // cout << " 1st num: " << first_num << ", less_num_size: " << less_num_size << ", first_num_size: " << first_num_size << ", rest: " << rest <<endl;

        int second_num = nums[(rest-1) / first_num_size];
        res.push_back(first_num);
        res.push_back(second_num);
        return res;
    }

    /*    
        在第二种方法中，首先需要对nums排序，这就要N*log(N)的时间复杂度。我们发现之所以要排序，是因为我们想找第a小的数比较方便才做的。如果能在O(N)复杂度内拿到第k小的数，是不是就不用排序了
        可以采用bfprt算法，也可以使用简易的partition topK算法。
    //
    */
    vector<int> kthMinPair3(vector<int> &nums, int k) {
        int len = nums.size();
        vector<int> res;
        if (len == 0)
        {
            return res;
        }
        
        // 第一个数位于的位置是：a=(K-1)/10
        int a = (k - 1) / len;
        //在a位置，那就是数组中的第a+1小
        int first_num = get_top_k(nums, a+1);
        // cout << "first num: " << first_num <<endl;
        int less_num_size = 0;
        int first_num_size = 0;

        //O(N)
        for (int i=0; i< len; i++) {
            if (nums[i] < first_num) {
                less_num_size++;
            } else if (nums[i] == first_num) {
                first_num_size++;
            }
        }
        int rest = k - less_num_size * len;
        // cout << " 1st num: " << first_num << ", less_num_size: " << less_num_size << ", first_num_size: " << first_num_size << ", rest: " << rest <<endl;

        // 在(rest-1) / first_num_size位置的数，其实就是求nums的第(rest-1) / first_num_size+1小
        int second_num = get_top_k(nums, (rest-1) / first_num_size +1 );
        res.push_back(first_num);
        res.push_back(second_num);
        return res;
    }

    //找到第k小的数返回
    int get_top_k(vector<int> &nums, int k) {
        // cout << "top k:" <<  k <<endl;
        int len = nums.size();
        return get_top_core(nums, 0, len-1, k-1);
    }
    //在l和r位置上找到排序后应该在k位置上的数
    int get_top_core(vector<int> &arr, int l, int r, int k) {
        if (l==r) {
            return arr[l];
        }
        int pivot = (int)((std::rand() % (r-l)) + l);
        vector<int> range;
        partition(arr, l, r, pivot, range);
        // cout << "l:" << l << " r: "<< r << " k:" << k << endl;
        // cout << "pivot:" << pivot <<  " small:"<< range[0] << " big:" << range[1] << endl;
        // print(arr, "top_core");
        if (k>=range[0] && k<=range[1]) { //说明第k个位置已经找到了
            return arr[k];
        } else if (k<range[0]) {
            return get_top_core(arr, l, range[0]-1, k);
        } else if (k>range[1]) {
            return get_top_core(arr, range[1] + 1, r , k);
        }
    }
    //在l到r上做一次荷兰国旗问题
    void partition(vector<int> &arr,int l, int r, int pivot, vector<int>& range) {
        int small = l-1;
        int big = r+1;
        int cur = l;
        // cout <<" ===== " <<endl;
        // cout << "small:" << small << " big:" << big << endl;
        while (cur!=big) {
            // cout << "cur:" << cur << " big:" << big << " pivot:" << pivot << endl;
            if (arr[cur] < arr[pivot]) {
                swap(arr, ++small, cur);
                cur++;
            } else if (arr[cur] > arr[pivot]) {
                swap(arr, --big, cur);
            } else {
                cur++;
            }
        }
        //边界的下一个
        range.push_back(small+1);
        range.push_back(big-1);
        // print(range, "range");
        // cout << " ===== " << endl;
    }
    void swap(vector<int> &arr,int i, int j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j]= tmp;
    }

    /*    
        基于方法3，进一步的，将pivot的选择更稳定一些，采用bfprt算法。
        bfptr感觉有点鸡肋，工程上数据量大的时候概率选肯定很稳定，数据量小的时候稳不稳定无所谓。(研究者只关注研究本身)
    //
    */
    vector<int> kthMinPair4(vector<int> &nums, int k) {
        int len = nums.size();
        vector<int> res;
        if (len == 0)
        {
            return res;
        }
        
        // 第一个数位于的位置是：a=(K-1)/10
        int a = (k - 1) / len;
        //在a位置，那就是数组中的第a+1小
        int first_num = get_top_k_bfprt(nums, a + 1);
        // cout << "first num: " << first_num <<endl;
        int less_num_size = 0;
        int first_num_size = 0;

        //O(N)
        for (int i=0; i< len; i++) {
            if (nums[i] < first_num) {
                less_num_size++;
            } else if (nums[i] == first_num) {
                first_num_size++;
            }
        }
        int rest = k - less_num_size * len;
        // cout << " 1st num: " << first_num << ", less_num_size: " << less_num_size << ", first_num_size: " << first_num_size << ", rest: " << rest <<endl;

        // 在(rest-1) / first_num_size位置的数，其实就是求nums的第(rest-1) / first_num_size+1小
        int second_num = get_top_k_bfprt(nums, (rest - 1) / first_num_size + 1);
        res.push_back(first_num);
        res.push_back(second_num);
        return res;
    }

    //找到第k小的数返回
    int get_top_k_bfprt(vector<int> &nums, int k) {
        // cout << "top k:" <<  k <<endl;
        int len = nums.size();
        return get_top_core_bfprt(nums, 0, len-1, k-1);
    }
    //在l和r位置上找到排序后应该在k位置上的数
    int get_top_core_bfprt(vector<int> &arr, int l, int r, int k) {
        if (l==r) {
            return arr[l];
        }
        int pivot = (int)((std::rand() % (r-l)) + l);
        vector<int> range;
        partition(arr, l, r, pivot, range);
        // cout << "l:" << l << " r: "<< r << " k:" << k << endl;
        // cout << "pivot:" << pivot <<  " small:"<< range[0] << " big:" << range[1] << endl;
        // print(arr, "top_core");
        if (k>=range[0] && k<=range[1]) { //说明第k个位置已经找到了
            return arr[k];
        } else if (k<range[0]) {
            return get_top_core_bfprt(arr, l, range[0] - 1, k);
        } else if (k>range[1]) {
            return get_top_core_bfprt(arr, range[1] + 1, r, k);
        }
    }

    // arr L到R上的数字，每5个一组排序，每个组的中位数挑出来组成中位数数组。
    // 再从中位数数组中挑出来中位数
    int get_median_median(vector<int> &arr, int l, int r) {
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
        return get_top_core_bfprt(medians, 0, medians.size() - 1, medians.size() / 2 - 1); // 求medians的中位数
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
};



int main()
{
    Solution sol;
    vector<int> arr = {3,1,2};
    //上面的数值对排序结果为：(1, 1)(1, 2)(1, 3)(2, 1)(2, 2)(2, 3)(3, 1)(3, 2)(3, 3)
    int k = 9;
    vector<int> arr1 = arr;
    vector<int> res1 = sol.kthMinPair1(arr1, k);
    vector<int> arr2 = arr;
    vector<int> res2 = sol.kthMinPair2(arr2, k);
    vector<int> arr3 = arr;
    vector<int> res3 = sol.kthMinPair3(arr3, k);
    vector<int> arr4 = arr;
    vector<int> res4 = sol.kthMinPair4(arr4, k);
    print(res1);
    print(res2);
    print(res3);
    print(res4);
    return 0;
}