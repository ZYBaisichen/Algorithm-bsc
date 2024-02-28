/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-02-27 23:34:26
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
using namespace std;

class Solution
{
public:
    /*
        arr: 每个人喝完开始要洗咖啡杯的时间
        a: 咖啡机洗一个杯子的时间
        b: 咖啡杯自然挥发的时间
        咖啡机只能串行的洗杯子，问所有杯子都被洗完的最短时间点
    */
    int coffee(vector<int>& arr, int a, int b)
    {
        int len = arr.size();
        if (len == 0) {
            return 0;
        }
        return process(arr, a, b, 0, 0);
    }
    /*
    0...index号杯子都被洗完了，洗完index....len号杯子最近的时间点
    wash_last: 最近的咖啡机可以释放的时间点
    */
    int process(vector<int> &arr, int a, int b, int index, int wash_last) {
        if (index == arr.size() - 1) {
            return min(max(arr[index], wash_last)+a, arr[index]+b);
        }

        //洗当前的杯子
        int cur_wash_last = max(wash_last, arr[index]) + a;
        int next1 = process(arr, a, b, index+1, cur_wash_last);
        int ans1 = max(cur_wash_last, next1); //当前的洗完，后面的也洗完才能算index..len都洗完，所以取最大值

        //自然风干当前杯子
        int dry = arr[index] + b;
        int next2 = process(arr, a, b, index+1, wash_last);
        int ans2 = max(dry, next2); //同样是当前洗完，后面的也洗完，才算index...len都洗完

        return min(ans1, ans2); //返回两种方法中最小的时间点
    }

    int coffeeDp(vector<int>& arr, int a, int b)
    {
        if (a>b) {
            return arr[arr.size()-1]+b;
        }
        int len = arr.size();
        if (len == 0) {
            return 0;
        }

        int max_wash_last = 0; //全洗的最大值
        for (int i= 0; i<len; i++) {
            max_wash_last = max(max_wash_last,arr[i])+a;
        }

        vector<vector<int>> dp(len+1, vector<int>(max_wash_last+1, 0));

        for (int cur_wash_last = 0; cur_wash_last <= max_wash_last; cur_wash_last++) {
            dp[len-1][cur_wash_last] = min(max(arr[len-1], cur_wash_last)+a, arr[len-1]+b);
        }

        for (int index=len-2; index>=0; index--) {
            for (int cur_wash_last=0; cur_wash_last<=max_wash_last; cur_wash_last++) {
                //洗杯子
                int next_wash_last = max(cur_wash_last, arr[index]) + a;
                int next1 = dp[index+1][next_wash_last];
                int ans1 = max(next_wash_last, next1); //当前的洗完，后面的也洗完才能算index..len都洗完，所以取最大值

                //自然风干当前杯子
                int dry = arr[index] + b;
                int next2 = dp[index+1][cur_wash_last];
                int ans2 = max(dry, next2); //同样是当前洗完，后面的也洗完，才算index...len都洗完

                dp[index][cur_wash_last] = min(ans1, ans2);
            }
        }
        return dp[0][0];
    }
};

int main()
{
    Solution sol;
    //22
    vector<int> arr={1,1,5,5,7,10,12,12,12,12,12,12,15};
    int a=3;
    int b=10;
    int aim=1000;

    int ans0 = sol.coffee(arr, a, b);
    int ans1 = sol.coffeeDp(arr, a, b);

    cout << "ans0:" << ans0  << "ans1:" << ans1<< endl;
    cout << "测试结束" << endl;
}