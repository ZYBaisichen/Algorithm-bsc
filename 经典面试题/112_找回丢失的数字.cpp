/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-28 19:54:55
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
整型数组arr长度为n(3 <= n <= 10^4)，最初每个数字是<=200的正数且满足如下条件：
1. 0位置的要求：arr[0]<=arr[1]
2. n-1位置的要求：arr[n-1]<=arr[n-2]
3. 中间i位置的要求：arr[i]<=max(arr[i-1],arr[i+1])
但是在arr有些数字丢失了，比如k位置的数字之前是正数，丢失之后k位置的数字为0
请你根据上述条件，计算可能有多少种不同的arr可以满足以上条件
比如 [6,0,9] 只有还原成 [6,9,9]满足全部三个条件，所以返回1，即[6,9,9]达标
*/

/*
f(i,v,s)。现在来到了i位置，i位置的数字是v，s是右边数和当前数字的关系：
s=0: 表示arr[i] < arr[i+1]
s=1: arr[i]==arr[i+1]
s=2: arr[i]>arr[i] //右边的数比我小
在i位置数字是v，并且与右侧数字有s的关系，这样的前提下[0...i]有多少种变法。

主函数调用：
1. 比如：arr[6,0,9]， 最右侧数字不是0，主函数调用f(2,9,2), 右边没数，但为了符合规则，必须默认右边的数当前数小(不满足规则)。这样左边的数字就可以要求必须比我大。
2. 当最右侧数字是0时，则需要f(n-1,1,2)+f(n-1,1,2)+...+f(n-1,200,2)。挨个将n-1位置的数字变成1到200(最大限制)，求出来让0..n-1范围上数组达标的方法数

可能性：
见代码

*/

/*
一开始可能会枚举数组中的每个0位置，然后给它填上不同的数，但这样的话会改变数组arr的状态，让整个暴力枚举的状态无限增多。
所以需要想办法怎么样简化整个过程
*/

class Solution {
public:
    int restoreWays(vector<int>& arr) {
        int len = arr.size();
        if (arr[len-1] != 0) {
            return process(arr, len-1, arr[len-1], 2);
        }  else {
            int ways = 0;
            for (int v=1;v<=200;v++) {
                ways+= process(arr, len-1, v, 2);
            }
            return ways;
        }
    }
    /*
    f(i,v,s)。现在来到了i位置，i位置的数字是v，s是右边数和当前数字的关系：
    s=0: 表示arr[i] < arr[i+1]
    s=1: arr[i]==arr[i+1]
    s=2: arr[i]>arr[i+1] //右边的数比我小
    */
    int process(vector<int> &arr, int i, int v, int s)
    {
        /*
        来到最后一个数了, 让它变成v。
        比较v和右侧的关系
        */
        if (i==0) { 
            /*
            变出来有效数组的条件
            s==0 || s==1  : 小于等于右边的数
            arr[i] == 0 || arr[i] == v: 第0位置是0，或者本来就等于v，才可以变
            */
            if ((s==0||s==1) && (arr[i]==0 || arr[i] == v)) {
                return 1;
            }else {
                return 0;
            }
        }

        //i>0

        if (arr[i] != 0 && arr[i] != v) { //不能改变原来的数
            return 0;
        }

        //运行到这里说明arr[i]==0 || arr[i]==v
        
        int ways = 0;
        //arr[i]<=arr[i+1]，右边已经满足条件，所以左边可以任意指定
        if (s==0 || s==1) {
            for (int pre=1;pre<201;pre++) { //左边变成1到200，都试一遍。每次调用需要计算pre和当前的v的关系
                ways += process(arr, i-1, pre, pre < v ? 0 : (pre == v ? 1 : 2));
            }
        } else { //arr[i]>arr[i+1], 要求左边必须达标
            for (int pre=v;pre<201;pre++) {
                ways += process(arr, i-1, pre, pre == v ? 1 : 2);
            }
        }
        
        return ways;
    }

    //经典dp
    int restoreWays_dp(vector<int>& arr) {
        int len = arr.size();
        //三维，i,v,s，取值最大为len-1, 200, 2
        vector<vector<vector<int>>> dp(len, vector<vector<int>>(201, vector<int>(3)));

        //i依赖i-1.i-2，所以i从0开始填
        if (arr[0] != 0) { //0位置不是0，则必须变成arr[0]。右边比当前的大或等于时，找到1个答案
            dp[0][arr[0]][0] = 1;
            dp[0][arr[0]][1] = 1;
        } else {
            for(int v =1;v<=200;v++) {
                dp[0][v][0] = 1;
                dp[0][v][1] = 1;
            }
        }

        // cout << "i:" << 0 << endl;
        // for (int v=1;v<201;v++) {
        //     for (int s=0;s<3;s++) {
        //         cout << dp[0][v][s] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        for (int i=1;i<len;i++) {
            //因为v和s所在平面，都依赖下层平面，所以顺序没有关系
            for (int v=1;v<201;v++) {
                for (int s=0;s<3;s++) {
                    int ways=0;
                    if (s==0 || s==1) { //当右边的数比我小或等于时
                        for (int pre=1;pre<201;pre++) { //左边变成1到200，都试一遍。每次调用需要计算pre和当前的v的关系
                            ways += dp[i-1][pre][pre < v ? 0 : (pre == v ? 1 : 2)];
                        }
                    } else { //arr[i]>arr[i+1], 要求左边必须达标
                        for (int pre=v;pre<201;pre++) {
                            ways += dp[i-1][pre][pre == v ? 1 : 2];
                        }
                    }
                    dp[i][v][s] = ways;
                }
            }

            // cout << "i:" << i << endl;
            // for (int v=1;v<201;v++) {
            //     for (int s=0;s<3;s++) {
            //         cout << dp[i][v][s] << " ";
            //     }
            //     cout << endl;
            // }
            // cout << endl;
        }

        if (arr[len-1] != 0) {
            return dp[len-1][arr[len-1]][2];
        } else {
            int ways = 0;
            for (int v=1;v<=200;v++) {
                ways += dp[len-1][v][2];
            }
            return ways;
        }
    }


    //dp优化
    int restoreWays_dp_youhua(vector<int>& arr) {
        int len = arr.size();
        //三维，i,v,s，取值最大为len-1, 200, 2
        vector<vector<vector<int>>> dp(len, vector<vector<int>>(201, vector<int>(3)));

        //i依赖i-1.i-2，所以i从0开始填
        if (arr[0] != 0) { //0位置不是0，则必须变成arr[0]。右边比当前的大或等于时，找到1个答案
            dp[0][arr[0]][0] = 1;
            dp[0][arr[0]][1] = 1;
        } else {
            for(int v =1;v<=200;v++) {
                dp[0][v][0] = 1;
                dp[0][v][1] = 1;
            }
        }

        vector<int> sum0(201, 0);
        vector<int> sum1(201, 0);
        vector<int> sum2(201, 0);

        //先计算i=0时的前缀和数组
        for (int v=1;v<201;v++) {
            sum0[v] = sum0[v - 1] + dp[0][v][0];
            sum1[v] = sum1[v - 1] + dp[0][v][1];
            sum2[v] = sum2[v - 1] + dp[0][v][2];
        }

        // cout << "i:" << 0 << endl;
        // for (int v=1;v<201;v++) {
        //     for (int s=0;s<3;s++) {
        //         cout << dp[0][v][s] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        /*
        注意到三层循环内部有枚举行为：
        当s==0和s==1时，v在0-200之间变化。假设当前i=5，v=100, 则需要枚举dp[4][1-200][0-3]。
        注意到累加和是以下三部分组成的：
            dp[4][1-99][0]
            dp[4][100][1]
            dp[4][101-200][2]

        所以如果能有三个前缀和数组，sum0,sum1,sum2
        分别为dp[4][1-200][0],dp[4][1-200][1],dp[4][1-200][2]的前缀和数组，则可以在O(1)时间获得上述三个部分的值

        */
        for (int i=1;i<len;i++) {
            //因为v和s所在平面，都依赖下层平面，所以顺序没有关系
            for (int v=1;v<201;v++) {
                for (int s=0;s<3;s++) {
                    int ways=0;

                    //累加1到v-1
                    if (s==0 || s==1) {
                        ways += sum0[v-1]-sum0[0];
                    }
                    //累加v
                    ways += dp[i-1][v][1];
                    //累加v+1到200
                    ways += sum2[200] - sum2[v];

                    dp[i][v][s] = ways;
                }
            }

            //计算新的前缀和数组，用于i+1平面
            for (int v=1;v<201;v++) {
                sum0[v] = sum0[v - 1] + dp[i][v][0];
                sum1[v] = sum1[v - 1] + dp[i][v][1];
                sum2[v] = sum2[v - 1] + dp[i][v][2];
            }

            // cout << "i:" << i << endl;
            // for (int v=1;v<201;v++) {
            //     for (int s=0;s<3;s++) {
            //         cout << dp[i][v][s] << " ";
            //     }
            //     cout << endl;
            // }
            // cout << endl;
        }

        if (arr[len-1] != 0) {
            return dp[len-1][arr[len-1]][2];
        } else {
            //len-1行右边比当前行小的情况下，1-200的累加和
            return sum2[200];
        }
    }
};

int main() {
    Solution sol;
    vector<int> nums = {6,0,0,9};
    cout << sol.restoreWays(nums) << endl;
    cout << sol.restoreWays_dp(nums) << endl;
    cout << sol.restoreWays_dp_youhua(nums) << endl;

    return 0;
}