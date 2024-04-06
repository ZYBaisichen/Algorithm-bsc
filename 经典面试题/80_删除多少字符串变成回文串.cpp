/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-06 12:05:04
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
给定一个字符串str，通过删除一些字符可以将其编程回文串，规定空串不是回文串，删除的位置不同，也认为是不同回文串。问有多少种删除方案？
有多少删除方案就相当于有多少保留方案。例如:ABA有5种，取：A、B、A、AA、ABA


思路：
范围上的尝试模型，dp[l][r]表示l..r上有多少保留方案。
1. 对角线只有1种。次对角线相等就是3种，不等就是2种。
2. dp[l][r]普遍位置:
    a. 不保留l和r，a种方案
    b. 保留l但不保留r，b种方案
    c. 保留r但不保留l, c种方案
    d. 两个都保留，条件是str[l]==str[r], 假设方案数是d
3. 临近位置分析
    a. dp[l+1][r]可能含有r也可能不含有r，所以其值是a+c（不含l）
    b. dp[l][r-1]可能含有l也可能不含有l，所以其值是a+b（不含r）
    c. 因为最终要求多种方案的累加，要求a+b+c。观察到dp[l+1][r]+dp[l][r-1]=2a+b+c
        所以a+b+c=dp[l+1][r]+dp[l][r-1]-dp[l+1][r-1]， 设甲=a+b+c
    d. 如果[l]==[r]就有了第四种情况。因为要保留l和r，中间的保留数是a。中间又可以是空串，所以要再加个1.即d=a+1
*/

class Solution
{
public:
    int ways(string &str)
    {
        int len = str.length();
        vector<vector<int>> dp(len, vector<int>(len, 0));

        // 对角线
        for (int i=0;i<len;i++){
            dp[i][i] = 1;
        }
        //次对角线
        for (int i=0;i<len-1;i++){
            if(str[i]==str[i+1]){
                dp[i][i+1] = 3;
            }else{
                dp[i][i+1] = 2;
            }
        }
        // 普遍位置
        for (int l=len-3;l>=0;l--){
            for (int r=l+2;r<len;r++) { 
                dp[l][r] = dp[l+1][r]+dp[l][r-1]-dp[l+1][r-1];
                if(str[l]==str[r]){
                    dp[l][r] += dp[l+1][r-1]+1;
                }
            }
        }
        // for (int i=0;i<len;i++){
        //     for (int j=0;j<len;j++){
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return dp[0][len-1];
    }
};

int main()
{

    Solution sol;
    // string s = "aba";
    string s = "xxy";
    cout << sol.ways(s) << endl;
    return 0;
}