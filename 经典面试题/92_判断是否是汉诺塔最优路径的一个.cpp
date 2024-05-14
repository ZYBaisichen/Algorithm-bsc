/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-12 11:46:53
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

https://blog.csdn.net/weixin_46838716/article/details/124289803

// 目标是: 把0~i的圆盘，从from全部挪到to上
// 返回，根据arr中的状态arr[0..i]，它是最优解的第几步？
如果arr不是最优解的某一步，返回-1。

*/

/*

*/

class Solution
{
public:
    int step(vector<int> &arr)
    {
        int len = arr.size();
        if (len==0) {
            return -1;
        }
        //在0...i上，将所有圆盘从1挪到3上
        return process(arr, len-1, 1, 2, 3);
    }
    /*
    把0-i的圆盘，从from全部挪到to上
    返回，arr中的状态arr[0...i]，它是最优解的第几步。
    */
   //递归每个if只会走一次，从n到0只会是单路径的，所以总复杂度是O(N)
    int process(vector<int>& arr, int i, int from, int other, int to) { 
        cout << "i:" << i << " from:" << from << " other:" << other << " to:" << to << endl;
        if (i == -1) {
            cout << "i:" << i << " from:" << from << " other:" << other << " to:" << to  << " res1:" << 0 << endl;
            return 0;
        } 

        //任何递归过程出现-1，都是违规的
        if (arr[i] != from && arr[i] != to) {
            return -1;
        }

        if (arr[i] == from) {
            int ans = process(arr, i - 1, from, to, other);
            cout << "i:" << i << " from:" << from << " other:" << other << " to:" << to  << " res2:" << ans << endl;
            return ans;
        } else { //arr[i] == to
            //已经走完了1,2两步了
            /*
            n层汉诺塔问题，固定最优解是2^n-1步，第一步将n-1层从from挪到other上，步数是2^(n-1)-1
            第二步将n从from挪到to上，花了一步。
            前两步总步数是2^n

            第三步走到了哪里，继续调用递归函数算出来
            */
            int rest = process(arr, i-1, other, from, to);
            if (rest == -1) {
                return -1;
            }
            cout << "i:" << i << " from:" << from << " other:" << other << " to:" << to << " res3:" << (1 << i) + rest << endl;
            return (1<<i) + rest;
        }
    }
};

int main()
{

    Solution sol;
    vector<int> arr={1,2,3};
    cout << sol.step(arr) << endl;
    return 0;
}