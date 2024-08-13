/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-25 14:55:57
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

https://leetcode.cn/problems/divide-two-integers/

给你两个整数，被除数 dividend 和除数 divisor。将两数相除，要求 不使用 乘法、除法和取余运算。

整数除法应该向零截断，也就是截去（truncate）其小数部分。例如，8.345 将被截断为 8 ，-2.7335 将被截断至 -2 。

返回被除数 dividend 除以除数 divisor 得到的 商 。

注意：假设我们的环境只能存储 32 位 有符号整数，其数值范围是 [−231,  231 − 1] 。本题中，如果商 严格大于 231 − 1 ，则返回 231 − 1 ；如果商 严格小于 -231 ，则返回 -231 。

 

示例 1:

输入: dividend = 10, divisor = 3
输出: 3
解释: 10/3 = 3.33333.. ，向零截断后得到 3 。
示例 2:

输入: dividend = 7, divisor = -3
输出: -2
解释: 7/-3 = -2.33333.. ，向零截断后得到 -2 。
 

提示：

-231 <= dividend, divisor <= 231 - 1
divisor != 0
*/

/*
1. 加法：比如5(0101)和6(0110)，0101^0110=0011是无进位的加法，(0101&0110)<<1=1000是进位信息。
    然后再降无进位信息和进位信息相加，得到0011^1000=1011 (0011&1000)<<1=0000, 此时进位信息是0了，无进位信息就是要的答案
2. 减法：a-b就是a加上b的相反数， 即a+(~b+1)

3. 乘法：a=011010,b=001101
    a. res = 0，a<<0,b>>0, b最后一位是1的话，res+=a
    b. a<<1,b>>1, b最后一位是1的话，res+=a
    c. 循环直到b为0为止。
 
4. 除法： a/b
    a. a、b两个数都转换成绝对值的形式
    b. 系统最小绝对值要比系统最大绝对值要大1。
        a和b都是系统最小，返回1
        a不是系统最小，b不是，返回0。
        a和b都不是系统最小，则正常使用绝对值运算
        a不是系统最小，b是系统最小，需要分开讨论

*/


/*
1. 加法：比如5(0101)和6(0110)，0101^0110=0011是无进位的加法，(0101&0110)<<1=1000是进位信息。
    然后再将无进位信息和进位信息相加，得到0011^1000=1011 (0011&1000)<<1=0000, 此时进位信息是0了，无进位信息就是要的答案
2. 减法：a-b就是a加上b的相反数， 即a+(~b+1)

3. 乘法：a=011010,b=001101
    a. res = 0，a<<0,b>>0, b最后一位是1的话，res+=a
    b. a<<1,b>>1, b最后一位是1的话，res+=a
    c. 循环直到b为0为止。
 
4. 除法： a/b
    a. a、b两个数都转换成绝对值的形式
    b. 系统最小绝对值要比系统最大绝对值要大1。
        a和b都是系统最小，返回1
        a不是系统最小，b不是，返回0。
        a和b都不是系统最小，则正常使用绝对值运算
        a不是系统最小，b是系统最小，需要分开讨论

*/
class Solution {
public:
    int add ( int a,  int b) {
         int sum = a;
        while (b!=0) {
            sum = a ^b ;
            b= (a&b)<<1;
            a = sum;
        }
        return sum;
    }
    //求b的相反数，取逆加1
     int neg_num( int b) {
        return add(~b, 1); //~b+1
    }
    //减法
    int minus( int a,  int b) {
        return add(a, neg_num(b)); //a加上b的相反数
    }
    //输出二进制
    void print_2(int n) {
        string a = "";
        while (n) {
            a+=(n&1) == 1?'1':'0';
            // cout << "cur a:" << a << endl;
            cout << "n:" << n << endl;
            n >>= 1; 
        }
        reverse(a.begin(), a.end());
        cout << "cur:" << n << " 2_he:" << a << endl;
    }
    //乘法
    int multi( int a,  int b) {
        int x= is_neg(a) ? neg_num(a) : a;
        int y = is_neg(b) ? neg_num(b) : b;
        int flag = is_neg(a) ^ is_neg(b); //不同符号取反，相同符号取正

        int res = 0;
        while (y!=0) {
            if ((y&1) != 0) {
                res = add(res, x);
            }
            x = (x<<1);
            y = (y>>1);
        }
        return flag ? neg_num(res) : res;
    }
    //判断是否是负数
    bool is_neg(int n) {
        return n<0;
    }
    //假设a和b都能转成整数，做除法
    /*
    1. b往左移动，找到最接近a，但又不超过a的情况。比如a=0110100, b=-0000010，
        b往左移动到b1=100000最接近a，让a减去该值变成0010100
    2. 接下来再看b=-0000010，往左移动到b2=0010000时，最接近a但不超过a，然后a变成了0000100
    3. 再看b=-0000010，往左移动到b3=0000100时等于a，a-b3=0。 
    4. 移动到b1,b2,b3的移动了多少位，再res对应的位上设置为1就是答案
    核心是a和b的关系转换成：a=b*甲+b*乙+b*丙+b*丁，而这里的甲乙丙丁一定是2的某次方

    */
    int div(int a, int b) {
        int x = is_neg(a) ? neg_num(a) : a;
        int y = is_neg(b) ? neg_num(b) : b;
        int res = 0;
        //x往右移动，相当于y向做移动。i从31到0试一遍，看能否减y
        //之所以让x往右移动，而不是让y往左移动，是为了防止出现最高位符号位为1，变成负数了
        for (int i=31;i>neg_num(1);i=minus(i,1)) {
            if ((x>>i) >= y) {
                res |= (1<<i);
                x = minus(x, y<<i); 
            }
        }
        return is_neg(a) ^ is_neg(b) ? neg_num(res) : res; //不同符号取反，相同符号取正
    }

    int divide(int dividend, int divisor) {
        if (divisor == INT_MIN) {
            return dividend == INT_MIN ? 1: 0;
        }
        if (divisor == 0) {
            return 0;
        }
        //除数不是系统最小
        // cout << "dividend:"<<dividend<< " INT_MIN:" << INT_MIN << " dividend == INT_MIN:" << (dividend == INT_MIN)<<endl;
        //被除数是系统最小
        if (dividend == INT_MIN) {
            if (divisor == neg_num(1)) {
                return INT_MAX;
            }
            //假设-20是系统最小，除以4时。先加1然后除4，-19/4=-4, 然后还少算了-20-(-4*4)=-4， 然后再降-4/4=-1加到结果上去，就是-4+-1=-5
            int res = div(add(dividend, 1), divisor);
            // cout << "res:" << res << endl;
            // cout << " buchong:" << multi(res, divisor) << endl;
            return add(res, div(minus(dividend, multi(res, divisor)), divisor)); //加上补偿的量

        }

        //到这里说明除数和被除数都不是系统最小
        return div(dividend, divisor);
    }
};

int main()
{
    Solution sol;
    cout << "INT_MIN:" << INT_MIN << " INT_MAX:" << INT_MAX <<endl;
    int dividend = -2147483648, divisor = -3;
    // cout << sol.add(dividend, 1) << endl;
    cout << sol.divide(dividend, divisor) << endl;
    cout << sol.multi(715827882,-3);
    return 0;
}
