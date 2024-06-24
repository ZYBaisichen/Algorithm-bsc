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

https://leetcode.cn/problems/palindromic-substrings/


给你一个字符串 s ，请你统计并返回这个字符串中 回文子串 的数目。

回文字符串 是正着读和倒过来读一样的字符串。

子字符串 是字符串中的由连续字符组成的一个序列。

 

示例 1：

输入：s = "abc"
输出：3
解释：三个回文子串: "a", "b", "c"
示例 2：

输入：s = "aaa"
输出：6
解释：6个回文子串: "a", "a", "a", "aa", "aa", "aaa"
 

提示：

1 <= s.length <= 1000
s 由小写英文字母组成
*/

/*


*/

class Solution {
public:
    /*
    O(n^2)的算法
    1. 每到一个i位置，往左右两边扩，算上自己求出来回文半径count，去掉#号之后，从i找到的回文串的半径是count/2
    2. 回文串的个数其实就是回文半径的大小
    */
    int countSubstrings(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }

        int ans = 0;
        string ss = "#";
        for (int i=0;i<len;i++) {
            ss += s[i];
            ss += "#";
        }

        int ss_len = ss.length();
        int count = 0,l=0,r=0;
        int ans = 0;
        for (int i=1;i<ss_len-1;i++) {
            l=i-1,r=i+1;
            count = 1; //算上自己的回文半径
            while (l>=0 && r<ss_len && ss[l] == ss[r]) {
                count++;
                l--;
                r++;
            }
            ans += (count/2); //去掉#的回文半径是count/2
        }
        return ans;
    }

    /*
    manacher算法求出整个数组的回文半径数组
    manacher基本概念有：回文半径数组p，最右位置，最右回文串的中心点C。
    算法流程：
        1. 将原始字符串插入#号，为了找到偶数长度回文串的情况
        2. 初始C=0，r=0, 回文半径数组p[0]=1
        3. 当前cur位置如果超过了r，则从cur位置开始暴力往两边扩
        4. 当cur位置在r内的时候，分别讨论：
            a. cur位置跟c对称点在tmp位置，以tmp位置为中心的回文半径可以通过p[tmp]知道，其回文串的左边界在l_tmp位置
            b. 当l_tmp在l和c之间的时候：p[cur] = p[tmp]
            c. 当l_tmp在l左边时：p[cur]=r-cur+1
            d. 当l_tmp在l之上时：至少可以扩充到r位置，从r位置开始往右扩，如果能扩得动就同时更新r。
    */
    int countSubstrings(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        string ss = "#";
        for (int i=0;i<len;i++) {
            ss += s[i];
            ss += "#";
        }

        int ss_len = ss.length();
        vector<int> p(ss_len, 0);
        p[0]=1;
        int c=0;
        int r=0;
        int l_tmp = 0, r_tmp=0, cnt_tmp=0;
        for (int cur=1;cur<ss_len;cur++) {
            // cout << "=====init cur:" << cur << " r:" << r << " c:" << c << endl;
            if (cur > r) {
                l_tmp = cur-1;
                r_tmp = cur+1;
                cnt_tmp = 1;
                while (l_tmp >= 0 && r_tmp < ss_len && ss[l_tmp] == ss[r_tmp]) {
                    cnt_tmp++;
                    l_tmp--;
                    r_tmp++;
                }
                p[cur] = cnt_tmp;
                r=r_tmp-1;
                c=cur;
                // cout << "big r, c:" << c << " r:" << r  << " cur:" << cur<< endl;
            } else {
                int l_c = c-p[c]+1; //关于c的回文串的左边界
                int cur_tmp = c-(cur-c);
                int l_cur_tmp = cur_tmp-p[cur_tmp]+1;
                // cout << "cur:" << cur << " l_cur_tmp:" << l_cur_tmp << " cur_tmp:" << cur_tmp << endl;
                if (l_cur_tmp > l_c) {
                    p[cur] = p[cur_tmp];
                } else if (l_cur_tmp < l_c) {
                    p[cur] = r-cur+1; //最多到r
                } else {
                    cnt_tmp = r-cur+1;
                    l_tmp = cur-(r-cur)-1;
                    r_tmp = r+1;
                    while (r_tmp<ss_len && l_tmp>=0 && ss[l_tmp] == ss[r_tmp]) {
                        r_tmp++;
                        l_tmp--;
                        cnt_tmp++;
                    }
                    c=cur; //因为最右边界代表的回文串往右移动了。如果往右扩不动，c的值也应该放在cur上
                    p[cur]=cnt_tmp;
                    r = r_tmp-1;
                    // cout << "rr r, c:" << c << " r:" << r  << " cur:" << cur<< endl;
                }
            }
        }
        // cout<<"p res:";
        // for (auto it:p) {
        //     cout << it << " ";
        // }
        // cout << endl;
        int ans = 0;
        for (int i=0;i<p.size();i++) {
            ans += p[i]/2; //每个位置去掉#号之后的回文半径是p[i]/2
        }
        return ans;
    }
};

int main()
{
    Solution sol;

    return 0;
}
