/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-24 15:18:04
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
#include <unordered_map>
#include <list>

using namespace std;

/*

https://leetcode.cn/problems/task-scheduler/description/

给你一个用字符数组 tasks 表示的 CPU 需要执行的任务列表，用字母 A 到 Z 表示，以及一个冷却时间 n。每个周期或时间间隔允许完成一项任务。任务可以按任何顺序完成，但有一个限制：两个 相同种类 的任务之间必须有长度为 n 的冷却时间。

返回完成所有任务所需要的 最短时间间隔 。

 

示例 1：

输入：tasks = ["A","A","A","B","B","B"], n = 2
输出：8
解释：A -> B -> (待命) -> A -> B -> (待命) -> A -> B
     在本示例中，两个相同类型任务之间必须间隔长度为 n = 2 的冷却时间，而执行一个任务只需要一个单位时间，所以中间出现了（待命）状态。 
示例 2：

输入：tasks = ["A","A","A","B","B","B"], n = 0
输出：6
解释：在这种情况下，任何大小为 6 的排列都可以满足要求，因为 n = 0
["A","A","A","B","B","B"]
["A","B","A","B","A","B"]
["B","B","B","A","A","A"]
...
诸如此类
示例 3：

输入：tasks = ["A","A","A","A","A","A","B","C","D","E","F","G"], n = 2
输出：16
解释：一种可能的解决方案是：
     A -> B -> C -> A -> D -> E -> A -> F -> G -> A -> (待命) -> (待命) -> A -> (待命) -> (待命) -> A
 

提示：

1 <= tasks.length <= 104
tasks[i] 是大写英文字母
0 <= n <= 100
*/

/*
来自题解：https://leetcode.cn/problems/task-scheduler/solutions/196302/tong-zi-by-popopop/
桶的思想
1. 建立大小为n+1的桶（因为任务本身要消耗1时间），个数是任务数量最多的那个任务。比如n=2，重复任务数最多的是A有6个, 则建立6个桶，每个容量为3，我们可以把每个桶看做一轮任务
    A - -
    A - -
    A - -
    A - -
    A - -
    A - -
2. 最简单的，如果没有其他任务，我们完成任务A的时间应该是(6-1)*3+1=16. 最后一个桶结束后不需要再等待了。
3. 接下来我们添加些其他任务，当加入C时，对总时间没有产生影响，因为C全在冷却时间内；加入B后，B的数量和A相同，这就导致了最后一个桶需要多执行一个任务，总时间是hi(6-1)*3+2=17
    A B C
    A B C
    A B
    A B
    A B
    A B
4. 总结下来，总时间=(桶个数-1)*(n+1)+最后一个桶的任务数
5. 当冷却时间短，任务种类多时，需要临时在每个桶内加入新的元素。比如加入了F，同样满足冷却时间。可以认为此时，所有的桶都填满了，所有任务都紧凑的放在一起执行。
   总时间就是任务的大小len
    A B C F
    A B C F
    A B C
    A B C
    A B C 
    A B
6. 4、5两种情况取最大值就可以。因为当没有填满桶时，总时间一定比len小
*/
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> c_cnt(26, 0);
        int len = tasks.size();
        for (int i=0;i<len;i++) {
            c_cnt[tasks[i]-'A']++;
        }
        sort(c_cnt.rbegin(), c_cnt.rend()); //倒序排列
        int max_num = c_cnt[0];
        int max_cnt = 0;//任务数量最大的数的统计。也代表着最后一个桶的任务数量
        int idx = 0;
        while (idx<c_cnt.size() && c_cnt[idx] == max_num) {
            max_cnt++;
            idx++;
        }
        return max(len, (max_num-1)*(n+1)+max_cnt);
    }
};

int main()
{
    Solution sol;

    return 0;
}
