/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-28 11:11:27
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
盒子消消乐。https://leetcode.cn/problems/remove-boxes/
给出一些不同颜色的盒子 boxes ，盒子的颜色由不同的正数表示。

你将经过若干轮操作去去掉盒子，直到所有的盒子都去掉为止。每一轮你可以移除具有相同颜色的连续 k 个盒子（k >= 1），这样一轮之后你将得到 k * k 个积分。

返回 你能获得的最大积分和 。

 

示例 1：

输入：boxes = [1,3,2,2,2,3,4,3,1]
输出：23
解释：
[1, 3, 2, 2, 2, 3, 4, 3, 1] 
----> [1, 3, 3, 4, 3, 1] (3*3=9 分) 
----> [1, 3, 3, 3, 1] (1*1=1 分) 
----> [1, 1] (3*3=9 分) 
----> [] (2*2=4 分)
示例 2：

输入：boxes = [1,1,1]
输出：9
示例 3：

输入：boxes = [1]
输出：1
 

提示：

1 <= boxes.length <= 100
1 <= boxes[i] <= 100

*/

/*
范围尝试：
f(l,r)表示l，r范围上消除所有盒子最大得分。但f(l,r)还需要补充一个当前的arr状态，arr状态很多，不是一个很好尝试

另外的尝试：核心思想方向是简化arr的状态
f(l,r,k) 消除l..r范围上的盒子，前面跟了k个和arr[l]一样样色的盒子包袱。
比如11111222111222111，长度为17。初始调用为f(0,16, 0), 在0..16范围上消除，前面跟了0个1。下面列举可能性
    1. 前面的k个1和第一个1消除，后面调用f(1,17,1)；前面的k个1和前2个1消除，后面调用f(2,17,2)；
    2. 这里有个小贪心，前面的k个1，完全没必要和后面的m个1分开消除。比如最开始前面跟着0个1，没有必要和0..4位置上的1分开消除。
    3. 所以假设前面k个1，连上左边的1共kk个。则枚举这kk个1怎么消除的可能性：
        a. kk个1单独消除，后面调用f(kk+1, r, 0)
        b. 假设kk之后下次出现1的位置分别在h,i,j，则可以先消除kk+1..h-1范围上的数，将这kk个1扔给h位置之前消除，调用f(h,r,kk);
            或者将kk个1扔给i位置一起消除，先消除kk+1...i-1位置上的盒子，然后调用f(i,r,kk);
            或者将kk个1扔给j位置一起消除，先消除kk+1...j-1位置上的盒子，然后调用f(j,r,kk);
            上面几种可能性取最大值
        c. a和b的可能性取最大值得到f的答案

        
*/

class Solution {
public:
    int removeBoxes(vector<int>& boxes) {
        int len = boxes.size();
        if (len == 0) {
            return 0;
        }    
        //l取值0..len-1, r取值0..len-1, k取值0..len-1
        vector<vector<vector<int>>> dp(len, vector<vector<int>>(len, vector<int>(len, -1)));

        //3个维度是O(N^3)，内部还有个枚举，O(n^4)
        return f(boxes, 0,len-1, 0, dp);
    }
    int f(vector<int>& boxes, int l, int r, int k, vector<vector<vector<int>>>& dp) {
        if (l>r) {
            return 0;
        }
        if (dp[l][r][k] != -1) {
            return dp[l][r][k];
        } 
        
        if (l == r) { //只有一个数了，则连着前面的k个数一起消除
            return (k+1)*(k+1);
        }

        int origin_l = l;
        int kk = k;
        int len = boxes.size();
        //l将到达最后一个和第一个字符相同的地方的前一个
        while (l+1<=r && boxes[l] == boxes[l+1]) {
            l++;
            kk++;
        }
        kk++; //当前l位置还在最后一个和包袱相等的位置，所以这里加1.
        // cout << "origin_l:" << origin_l << " l:" << l << " r:" << r << " k:" << k << " kk:" << kk << endl;
        int ans = kk*kk + f(boxes, l+1, r, 0, dp); //前面kk个boxes[l]一起消除

        for (int i=l+1;i<=r;i++) { 
            if (boxes[i] == boxes[l]) {
                ans = max(ans, f(boxes, l+1, i-1, 0, dp) + f(boxes, i, r, kk, dp));
            }
        }
        dp[origin_l][r][k] = ans;
        // cout << "l:" << origin_l << " r:" << r << " k:" << k << " ans: " << ans << endl;
        return ans;
    }

    //考察是否可以改成迭代动态规划
    int removeBoxes_dp(vector<int>& boxes) {
        int len = boxes.size();
        if (len == 0) {
            return 0;
        }    
        /*
        f函数中l依赖l+1,l+2...
        r依赖r-1,r-2,
        k依赖0..len-1都有可能
        将l看成高度，从上往下填。
        r和k看成二维平面的行和列，按照行的从左往右，列的从上往下。因为每一列依赖前一列的所有值，所以需要填好一列再填一列
        r和k的二维平面，l==r时等于(k+1)*(k+1)
        */

        vector<vector<vector<int>>> dp(len, vector<vector<int>>(len, vector<int>(len, 0)));

        for (int l=len-1;len>=0;--l) {
            for (int k=0;k<len;k++) {
                for (int r=0;r<len;r++) {
                    if (l == r) {
                        dp[l][r][k] = (k+1)*(k+1);
                    } else if (l<r) {
                        //不是严格的k依赖关系，所以严格动态规划有点难。
                    }//l>r的情况不用管
                }
            }
        }
    }


};

int main() {
    Solution sol;
    vector<int> boxes = 
        {1,1,1,1,1,1,1,1,1,1,2,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
     // {0,1,2, 3,4,5,6,7,8,9}
    cout << sol.removeBoxes(boxes) << endl;

    return 0;
}