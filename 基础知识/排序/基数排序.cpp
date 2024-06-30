/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-30 12:13:35
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
    //找到nums的第d位数返回
    int get_digit(int num, int d) {
        int tmp = 1;
        for (int i=1;i<d;i++) {
            tmp *= 10;
        }

        return (num/tmp)%10;
    }
    //在l..r上排序，最高位数有digit个
    /*
    实现比较有意思，使用了一个前缀数组，好比当前轮次是比较个位数。
    count数组第i个元素代表的是个位数<=i的数量是count[i]，到时候出队的时候个位数<=i的数量放在help数组的0-count[i]-1范围内，最大下标为count[i]-1。
    然后从后往前遍历nums，后面的一定是后出的（(模拟的就是每个桶是一个队列，先进的先出来，一定是在前面的，这样是为了保持低位的相对次序)），
    所以后面的如果遇到一个个位数为i，则将其放在help的count[i]-1位置，如此一遍即可完成个位数的排序，反复执行完十位数、百位数等就可以完成整体排序。
    */
    void radixSort(vector<int>& nums) {
        int len = nums.size();
        if (len==0) {
            return;
        }
        int digit = 0;
        for (int i=0;i<len;i++) {
            string str = to_string(nums[i]);
            digit = max(digit, (int)str.length());
        }
        cout << digit << endl;

        vector<int> count(10,0); //10个桶
        vector<int> help(len, 0); //暂存arr中的数据
		int redix = 10;
        int i = 0,j=0;

        for (int d=1; d<=digit;d++) {
            for (int j=0;j<len;j++) {
                int idx = get_digit(nums[j], d); //获得第d位数
                cout << "idx:" << idx << endl;
                count[idx]++;
            }
            //计算累加和
            for (int j=1;j<10;j++) {
                count[j] += count[j-1];
            }
            // cout << "count:" << endl;
            // for (auto it:count) {
            //     cout << it << " ";
            // }
            // cout << endl;
            //从数组最后开始遍历
            for (int j=len-1;j>=0;j--) {
                int idx = get_digit(nums[j], d);
                if (count[idx]-- > 0) {//假设有多个当前位是idx的数，下个数过来排名就需要递减了
                    //假设count[idx]=3,则表示0~2范围可填入数据，当前数填入2下标处
                    help[count[idx]] = nums[j];
                }
            }
            //将数据从辅助数组中转移到nums中
            for (int i=0;i<len;i++){
                nums[i] = help[i];
            }
            // for (auto it:nums) {
            //     cout << it << " ";
            // }
            // cout << endl;
            std::fill(help.begin(), help.end(), 0);
            std::fill(count.begin(), count.end(), 0);
        }
	}


};

int main()
{
    Solution<int> sol;
    vector<int> nums = {100,5,13,17,21,29};
    sol.radixSort(nums);
    for (auto iter:nums) {
        cout << " " << iter;
    }
    cout << endl;
    return 0;
}