/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-22 23:05:28
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
// 俄罗斯套娃信封问题
// 给你一个二维整数数组envelopes ，其中envelopes[i]=[wi, hi]
// 表示第 i 个信封的宽度和高度
// 当另一个信封的宽度和高度都比这个信封大的时候
// 这个信封就可以放进另一个信封里，如同俄罗斯套娃一样
// 请计算 最多能有多少个信封能组成一组“俄罗斯套娃”信封
// 即可以把一个信封放到另一个信封里面，注意不允许旋转信封
// 测试链接 : https://leetcode.cn/problems/russian-doll-envelopes/

class Solution {
public:
    struct node {
        int w;
        int h;
        node(int _w, int _h) {
            w = _w;
            h = _h;
        }
    };
    
    int maxEnvelopes(vector<vector<int>>& envelopes) {
        
        int len = envelopes.size();
        if (len==0) {
            return 0;
        }

        vector<node> tmps;
        for (int i=0;i<envelopes.size();i++) {
            tmps.push_back(node(envelopes[i][0],envelopes[i][1]));
        }
        // 使用lambda表达式进行排序  
        std::sort(tmps.begin(), tmps.end(), [](const node& a, const node& b) {  
            if (a.w != b.w) {
                return a.w < b.w; //按照宽度升序
            }
            return a.h > b.h; // 按照高度降序
        });

        //寻找排序后高度的最长递增子序列长度
        /*
        1. 当宽度相同时，因为相同宽度内高度是按照递减排序，所以相同宽度情况下，不可能出现高度增加的情况，不会在递增子序列数组中出现
        2. 当宽度不相同时，则一定是递增的。又因为按照高度找到了递增序列，所以后面的信封肯定可以套前面的信封。
        3. 因为已经找到了最长的序列，所以不会有比它更优的，这个贪心比较难证，所以略过
        */

        // for (auto& iter:tmps) {
        //     cout << "("<< iter.w << "," << iter.h << ")";
        // }
        // cout<< endl;

        vector<int> ends(len, 0);
        ends[0] = tmps[0].h;
        int e_right = 0;
        for (int i=1;i<tmps.size();i++) {
            int ans = bs(ends, e_right, tmps[i].h);
            // cout << "tmps[i].h:" << tmps[i].h <<" ans:" << ans << endl;
            if (ans == -1) { //没有找到比tmps[i].h大的数，ends扩展
                ends[++e_right] = tmps[i].h;
            } else {
                ends[ans] = tmps[i].h;
            }
        }
        return e_right+1;
    }

    //在0到r上二分找比大于等于target的位置
    int bs(vector<int>& ends, int r, int target) {
        int ans = -1;
        int l=0, mid = l+((r-l)>>1);
        while (l<=r) {
            // cout << "l:" << l << " r:" << r << " mid:" << mid << " ends[mid]:"<<ends[mid] << " target:" << target<< endl;
            if (ends[mid] >= target) { //要着重注意这里的二分。最终l和r可能错过，也可能相等，这里巧妙用了个ans来记录大于等于nums[i]的情况
                ans = mid;
                r = mid-1;
            } else {
                l = mid+1;
            }
            mid = l+((r-l)>>1);
        }
        return ans;
    }
};

int main()
{
    Solution sol;
    vector<string> arr= {"b#cst#1223#123","b#kl#fd","b#kl#io","d#", "a#de", "ab#c"};
    sol.print_contents(arr);
    return 0;
}