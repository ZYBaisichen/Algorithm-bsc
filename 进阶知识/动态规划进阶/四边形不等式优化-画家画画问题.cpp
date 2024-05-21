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
#include <algorithm>

using namespace std;

/*
* Description: 测试链接：https://leetcode.cn/problems/split-array-largest-sum/
 * 给定一个整型数组 arr，数组中的每个值都为正数，表示完成一幅画作需要的时间，再给定一个整数num
 * 表示画匠的数量，每个画匠只能画连在一起的画作
 * 所有的画家并行工作，返回完成所有的画作需要的最少时间
 * arr=[3,1,4]，num=2。
 * 最好的分配方式为第一个画匠画3和1，所需时间为4
 * 第二个画匠画4，所需时间为4
 * 所以返回4
 * arr=[1,1,1,4,3]，num=3
 * 最好的分配方式为第一个画匠画前三个1，所需时间为3
 * 第二个画匠画4，所需时间为4
 * 第三个画匠画3，所需时间为3
 * 返回4
*/

/*
每个画家负责的范围，总时间是sum_i，要求的即是min{max{sum_0,sum_1...sum_i}}。是将数组分成2块问题扩展到了k块问题


dp[i][j] 0...i幅画，分j个画家，结束时间。dp[n][num]就是答案
dp[...][0]，0个画家，结束时间无穷大
dp[0][1...] 只有一幅画，结束时间都是arr[0]
dp[i][j], 看最后一个画家负责范围， 以dp[7][3]: 
    1. 最后一个画家，负责第7幅画，前6幅画由2个画家负责，结束时间是max{arr[7], dp[6][2]}
    2. 最后一个画家，负责6..7幅画，前5幅画由2个画家负责，结束时间是max{sum[6...7], dp[5][2]} 
    3. 最后一个画家，负责5..7幅画，前4幅画由2个画家负责，结束时间是max{sum[5...7], dp[4][2]} 
    ...
综上，dp的求解过程满足以下特征：
    1. 枚举行为
    2. dp[i][j]依赖左上角的值，不同时依赖本行和本列
    3. 单调性：0..6上有2个画家，当再加一个画家时，结束时间肯定会变小；0...6上有3个画家，当减少一个画家时，总时间肯定会增加
    4. 区间划分
所以可以用四边形不等式优化dp[i][j]的枚举过程，以dp[7][3]为例：
    1. 上边的格子dp[6][3]代表0...6范围上有3个画家，当增加一幅画时，最后一个画家所负责范围不需要往左挪动
    2. 右边格子dp[7][4]代表0...7上有4个画家，当减少一个画家时，最后一个画家所负责范围，往右不会超过有4个画家时所负责的范围。
    3. dp从右往左，从上往下枚举计算


拓展：dp[i][j]依赖本行左边的值，且依赖左上角的值，则可以用左边和下边的值推四边形不等式。
上左行不行？得具体问题具体分析。
*/

/*
最优解：找到一个合适的划块累加和目标，看能划分多少块
nums = [3,2,4,2,3,1,3,1,4,1], k=2. 对画划块，假设每一块目标不超过t=5。
在每个块累加和都不超过5且最长的情况下，nums可以分成[3,2][4][2,3][1,3,1][4,1] 5块，说明目标定的太小了，2个人无法在5时完成。


总的累加和是sum，可以通过二分找到合适的目标，先定t=sum/2
1.还是以上面的nums为例，总累计和sum=24，t=12，可以分成[3,2,4,2][3,1,3,1,4][1] 3块。需要3个人，说明t还是小了，要想12这个时间点完成，至少需要3个画家
2.下一步在t=(t+sum)/2 = 18，可以分成[3,2,4,2,3,1,3][1,4,1]两块，2个人能完成，找到了一个答案，总时间是18
3. 在13-18中间再找重点t=15, 可以分成[3,2,4,2,3,1][3,1,4,1]两块，2个人也能完成，总时间是15，更新答案
4. ... 一直二分直到只剩一个数了
5. 对sum二分，每次二分遍历一遍数组，总复杂度是O(N*log(sum)) ，sum即使来到了long类型最大，log(sum)最大也才64
6. 之所以能对sum二分找目标，是因为结束时间目标越大，需要的画家数量越少；越小需要的画家数量越多。这样的单调性。


注：对比两种方法，最优复杂度上限来源于试法，比如dp的试法不管怎么优化都不如好的试法。
*/
typedef long long ll;
class Solution {
public:
    //四边形等式优化的解，但不是最优解。presum复杂度O(N), 填dp复杂度O(N*k)
    int splitArray_sibianxing(vector<int>& nums, int k) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }

        //使用前缀和计算任何一个范围上所需要的和
        vector<int> presum(len+1, 0);
        for (int i=0;i<len;i++) {
            presum[i+1] = presum[i] + nums[i];
        }
        // for (int i=0;i<=len;i++) {
        //     cout << presum[i] << " ";
        // }
        // cout << endl;
        vector<vector<int>> dp(len, vector<int>(k+1, 0));
        //求dp[i][j]最优解时，最后一个画家负责kk...i范围上的画，将kk记录到choose[i][j]里
        vector<vector<int>> choose(len, vector<int>(k+1, 0));

        for (int i=0;i<len;i++) {
            dp[i][0] = INT_MAX;
        }
        //一幅画时
        for (int j=1;j<=k;j++) {
            dp[0][j] = nums[0];
            choose[0][j] = 0;
        }
        //一个画家时
        for (int i=1;i<len;i++) {
            dp[i][1] = presum[i+1];
            choose[i][1] = 0; //一个画家负责所有的，所以最右边画家负责的范围左边界是0位置
        }
        // cout << "before dp:" << endl;
        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<=k;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        for (int i=1;i<len;i++) {
            for (int j=k;j>=2;j--) {
                int down = choose[i-1][j];//下界
                int up = j == k ? i : choose[i][j+1];
                dp[i][j] = presum[i+1];//最后一个邮局负责0...i, chooose[i][j]=0
                for (int kk=max(down, 1);kk<=up;kk++) { //上面单伶出来了0...i，所以从1..i开始试
                    int tmp = max(presum[i+1]-presum[kk], dp[kk-1][j-1]); //kk...i的累加和，还有0...kk-1幅画有j-1个画家负责，j最小取值2，所以j-1至少有一个画家
                    if (dp[i][j] > tmp) { //发现了更好的答案
                        dp[i][j] = tmp;
                        choose[i][j] = kk;
                    }
                }
            }
        }
        // cout << "after dp:" << endl;
        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<=k;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        // cout << "choose:" << endl;
        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<=k;j++) {
        //         cout << choose[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return dp[len-1][k];
    }
    //二分法解，最优解，复杂度O(N*log(sum))
    int splitArray(vector<int>& nums, int k) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }

        ll sum = 0;
        int max_num = 0;
        for (int i=0;i<len;i++) {
            sum+=nums[i];
        }

        ll l = 0, r = sum;
        ll t = 0;
        ll ans = INT_MAX;
        cout << "sum:" << sum << endl;
        while (l <= r) {
            t = l + ((r-l) >> 1);
            // cout << "l:" << l << " r:" << r << " t:" << t << " ans:" << ans<< endl;
            //看能分成多少组
            int parts=0;
            int cur_sum =0;
            bool flag = false;
            for (int i=0;i<len;i++) {
                if (nums[i] > t) { //单独的一个数 已经超过了目标
                    flag = true;
                    parts = INT_MAX;
                }
            }
            if (!flag) {
                for (int i=0;i<len;i++) {
                    if (cur_sum + nums[i] > t) {
                        parts++;
                        cur_sum = nums[i];
                    } else {
                        cur_sum += nums[i];
                    }
                }
                //最后一部分单独处理
                if (cur_sum<=t) {
                    parts++;
                }
            }
            // cout << "parts:" << parts << " k:" << k<< endl;
            //当前目标可以
            if (parts <= k) {
                ans = min(ans, t);//理论上这里其实不用求min，因为t不断逼近最优目标，当求出来一个可以达到的目标后，后面的t会越来越小
                r = t-1;
            } else {
                l = t+1;
            }
        }

        return (int)ans;
    }
};

int main() {
    Solution sol;

    /*
    输入：nums = [7,2,5,10,8], k = 2
    输出：18
    解释：
    一共有四种方法将 nums 分割为 2 个子数组。 
    其中最好的方式是将其分为 [7,2,5] 和 [10,8] 。
    因为此时这两个子数组各自的和的最大值为18，在所有情况中最小。

    */
    vector<int> arr = {0};
    int k = 1;
    cout << sol.splitArray_sibianxing(arr, k) << endl;
    cout << sol.splitArray(arr, k) << endl;
}