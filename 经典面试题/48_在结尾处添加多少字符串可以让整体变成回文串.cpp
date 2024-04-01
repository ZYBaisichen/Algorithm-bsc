/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-01 16:14:34
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

using namespace std;

/*
 manacher应用
 给定一个字符串，问在结尾处添加多少个字符可以让整体变成回文串

/*

*/
typedef long long ll;
class Solution
{
public:
    // 扩充字符串
    string expand(string &s)
    {
        string res = "#";
        int len = s.length();
        for (int i = 0; i < len; i++)
        {
            res += s[i];
            res += '#';
        }
        return res;
    }
    //d为怪兽的能力值。p是贿赂每只怪兽的价格
    string add_shortest_char(string s) {
        int len = s.length();
        if (len==0) {
            return 0;
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

        int ans=INT_MAX;
        cout << ss << endl;
        for (int i=0;i<ss_len;i++) {
            if (i + p[i] == ss_len) { //包含最后一个字符
                //需要增加前面i-p[max_idx]+1个字符
                ans = min(ans, i-p[i]+1);
                // cout << "i:" << i << " p[i]:" << p[i] << " ans:"<< ans <<  endl;
            }
        }
        string res = "";
        for (int i=0;i<ans/2;i++) {
            res += s[i];
        }
        return res;
    }
    
};

int main()
{

    Solution sol;
    string str = "ddAb3bA";

    cout << sol.add_shortest_char(str) << endl;

    return 0;
}