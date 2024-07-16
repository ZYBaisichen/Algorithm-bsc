/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-04 12:04:42
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
f(i)，返回还有i个人时活着的人时，最后活下来的人的编号

- 线段剃刀感觉的函数(0,0),(1,1),...(i-1,i-1),(i,0),(i+1,1)..(2i-1,i-1)，函数是y=x%i ......(1)
- 报数和号码的关系：从1开始报数，报到i(当前总人数)后，报i+1数的人回到第1个人。
  画出曲线，就是y=x%i，往上移动一位，往右移动一位。（左加右减，上加下减），得到函数为y=(x-1)%i+1 ...(2)
- 杀死之前的编号和杀死之后编号的关系：旧编号=(新编号+s-1)%i+1 ... (3) 
    其中s是上一轮被杀掉的节点，在上一轮编号中的号码是，(m-1)%i+1， 带入(3):
    旧编号=(新编号+(m-1)%i+1-1)%i+1=(新编号+m-1)%i+1 (4)

其中(4)中的i是上一轮人数。

// 本题测试链接 : https://leetcode-cn.com/problems/yuan-quan-zhong-zui-hou-sheng-xia-de-shu-zi-lcof/

*/

class Solution
{
public:

    /*
    约瑟夫环的长度N
    报数到M就杀掉
    */
    int calcLiveNum(int N, int M) {
        return get_live(N,M);
    }

    //现在一共有i个节点，数到m就杀死节点，最终会活下来的节点，请求返回它在有i个节点时的编号
    int get_live(int i, int m) {
        if (i==1) {
            return 1; //只有一个节点时，1号节点会活
        }
        //公式：y = （x + m - 1）% i + 1
        //y指的是老编号，x指的是新编号
        //这里的x和y都是指存活下来那个节点
        //m就是报数，
        /*
        拿到了有i-1个数时谁会活的编号，推出来其在i个数时的编号。
        和上面思想反过来求了，不过意思是一样的。
        */
        return (get_live(i-1,m) + m -1)%i+1;
    }

    /*
    扩展。
    N个人坐在圆桌上玩游戏，给一个数组arr=[3,1,5,2,7]表示每次数到arr[i]时被杀死。arr不够用时从头再用
    这也是约瑟夫环问题，只不过m每次都会变化。
    */

};

int main()
{

    Solution sol;

    return 0;
}