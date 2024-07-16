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

   给定一个N*3的矩阵matrix，对于每一个长度为3的小数组arr，都表示一个大楼的是三个数据。
arr[0]表示大楼的左边界，arr[1]表示大楼的右边界，arr[2]表示大楼的高度(一定大于0)。每座大楼的地基都在x轴上，大楼之间可能会有重叠，请返回整体的轮廓线数组。
例子：matrix={{2,5,6},{1,7,4},{4,6,7},{3,6,5},{10,13,2},{9,11,3},{12,14,4},{]10,12,5}}
返回：{{1,2,4},{2,4,6},{6,7,4},{9,10,3},{10,12,5},{12,14,4}}


轮廓线主要在于最大高度，需要在某个区间上设置最大高度。但线段树只能add和update，所以还需要改写，不够高效。

如果能在到达每个位置时拿到最大高度是否变化，就可以找到所有轮廓线了。
所有大楼的开始和结束的点的最大高度如果能记录下来放在一个map中，就可以通过这个变化或者大楼轮廓线。下面讨论怎么生成这个map：
流程：
1. 将每个大楼抽象成增加和结束的节点，比如{5,9,3}，变成{5位置增加了高度3}, {9位置高度减少了3}
2. 为防止同一个位置多次降低或增加, 还需要增加一个map，记录高度出现的次数。因为所有key中，可能需要知道当前最大的高度是什么，所以需要使用到有序表
3. 按照位置排序，这样就可以从左到右来遍历。如果有纸片大楼，即在7位置开始在7位置结束，所以还需要将增加的节点放在前面
4. 到一个位置时，每次获取最大高度。
5. 根据每个位置的最大高度生成轮廓线

*/

struct Node {
public:
    int x;
    int h;
    bool is_add;
    Node(int _x, int _h, bool _add) {
        x=_x;
        h=_h;
        is_add = _add;
    }
};
class Solution
{
public:

    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        int len =buildings.size();
        //构建节点数组
        vector<Node> ops;
        for (int i=0;i<len;i++) {
            ops.push_back(Node(buildings[i][0], buildings[i][2], true)); // 增加高度
            ops.push_back(Node(buildings[i][1], buildings[i][2], false)); // 降低高度
        }
        
        std::sort(ops.begin(), ops.end(), [](const Node& a, const Node& b) {  
            if (a.x != b.x) {  
                return a.x < b.x;  //x不相等时按照从小到大排序
            } else {  
                if (a.is_add) { //如果是增加的排在前面
                    return true;
                } else {
                    return false;
                }
            }  
        });

        // for (int i=0;i<ops.size();i++) {
        //     cout << "x:" << ops[i].x << " h:" << ops[i].h << " is_add:" << ops[i].is_add << endl;
        // }

        //key 高度， value 该高度出现的次数。这里利用到了一个贪心，前面加进去的高度一定会在对应大楼结尾处删掉
        map<int, int> max_h_times;
        map<int, int> max_height;//每个位置的最大高度

        //计算每个位置的最大高度
        for (int i=0;i<ops.size();i++) {
            // cout << "x:" << ops[i].x << " h:" << ops[i].h << " is_add:" << ops[i].is_add << endl;
            if (ops[i].is_add) {
                if (max_h_times.find(ops[i].h) == max_h_times.end()) {
                    max_h_times[ops[i].h] = 1;
                } else {
                    max_h_times[ops[i].h]++;
                }
            } else {
                if (max_h_times[ops[i].h]==1) {
                    max_h_times.erase(ops[i].h);
                } else {
                    max_h_times[ops[i].h]--;
                }
            }

            //如果减到了空，说明当前最大高度是0
            if (max_h_times.empty()) {
                // cout << "x:" << ops[i].x << " max_height:" << 0 << endl;
                max_height[ops[i].x] = 0;
            } else {
                max_height[ops[i].x] = max_h_times.rbegin()->first; //返回key最大的位置的高度
                // cout << "x:" << ops[i].x << " max_height:" << max_height[ops[i].x] << endl;
            }
        }
        

        //根据最大高度拿到轮廓线或者本题要求轮廓线开始的点
        vector<vector<int>> res;
        // for (auto& iter : max_height) {
        //     cout << "cur_x:" << iter.first << " cur_h:" << iter.second << endl;
        // }
        // return res;
        int last_h = -1;
        for (auto& iter : max_height) {
            // cout << "cur_x:" << iter.first << " cur_h:" << iter.second << endl;
            int cur_x = iter.first;//一定是按照x从小到大排序的
            int cur_h = iter.second;
            if (cur_h == last_h) { //没找到高度跳变点则不更新答案
                continue;
            }
            vector<int> tmp(2,0);
            tmp[0] = cur_x;
            tmp[1] = cur_h;
            res.push_back(tmp);
            last_h = cur_h;
        }
        return res;
    }
};

int main()
{

    Solution sol;
    vector<vector<int>> arr = {{0,2,3},{2,5,3}};
    vector<vector<int>>  res = sol.getSkyline(arr);
    for (int  i=0; i<res.size();i++) {
        for (int j=0;j<res[i].size();j++) {
            cout<< res[i][j]<<" ";
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}