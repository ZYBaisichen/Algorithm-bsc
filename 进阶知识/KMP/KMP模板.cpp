/*
 * @Author: baisichen
 * @Date: 2024-03-02 16:10:58
 * @LastEditTime: 2024-03-02 21:23:34
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

using namespace std;
// https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/description/

/*

// KMP算法
    int kmp(char[] s1, char[] s2) {
        // s1中当前比对的位置是x
        // s2中当前比对的位置是y
        int n = s1.length, m = s2.length, x = 0, y = 0;
        // O(m)
        int[] next = nextArray(s2, m);
        // O(n)
        while (x < n && y < m) {
            if (s1[x] == s2[y]) {
                x++;
                y++;
            } else if (y == 0) {
                x++;
            } else {
                y = next[y];
            }
        }
        return y == m ? x - y : -1;
    }

    // 得到next数组
    public static int[] nextArray(char[] s, int m) {
        if (m == 1) {
            return new int[] { -1 };
        }
        int[] next = new int[m];
        next[0] = -1;
        next[1] = 0;
        // i表示当前要求next值的位置
        // cn表示当前要和前一个字符比对的下标。也代表了最长后缀和前缀相等的长度,
        // 初始时cn为0代表next[1]的值，也代表需要跳到0位置开始和1位置比较
        int i = 2, cn = 0;
        while (i < m) {
            // cn = next[i]; 在里面++cn
            if (s[i - 1] == s[cn]) {
                // next[i] = cn+1;
                // i++;
                // cn++; //cn来到跳到next[i]
                next[i++] = ++cn;
            } else if (cn > 0) { //还可以往前跳
                cn = next[cn];
            } else { //cn小于等于0
                next[i++] = 0;
            }
        }
        return next;
    }
*/
class Solution
{
public:
    int strStr(string haystack, string needle)
    {
        return kmp(haystack, needle);
    }
    // KMP算法
    int kmp(string &str, string &match)
    {
        int n = str.length();
        int m = match.length();
        // s1中当前比对的位置是x
        // match中当前比对的位置是y
        int x = 0, y = 0;
        int* _next = new int[m];    // 这里使用了C语言的数组风格，避免vector太慢
        next_array(match, m, _next); // 求next数组
        // for (int i=0;i<m;i++) {
        //     cout << _next[i] << " ";
        // }
        // cout<<endl;
        while (x < n && y < m)
        {
            // cout << "x:" << x << " y:" << y  << " str[x]:" << str[x] << " str[y]" << str[y]<< endl;
            if (str[x] == match[y])
            {
                x++;
                y++;
            }
            else if (y == 0)
            { // 不相等则y往前跳，或者x往后走
                x++;
            }
            else
            {
                y = _next[y];
            }
        }

        return y == m ? x - y : -1; // y越界，则表示匹配出来了，x停留在匹配出来的下一个字符，计算首字符返回；否则返回-1.
    }

    void next_array(string s, int m, int* _next)
    {
        if (m == 1)
        {
            _next[0] = -1;
            return;
        }
        // 运行到这里这里开始m>1
        _next[0] = -1;
        _next[1] = 0;
        // i表示当前要求next值的位置
        // cn表示当前要和前一个字符比对的下标。也代表了最长后缀和前缀相等的长度,
        // 初始时cn为0代表next[1]的值，也代表需要跳到0位置开始和1位置比较
        int i = 2, cn = 0;
        while (i < m)
        {
            // cn = next[i]; 在里面++cn
            if (s[i - 1] == s[cn])
            { // cn跳到0位置还不相等的话，next[i]就是0了
                // next[i] = cn+1;
                // i++;
                // cn += 1;
                _next[i++] = ++cn;
            }
            else if (cn > 0)
            {
                cn = _next[cn];
            }
            else
            {
                _next[i++] = 0;
            }
        }
    }
};
int main()
{
    Solution sol;
    int ans0 = 0;
    int ans1 = 0;
    // 22
    string haystack = "sadbutsad", needle = "uts";
    ans0 = sol.strStr(haystack, needle);

    cout << "ans0:" << ans0 << " ans1:" << ans1 << endl;
    cout << "测试结束" << endl;
}
