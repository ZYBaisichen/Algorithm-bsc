/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-05 14:34:33
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

using namespace std;

/*
nim博弈问题
给定一个非负数组arr，每一个值代表该位置上有几个铜板。a和b玩游戏，a先手，b后手，轮到某个人的时候，只能在一个位置上拿任意数量的铜板，但不能不拿。
谁最先把铜板拿完谁赢。假设a和b都是极度聪明，请返回获胜者的名字。

流程：arr中所有的数字异或到一起，如果不等于0先手赢，否则后手赢
解释：
输的条件：先面对数组都是0，所有0异或起来是0.
先手赢: 如果能做到更严格个目标，让自己都面对异或和不是0状态，并且让后手每次都面对异或和是0的状态。

[7,4,2], [111,100,010], 总异或和001 != 0
假设先手从7上拿，因为4和2异或和为110，从7上拿走一个变成110，总异或和为0。
此时，每个位置上的1都是偶数，后手不管怎么拿，拿完之后肯定会让某个位置上的1不是偶数，总异或和不再是0。


*/

class Solution
{
public:

    void nim(vector<int>& arr) {
        int len = arr.size();
        int eor = 0;
        for (int i=0;i<len;i++) {
            eor ^= arr[i];
        }
        if (eor == 0) {
            cout << "后手赢" << endl;
        } else {
            cout << "先手赢" << endl;
        }
    }
};

int main()
{

    Solution sol;
    return 0;
}