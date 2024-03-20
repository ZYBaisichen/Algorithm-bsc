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
** 给定一个数组arr长度为N， 你可以把任意长度大于0且小于N的前缀作为左部分， 剩下的
 * 作为右部分。 但是每种划分下都有左部分的最大值和右部分的最大值， 请返回最大的，
 * 左部分最大值减去右部分最大值的绝对值。
 * 
*/

class Solution
{
public:
    /*
    流程：找到全局max，0位置数和n-1位置数，谁小，max减去谁，就是答案
    找到全局最大数max, 无论max这个数被划分到了左右哪个数组，都会是那个数组的最大值。
    假设max被划归到左半部分数组，则max一定是左半部分的最大值。
        要想左右数组最大值差值最小，则需要让右半部分的最大值尽可能的小。
        因为右半部分一定会包含n-1的位置，所以右半部分的最大值的最小情况不可能低于arr[n-1]。
        干脆就给让n-1自己成一部分，max划归到左半部分的最大答案就是max-arr[n-1]
    同理，max被划归到右半部分数组的情况下的最大答案是max-arr[0]
    两个答案取最大max(max-arr[n-1], max-arr[0])

    另外还发现了单调性：范围增大max只可能增大，不可能减小

    //总结：优化一个题目从数据状况、问的原始的事两个方面入手。原始的事凝练到最简洁的小问题时，往往就是优化点
    */
    int calcMax(vector<int>& arr) {
        int len = arr.size();
        if (len == 0) {
            return 0l
        }

        int max_num = 0;
        for (auto iter: arr) {
            max_num = max(max_num, iter);
        }
        return max(max_num-arr[0], max_num-arr[len-1]);
    }
};

int main()
{
    Solution sol;
}