/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-05 17:57:11
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
// 给定一个正数数组arr，代表若干人的体重
// 再给定一个正数limit，表示所有船共同拥有的载重量
// 每艘船最多坐两人，且不能超过载重
// 想让所有的人同时过河，并且用最好的分配方法让船尽量少
// 返回最少的船数
// 测试链接 : https://leetcode.com/problems/boats-to-save-people/


贪心：
arr排序，找到<=limit/2最右的位置让l指向它，r指向>limit/2最左的位置。
让arr[l]和arr[r]凑一船：
1. arr[l]+arr[r]>limit l往左滑, 并标记叉号
2. arr[l]+arr[r]<=limit r往右划到所有满足arr[l]+arr[r]<=limit的下一个位置。l往左同样移动相同的数量做配对。
    假设此时l来到了a位置，因为a再往左一定是小于等于arr[a]的
3. 考虑边界条件：
    a. 左侧先耗尽时：左侧标记叉号数量a两个人一条船,需要a/2条船。右侧还有没搞定的人每个人一条船b。配对好的c条船，共需要a/2+b+c条船
    b. 右侧先耗尽时: 比如[1,1,1,1,1,1,1,1,5,5,5,9,9,9]，limit=10。 三个5标记叉号a，需要a/2条船。左侧没搞定的b,每两个人一条船b/2。三个1和三个9配对一条船c。总共需要的船数是a/2+b/2+c

*/

class Solution
{
public:

    int numRescueBoats(vector<int>& people, int limit) {
        int len = people.size();
        sort(people.begin(), people.end());
        if (people[len-1]>limit) { //有人体重超过了船的载重，没有方案
            return -1;
        }
        int l=-1,r=-1;//l指向最右边<=limit/2的数
        for (int i=0;i<len;i++) {
            if (people[i] <= limit/2) {
                l = i;
            } else {
                break;
            }
        }
        
        if (l==len-1) { //如果都是小于limit/2的，则每两个人坐一条船
            return (len+1)/2;//向上取整
        } else if (l==-1) { //全都大于limit/2，每个人一条船
            return len;
        }
        r=l+1;
        int no_used = 0; //叉号标记数量
        int solved = 0;
        while (l>=0 && r<len) {
            if (people[l]+people[r]>limit) {
                no_used++;
                l--;
            } else {
                //一次解决多个
                while (l>=0 && r<len && people[l]+people[r]<=limit) {
                    // cout << "11111111111==" << endl;
                    solved++;
                    l--;
                    r++;
                }
                // cout << "l:" << l << " r:" << r << endl;
            }
        }
        // cout << "asdfasd===" << endl;
        if (l==-1) { //左边先耗尽
            return (no_used+1)/2 + (len-r) + solved; //标记叉号的两个人一条船（向上取整），右侧还有(len-r)个人没有搞定，一人一条船。再加上配对好的。
        } else {
            return (l+1 + no_used + 1)/2 + solved; //左侧没走到的人数有l+1个，标记叉号的有no_used个，总人数相加除2向上取整。再加上配对好的
        }
    }
};

int main()
{

    Solution sol;
    vector<int> arr = {
        19524,46,5769,10032,7182,
        19121,11359,26261,17044,12432,
        12068,21054,12546,19865,1404,
        13360,7777,4731,15827,14617};
    int limit = 29998;
    cout << sol.numRescueBoats(arr, limit) << endl;
    return 0;
}