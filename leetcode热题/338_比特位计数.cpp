/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-22 00:27:15
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

using namespace std;

/*
https://leetcode.cn/problems/counting-bits/

给你一个整数 n ，对于 0 <= i <= n 中的每个 i ，计算其二进制表示中 1 的个数 ，返回一个长度为 n + 1 的数组 ans 作为答案。

 

示例 1：

输入：n = 2
输出：[0,1,1]
解释：
0 --> 0
1 --> 1
2 --> 10
示例 2：

输入：n = 5
输出：[0,1,1,2,1,2]
解释：
0 --> 0
1 --> 1
2 --> 10
3 --> 11
4 --> 100
5 --> 101
 

提示：

0 <= n <= 105
 

进阶：

很容易就能实现时间复杂度为 O(n log n) 的解决方案，你可以在线性时间复杂度 O(n) 内用一趟扫描解决此问题吗？
你能不使用任何内置函数解决此问题吗？（如，C++ 中的 __builtin_popcount ）

*/

/*

https://leetcode.cn/problems/counting-bits/solutions/627418/bi-te-wei-ji-shu-by-leetcode-solution-0t1i/
官方题解中特别秀的三种位dp。


*/

class Solution {
public:
    //O(nlog(n))
    vector<int> countBits1(int n) {
        vector<int> res(n+1, 0);
        for(unsigned int i=1;i<=n;i++) {
            int ans = 0;
            unsigned int tmp = i;
            while (tmp != 0) {
                // cout << "tmp:" << tmp << " you:" << (tmp>>1)<< endl;
                if (tmp&1 == 1) {
                    ans++;
                }
                tmp = (tmp>>1);
            }
            res[i] = ans; 
        } 
        return res;       
    }
    /*
     按照高位dp
     1. 对于1到n中的每个数x，肯定会有前面一个数，它的1比自己少1个。
     2. 对于base case， 如果一个数是2^k， 则1的个数是1. 
        使用brain kernighan算法，可以使用x&(x-1)是否是0，判断一个数是否是2的某次方
     3. 所以对于x，假设最接近x，且是2的某次方的值是y。则一定有res[i] = res[x-y]+1;
     */
    vector<int> countBits2(int n) {
        vector<int> res(n+1, 0);
        int pre = 0;
        for (int i=1;i<=n;i++) {
            if ((i&(i-1)) == 0) {
                pre = i;
                res[i] = 1;
            } else {
                // cout << "i:" << i << " pre:" << pre << endl;
                res[i] = res[i-pre] + 1;
            }
        }
        return res;
    }


    /*
    看最后一位，如果当前数x是偶数，则右移一位变成y，不会改变二进制中1的个数，则res[x]=res[y]
    如果当前数x是奇数，则右移一位变成y，会让二进制中1的个数少1个，则res[x]=res[y]+1
    后面是否加1，即加上x%2
    */
    vector<int> countBits3(int n) {
        vector<int> res(n+1, 0);
        int pre = 0;
        for (int i=1;i<=n;i++) {
            res[i] = res[i>>1] + i%2;
        }
        return res;
    }

     /*
    对于任意的x，将最后面的1去掉之后，就少了个1。
    所以res[x] = res[x&(x-1)] + 1
    */
    vector<int> countBits(int n) {
        vector<int> res(n+1, 0);
        int pre = 0;
        for (int i=1;i<=n;i++) {
            res[i] = res[i&(i-1)] + 1;
        }
        return res;
    }
};

int main()
{
    Solution sol;
    int n=2;
    auto res = sol.countBits(n);
    for (auto it:res) {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}
