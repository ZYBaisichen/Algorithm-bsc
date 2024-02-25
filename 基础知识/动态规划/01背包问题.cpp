//参考自：https://github.com/algorithmzuo/algorithmbasic2020/blob/master/src/class19/Code02_ConvertToLetterString.java

// 规定1和A对应、2和B对应、3和C对应…26和Z对应，那么一个数字字符串比如"111”就可以转化为:“AAA”、“KA"和"AK”。给定一个只有数字字符组成的字符串str，请问有多少种转化结果？

/*
 * @Author: baisichen
 * @Date: 2024-02-22 21:05:49
 * @LastEditTime: 2024-02-26 00:50:33
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
class Solution {
public:
    int res;
    int _01bag(vector<int> w, vector<int> v, int bag) {
        return process1(w, v, 0, 0, bag);
    }
    //正着想，用了多少空间
    int process1(vector<int>& w ,vector<int>& v, int index, int alreadyW, int bag) {
        if (alreadyW > bag) {
            return 0;
        }
        if (index == w.size()) {
            return 0;
        }

        int no = process1(w, v, index+1, alreadyW, bag);
        // cout << "no:" << no << endl;
        int yes = 0;
        if (alreadyW + w[index] <= bag) {
            yes = v[index] + process1(w, v, index + 1, alreadyW + w[index], bag);
        }
        return std::max(no, yes);
    }
    //反着想，还剩多少空间可以用
    //返回可以产生的最大价值
    int process2(vector<int> w ,vector<int> v, int index, int rest, int bag) {
        if (rest <= 0) {
            return 0;
        }
        if (index == w.size()) {
            return 0;
        }

        int no = process2(w, v, index+1, rest, bag); //不要当前的物品
        int yes = 0;
        if (rest>=w[index]) {
            yes = v[index] + process2(w, v, index + 1, rest - w[index], bag);
        }
        return std::max(no, yes);
    }
};
int main()
{
    Solution sol;
    vector<int> w = {2,3,1,2};
    vector<int> v = {4, 6, 1, 2};
    int N = 1;
    int bag = 5;
    int testTime = 1;
    std::srand(std::time(0));  
    cout << "测试开始" << endl;
    // for (int i = 0; i < testTime; i++) {
    //     int len = (int) ((std::rand()%10) * N);
    //     bag = (int) (std::rand()%10000);

    //     for (int j=0; j<len;j++) {
    //         w.push_back((int) (std::rand()%10000));
    //         v.push_back((int) (std::rand()%100));
    //     }
    //     int ans0 = sol.process1(w, v, 0, 0, bag);
    //     int ans1 = sol.process1(w, v, 0, bag, bag);
    //     if (ans0 != ans1) {
    //         cout << "ans0:" << ans0 << endl;
    //         cout << "ans1:" << ans1 << endl;
    //         cout << "Oops!" << endl;
    //         break;
    //     }
    // }
    int ans0 = sol.process1(w, v, 0, 0, bag);
    int ans1 = sol.process2(w, v, 0, bag, bag);
    cout << "ans0:" << ans0 << ", ans1:" << ans1<<endl;
    cout << "测试结束" << endl;
}