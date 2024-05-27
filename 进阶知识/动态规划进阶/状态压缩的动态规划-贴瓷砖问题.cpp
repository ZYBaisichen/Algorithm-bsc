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
#include <algorithm>

using namespace std;

/*
铺瓷砖问题，使用一个1*2的瓷砖铺路。
1. 铺满2*n的路面有多少种铺的方法
2. 铺满m*n的路面有多少种铺的方法

*/

/*
第一问：
对于2*n的路面，假设f(n)表示铺满2*n的路面有多少种方法：
    1. 假设第一个瓷砖在0位置竖着铺，则整体转化成2*(n-1)的路面有多少种方法。f(n-1)
    2. 假设第一个瓷砖在0位置横着铺，则下方一定也得是横着铺。问题转化成铺满2*(n-2)的路面有多少种方法。f(n-2)
    3. 综上f(n)=f(n-1)+f(n-2)。即斐波那契数列，有log(n)的快速幂解法
    4. base：f(1)=1 (只能竖着)，f(2)=2 (竖着或者横着两种)

第二问：采用了状态压缩的思想

首先暴力思想：
规定m*n的地面，当在某一行时，瓷砖只能往上贴或者往右贴；这样不会漏掉，因为对于每个位置(i,j)，往左贴的情况可以使用(i,j-1)位置往右贴来代替。往下贴的情况可以使用(i,j+1)往上贴来代替
定义f(status, i) 表示来到了第i行，i-1行每个位置贴没贴瓷砖使用status集合来表示。i-2及往上都已经贴满了瓷砖，返回将i到n行所有路面全铺面瓷砖的方法数。
假设有3列，初始时主函数调用f({1,1,1}, 0)， 表示-1行已经贴满了瓷砖，从0行开始到第n行贴满的方法数。

假设调用到了f({1,1,0,1}, 1)，当前在第1行，第0行状态是{1,1,0,1}
    1. 第三列上一行没有贴瓷砖，所以本行必须在第三列竖着贴一个瓷砖补齐，否则后面将无法补齐。所以本行状态至少是{0,0,1,0}
    2. 当前行的其他位置可以选择放瓷砖，也可以选择不放瓷砖，做dfs枚举。规定dfs(status, cur, i)表示在当前status时，到了cur列位置要枚举，行数是i，返回总方法数。
        a. 第一列可以横着放一个，往下调用dfs({1,1,1,0},2, 1)；也可以不放，往下调用dfs({0,0,1,0},1,1)
        b. dfs的base case，当cur来到了结尾处时，说明本行枚举来到了一个路径的结尾。则进入到下一层，调用f(status, 2)

*/

class Solution
{
public:
    // 第一问, O(n)
    int answer1_ways1(int n)
    {
        if (n == 1)
        {
            return 1;
        }
        if (n == 2)
        {
            return 2;
        }
        int a = 1, b = 2;
        int ans = 0;
        for (int i = 3; i <= n; i++)
        {
            ans = a + b;
            a = b;
            b = ans;
        }
        return ans;
    }

    // 第一问，复习一下快速幂。O(log(n))
    int answer1_ways2(int n)
    {
        if (n == 1)
        {
            return 1;
        }
        if (n == 2)
        {
            return 2;
        }
        vector<vector<int>> base = {
            {1, 1},
            {1, 0}};

        /*
        |F_N F_N-1| = |F2 F1|*base^(n-2)
        */
        const vector<vector<int>> base_pow = matrix_power(base, n-2);

        return 2*base_pow[0][0] + 1*base_pow[0][1];
    }

    vector<vector<int>> matrix_power(vector<vector<int>> base, int n) {
        vector<vector<int>> t = base;
        vector<vector<int>> res = {
            {1,0},
            {0,1}
        };
        while (n!=0) {
            if (n & 1) { //按照位来分哪些要乘到res上
                res = multy(res, t);
            }
            t = multy(t, t);
            n >>= 1;
        }
        return res;
    }

    //n*n的矩阵相乘
    vector<vector<int>> multy(const vector<vector<int>>& a, const vector<vector<int>>& b) {
        int n = a.size();
        vector<vector<int>> res(n, vector<int>(n, 0)); //乘出来还是n*n的
        for (int i=0;i<n;++i) {
            for (int j=0;j<n;++j) {//枚举a的i行和b的j列

                for (int k=0;k<n;++k) {
                    res[i][j] += a[i][k]*b[k][j];
                }
                
            }
        }
        return res;
    }


    //第二问，暴力方法
    int answer2_ways1(int m, int n) {
        if (m==0||n==0) {
            return 1;
        }
        vector<int> pre(n, 1); //-1行全是1
        return process(pre, 0, m, n);
    }

    int process(vector<int> pre, int cur_level, int m, int n) {
        if (cur_level == m) { //来到了最后一层
            //判断前一层是不是全满了，则找到一种方法
            for (int i=0;i<pre.size();i++) {
                if (pre[i] == 0) {
                    return 0;
                }
            }
            return 1;
        }

        vector<int> cur_status(pre.size(), 0);
        for (int i=0;i<pre.size();i++) {
            if (pre[i] == 0) { //上面一层如果没有铺瓷砖，则没得选，只能竖着贴
                cur_status[i] = 1;
            }
        }

        return dfs(cur_status, 0, cur_level, m, n);
        
    }

    int dfs(vector<int>& cur_status, int cur_idx, int level, int m, int n) {
        if (cur_idx == n) { //本行枚举出了一条路径，则进入下一层
            return process(cur_status, level+1, m, n);
        }

        //决定本不横着贴瓷砖
        int p1 = dfs(cur_status, cur_idx+1, level, m, n);
        int p2 = 0;

        //决定横着贴瓷砖，但有条件
        //右边有位置，并且还没有被铺上瓷砖
        if (cur_idx+1<n && cur_status[cur_idx] == 0&& cur_status[cur_idx+1] == 0) {
            cur_status[cur_idx] = 1;
            cur_status[cur_idx+1] = 1;
            p2 = dfs(cur_status, cur_idx+2, level, m, n);
            cur_status[cur_idx] = 0;
            cur_status[cur_idx+1] = 0;
        }
        return p1+p2;
    }


    //第二问，状态压缩方法
    int answer2_ways2(int m, int n) {
        if (m==0||n==0) {
            return 1;
        }
        unsigned int tmp_1=1;
        unsigned int pre = (tmp_1<<n) - 1;//-1行全是1
        return process2(pre, 0, m, n);
    }

    int process2(unsigned int pre, int cur_level, int m, int n) {
        if (cur_level == m) { //来到了最后一层
            // 判断前一层是不是全满了，则找到一种方法
            if (pre == ((1<<n)-1)) {
                return 1;
            } else {
                return 0;
            }
        }

        //设置初始的当前的状态
        //上一层如果贴瓷砖了，则当前层就是可以考虑贴瓷砖，该位为0
        //否则就是1。pre的每一位如果是1，则cur_status是0.如果是0，则cur_status是1
        //对pre取反
        unsigned int cur_status = ((1<<n)-1) & (~pre);

        return dfs2(cur_status, 0, cur_level, m, n);
        
    }

    int dfs2(unsigned int cur_status, int cur_idx, int level, int m, int n) {
        if (cur_idx == n) { //本行枚举出了一条路径，则进入下一层
            return process2(cur_status, level+1, m, n);
        }

        //决定本不横着贴瓷砖
        int p1 = dfs2(cur_status, cur_idx+1, level, m, n);
        int p2 = 0;

        //决定横着贴瓷砖，但有条件
        //右边有位置，并且还没有被铺上瓷砖
        unsigned int tmp_1 = 1;
        unsigned int tmp_3 = 3;
        if (cur_idx+1<n && (cur_status&(tmp_1<<cur_idx)) == 0 && (cur_status & (tmp_1 << (cur_idx+1))) == 0) {
            //cur_idx和cur_idx设置为1
            //cur_status | 1 << cur_idx
            //cur_status | 1 << (cur_idx+1)
            //相当于0b11 << cur_idx 即3<<cur_idx
            p2 = dfs2(cur_status|(tmp_3<<cur_idx), cur_idx+2, level, m, n);
        }
        return p1+p2;
    }

    //第二问，状态压缩，记忆化搜索
    int answer2_ways3(int m, int n) {
        if (m==0||n==0) {
            return 1;
        }
        unsigned int tmp_1=1;
        unsigned int pre = (tmp_1<<n) - 1;//-1行全是1

        /*
        可变参数有status，和cur_level
        status取值是0到1<<n-1, 行数取值0到m
        */
       unsigned int max_status = (tmp_1<<n);
    //    cout << "max_status:" << max_status << endl;
       vector<vector<int>> dp(max_status, vector<int>(m+1, -1));

        return process3(pre, 0, m, n, dp);
    }

    int process3(unsigned int pre, int cur_level, int m, int n, vector<vector<int>>& dp) {
        if (dp[pre][cur_level] != -1) {
            return dp[pre][cur_level];
        }
        if (cur_level == m) { //来到了最后一层
            // 判断前一层是不是全满了，则找到一种方法
            dp[pre][cur_level] =  pre == ((1<<n)-1) ? 1 : 0;
            return dp[pre][cur_level];
        }

        //设置初始的当前的状态
        //上一层如果贴瓷砖了，则当前层就是可以考虑贴瓷砖，该位为0
        //否则就是1。pre的每一位如果是1，则cur_status是0.如果是0，则cur_status是1
        //对pre取反
        unsigned int cur_status = ((1<<n)-1) & (~pre);

        dp[pre][cur_level] = dfs3(cur_status, 0, cur_level, m, n, dp);
        return dp[pre][cur_level];
        
    }

    int dfs3(unsigned int cur_status, int cur_idx, int level, int m, int n, vector<vector<int>>& dp) {
        if (cur_idx == n) { //本行枚举出了一条路径，则进入下一层
            return process3(cur_status, level+1, m, n, dp);
        }

        //决定本不横着贴瓷砖
        int p1 = dfs3(cur_status, cur_idx+1, level, m, n, dp);
        int p2 = 0;

        //决定横着贴瓷砖，但有条件
        //右边有位置，并且还没有被铺上瓷砖
        unsigned int tmp_1 = 1;
        unsigned int tmp_3 = 3;
        if (cur_idx+1<n && (cur_status&(tmp_1<<cur_idx)) == 0 && (cur_status & (tmp_1 << (cur_idx+1))) == 0) {
            //cur_idx和cur_idx设置为1
            //cur_status | 1 << cur_idx
            //cur_status | 1 << (cur_idx+1)
            //相当于0b11 << cur_idx 即3<<cur_idx
            p2 = dfs3(cur_status|(tmp_3<<cur_idx), cur_idx+2, level, m, n, dp);
        }
        return p1+p2;
    }


    //第二问，动态规划
    int answer2_ways4(int m, int n) {
        if (m==0||n==0) {
            return 1;
        }
        unsigned int tmp_1=1;
        unsigned int pre = (tmp_1<<n) - 1;//-1行全是1

        /*
        可变参数有status，和cur_level
        status取值是0到1<<n-1, 行数取值0到m
        */
       unsigned int max_status = (tmp_1<<n);
    //    cout << "max_status:" << max_status << endl;
       vector<vector<int>> dp(max_status, vector<int>(m+1, -1));

       /*
       状态依赖：
       第0列，除了dp[max_status][0]均没有意义
       第1列, 依赖上一列的所有状态，即dp[0~max_status][0]，且不依赖前面的一行
       第2列, 依赖上一列的所有状态，即dp[0~max_status][1]，
       所以dp按照列填，从右往左，从上到下
       */

        //最后一列
        for (int i=0;i<max_status;i++) {
            dp[pre][m] = pre == max_status - 1 ? 1 : 0;
        }

        for (int level=m-1;level>=0;level--) {
            for (int pre = 0; pre < max_status; pre++) {
                // 最后一层，判断前一层是不是全满了，则找到一种方法
                if (level == m) {
                    dp[pre][level] = pre == max_status-1 ? 1 : 0; 
                }

                int cur_status = (max_status-1) & (~pre);
                dp[pre][level] = dfs4(cur_status, 0, level, m, n, dp);
            }
        }

        return dp[max_status - 1][0];
    }

    int dfs4(unsigned int cur_status, int cur_idx, int level, int m, int n, vector<vector<int>>& dp) {
        if (cur_idx == n) { //本行枚举出了一条路径，则进入下一层
            return dp[cur_status][level+1];
        }

        //决定本不横着贴瓷砖
        int p1 = dfs3(cur_status, cur_idx+1, level, m, n, dp);
        int p2 = 0;

        //决定横着贴瓷砖，但有条件
        //右边有位置，并且还没有被铺上瓷砖
        unsigned int tmp_1 = 1;
        unsigned int tmp_3 = 3;
        if (cur_idx+1<n && (cur_status&(tmp_1<<cur_idx)) == 0 && (cur_status & (tmp_1 << (cur_idx+1))) == 0) {
            //cur_idx和cur_idx设置为1
            //cur_status | 1 << cur_idx
            //cur_status | 1 << (cur_idx+1)
            //相当于0b11 << cur_idx 即3<<cur_idx
            p2 = dfs3(cur_status|(tmp_3<<cur_idx), cur_idx+2, level, m, n, dp);
        }
        return p1+p2;
    }
};

int main()
{
    Solution sol;
    //第一问
    // int n1 = 20;
    // cout << sol.answer1_ways1(n1) << endl;
    // cout << sol.answer1_ways2(n1) << endl;
    //第二问
    // int m = 8;
    // int n = 6;
    // cout << sol.answer2_ways1(m,n) << endl;
    // cout << sol.answer2_ways2(m, n) << endl;
    // cout << sol.answer2_ways3(m, n) << endl;
    // cout << sol.answer2_ways4(m, n) << endl;

    int max_m = 10;
    int max_n = 6;
    int test_time = 100;
    for (int i=0;i<test_time;i++) {
        int m = std::rand() % max_m +1;
        int n = std::rand() % max_n + 1;
        int ans1 = sol.answer2_ways1(m, n);
        int ans2 = sol.answer2_ways2(m, n);
        int ans3 = sol.answer2_ways3(m, n);
        int ans4 = sol.answer2_ways4(m, n);
        if (ans1 != ans2 || ans1 != ans3 || ans1 != ans4) {
            cout << "error" << endl;
            cout << "ans1:" << ans1 << " ans2:" << ans2 << " ans3:" << ans3 << " ans4:" << ans4 << endl;
        }
    }
    return 0;
}