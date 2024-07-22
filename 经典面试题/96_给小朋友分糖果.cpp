/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-15 13:30:22
 * @LastEditors: baisichen
 * @Description: 指针指向有点问题，leetcode没有通过，但思想已经通了
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
https://leetcode.cn/problems/candy/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china


给定一个正数数组arr，表示每个小朋友的得分
任何两个相邻的小朋友，如果得分一样，怎么分糖果无所谓，但如果得分不一样，分数大的一定要比分数少的多拿一些糖果。
1. 假设所有小朋友做成一排，返回在不破坏主规则的情况下，需要的最少糖果数。[1,2,2]最少分配[1,2,1]共4颗糖
2. 再加一条规则，任意两个相邻孩子之间如果得分一样，糖果数必须相同，需要的最少糖果数。[1,2,2]最少分配[1,2,2]共5颗

进阶：
1. arr长度为n，要求时间复杂度O(N)，额外空间复杂度O(1)
2. 假设所有的小朋友坐成一个环形，返回在不破坏上一条规则的情况下，需要的最少糖果数。[1,2,2]最少分配[1,2,2]共5颗。 （来自网易）
*/

/*
1. 当排成一排时，认为开头的小朋友和结尾的小朋友是不可见的，将小朋友得分曲线，画出来可以观察到，波谷只需要分配一个糖果就可以。每上升一次就需要增加一个糖果分配。波顶需要两个方向的上坡取最大值。
    a. 采用两个辅助数组分别表示从右往左，和从左往右的上升坡度。        比如arr=[3,2,1,4,6,2,3,5,2,1,0,9]。
    b. 从左往右，第一个元素是1，当比左边大时+1， 比左边小或等于时恢复成1。left =[1,1,1,2,3,1,2,3,1,1,1,2]。 是左边的坡
    c. 从右往左，第一个元素是1，当比右边大时+1，比右边小或等于时恢复成1. right =[3,2,1,1,2,1,1,4,3,2,1,1]。 是右边的坡
    d. left和right取最大值就是答案，ans[i] = max(left[i],right[i])
2. 如果得分相同，糖果数也需要相同，则可以将计算left的逻辑改成，比左边大时+1，比左边小时恢复成1，和左边相等时置为相等的糖果


进阶1：参考https:// leetcode.cn/problems/candy/solutions/533150/fen-fa-tang-guo-by-leetcode-solution-f01p/
曲线要么是上升，要么是下降
    a，上升过程中，分配糖果加1，逐个累加
    b. 下降过程中，分配的糖果恢复成1；每持续下降一个，前面下降的节点都需要再多分一个糖果。
    c. 有一个特殊情况是，当下降的次数第一次超过了上一次上升的次数，需要将顶点也归类到下降节点中，之后的每次下降都需要再多给顶点分配一个糖果


*/
class Solution {
public:
    int candy_two_arr(vector<int>& ratings) {
        int len = ratings.size();
        if (len == 0) {
            return 0;
        }

        vector<int> left(len, 1);
        vector<int> right(len, 1);
        for (int i=1;i<len;i++) {
            if (ratings[i] > ratings[i-1]) {
                left[i] = left[i-1]+1;
            } else {
                left[i] = 1;
            }
        }
        for (int i=len-2;i>=0;i--) {
            if (ratings[i] > ratings[i+1]) {
                right[i] = right[i+1]+1;
            } else {
                right[i] = 1;
            }
        }

        int ans = 0;
        for (int i=0;i<len;i++) {
            // cout << "i:" << i << " left:" << left[i] << " right:" << right[i]<< endl;
            ans += max(left[i], right[i]);
        }
        return ans;
    }


    int candy(vector<int>& ratings) {
        int len = ratings.size();
        if (len == 0) {
            return 0;
        }

        int ans = 1; //第一个孩子分配一个糖果
        int pre=1;//前一个节点分配的糖果数量
        int up = 1;//上升的节点个数
        int down = 0;//下降的节点个数

        for (int i=1;i<len;i++) {
            if (ratings[i] >= ratings[i-1]) { //等于算到了上升里
                down = 0;
                pre = ratings[i] == ratings[i-1] ? 1 : pre+1;
                ans += pre;
                up = pre; //当和前一个数相等时，上升节点数重新计数
            } else { //下降
                down++; //下降节点的个数
                if (down == up) { //当下降次数首次超过上升次数时，比如左边1,2,5,3,2,2 到最后一个2时up=3，down=3，将5算到下降里，下降节点数按照4计算
                    down++;
                }
                ans += down; //每次下降，前面下降的节点数都需要额外多分一个糖果
                pre = 1; //每次都重置当前糖果数为1，为上升做准备
            }
        }
        return ans;
    }

    //第二问
    int candy2_two_arr(vector<int>& ratings) {
        int len = ratings.size();
        if (len == 0) {
            return 0;
        }

        vector<int> left(len, 1);
        vector<int> right(len, 1);
        for (int i=1;i<len;i++) {
            if (ratings[i] > ratings[i-1]) {
                left[i] = left[i-1]+1;
            } else if (ratings[i] == ratings[i-1]) {
                left[i] = left[i-1];
            } else  {
                left[i] = 1;
            }
        }
        for (int i=len-2;i>=0;i--) {
            if (ratings[i] > ratings[i+1]) {
                right[i] = right[i+1]+1;
            } else if (ratings[i] == ratings[i+1]) {
                right[i] = right[i + 1];
            } else {
                right[i] = 1;
            }
        }

        int ans = 0;
        for (int i=0;i<len;i++) {
            // cout << "i:" << i << " left:" << left[i] << " right:" << right[i]<< endl;
            ans += max(left[i], right[i]);
        }
        return ans;
    }

    /*
    如果考虑相等情况下分配同样的糖果，当在递减过程中出现多次相等情况，单个down就没法记录多种状态。那么就需要有额外的参数来记录.
    */
    int candy2(vector<int>& ratings) {
        int len = ratings.size();
        if (len == 0) {
            return 0;
        }

        int idx = next_min_idx(ratings, 0); //找到第一个波谷节点
        cout << "first idx:" << idx << endl;
        vector<int> data = right_cands_and_base(ratings, 0, idx++);
        cout << "first data:" << data[0] << "," << data[1] << endl;
        int ans = data[0];
        int lbase = 1;//上升期，当前位置需要分配的糖果数
        int next_min;
        int same = 1;
        //整体循环从第一个上升点开始算
        while (idx < len) {
            cout << "idx:" << idx << " lbase:" << lbase << " ans:" << ans<< " same:" << same << endl;
            if (ratings[idx] > ratings[idx-1]) { //上升
                ans += ++lbase;
                idx++;
                same = 1;
            } else if (ratings[idx] < ratings[idx-1]) { //下降的第一个
                next_min = next_min_idx(ratings, idx-1);
                cout << "[little] idx:" << idx << " lbase:" << lbase << " next_min:" << next_min << endl;
                data = right_cands_and_base(ratings, idx-1, next_min++); //计算从定点到最波谷位置的糖果数
                cout << "[little] idx:" << idx << " next_min:" << next_min << " data[0]:"<< data[0] << " data[1]:" <<data[1]  << endl;
                if (data[1] <= lbase) { //左边上升上来的高度比较高
                    ans += data[0] - data[1];//减去下降的最左边的。因为最高点前面已经加上了，所以这里不用重复加
                } else { //下降高度比较高，将最高点算到下降高度里。最高点得分lbase算了same次，需要减去。然后最高点新的得分是data[1]*same。然后right_cands_and_base内部计算的时候，也算了一个最高点的得分，所以要将data[1]干掉
                    ans += data[0] - data[1] + data[1]*same - lbase*same; 
                }
                idx = next_min;
                lbase = 1;
                same = 1;
            } else {
                ans += lbase; //相等直接加
                same++;
                idx++;
            }
        }

        return ans;
    }
    //从start开始找到下一个波谷
    int next_min_idx(vector<int>& arr, int start) {
        for (int i=start;i<arr.size()-1;i++) {
            if (arr[i+1] > arr[i]) {
                return i;
            }
        }
        return arr.size()-1;
    }
    //根据最小值，计算最小值左边的得分
    vector<int> right_cands_and_base(vector<int> &arr, int left, int right) {
        int base = 1;
        int cands = 1;
        cout << "[cal right] left:" << left << " right:" << right << endl;
        for (int i=right-1;i>=left;i--) {
            if (arr[i] == arr[i+1]) {
                cands += base;
            } else {
                base++;
                cands+=base;
            }
        }
        cout << "[cal right] cands:" << cands << " base:" << base << endl;
        //base为左边的最高得分，cands为总得分
        return {cands, base};
    }
};

int main() {
    Solution sol;
    vector<int> arr = {1,3,5,5,3,2,2,1,4,5,6,10,5,9};
    cout << sol.candy(arr) << endl;
    cout << sol.candy_two_arr(arr) << endl;
    cout << sol.candy2(arr) << endl;
    cout << sol.candy2_two_arr(arr) << endl;
    return 0;
}