
/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-28 21:21:47
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

 https://leetcode.com/problems/create-maximum-number/
给你两个整数数组 nums1 和 nums2，它们的长度分别为 m 和 n。数组 nums1 和 nums2 分别代表两个数各位上的数字。同时你也会得到一个整数 k。

请你利用这两个数组中的数字中创建一个长度为 k <= m + n 的最大数，在这个必须保留来自同一数组的数字的相对顺序。

返回代表答案的长度为 k 的数组。

 

示例 1：

输入：nums1 = [3,4,6,5], nums2 = [9,1,2,5,8,3], k = 5
输出：[9,8,6,5,3]
示例 2：

输入：nums1 = [6,7], nums2 = [6,0,4], k = 5
输出：[6,7,6,0,4]
示例 3：

输入：nums1 = [3,9], nums2 = [8,9], k = 3
输出：[9,8,9]
 

提示：

m == nums1.length
n == nums2.length
1 <= m, n <= 500
0 <= nums1[i], nums2[i] <= 9
1 <= k <= m + n

*/

/*
方法一：单调数组方法：
在num1中拿i个数字， 在nums2中拿k-i个数字。
在nums中拿i个数字，使用一个单调栈，遍历一遍，在单调栈中从底到顶是递减的序列，最后保证栈中保留i个数字就可以
每次这样找到数组复杂度是O(m+n)

两个数组挑选出来后，做merge
nums1_i和nums2_k_i，双指针i和j分别指向两个数组的开始。然后每次到达一个新的位置时，从i和j开始向后比较，直到分出胜负，将字典序大的放在前面。向后移动。
最坏的情况是[9,9,9,9]和[9,9,9]，每次都需要遍历到数组结尾才能知道谁的字典序比较大，每次i和j到达新的位置，最坏遍历m+n次。复杂度O((N+M)^2)

枚举k次求结果，总复杂度O(k*(m+n+(n+m)^2))， 简化是O(k*(m+n)^2), k最大取到m+n，所以是O((m+n)^3)


方法二：dp+merge加速
首先第一个方法不再使用单调栈，而是先准备一个dp， dp[i][j]表示[i...]范围上，拿出j个数，最大数列的开头位置
   1. dp[i][len-i]，表示从i开始，往后取len-i个数，只有一种取法(因为i往后就只有len-i个数了), 就是i往后所有，开头位置是i
   2. dp[i][j]， st. j<len-i， 有两种情况，必须取i，依赖dp[i+1][j-1](i+1上取j-1个数)；必须不取i，依赖dp[i+1][j](i+1往后取j个数)
        取两个可能背后的arr对应元素，最大的；当相等时，取前面的，因为开头位置选的靠前可以有笔后面更多的可能性，方便后面取最大的。
每次在nums数组中取i个数组最大的数时，可以使用dp方便拿到。
比如要在arr中拿3个数，即从0开始往后范围，拿3个数：
    首先找到第一个数的位置dp[0][3]=2， 将arr[2]作为第一个数
    接下来需要在3...n范围上拿剩余的2个数，找到dp[3][2]=4， 则将arr[4]作为第二个数
    接下来在4...n范围上拿剩余的1个数，找到dp[4][1]=8, 则将arr[8]作为第三个数。
    可以在O(K)复杂度拿到，但dp数组是O(n*k)和O(m*k)的。

    同样使用方法一中的merge O((n+m)^2)

    总复杂度是O(m*k)+O(n*k)+O(k*(k+(n+m)^2)) 化简保留高阶是O(k*(n+m)^2)，k最大是m+n，复杂度还是O((m+n)^3)


方法三：后缀数组方法优化merge
第一步从nums中拿i个数的最大数列，还是使用方法二中的dp方法，复杂度O(k)，dp生成O(n*k)+O(m*k)
第二步，将两个数列拼接到一起，中间使用0隔断，得到后缀数组。比如:
    [3339] 下标为[0,1,2,3]
    [331] 下标为[0',1',2']
    排名一样，则右边的靠前，因为更短
    [33390331]的后缀数组是
    [45670321]
    排名越大越在前面，所以整体排名是：[3339331] 对应到原数组是[0,1,2,3,0',1',2']

    生成后缀数组DC3是O(N), merge时每次可以在O(1)时间知道以i和j开头时的后缀数组排名，排名越大越排在前面，有O(n+m)复杂度。
    外层枚举k次，所以整体复杂度是O(n*k)+O(m*k)+O(k*(k + (n+m) + (m+n)*1)) = O(n*k+m*k+k^2+2*k*m+2*k*n)=O(3k*(m+n)+k^2)
    k最多取到M+N， 所以复杂度可以到达O((m+n)^2)

*/

/*
摘自《Simple Linear Work Suffix Array Construction》论文
*/



class Solution {
public:
    int longestRepeatingSubstring(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        vector<vector<int>> dp(len, vector<int>(len, 0));
        int ans = 0; //至少是1
        //第一行
        for (int j=1;j<len;j++) {
            dp[0][j] = s[0]==s[j]?1:0;
        }
        for (int i=1;i<len;i++) {
            for (int j=i+1;j<len;j++) {
                if (i!=j && s[i] == s[j]) {
                    dp[i][j] = dp[i-1][j-1]+1;
                    ans = max(ans, dp[i][j]);
                }
            }
        }
        // for (int i=0;i<len;i++) {
        //     for (int j=0;j<len;j++) {
        //         cout << dp[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
        return ans;
    }
};

int main() {
    Solution sol;
    string s = "aaaaa";
    cout << sol.longestRepeatingSubstring(s) << endl;

    return 0;
}