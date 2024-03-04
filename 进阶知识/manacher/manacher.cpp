
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
                    } else {
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
};

int main()
{
    Solution sol;
    //1 3 5 78 100
    //60 90
    // vector<int> arr = {3,5,1,100,78 ,60,90, 1,5,6,1,2,3};
    // int test_time = 1000;
    // int len = 0;
    // int k= 0;

    // int ans0 = 0;
    // int ans1 = 0;
    // int ans2 = 0;
    // for (int i=0;i<test_time;i++) {
    //     len = (int)((std::rand() % 100) + 1);
    //     arr.resize(len);
    //     for (int j=0;j<len;j++) {
    //         arr[j] = (int)((std::rand() % 1000) + 1);
    //     }
    //     k = (int)((std::rand() % len)+1);
    //     vector<int> arr1 = arr;
    //     vector<int> arr2 = arr;
    //     vector<int> arr3 = arr;

    //     sol.select_sort(arr2, 0, arr2.size() - 1);
    //     ans0 = arr2[k - 1];
    //     ans1 = sol.get_k_with_bfprt(arr1, k);
    //     ans2 = sol.get_k_with_rand(arr3, k);
    //     if (ans0!=ans1) {
    //         cout << " ans0:" << ans0 << " ans1:" << ans1 << endl;
    //     }
    //     if (ans0!=ans2) {
    //         cout << " ans0:" << ans0 << " ans2:" << ans2 << endl;
    //     }
    // }
    string  s = "adcredfabccbakl";
    int k = 4;
    int ans1 = 0;
    //这个函数求的是假设将arr排序完成后，下标在k-1位置的数字
    string ans0 = sol.longestPalindrome(s);

    cout << " ans0:" << ans0 << endl;
    cout << "测试结束" << endl;
}
