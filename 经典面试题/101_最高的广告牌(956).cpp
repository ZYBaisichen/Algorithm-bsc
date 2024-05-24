/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-24 15:10:35
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
https://leetcode.cn/problems/tallest-billboard/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china
你正在安装一个广告牌，并希望它高度最大。这块广告牌将有两个钢制支架，两边各一个。每个钢支架的高度必须相等。

你有一堆可以焊接在一起的钢筋 rods。举个例子，如果钢筋的长度为 1、2 和 3，则可以将它们焊接在一起形成长度为 6 的支架。

返回 广告牌的最大可能安装高度 。如果没法安装广告牌，请返回 0 。

 

示例 1：

输入：[1,2,3,6]
输出：6
解释：我们有两个不相交的子集 {1,2,3} 和 {6}，它们具有相同的和 sum = 6。
示例 2：

输入：[1,2,3,4,5,6]
输出：10
解释：我们有两个不相交的子集 {2,3,5} 和 {4,6}，它们具有相同的和 sum = 10。
示例 3：

输入：[1,2]
输出：0
解释：没法安装广告牌，所以返回 0。
 

提示：

0 <= rods.length <= 20
1 <= rods[i] <= 1000
sum(rods[i]) <= 5000

*/

/*

使用一个map，key为两个钢管的长度差值。value是差值为key的钢管对中，最大的钢管对中的较短的长度值。
这样就可以记录下来，差值为key的最高的钢管高度。

零、初始时只有长度为0的两个钢管，map[0]=0
一、遍历rods数组，对于每个rods[i]:
    1. 遍历map的每个key、value。可以得到此时的长钢管为key+value, 短钢管为value。
        a. 假设要将rods[i]放到长钢管上，则新的差值为(key+value+rods[i])-value = key+rods[i]，先放到map'中
        b. 假设将rods[i]放在短钢管上，则新的差值为abs((key+value)-(value+rods[i]))= abs(key-rods[i])，放在map'中
    2. 将map'中的元素合并至map，如果没有则直接拷贝；如果有则取较大值
二、最终的答案就是map中key=0的value。
*/


class Solution {
public:
    int tallestBillboard(vector<int>& rods) {
        int len = rods.size();
        if (len == 0) {
            return 0;
        }
        map<int, int> _map;
        map<int, int> _map_tmp;
        _map[0] = 0;
        for (int i=0;i<len;i++) {
            _map_tmp.clear();
            //先复制一份
            // cout << "i:" << i << endl;
            for (auto iter:_map) {
                _map_tmp[iter.first] = iter.second;
                // cout << iter.first << "," << iter.second << " ";
            }
            // cout << endl;

            //遍历每个原始map
            for (auto iter:_map_tmp) {
                int diff = iter.first;
                int v = iter.second;
                //将rods[i]放在长的钢管上面
                int new_diff = diff+rods[i];
                if (_map.find(new_diff) != _map.end()) {
                    if (_map[new_diff] < v) {
                        _map[new_diff] = v;
                    }
                } else {
                    _map[new_diff] = v;
                }

                //将rods放在短的上面
                int long_v = diff+v;
                int new_v = v + rods[i] > long_v ? long_v: (v + rods[i]); //新的value是加到短的上面之后，较短的钢管
                new_diff = abs(diff-rods[i]);
                if (_map.find(new_diff) != _map.end()) {
                    if (_map[new_diff] < new_v) {
                        _map[new_diff] = new_v;
                    }
                } else {
                    _map[new_diff] = new_v;
                }
            }
        }
        return _map[0];
    }
};

int main() {
    Solution sol;
    vector<int> arr = {1,2,3,6};
    //{1,1,3,2,3,0,0,0,0,0}
    cout << sol.tallestBillboard(arr) << endl;
    return 0;
}