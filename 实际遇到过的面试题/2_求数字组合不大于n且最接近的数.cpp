/*
 * @Author: baisichen
 * @Date: 2024-08-27 18:55:17
 * @LastEditTime: 2024-08-29 16:30:40
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
#include <cmath>
#include <algorithm>

using namespace std;
// 字节面试题 https://www.jianshu.com/p/62659552e2ac
int dfs(string& str_n, vector<int>&nums, int cur, bool pre_eq) {
    int len = str_n.length();
    // cout << "len:" << len << " cur:" << cur<< endl;
    if (cur == len) {
        return 0;
    }
    if (pre_eq) {
        int cur_num = str_n[cur] - '0';
        // cout << "cur_num:" << cur_num << endl;
        for (int j=nums.size()-1; j>=0; --j) { //对于当前位置，在nums找到小于等于且最大的
            if (nums[j] <= cur_num) { //从后往前，找到第一个大于他的数
                // cout << "j:" << j << " nums[j]:" << nums[j] << " cur:" << cur <<  " len:" << len << " tmp:" <<  (len - cur - 1) * 10 << endl;
                int res = nums[j]*pow(10, (len-cur-1)) + dfs(str_n, nums, cur+1, nums[j] == cur_num);
                if (res != -1) {
                    return res;
                } 
            }
        }
        //所有的数都比当前数大了
        return dfs(str_n, nums, cur+1, false);
    } else { //如果前面的枚举数不相等，有两种情况，一种是小于前面的数，一种是舍弃了前面的数.两种情况都可以直接放最大的数
        // cout << "noeq cur:" << nums[nums.size() - 1] * pow(10, (len - cur - 1)) << endl;
        return nums[nums.size() - 1] * pow(10, (len - cur - 1)) + dfs(str_n, nums, cur + 1, false);
    }
    
}
// n=5213，arr={0,3,5,2}，返回结果应该为5205。
int get_num(vector<int>& nums, int n) {
    int len=to_string(n).size();
    sort(nums.begin(), nums.end());
    string str_n = to_string(n);
    cout << "str_n:" << str_n << endl;
    return dfs(str_n, nums, 0, true);
}


int main() {
    // int target = 12345;
    // vector<int> nums = {0,1,2,3};

    int target = 5205;
    vector<int> nums = {0, 3, 5,2};
    cout << get_num(nums, target) << endl;

    return 0;
}