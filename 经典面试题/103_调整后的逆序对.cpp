/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-24 18:59:48
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
#include <sstream>

using namespace std;

/*
    * 腾讯原题
    * 
    * 给定整数power，给定一个数组arr，给定一个数组reverse。含义如下：
    * arr的长度一定是2的power次方，reverse中的每个值一定都在0~power范围。
    * 例如power = 2, arr = {3, 1, 4, 2}，reverse = {0, 1, 0, 2}
    * 任何一个在前的数字可以和任何一个在后的数组，构成一对数。可能是升序关系、相等关系或者降序关系。
    * 比如arr开始时有如下的降序对：(3,1)、(3,2)、(4,2)，一共3个。
    * 接下来根据reverse对arr进行调整：
    * reverse[0] = 0, 表示在arr中，划分每1(2的0次方)个数一组，然后每个小组内部逆序，那么arr变成
    * [3,1,4,2]，此时有3个逆序对。
    * reverse[1] = 1, 表示在arr中，划分每2(2的1次方)个数一组，然后每个小组内部逆序，那么arr变成
    * [1,3,2,4]，此时有1个逆序对
    * reverse[2] = 0, 表示在arr中，划分每1(2的0次方)个数一组，然后每个小组内部逆序，那么arr变成
    * [1,3,2,4]，此时有1个逆序对。
    * reverse[3] = 2, 表示在arr中，划分每4(2的2次方)个数一组，然后每个小组内部逆序，那么arr变成
    * [4,2,3,1]，此时有5个逆序对。
    * 所以返回[3,1,1,5]，表示每次调整之后的逆序对数量。
    * 
    * 输入数据状况：
    * power的范围[0,20]
    * arr长度范围[1,10的7次方]
    * reverse长度范围[1,10的6次方]
    * 
    * 
*/

/*
[3,0,6,2,7,8,6,5]
定义一个dp
dp[0]表示以2^0=1个数分组，每个小组内有多少逆序对。dp[0]=0
dp[1]表示以2^1=2个数分组，每个小组内，必须一个数在前面1个，一个数在后面1，构成多少逆序对。
    所以[3,0],[6,2],[7,8],[6,5]有3个逆序对，p[1]=3
dp[2]表示以2^2=4个数分组，每个小组内，必须一个数在前面2个，一个数在后面2，构成多少逆序对。
    所以[3,0,6,2]中1个(3,2)，[7,8,6,5]中有(7,6),(7,5),(8,6),(8,5)共5个，总结下来dp[2]=5
dp[3]表示以2^3=8个数分组，前4个数选一个，后4个数选一个，的逆序对只有(6,5)， 所以dp[3]=1.

dp2[i]，表示以2^i个数分组，组内前一半选一个数，后一半选一个数，正序对有多少个。求出来：
    dp2[0] = 0
    dp2[1] = 1
    dp2[2] = 3
    dp2[3] = 14

大流程：
1. 假设此时revers[i]=1, 以2个数分组，内部逆序交换。数组变成了[0,3,2,6,8,7,5,6]
   此时dp[1]=1, dp2[1]=3，是原来的值交换。可以观察到dp[2]和dp2[2]的值不会有变化，dp[3]也不会有变化。
2. 假设此时revers[i]=3, 以8个数分组，内部逆序交换。数组变成了[5,6,8,7,2,6,0,3]
   所有组别都变成了逆序，所以dp[2],dp[1]都将交换
3. 每来一次reverse[i]，调整后，求一次dp中的累计和就是答案。

求dp和dp2:
    在归并排序过程中，会有merge的过程，merge过程中，左边的比右边的小时，就产生了一个这样的逆序对。
    递归归并排序的过程就是求解dp2的过过程
    要求dp，即逆序对，只需要将整个数组逆序，然后做归并排序，就可以求出逆序对

*/


typedef unsigned long long ll;
class Solution {
public:
    //暴力方法，用作对数器
    vector<int> reverse_pair_baoli(vector<int>& arr, vector<int>& reverse, int power) {
        int len = reverse.size();
        vector<int> res(len, 0);
        for (int i=0;i<len;i++) {
            //分组反转数组
            reverse_arr(arr, reverse[i]);
            //求解答案
            res[i] = get_reverse_pair(arr);
        }
        return res;
    }
    void reverse_arr(vector<int>& arr, int power) {
        int group_num = pow(2, power);
        int len = arr.size();
        int s = 0;
        while (s<len) {
            int end_idx = s + group_num-1;
            for (int i=s;i<s+(group_num/2);i++, end_idx--) {
                int tmp = arr[i];
                arr[i] = arr[end_idx];
                arr[end_idx] = tmp;
            }
            s=s+group_num;
        }
    }
    int get_reverse_pair(vector<int> &arr) {
        int len = arr.size();
        int ans = 0;
        for (int i=0;i<len;i++) {
            for (int j=i+1;j<len;j++) {
                if (arr[i] > arr[j]) {
                    ans++;
                }
            }
        }
        return ans;
    }

    vector<int> reverse_pair(vector<int>& arr, vector<int>& reverse, int power) {
        int r_len = reverse.size();
        int len = arr.size();
        
        vector<int> reverse_arr_tmp = arr; //拷贝出来一份
        int l=0,r=len-1;
        while (l<r) {
            int tmp = reverse_arr_tmp[l];
            reverse_arr_tmp[l] = reverse_arr_tmp[r];
            reverse_arr_tmp[r] = tmp;
            l++;
            r--;
        }

        //求出来两个dp
        vector<int> dp(power+1, 0);//最大范围为power
        vector<int> dp2(power + 1, 0); // 最大范围为power

        process(arr, dp2, power, 0, len-1); //获取正序
        process(reverse_arr_tmp, dp, power, 0, len-1); //获取逆序对dp

        vector<int> res(r_len, 0);

        for (int i=0;i<r_len;i++) {
            //调整时，reverse[i]及以下的dp值都将互换
            for (int j=1;j<=reverse[i];j++) {
                int tmp = dp[j];
                dp[j] = dp2[j];
                dp2[j] = tmp;
            }
            // cout << "after i:" << i << 
            // for (int )

            for (int j=0;j<=power;j++) {
                res[i] += dp[j];
            }
        }
        return res;
    }

    void process(vector<int>& arr, vector<int> &dp, int power, int l, int r) {
        if (l >= r) {
            return;
        }
        int mid = l + ((r-l)>>1); //上分位中位数
        
        process(arr, dp, power - 1, l, mid);
        process(arr, dp, power - 1, mid+1, r);

        //做一次归并merge，产生的正序对记录到dp中
        // cout << "before merge:";
        // for (auto iter:arr) {
        //     cout << iter << " ";
        // }
        // cout << endl;
        int tmp = merge(arr, l, mid, r);
        // cout << "l:" << l << "r:" << r << " mid:" << mid << " power:" << power << " tmp:" << tmp << endl;
        // cout << "after merge:";
        // for (auto iter:arr) {
        //     cout << iter << " ";
        // }
        // cout << endl;
        dp[power] += tmp;
    }

    int merge(vector<int>& arr, int l, int mid, int r) {
        vector<int> help(r-l+1, 0);
        for (int i=l;i<=r;i++) {
            help[i-l] = arr[i];
        }
        int i=0;
        int p1=l,p2=mid+1;
        int ans = 0;
        while (p1<=mid&&p2<=r) {
            //有序，假设左边的比右边的小，则产生r-p2+1个
            /*比如:
            1 1 2 2 1 1 3 3
            0 1 2 3 4 5 6 7
            当p1=0,p2=6时，将产生7-6+1=2个正序对。下面合并时p1会往右挪动
            */
            ans += arr[p1] < arr[p2] ?  r-p2+1 : 0;
            help[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];
        }

        while (p1<=mid) {
            help[i++] = arr[p1++];
        }
        while (p2<=r) {
            help[i++] = arr[p2++];
        }
        for (int i=0;i<help.size();i++) {
            arr[l+i] = help[i];
        }
        return ans;
    }
};

int main() {
    // Solution sol;
    // vector<int> arr = {3, 1, 4, 2};
    // vector<int> arr2 = arr;
    // vector<int> reverse = {0, 1, 0, 2};
    // int power = 2;

    // vector<int> res = sol.reverse_pair_baoli(arr, reverse, power);
    // cout << "baoli:" << endl;
    // for (auto iter:res ) {
    //     cout << iter << " ";
    // }
    // cout << endl;

    // res = sol.reverse_pair(arr2, reverse, power);
    // cout << "rs:" << endl;
    // for (auto iter:res ) {
    //     cout << iter << " ";
    // }
    // cout << endl;

    int test_time = 1;
    int max_len = 10;
    int max_num = 100;
    int max_power = 6;

    int max_revers_len = 10;
    int max_reverse = 15;
    Solution sol;
    for (int testi = 0; testi < test_time; testi++) {
        int power = std::rand() % max_power + 1;

        int len = pow(2, power);
        vector<int> arr1(len,0), arr2(len,0);
        // cout << "arr:";
        for (int i=0;i<len;i++) {
            int tmp = std::rand() % max_num;
            arr1[i] = tmp;
            arr2[i] = tmp;
            // cout <<tmp << " ";
        }
        // cout << endl;

        int reverse_len = std::rand() %max_revers_len+1;
        vector<int> revers1(reverse_len, 0), revers2(reverse_len, 0);
        // cout << "revers:";
        for (int i=0;i<reverse_len;i++) {
            int tmp = std::rand() % (power+1); //最大是power
            revers1[i] = tmp;
            revers2[i] = tmp;
            // cout << tmp<< " ";
        }
        // cout << endl;

        vector<int> res1 = sol.reverse_pair_baoli(arr1, revers1, power);
        vector<int> res2 = sol.reverse_pair(arr2, revers2, power);
        int fail_flag = false;
        for (int i=0;i<res1.size();i++) {
            if (res1[i] != res1[i]) {
                cout << "error" << endl;
                fail_flag = true;
                break;
            }
        }
        if (fail_flag) {
            break;
        }
    }
    return 0;
}