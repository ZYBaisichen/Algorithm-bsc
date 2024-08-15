/*
 * @Author: baisichen
 * @Date: 2024-08-13 22:05:34
 * @LastEditTime: 2024-08-13 22:18:53
 * @LastEditors: baisichen
 * @Description: 
 */
/*
表达式字符串求值，add(2,sub(4,5))
*/
#include <iostream>
using namespace std;
int get_res_core(string &str, int s, int e);

int get_res(string &str)
{
    int len = str.length();
    if (len == 0)
    {
        return -1;
    }
    if (str[0] != 'a' && str[0] != 's')
    {
        return -1;
    }
    return get_res_core(str, 0, len - 1);
}
int get_num(string &str, int &i)
{
    int res = 0;
    int len = str.length();
    while (i<len && str[i] >= '0' && str[i] <= '9')
    {
        res = res * 10 + str[i] - '0';
        i++;
    }
    return res;
}
// s..e范围上的计算结果返回
// s到e上是一个完整表达式
int get_res_core(string &str, int s, int e) {
    // cout << "=====s:" << s << "e:" << e << endl;
    int len = str.length();
    int flag = -1; // flag=1是加法，0是减法
    if (str[s] == 'a') {
        flag = 1;
    } else {
        flag = 0;
    }
    // 找到二元运算符的第一个数字
    int i = s;
    while (str[i] != '(') {
        i++;
    }
    int first_num = -1;
    i++;
    // cout << "cur_i:" << i << endl;
    if (i < len && str[i] >= '0' && str[i] <= '9') {                                // 第一个运算符是数字
        first_num = get_num(str, i); // 会累加i
        // cout << "first_num get_num:" << first_num << endl;
    }
    else
    {
        int j = i ;
        while (str[j] != ')') {
            j++;
        }
        first_num = get_res_core(str, i, j);
        i = j+1;
    }
    // cout << "first_num: " << first_num << endl;

    // 当前i来到了逗号处
    i++;
    int second_num = -1;
    if (i < len && str[i] >= '0' && str[i] <= '9')
    {                                 // 第二个运算符直接是数字
        second_num = get_num(str, i); // 会累加i
    }
    else
    {
        second_num = get_res_core(str, i, e-1);
    }
    // cout << "second_num:" << second_num << endl;
    // int res = flag == 1 ? first_num + second_num : first_num - second_num;
    // cout << "=====s:" << s << "e:" << e << " res:"<<  res << endl;
    return flag == 1 ? first_num + second_num : first_num - second_num;
}
int main()
{
    string str = "add(sub(6,5),sub(5,3))";

    cout << "res:" << get_res(str)<< endl;
}
