/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-24 18:52:38
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

using namespace std;

// 删除至少几个字符可以变成另一个字符串的子串
// 给定两个字符串s1和s2
// 返回s1至少删除多少字符可以成为s2的子串
// 对数器验证

class Solution {
public:
    int minDelete1(string s1, string s2) {
        //枚举s1的子序列（O(2^n)），对于每一个，按个看是否在s2中(KMP，O(m))
        //总复杂度是2^n+m*log(m)
        //当n比较小时，这种方法表现还是比较好的
        vector<string> str_list;
        f(s1, 0, str_list, "");


        std::sort(str_list.begin(), str_list.end(), [](const string& a, const string& b) {  
            return a.length() > b.length(); // 按照字符串长度降序，目的是找到最长的字符串在s2中，可以找到最小的需要删除的字符数量
        });

        for (auto iter:str_list) {
            // cout << " " << iter << endl;
            if (s2.find(iter) != std::string::npos) {
                return s1.length()-iter.length();                
            }
        }
        return s1.length(); //找不到的话全删掉，空字符串肯定是s2的子串
    }
    void f(string s2, int idx, vector<string>& str_list, string path) {
        if (idx == s2.length()) {
            str_list.push_back(path);
            return;
        }
        f(s2, idx+1, str_list, path);
        f(s2, idx+1, str_list, path+s2[idx]);
    }

    int minDelete2(string s1, string s2) {
        //方法2。将s1删除多少字符可以变成s2的子串。
        //枚举s2的所有子串(O(m^2)), 假设删除代价为1，就转换成了s1变成s2子串的编辑距离问题(n*m)
        //或者是s2子串只有插入操作，变成s1的编辑距离问题
        //总时间复杂度是O(n*m^3)
        int len1 = s1.length();
        int len2 = s2.length();
        if (len2==0) {
            return len1;
        }
        if (len1==0) {
            return -1;
        }
        int min_res = len1; 

        for (int start=0;start<len2;start++) { //枚举子串的开头
            for (int end=start; end<len2; end++) { //枚举子串的结尾
                string s2_sub = s2.substr(start, end-start+1);

                //求s1和s2_sub的边际距离
                //n*m
                int n=len1, m=s2_sub.length();
                vector<vector<int>> dp(n+1, vector<int>(m+1, INT_MAX-1)); //dp[i][j]标识s1[0..i-1]到s2_sub[0...j-1]的最小编辑距离
                for (int i=0;i<=n;i++) {
                    dp[i][0] = i;
                }
                for (int i=1;i<=n;i++){
                    for (int j=1;j<=m;j++) {
                        // cout << "s1[i-1]: " << s1[i-1] << ",  s2_sub[j-1]:" <<  s2_sub[j-1] << endl;
                        if (s1[i-1] == s2_sub[j-1]) { //当最后一个字符相等时，依赖上一个字符是否匹配
                            dp[i][j] = dp[i-1][j-1];
                        } else {
                            // s1[i-1] != s2_sub[j-1]时 ，只能尝试将s1[0...i-2]变换到s2_sub[0..j-1]然后删除s1[i-1]
                            dp[i][j] = min(dp[i][j], dp[i-1][j]+1);
                        }
                    }
                }
                min_res = min(min_res, dp[n][m]);
                // cout << "s2_sub: " << s2_sub << " dp[n][m]:" << dp[n][m] << endl;
                // for (int i=0;i<=n;i++) {
                //     for (int j=0;j<=m;j++) {
                //         cout << dp[i][j] << " ";
                //     }
                //     cout << endl;
                // }
                // cout << endl;
                
            }
        }
        return min_res;
    }

    int minDelete3(string s1, string s2) {
        int len1 = s1.length();
        int len2 = s2.length();
        if (len2==0) {
            return len1;
        }
        if (len1==0) {
            return -1;
        }
        int min_res = INT_MAX-1;
        int max_1 = INT_MAX-1;
        // dp[i][j]标识s1[0..i-1]到s2_sub[0...j-1]的最小编辑距离
        // s2_sub长度最大到s1长度，所以准备下面的dp就够用了
        vector<vector<int>> dp(len1+1, vector<int>(len2+1, INT_MAX-1)); 
        dp[0][0] = 0;
        for (int i=0;i<=len1;i++) {
            dp[i][0] = i;
        }

        /*
        我们观察到枚举s2字符的过程其实就是在dp表中每次增加一列。所以每次枚举新的开头时重新填dp，枚举结尾时新填dp的一列。
        */
       int max_res = len1; //最多删成空字符串
        for (int start=0;start<len2;start++) { //枚举子串的开头
            //填dp start开始的第一列
            for (int i=1;i<=len1;i++) {
                //如果当前的字符和start位置的字符相等或者前面已经发现过相等的字符
                if (s1[i-1] == s2[start] || dp[i-1][start+1]!=(INT_MAX-1)) {
                    dp[i][start+1]=i-1;
                }
            }
            max_res = min(max_res, dp[len1][start+1]);
            // cout << "start:" << start << " max_res: " << max_res << endl;


            for (int end = start+1;end<len2;end++) { //枚举结尾
                //每枚举一次结尾，相当于多枚举了一个s2的子字符串，需要抓一次结果
                //填end这一列
                for (int i=1;i<=len1;i++) {
                    if (i<(end-start+1)) { //s1长度为0..i-1的字符串如果小于需要转换的s2子串长度(end-start+1))，则跳过
                        dp[i][end+1] = max_1;
                        continue;
                    }
                    dp[i][end+1] = max_1;
                    if (s1[i-1] == s2[end]) { //如果当前的字符和start位置的字符相等或者前面已经发现过相等的字符
                        dp[i][end+1]=dp[i-1][end];
                    } else {
                        //s1[0...i-2]到s2_sub[start...end]，然后删除s1[i-1]
                        if (dp[i-1][end+1] != max_1) {
                            dp[i][end + 1] = dp[i-1][end+1] + 1;
                        }
                    }
                }
                max_res = min(max_res, dp[len1][end+1]);
                // cout << "start:" << start <<  " end:" << end<< " max_res: " << max_res << endl;
            }
            // for (int i=0;i<=len1;i++) {
            //     for (int j=0;j<=len2;j++) {
            //         cout << dp[i][j] << " ";
            //     }
            //     cout << endl;
            // }
            // cout << endl;
        }
        return max_res;
    }

};

//生成长度为len，有v种字符的字符串
string random_string(int len, int v) {
    string res;
    for (int i = 0; i < len; ++i) {
        res += rand() % v + 'a';
    }
    return res;
}
int main()
{
    Solution sol;

    int n = 10;
    int v = 3;
    int testTime = 100;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++) {
        int len1 = (int) (rand() % n) + 1;
        int len2 = (int) (rand() % n) + 1;
        string s1 = random_string(len1, v);
        string s2 = random_string(len2, v);
        // cout<< "s1: " << s1 << ",  s2:" << s2 << endl;
        int ans1 = sol.minDelete1(s1, s2);
        int ans2 = sol.minDelete2(s1, s2);
        int ans3 = sol.minDelete3(s1, s2);
        // cout << "ans1: " << ans1 << ",  ans2:" << ans2 << ",  ans3:" << ans3 << endl;
        if (ans1 != ans2) {
            cout << "出错了!" << endl;
        }
        if (ans1 != ans3) {
            cout << "出错了! ans3:"  << s1 <<  ", "<<s2<< endl;
        }

        if (ans2 != ans3) {
            cout << "出错了! ans3:"  << s1 <<  ", "<<s2<< endl;
        }
    }
    cout << "测试结束" << endl;


    // string s1= "bbbbc";
    // string s2 = "aa";
    // //输出6
    // cout << sol.minDelete1(s1,s2) << endl;
    // cout << sol.minDelete2(s1,s2) << endl;
    // cout << sol.minDelete3(s1, s2) << endl;
    return 0;
}