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

/**
 * Created by Terry
 * User: Administrator
 * Date: 2022-06-14
 * Time: 10:49
 * Description:
 * 请同学们自行搜索或者想象一个象棋的棋盘，
 * 然后把整个棋盘放入第一象限，棋盘的最左下角是(0,0)位置
 * 那么整个棋盘就是横坐标上9条线、纵坐标上10条线的区域
 * 给你三个 参数 x，y，k
 * 返回“马”从(0,0)位置出发，必须走k步
 * 最后落在(x,y)上的方法数有多少种?
 */
class Solution
{
public:
    //  递归版本
    int HorseJump1(int x, int y, int k)
    {
        return process1(x, y, k);
    }
    // 要到达x，y，还有rest步可以走，有多少种走法
    // x最大是8，y最大是9
    int process1(int x, int y, int rest)
    {
        if (rest == 0)
        {
            // cout << "x:" << x << ",y:" << y << ",rest:" << rest << endl;
            return (x == 0 && y == 0) ? 1 : 0;
        }
        if (x < 0 || x > 8 || y < 0 || y > 9)
        {
            return 0;
        }
        int ways = 0;

        ways += process1(x - 2, y - 1, rest - 1);
        ways += process1(x - 2, y + 1 , rest - 1);
        ways += process1(x - 1, y + 2, rest - 1);
        ways += process1(x + 1, y + 2, rest - 1);
        ways += process1(x + 2, y + 1, rest - 1);
        ways += process1(x + 2, y - 1, rest - 1);
        ways += process1(x + 1, y - 2, rest - 1);
        ways += process1(x - 1, y - 2, rest - 1);
        return ways;
    }

    //  递归版本2
    int HorseJump2(int x, int y, int k)
    {
        return process2(x, y, k,0,0);
    }
    // 要到达x，y，当前走到了curX、curY， 还有rest步可以走，有多少种走法。将第一种方法思维反了过来
    // x最大是8，y最大是9
    int process2(int x, int y, int rest, int curX, int curY)
    {
        if (curX < 0 || curX > 8 || curY < 0 || curY > 9)
        {
            return 0;
        }
        if (rest == 0)
        {
            return (x == curX && y == curY) ? 1 : 0;
        }
        int ways = 0;
        // 从当前位置往外出发，8个方向跳
        ways += process2(x, y, rest - 1, curX - 2, curY - 1);
        ways += process2(x, y, rest - 1, curX - 2, curY + 1);
        ways += process2(x, y, rest - 1, curX - 1, curY + 2);
        ways += process2(x, y, rest - 1, curX + 1, curY + 2);
        ways += process2(x, y, rest - 1, curX + 2, curY + 1);
        ways += process2(x, y, rest - 1, curX + 2, curY - 1);
        ways += process2(x, y, rest - 1, curX + 1, curY - 2);
        ways += process2(x, y, rest - 1, curX - 1, curY - 2);
        return ways;
    }

    //  dp版本2
    int HorseJump3(int x, int y, const int k)
    {
        if (x < 0 || x > 8 || y < 0 || y > 9) {
            return 0;
        }
        if (k == 0) {
            return 0;
        }
    
        // 分配内存空间
        int*** dp = new int**[9];
        for (int i = 0; i < 9; ++i) {
            dp[i] = new int*[10];
            for (int j = 0; j < 10; ++j) {
                dp[i][j] = new int[k+1];
                for (int kk = 0; kk < k+1; ++kk) {
                    dp[i][j][kk] = 0;
                }
            }
        }

        dp[0][0][0] = 1;
        for (int level=1; level<=k; ++level) {
            for (int i=0; i<9; ++i) {
                for (int j=0; j<10; ++j) {
                    // cout << "dp[" << i << "][" << j << "][" << level << "]:" << dp[i][j][level] << endl;
                    dp[i][j][level] = get_dp_value(i-2, j-1, level-1, dp);
                    dp[i][j][level] += get_dp_value(i-2, j+1, level-1, dp);
                    dp[i][j][level] += get_dp_value(i - 1, j + 2, level - 1, dp);
                    dp[i][j][level] += get_dp_value(i + 1, j + 2, level - 1, dp);
                    dp[i][j][level] += get_dp_value(i + 2, j + 1, level - 1, dp);
                    dp[i][j][level] += get_dp_value(i + 2, j - 1, level - 1, dp);
                    dp[i][j][level] += get_dp_value(i + 1, j - 2, level - 1, dp);
                    dp[i][j][level] += get_dp_value(i - 1, j - 2, level - 1, dp);
                }
            }
        }
        return dp[x][y][k];
    }

    int get_dp_value(int x, int y, int k, int*** dp) {
        if (x < 0 || x > 8 || y < 0 || y > 9) {
            return 0;
        }
        return dp[x][y][k];
    }
};

int main()
{
    Solution sol;
    // 2,3,3 》4
    // 6,8,10 》 405766
    int x = 6;
    int y = 8;
    int k = 10;
    int ans0 = 0;
    int ans1 = 0;
    int ans2 = 0;

    ans0 = sol.HorseJump1(x, y, k);
    ans1 = sol.HorseJump2(x, y, k);
    ans2 = sol.HorseJump3(x, y, k);

    cout << "ans0:" << ans0 << " ans1:" << ans1 << " ans2:" << ans2 << endl;
    cout << "测试结束" << endl;
    return 0;
}
