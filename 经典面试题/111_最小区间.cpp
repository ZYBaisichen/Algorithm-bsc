/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-28 14:18:10
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
#include <unordered_set>

using namespace std;

/*
https://leetcode.cn/problems/smallest-range-covering-elements-from-k-lists/description/

你有 k 个 非递减排列 的整数列表。找到一个 最小 区间，使得 k 个列表中的每个列表至少有一个数包含在其中。

我们定义如果 b-a < d-c 或者在 b-a == d-c 时 a < c，则区间 [a,b] 比 [c,d] 小。

 

示例 1：

输入：nums = [[4,10,15,24,26], [0,9,12,20], [5,18,22,30]]
输出：[20,24]
解释： 
列表 1：[4, 10, 15, 24, 26]，24 在区间 [20,24] 中。
列表 2：[0, 9, 12, 20]，20 在区间 [20,24] 中。
列表 3：[5, 18, 22, 30]，22 在区间 [20,24] 中。
示例 2：

输入：nums = [[1,2,3],[1,2,3],[1,2,3]]
输出：[1,1]
 

提示：

nums.length == k
1 <= k <= 3500
1 <= nums[i].length <= 50
-105 <= nums[i][j] <= 105
nums[i] 按非递减顺序排列

*/

/*
每个list是有序的
准备一个有序表，可以O(1)时间拿到最大值和最小值。
    1. 首先将每个list的第一个数放入有序表。拿到最大值和最小值，则这两个值所包含的范围[min,max]一定包含了有序表中所有的数。
        而有序表中包含了所有list中的一个数，所以这个[min,max]范围一定至少包含了每个链表中的一个数。这样就得到了一个答案
    2. 弹出有序表最小的值，然后将其对应的链表中，再拿出来一个，放入有序表中。又得到一个新的[min,max]范围，和原来的范围比较，取间距最小，且首位最小的。

里面的一个贪心:
    不用从没有出现过的数字开始试区间边界
*/

class Solution {
public:
    struct node {
        int val;
        int from;
        int idx; //在链表中的位置
        node(int v, int f, int id) {
            val = v;
            from = f;
            idx = id;
        }
    };

    struct comp {
        bool operator()(const node& a, const node& b) const {
            // cout << "a:" << a.val << " b:" << b.val << endl;
            return a.val < b.val; //有序表中，这里a是新到来的元素，新来的元素和开始元素相比较，如果比较小的话，则放在前面
        }
    };

    vector<int> smallestRange(vector<vector<int>>& nums) {
        multiset<node, comp> ml_set;
        int len = nums.size();//链表数量
        for (int i=0;i<len;i++) {
            ml_set.insert(node(nums[i][0], i, 0));
        }

        vector<int> res(2, 0);
        res[0] = ml_set.begin()->val;
        res[1] = ml_set.rbegin()->val;

        int min_range = res[1]-res[0];

        //当set中的数小于len时。一定是某个链表没有数据再入了，且它的最后一个元素是最后set弹出的元素。因为弹出的最后一个数肯定是set中最小的数，后面再加的数都不会再包含该链表中的数了，所以要结束循环
        while (ml_set.size() == len) {
            auto tmp = ml_set.begin();
            int from = tmp->from;
            int idx = tmp->idx;
            ml_set.erase(ml_set.begin());
            if (idx+1 < nums[from].size()) {
                ml_set.insert(node(nums[from][idx+1], from, idx+1));
                //每入一次set，结算一次答案
                int begin = ml_set.begin()->val;
                int end = ml_set.rbegin()->val;
                if ((end-begin) < min_range) { //等于的时候不更新，肯定不会比之前的答案好
                    min_range = end-begin;
                    res[0] = begin;
                    res[1] = end;
                }
            }
        }
        return res;
    }
};

int main() {
    Solution sol;
    vector<vector<int>> nums = {
        {4,10,15,24,26}, 
        {0,9,12,20}, 
        {5,18,22,30}};
    //输出：[20,24]

    string s = "dcdcdddabcaddcdcbdbcadadadddac";
    // cout << sol.strangePrinter_baoli(s) << endl;
    vector<int> res = sol.smallestRange(nums);
    for (auto it:res) {
        cout << it << " ";
    }
    cout << endl;

    return 0;
}