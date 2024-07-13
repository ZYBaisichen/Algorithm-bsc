/*
 * @Author: baisichen
 * @Date: 2024-03-13 10:34:12
 * @LastEditTime: 2024-07-13 09:16:35
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

using namespace std;

// 有装下8个苹果的袋子、装下6个苹果的袋子，一定要保证买苹果时所有使用的袋子都装满
// 对于无法装满所有袋子的方案不予考虑，给定n个苹果，返回至少要多少个袋子
// 如果不存在每个袋子都装满的方案返回-1

class Solution
{
public:
    int bags1(int apple)
    {
        int res = -1;
        if (apple == 0)
        {
            return 0;
        }
        int max_8 = apple / 8;
        // cout << "max_8:" << max_8 << endl;
        for (int i = max_8; i >= 0; i--)
        {
            int rest = apple - i * 8;
            // cout << "rest:" << rest  << " i:" << i << " apple:" << apple << endl;
            if (rest == 0 || rest % 6 == 0)
            {
                res = i + rest / 6;
                break;
            }
        }
        cout << "apple_res:" << apple << "," << res << endl;
        /*
        apple_res:1,-1
        apple_res:2,-1
        apple_res:3,-1
        apple_res:4,-1
        apple_res:5,-1
        apple_res:6,1
        apple_res:7,-1
        apple_res:8,1
        apple_res:9,-1
        apple_res:10,-1
        apple_res:11,-1
        apple_res:12,2
        apple_res:13,-1
        apple_res:14,2
        apple_res:15,-1
        apple_res:16,2
        apple_res:17,-1
        apple_res:18,3
        apple_res:19,-1
        apple_res:20,3
        apple_res:21,-1
        apple_res:22,3
        apple_res:23,-1
        apple_res:24,3
        apple_res:25,-1
        apple_res:26,4
        apple_res:27,-1
        apple_res:28,4
        apple_res:29,-1
        apple_res:30,4
        apple_res:31,-1
        apple_res:32,4
        apple_res:33,-1
        apple_res:34,5
        apple_res:35,-1
        apple_res:36,5
        apple_res:37,-1
        apple_res:38,5
        apple_res:39,-1
        apple_res:40,5
        apple_res:41,-1
        apple_res:42,6
        apple_res:43,-1
        apple_res:44,6
        apple_res:45,-1
        apple_res:46,6
        apple_res:47,-1
        apple_res:48,6
        apple_res:49,-1
        apple_res:50,7
        apple_res:51,-1
        apple_res:52,7
        apple_res:53,-1
        apple_res:54,7
        apple_res:55,-1
        apple_res:56,7
        apple_res:57,-1
        apple_res:58,8
        apple_res:59,-1
        apple_res:60,8
        apple_res:61,-1
        apple_res:62,8
        apple_res:63,-1
        apple_res:64,8
        apple_res:65,-1
        apple_res:66,9
        apple_res:67,-1
        apple_res:68,9
        apple_res:69,-1
        apple_res:70,9
        apple_res:71,-1
        apple_res:72,9
        apple_res:73,-1
        apple_res:74,10
        apple_res:75,-1
        apple_res:76,10
        apple_res:77,-1
        apple_res:78,10
        apple_res:79,-1
        apple_res:80,10
        apple_res:81,-1
        apple_res:82,11
        apple_res:83,-1
        apple_res:84,11
        apple_res:85,-1
        apple_res:86,11
        apple_res:87,-1
        apple_res:88,11
        apple_res:89,-1
        apple_res:90,12
        apple_res:91,-1
        apple_res:92,12
        apple_res:93,-1
        apple_res:94,12
        apple_res:95,-1
        apple_res:96,12
        apple_res:97,-1
        apple_res:98,13
        apple_res:99,-1
        apple_res:100,13
        apple_res:101,-1
        apple_res:102,13
        apple_res:103,-1
        apple_res:104,13
        apple_res:105,-1
        apple_res:106,14
        apple_res:107,-1
        apple_res:108,14
        apple_res:109,-1
        apple_res:110,14
        apple_res:111,-1
        apple_res:112,14
        apple_res:113,-1
        apple_res:114,15
        apple_res:115,-1
        apple_res:116,15
        apple_res:117,-1
        apple_res:118,15
        apple_res:119,-1
        apple_res:120,15
        apple_res:121,-1
        apple_res:122,16
        apple_res:123,-1
        apple_res:124,16
        apple_res:125,-1
        apple_res:126,16
        apple_res:127,-1
        apple_res:128,16
        apple_res:129,-1
        apple_res:130,17
        apple_res:131,-1
        apple_res:132,17
        apple_res:133,-1
        apple_res:134,17
        apple_res:135,-1
        apple_res:136,17
        apple_res:137,-1
        apple_res:138,18
        apple_res:139,-1
        apple_res:140,18
        apple_res:141,-1
        apple_res:142,18
        apple_res:143,-1
        apple_res:144,18
        apple_res:145,-1
        apple_res:146,19
        apple_res:147,-1
        apple_res:148,19
        apple_res:149,-1
        apple_res:150,19
        apple_res:151,-1
        apple_res:152,19
        apple_res:153,-1
        apple_res:154,20
        apple_res:155,-1
        apple_res:156,20
        apple_res:157,-1
        apple_res:158,20
        apple_res:159,-1
        apple_res:160,20
        apple_res:161,-1
        apple_res:162,21
        apple_res:163,-1
        apple_res:164,21
        apple_res:165,-1
        apple_res:166,21
        apple_res:167,-1
        apple_res:168,21
        apple_res:169,-1
        apple_res:170,22
        apple_res:171,-1
        apple_res:172,22
        apple_res:173,-1
        apple_res:174,22
        */
        return res;
    }
    int bags2(int apple)
    {
        if (apple == 0)
        {
            return 0;
        }
        if (apple % 2 == 1)
        {
            return -1;
        }
        if (apple < 18)
        {
            if (apple == 6 || apple == 8)
            {
                return 1;
            }
            else if (apple == 12 || apple == 14 || apple == 16)
            {
                return 2;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            //18个苹果时用3个袋子，每增加8个苹果用加一个容量为8的袋子
            return (apple - 18) / 8 + 3;
        }
    }
};
int main()
{
    int n = 1000;
    Solution sol;
    for (int i = 0; i < n; i++)
    {
        int ans0 = sol.bags1(i);
        // cout << i << ","<< ans0 << endl;
        int ans1 = sol.bags2(i);
        if (ans0 != ans1)
        {
            cout << i << "," << ans0 << "," << ans1 << endl;
        }
    }
    return 0;
}