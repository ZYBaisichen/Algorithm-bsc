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

/*
一排正方形，每个正方形已经被染成了红色或者绿色。现在可以选择任意一个正方形，用这两个颜色中的任意一种进行染色。
原来的颜色会被覆盖，目标是染色后，每个红色的R都在左边，每个绿色的G都在右边。
比如s=RGRGR, 染色后变成RRRGG, 染色次数为2，为最优方案

类似进阶问题：
// 涂色 & 奇怪打印机
// 假设你有一条长度为5的木板，初始时没有涂过任何颜色
// 你希望把它的5个单位长度分别涂上红、绿、蓝、绿、红
// 用一个长度为5的字符串表示这个目标：RGBGR
// 每次你可以把一段连续的木板涂成一个给定的颜色，后涂的颜色覆盖先涂的颜色
// 例如第一次把木板涂成RRRRR
// 第二次涂成RGGGR
// 第三次涂成RGBGR，达到目标
// 返回尽量少的涂色次数
// 测试链接 : https://www.luogu.com.cn/problem/P4170
// 测试链接 : https://leetcode.cn/problems/strange-printer/
*/

class Solution
{
public:
    int min_paint(string s)
    {
        int len = s.length();
        if (len == 0 || len == 1)
        {
            return 0;
        }
        vector<int> r(len, 0);
        // 统计右边R的数量。累加
        r[len - 1] = s[len - 1] == 'R' ? 1 : 0;
        for (int i = len - 2; i >= 0; i--)
        {
            r[i] = r[i + 1] + (s[i] == 'R' ? 1 : 0);
        }

        int res = r[0]; // 初始时是将右侧全部染色成G的情况。为右边R的数量
        // cout << "res: " << res << endl;
        int l_g = 0;
        for (int i = 0; i < len - 1; i++)
        { // 枚举0..i，i+1...len-1范围
            l_g += s[i] == 'G' ? 1 : 0;
            res = min(res, l_g + r[i + 1]);
            // cout << "res: " << res << endl;
        }
        l_g += s[len - 1] == 'G' ? 1 : 0;
        res = min(res, l_g); // 枚举左侧全部染色成R的情况
        // cout << "res: " << res << endl;
        return res;
    }

    int min_paint2(string s)
    {
        int len = s.length();
        if (len == 0 || len == 1)
        {
            return 0;
        }
        // 小优化：统计R总数量，从左往右走的时候，逐个递减。
        int r_r = 0;
        for (int i = len - 1; i >= 0; i--)
        {
            r_r += (s[i] == 'R' ? 1 : 0);
        }

        int res = r_r; // 初始时是将右侧全部染色成G的情况。为右边R的数量
        // cout << "res: " << res << endl;
        int l_g = 0;
        for (int i = 0; i < len; i++)
        { // 枚举0..i，i+1...len-1范围
            l_g += s[i] == 'G' ? 1 : 0;
            r_r -= s[i] == 'R' ? 1 : 0;
            res = min(res, l_g + r_r);
            // cout << "res: " << res << endl;
        }
        return res;
    }
};

int main()
{
    Solution sol;
    string str = "RGRGR";
    cout << "str1:" << sol.min_paint(str) << endl;
    cout << "str2:" << sol.min_paint2(str) << endl;
}