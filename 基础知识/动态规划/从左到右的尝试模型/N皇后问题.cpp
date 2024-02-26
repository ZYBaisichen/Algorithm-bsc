/*
 * @Author: baisichen
 * @Date: 2024-02-22 21:05:49
 * @LastEditTime: 2024-02-26 11:40:08
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

using namespace std;
class Solution
{
public:
    int res;
    // 用数组表示路径实现的N皇后问题，不推荐
	int totalNQueens1(int n) {
		if (n < 1) {
			return 0;
		}
        vector<int> path(n, -1);
		return f1(0, path, n);
	}

    //来到当前第i个皇后，第i行
    // 0..i-1都摆在了哪些列上
    //返回:i...n-1上共有多少中摆放方法
    int f1(int i, vector<int>& path, int n) {
        //找到了一种方法
        if (i==n) {
            return 1;
        }
        int ans = 0;
        for (int j=0; j<n; j++) {
            if (check(path, i, j)) {
                path[i] = j;
                ans += f1(i+1, path, n);
            }
        }
        return ans;
    }
    bool check(vector<int>& path, int i, int j) {
        bool valid = true;
        for (int idx=0; idx<=i-1; idx++) {
            if (path[idx] == j) {
                valid = false;
                break;
            }
            if (abs(i-idx) == abs(j-path[idx])) {
                valid = false;
                break;
            }
        }
        return valid;
    }

    // 使用位运算优化
    int totalNQueens2(int n) {
		if (n < 1) {
			return 0;
		}
        int limit = (1<<n) - 1;
        int col_limit = 0;
        int left_limit = 0;
        int right_limit = 0;

        vector<int> path(n, -1);
		return f2(limit, col_limit, left_limit, right_limit);
	}

    int f2(int limit, int col_limit, int left_limit, int right_limit) {
        if (col_limit == limit) {
            //所有皇后都放完了
            return 1;
        }
        //所有限制
        int ban = col_limit | left_limit | right_limit;
        int pos = limit & (~ban); //所有能放皇后的地方
    
        int ans = 0;
        while (pos != 0) {
            //提取最右侧的1
            int place = pos & (~pos+1);
            pos = pos ^ place;//将最后一个1干掉
            ans += f2(limit, 
                col_limit | place,
                (left_limit | place) << 1, 
                (right_limit | place) >> 1);
        }
        return ans;

    }

};
int main()
{
    Solution sol;
    cout << "测试开始" << endl;
    for (int n=1;n<=8;n++) {
        int ans0 = sol.totalNQueens1(n);
        int ans1 = sol.totalNQueens2(n);
        if (ans0!=ans1) {
            cout << "ans0:" << ans0 << ", ans1:" << ans1 <<endl;
        }
    }
    cout << "测试结束" << endl;
}