
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
//https://leetcode.cn/classic/problems/longest-palindromic-substring/description/
class Solution {
public:
    //最长回文子串
    string longestPalindrome(string s) {
        int len = s.length();
        if (len==0) {
            return "";
        }
        
        string ss = expand(s);
        
        int ss_len = ss.length();
        vector<int> p(ss_len, 0);//以每个字符为中心的回文半径, 包括自身
        int r = -1; //最右回文串的右边界
        int c = -1; //最右回文串的中心点

        for (int cur=0; cur<ss_len; cur++) {
            if (cur==0) {
                p[0] = 1;
                r = 0;
                c = 0;
            } else {
                if (cur > r) {
                    //暴力从cur往两边扩
                    int count = 1;
                    int l_tmp = cur-1;
                    int r_tmp = cur+1;
                    while (l_tmp >= 0 && r_tmp < ss_len && ss[l_tmp] == ss[r_tmp]) {
                        l_tmp--;
                        r_tmp++;
                        count++;
                    }
                    r = r_tmp-1;
                    c = cur;
                    p[cur] = count;
                } else {
                    int cur_tmp = c - (cur - c); //相对于c对称的位置
                    int l = c - (r-c); //c为中心点的回文串的左边界
                    int l_cur_tmp = cur_tmp-(p[cur_tmp]-1);//cur_tmp位置的左边界
                    if (l_cur_tmp > l) {
                        p[cur] = p[cur_tmp];
                    } else if (l_cur_tmp < l) { //最多扩到右边界
                        p[cur] = r - cur + 1;
                    } else { //至少扩到右边界
                        int count = r - cur +1;
                        int l_tmp = cur - (count-1) -1;
                        int r_tmp = r+1;
                        while (l_tmp >= 0 && r_tmp < ss_len && ss[l_tmp] == ss[r_tmp]) {
                            l_tmp--;
                            r_tmp++;
                            count++;
                        }
                        r = r_tmp - 1;
                        c = cur;
                        p[cur] = count;
                    }

                }
            }
        }

        int max_idx = 0;
        for (int i=1;i<ss_len;i++) {
            if (p[i] > p[max_idx]) {
                max_idx = i;
            }
        }

        int res_l = max_idx - p[max_idx] + 1;
        int res_r = max_idx + p[max_idx] - 1;

        string res="";
        for (int i=res_l;i<=res_r;i++) {
            if (ss[i] != '#') {
                res += ss[i];
            }
        }
        return res;
    }
    
    //扩充字符串
    string expand(string& s) {
        string res = "#";
        int len = s.length();
        for (int i=0; i<len;i++) {
            res += s[i];
            res += '#';
        }
        return res;
    }

     string longestPalindrome_zuoshen(string s) {
        int len = s.length();
        if (len==0) {
            return "";
        }
        
        string ss = expand(s);
        
        int ss_len = ss.length();
        vector<int> p(ss_len, 0);//以每个字符为中心的回文半径, 包括自身
        int r = 0; //最右回文串的右边界的下一个位置
        int c = -1; //最右回文串的中心点

        int max_idx = 0;
        for (int i=0, c=0, r=0, len; i< ss_len;i++) {
            //i>=r，至少不用验证的区域是1
            //i在r内的话, 不用验证的区域是i关于c对称点的回文半径，和r-i的最小值。(c=(i+x)/2 => x=2*c-i)
            len = r > i ? min(p[2*c-i], r-i) : 1;
            // cout << "1_len:" << len << " r:" << r << " i:" << i  << " c:" << c<< endl;
            //不用验证的区域已经设置好了，则尝试往两边扩
            while (i+len<ss_len && i-len>=0 && ss[i+len] == ss[i-len]) {
                len++;
            }
            //当右边超过了r，则更新r和c
            if (i + len > r) {
                r = i + len;
                c = i;
            }
            // cout << "2_len:" << len << " r:" << r << " i:" << i << " c:" << c << endl;
            p[i] = len;
            max_idx = p[max_idx] > len ? max_idx : i;
        }
        //"#a# b#a#"
        //1 2 1 2 1 2 1
        //c=1
        //r=3
        for (int i=0;i<ss_len;i++) {
            cout << "p[" << i << "]:" << p[i] << " ";
        }
        cout << endl;

        int res_l = max_idx - p[max_idx] + 1;
        int res_r = max_idx + p[max_idx] - 1;
        // cout << "res_l:" << res_l << " res_r:" << res_r << " max_idx:" << max_idx << endl;

        string res="";
        for (int i=res_l;i<=res_r;i++) {
            if (ss[i] != '#') {
                res += ss[i];
            }
        }
        return res;
    }
};

int main()
{
    Solution sol;
    string  s = "adcredfabccbakl";
    int k = 4;
    int ans1 = 0;
    //这个函数求的是假设将arr排序完成后，下标在k-1位置的数字
    string ans0 = sol.longestPalindrome(s);
    string ans1 = sol.longestPalindrome_zuoshen(s);

    cout << " ans0:" << ans0 << endl;
    cout << " ans1:" << ans1 << endl;

    cout << "测试结束" << endl;
}

