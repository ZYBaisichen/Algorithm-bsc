/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-02 11:46:17
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
给一个整数N，在纸上写出来1到N所有的数，返回写了多少N
https://leetcode.cn/problems/1nzheng-shu-zhong-1chu-xian-de-ci-shu-lcof/description/
*/

class Solution
{
public:
    /*
    数位dp
    1. 假设f（i）返回1-i范围内1的数量。给一个13625，则做拆解f(3625)+(3626-13625中1的数量)-> f(625)+(626-3625中1的数量)-> f(25)+(26-625中1的数量)
    上面过程可以看出来，每次都递减1位。当i为个位数时，只要大于等于1就是1个数。

    2. 非递归部分：
        a. 最高位是1的情况：3626-13625，万位上出现3626个1(10000-13625, 和开始是一样数量)；（归纳总结）
            i) 假设N共k位，最高位数字是1，则最高位出现1的次数是N%10^(k-1) + 1 个
            ii) 假设让千位是1：个位、十位、百位随意变化，通过万位调整，都可以找到一个让千位为1的数字，所以千位上1的数量是百位、十位、个位可能性相乘，就是10^3
            iii) 百位固定1: 千位、十位、个位可以随意调整，但万位不能随便调整，要么为0要么为1。所以百位为1的数量是千位、十位、个位可能性相乘，就是10^3
            iv) N整体K位，最高位已经求出来了。还剩k-1位，每一位的可能性是10^(k-2)，除了最高位剩下总的1的数量是10^(k-2)*(k-1)

        b. 最高位不是1的情况: 625-3625。 最高位如果固定是1的话，只有1000-1999共1000个1
            i) 抽象化：假设N共k位，最高位不是1，最高位是1的数量是10^(k-1)
            ii) 百位上固定是1时：将625-3625分成626-1625、1626-2625、2626-3625三段，最高位是几就分成几段。
                当百位固定为1，十位和个位随意变化，比如变成x100，在每一段里都可以找到唯一一个数字。比如分别调整千位落在三段里的数是1100、2100、3100
                当十位固定为1时，百位和个位随意变化，比如变成x919，同样在每一段里可以找到唯一的数字。比如分别调整千位落在三段里的数是0919、1919、2919
                同理个位固定为1时，百位和仕伟随意变化，比如变成了x871，同样在每一段里可以找到唯一的数字。比如分别调整千位落在三段里的数是0871、1871、2871
            iii) 抽象：N共K位，最高位数字是a=N/(10^(k-1))，分成了a段。除了最高位每位固定为1时，有10^(k-2)是随意变的, 共k-1个位置。所以除了最高位总1的数量是a*10^(k-2)*(k-1)

    
    大调度每次减少一个数字，共需要lg(N), 非递归需要计算10^(k-1)次方需要计算k-1次，而k是lg(N)，总复杂度是O(lg(N)^2)

    */
    int digitOneInNumber(int num) {
        if (num<0) {
            return 0;
        }
        if (num>=1 && num<10) {
            return 1;
        }
        return f(num);
    }
    int f(int num) {
        if (num<1) {
            return 0;
        }

        //计算有几位
        int k = 0;
        int cur = num;
        while (cur!=0) {
            cur /= 10;
            k++;
        }

        if (k == 1) {
            return 1;
        }
        
        /*
        num=13625
        tmp=10000
        */
        int tmp=1;
        //k-1次方 lg(N)
        for (int i=1; i<k; ++i) {
            tmp *= 10;
        }

        int first = num/tmp;
        int first_one_num = 0;
        if (first == 1) {
            first_one_num = num % tmp + 1;
        } else {
            first_one_num = tmp;
        }

        int other_one_num = 0;
        if (first == 1) {
            other_one_num = (tmp / 10) * (k - 1);
        } else {
            other_one_num = first * (tmp / 10) * (k-1);
        }
        cout << "num:" << num << " first_one_num:" << first_one_num << " other_one_num:" << other_one_num << endl;

        return first_one_num + other_one_num + f(num%tmp);

    }
};

int main()
{

    Solution sol;
    int num = 101;

    cout << sol.digitOneInNumber(num) << endl;

    return 0;
}