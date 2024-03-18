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

using namespace std;

// 完成配对需要的最少字符数量
// 给定一个由'['、']'、'('，')'组成的字符串
// 请问最少插入多少个括号就能使这个字符串的所有括号正确配对
// 例如当前串是 "([[])"，那么插入一个']'即可满足
// 输出最少需要插入多少个字符
// 测试链接 : https://www.nowcoder.com/practice/e391767d80d942d29e6095a935a5b96b
// 请同学们务必参考如下代码中关于输入、输出的处理
// 这是输入输出处理效率很高的写法
// 提交以下的code，提交时请把类名改成"Main"，可以直接通过


/*
1. 假设字符串中只有小括号。判断括号字符串是否有效
2. 计算出最少需要多少字符可以让字符串正确配对
*/

class Solution {
public:
    bool check_str_1(string str) {
        int count = 0;
        int len = str.length();
        for (int i=0; i< len; i++) {
            if (str[i] == '(') {
                count++;
            } else if (str[i] == ')'){
                count--;
            }
            if (count <0) {
                return false;
            }
        }
        return count == 0;
    }

    //返回需要添加几个字符可以让整体有效
    int check_str_2(string str) {
        int count = 0, res=0;
        int len = str.length();
        for (int i=0; i< len; i++) {
            if (str[i] == '(') {
                count++;
            } else if (str[i] == ')'){
                count--;
            }
            if (count <0) {
                res+=(-count);
                count = 0;
            }
        }
        res += count; //还需要count个右括号
        return res;
    }
};

int main() {
    Solution sol;
    string str= "()))((()";
    cout << "str1:" << sol.check_str_1(str) << endl;
    cout << "str2:" << sol.check_str_2(str) << endl;
}