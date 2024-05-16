/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-16 11:29:33
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
给定一个路径数组patchs，表示一张图。patch[i]==j代表城市i连向城市j，如果paths[i]==i，则表示i城市在首都，
一张图里只会有一个首都且图中除首都指向自己之外，不会有环。
例如， paths=[9,1,4,9,0,4,8,9,0,1]

       1
       |
       9
    /  | \
  0    3  7
/   \
4     8
| \    \
2  5    6

  
由数组表示的图可以知道，城市1是首都，所以距离是0，离首都距离为1的城市只有城市9，离首都距离为2的城市0、3、7，离首都距离为3的城市有城市4和8，
离首都距离为4的城市有2、5、6。所以距离首都为0的城市有1座，距离为1的城市有1座，距离为2的城市有3座，距离为3的城市有2座，距离为4的城市有3座。
那么统计数组为nums=[1,1,3,2,3,0,0,0,0,0]。nums[i]==j代表距离为i的城市有j座。要求实现一个void类型的函数，输入一个路径数组paths，直接在原数组上调整，
使之变为nums数组，即paths=[9,1,4,9,0,4,8,9,0,1]经过这个函数处理后变成[1,1,3,2,3,0,0,0,0,0]

【要求】
如果paths长度为N，请达到时间复杂度为O(N)，额外空间复杂度为O(1)
*/

/*
一、建图
直观的思路是，根据路径数组patchs建立图，然后宽度优先遍历即可求出来。
但这样就需要额外的空间。

二、下标循环怼(典型例题)
1. 首先需要生成这样一个中间数组tmp, tmp[i]==-j代表城市i到首都的距离是j。
以paths=[9,1,4,9,0,4,8,9,0,1]为例，从paths[0]出发，
    1) 记init={0,9}，next=9， last=0
    2) 根据next来到paths[9]位置，依次执行next=1;paths[9]=last=0;last=9; paths变成[9,1,4,9,0,4,8,9,0,0]
    3) 根据next来到paths[1]位置，发现paths[1]是首都，往回跳。next=last=9， 初始distance=0或distance=-paths[1]（st. paths[1]为负数时）
    4) 根据next位置来到paths[9], 依次执行next=paths[9]=0, path[9]=-(++distance)=1。path变成[9,1,4,9,0,4,8,9,0,-1]
    5) 根据next位置来到paths[0]，发现0==init[0],来到了初始位置，paths[0]=-(++distance)=-2，结束本轮下标循环怼。path变成[-2,1,4,9,0,4,8,9,0,-1]
    6) 找到下一个值非负且非首都的点(比如是paths[2])开始下标循环怼，直到最后。
    7) 最后再将首都位置的距离变成0。


    注：假设中间某个状态没有跳到首都，则初始化distance的距离可能不同。比如paths=[-1,1,8,-2,-1,7,-3,3,5]。
        从paths[2]位置开始，2->8->5->7->3 发现paths[3]=-2已经计算出来了，初始distance=2, 依次改paths[7]=-3;paths[5]=-4;paths[8]=-5;paths[2]=-6
2. 然后将中间距离数组变成统计数组，即paths[i]==j代表距离首都距离为i的城市有j座。比如初始paths=[-2,-1,-2,-1,0,-3,-2]
    1) 从paths[0]开始，init=0, 发现paths[0]是负数，开始循环怼。记录next={0,-2}， 将paths[0]=0。 此时paths = [0,-1,-2,-1,0,-3,-2]
    2) 来到paths[2]位置，首选记录next={2,-2}，然后执行paths[2]=0;paths[2]++ == 1;。此时paths=[0,-1,1,-1,0,-3,-2]
    3) 对于next={2,-2}，同样来到paths[2]位置，发现此时它的值已经是非负数了，则直接加，paths[2]=2;next={}。 此时paths=[0,-1,2,-1,0,-3,-2]
    4) 此时因为next为空了，结束了本轮循环怼。
    5) 依次往下遍历做循环怼
    6) 最后将paths[0]=1, 代表首都，即距离首都为0的城市只有首都一座
*/


class Solution {
public:
    void path_to_distance(vector<int>& paths) {
        int len = paths.size();
        int init_idx = 0;
        int next = 0;
        int last = 0;
        int cap = 0;
        for (int i=0;i<len;i++) {
            if (paths[i] < 0) { //已经计算过了
                continue;
            }
            if (paths[i]==i) {
                cap = i; //记录首都位置
                continue;
            }
            init_idx = i;
            next = paths[i];
            last = i;
            //往前怼
            while (paths[next] != next) { //没到首都就继续
                cout << "next:" << next << " last:" << last << endl;
                if (paths[next] < 0) { //已经计算过
                    break;
                }

                int next_tmp = paths[next];
                paths[next] = last;
                last = next;
                next = next_tmp;
            }

            // cout << "after front"<<endl;
            // for (auto iter:paths) {
            //     cout << iter << " ";
            // }
            // cout << endl;
            // break;

            int dis = paths[next] == next ? 0 : -paths[next];//如果next是首都，则距离初始化为0。否则用计算出来的值
            next = last;
            cout << "begin back: next:" << next << endl;
            while (next != init_idx) {
                cout << "paths:" << next<<endl;
                int next_tmp = paths[next]; // 先拿出来下一个要去的位置
                paths[next] = -(++dis); //更新距离
                next = next_tmp; 
            }
            paths[init_idx] = -(++dis); //更新初始化点
        }
        paths[cap] = 0;
    }
    void distance_to_nums(vector<int> &paths) {
        int len = paths.size();
        for (int i=0;i<len;i++) {
            if (paths[i] >= 0) { //首都和已经计算过的跳过
                continue;
            }
            int next = -paths[i];
            paths[i] = 0;
            int init_idx = i;
            while (paths[next] < 0) { //小于0一直怼，最后再补
                int next_tmp = -paths[next];
                paths[next]=0;
                paths[next]++;
                next = next_tmp;
            }
            paths[next]++;//最后来到大于0的位置，next肯定不会再更新了，跳出了循环，最后补一下
        }
        paths[0] = 1; //首都
    }
    void path_to_nums(vector<int>& paths) {
        int len = paths.size();
        if (len==0) return;
        path_to_distance(paths); // 将arr转换成距离数组
        cout << "distance:" << endl;
        for (auto iter:paths) {
            cout << iter << " ";
        }
        cout << endl;
        distance_to_nums(paths); // 将距离数组转化成统计数组
    }
    
};

int main() {
    Solution sol;
    vector<int> arr = {9,1,4,9,0,4,8,9,0,1};
    //{1,1,3,2,3,0,0,0,0,0}
    sol.path_to_nums(arr);
    for (auto iter:arr) {
        cout << iter<< " ";
    }
    cout << endl;
    return 0;
}