//参考自：https://github.com/algorithmzuo/algorithmbasic2020/blob/master/src/class19/Code02_ConvertToLetterString.java

// 规定1和A对应、2和B对应、3和C对应…26和Z对应，那么一个数字字符串比如"111”就可以转化为:“AAA”、“KA"和"AK”。给定一个只有数字字符组成的字符串str，请问有多少种转化结果？

/*
 * @Author: baisichen
 * @Date: 2024-02-22 21:05:49
 * @LastEditTime: 2024-02-23 15:09:06
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
  
using namespace std;
class Solution {
public:
    int res;
    // str只含有数字字符0~9
	// 返回多少种转化方案
    int number(string str) {
        int len = str.length();
        if (len==0) {
            return 0;
        }
        return process(str, 0);
    }
    //str[0...i-1]无需过问
    //str[i...]转化，返回多少种方案
    int process(string& s, int i) {
        int len =s.length();
        if (i==len) {
            return 1; //顺利到达了最后一个字符的后一个位置，返回找到了一种方案
        }

        if (s[i] == '0') {
            return 0; //0开头无意义
        }
        
        if (s[i] == '1') {
            int res = process(s, i+1);//使用一个
            if (i+1<len) { //第二个数0到9都行
                res += process(s, i+2);
            }
            // cout << "1:" << res << endl;
            return res; //不要重复算下面的3开头的
        } else if (s[i]=='2') {
            int res = process(s, i+1);//使用一个
            if (i+1<len&&s[i+1]<='6'&&s[i+1]>='0') {
                res+=process(s, i+2);
            }
            return res;
        }
        //3以上开头的都只能使用一个
        return process(s, i + 1);
    }


    int numberDp(string s) {
        int len = s.length();
        if (len==0) {
            return 0;
        }
        vector<int> dp(len+1, 0);
        dp[len] = 1;
        for (int i=len-1; i>=0; i--) {
            if (s[i] == '0') {
                dp[i] = 0;
            }
            if (s[i] == '1') {
                dp[i] = dp[i+1];
                if (i+1<len) {
                    dp[i] += dp[i+2];
                }
            } else if (s[i] == '2') {
                dp[i] = dp[i+1];
                if (i+1<len&&s[i+1]<='6'&&s[i+1]>='0') {
                    dp[i] += dp[i+2];
                }
            } else {
                dp[i] = dp[i+1];
            }
        }
        return dp[0];
    }

    // 为了测试
	string random_string(int len) {
		string str;
        // 使用当前时间作为随机数种子  
        std::srand(std::time(0));  
  
        // 生成随机数  
		for (int i = 0; i < len; i++) {
            int random_number = std::rand() % 10;  
			str += (char) (random_number + '0');
		}
        // cout << "str:" <<str<<endl;
		return str;
	}


};
int main()
{
    Solution sol;
    int N = 30;
    int testTime = 200;
    std::srand(std::time(0));  
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++) {
        int len = (int) ((std::rand()%100+10) * N);
        string s = sol.random_string(len);
        int ans0 = sol.number(s);
        int ans1 = sol.numberDp(s);
        if (ans0 != ans1) {
            cout << "s:" << s << endl;
            cout << "ans0:" << ans0 << endl;
            cout << "ans1:" << ans1 << endl;
            cout << "Oops!" << endl;
            break;
        }
    }
    cout << "测试结束" << endl;
}