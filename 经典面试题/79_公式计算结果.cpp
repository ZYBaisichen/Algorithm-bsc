/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-05 23:52:16
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

using namespace std;

/*
给定一个字符串str，str表示一个公式，公式里可能有整数、加减乘除符号和左右括号，返回公式的计算结果。
举例:
str="48*((70-65)-43)+8*1"，返回-1816
str="3+1*4" 返回7
【说明】
1. 可以认为str字符串一定正确的公式
2. 如果是负数会用括号括起来，比如4*(-3)



使用栈。
一、如果没有括号：遇到数字或者+-号，入栈，如果栈顶是乘除，则先计算乘除结果，将结果入到栈中。最终栈中都是+-符号，每次出3个，将结果入进去，直到栈空了为止。

二、如果有括号：
定义f(i)函数，代表从i开始计算，遇到右括号或者终止位置停止。外层遇到开始或者左括号调用。返回已经处理到了哪里的位置、和计算结果
1. 比如30+5*(3+7*(3-5)+4)+61。外层调用f(0)，遇到第一个左括号(5位置)，这时栈里内容是[*5+30]，需要得到括号里的计算内容，则会调用f(5)
2. f(5)到达10位置又遇到一个左括号，此时栈中内容是[*7+3]，继续调用f(10)获得括号里面的计算结果
3. f(10)中到达右括号，说明可以结束自己的计算了，将栈中的[5-3]依次弹出计算得到-2。并和计算结果一并返回
4. f(5)得到f(10)的返回值是-2，处理到了14位置。所以将-2压栈做计算，从15位置开始继续遍历。遇到17位置的左括号。弹出栈计算结果并返回，还会连着处理到17位置一起返回
5. f(0)得到f(5)的结果后，入栈，并从18位置继续处理



*/

class Solution
{
public:
    void print_st(stack<string>& st) {
        stack<string> st1;
        while (!st.empty()) {
            // cout << st.top() << " ";
            st1.push(st.top());
            st.pop();
        }
        while (!st1.empty()) {
            st.push(st1.top());
            st1.pop();
        }
    }
    int calculate(string s) {
        return value(s,0)[0]; //从0开始一定会计算到结尾
    }
    /*
    请从str[i...]往下算，遇到字符串终止位置或者右括号，就停止
    返回两个值，长度为2的数组
    0) 负责的这一段的结果是多少
	1) 负责的这一段计算到了哪个位置
    */
    vector<int> value(string& str, int i) {
        stack<string> st;
        int cur=0; //用于存储数字串
        //从i出发。终点或者遇到右括号结束
        while (i<str.length() && str[i] != ')') { 
            
            if (str[i]>='0' && str[i]<='9') {
                cur = cur*10 + str[i++]-'0';
            } else if (str[i] != '(') { //不是左括号，也不是数字，也不是右括号，所以只可能是运算符
                add_num(st, cur); //将数字放入栈中，栈顶如果是乘除，则先计算
                string tmp(1, str[i++]);
                st.push(tmp);
                // cout << "i:" << i << " cur:" << cur << endl;
                // cout << "st:";
                // print_st(st);
                // cout << endl;
                cur=0;//遇到运算符，清算当前数字
            } else { //遇到了做括号
                vector<int> bra = value(str, i + 1);
                cur = bra[0];//抓到计算结果
                i=bra[1]+1; //从下一个位置开始处理
            }
        }
        add_num(st,cur);//因为遇到运算符号才将数字加进去的，最后一个数字没处理
        // cout << "last num:" << cur << endl;
        // cout << "last_st:";
        // print_st(st);
        vector<int> res(2,0);
        res[0] = get_ans(st);
        res[1] = i;
        return res;
    }

    void add_num(stack<string>& st, int num) {
        if (st.empty()) {
            st.push(to_string(num));
            return;
        }
        if (st.top() == "*" || st.top() == "/") {
            string top_op = st.top();
            st.pop();
            int pre = stoi(st.top()); //因为从来都是先入数字再入运算符，所以一定能弹出两次
            st.pop();
            num = top_op=="*" ? (pre*num) : (pre/num);
        }
        st.push(to_string(num));
    }

    int get_ans(stack<string>& st) { //计算结果，此时栈中一定都是加减运算
        if(st.empty()) {
            return 0;
        }
        //计算需要从左往右计算
        stack<string> st_tmp;
        while (!st.empty()) {
            st_tmp.push(st.top());
            st.pop();
        }

        int ans = stoi(st_tmp.top());
        st_tmp.pop();
        while (!st_tmp.empty() && st_tmp.size() > 1) {
            string op = st_tmp.top();
            st_tmp.pop();
            int num2 = stoi(st_tmp.top());
            st_tmp.pop();
            // cout << "ans:"<< ans << " op:" << op << " num2:" << num2 << endl;
            ans = op=="+"? (ans+num2) : (ans-num2);
        }
        return ans;

    }
};

int main()
{

    Solution sol;
    string s = "1*2-3/4+5*6-7*8+9/10";
    cout << sol.calculate(s) << endl;
    return 0;
}