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

主要参考：https://blog.csdn.net/weixin_46838716/article/details/124289803

// 目标是: 把0~i的圆盘，从from全部挪到to上
// 返回，根据arr中的状态arr[0..i]，它是最优解的第几步？
如果arr不是最优解的某一步，返回-1。

给你一个arr数组，每一个arr[i]代表棍子的编号【最简单的3根棍子，编号123】，意味着i号盘子，现在正在arr[i]棍子上。
这个arr显然就是代表汉诺塔搬移过程中的某一个步骤【状态】，
如果arr是汉诺塔问题最优走法的其中一步，请问你这是第几步？
返回arr是最优走法的第几步，如果不是最优走法的步骤，请返回-1。



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
    相当于递归遍历了arr的状态，来到了第i号盘子。
    大步骤一共三步：
        1. 将n-1层盘子挪动到other上
        2. 将第n个盘子挪动到to上
        3. 将n-1层盘子挪动到to上
    n层汉诺塔问题最优总步数是2^n-1不，比如3层，最优时2^3-1=7步
    可以看出，对于i层汉诺塔问题，最后一个盘子要么在from上，要么在to上，不可能在other上。所以
    1) arr[i]在other上说明不是最优解中的某一步
    2) arr[i]在from上, 说明还需要将i-1层盘子从from挪动到other上
    3) arr[i]在to上，说明已完成了1、2两大🙅🏻步，这两步总步数是2^(i-1)步。下面还需要继续往后将i-1个盘子从other上挪动到to上   

    定义f函数，f(arr, i, from, other, to)， 当还有i+1个盘子，最后一个盘子编号是i时，当前需要做的是将盘子从from借助other搬到to上。返回arr的状态在第几步

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

        if (arr[i] == from) { //还一步没有动，所以查看i个盘子(编号0..i-1)时从from借助to挪动到other上的步数
            int ans = process(arr, i - 1, from, to, other);
            cout << "i:" << i << " from:" << from << " other:" << other << " to:" << to  << " res2:" << ans << endl;
            return ans;
        } else { //arr[i] == to
            //已经走完了1,2两步了。将前i个盘子挪动到了other上，已经走的步数是2^i-1。再加上将第i+1个盘子从from挪动到了to上
            /*
            n层汉诺塔问题，固定最优解是2^n-1步，第一步将n-1层从from挪到other上，步数是2^(n-1)-1
            第二步将n从from挪到to上，花了一步。
            前两步总步数是2^n

            第三步走到了哪里，继续调用递归函数算出来。
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