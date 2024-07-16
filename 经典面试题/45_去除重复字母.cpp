/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-30 17:40:22
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
给你一个字符串 s ，请你去除字符串中重复的字母，使得每个字母只出现一次。需保证 返回结果的
字典序
最小（要求不能打乱其他字符的相对位置）。



示例 1：

输入：s = "bcabc"
输出："abc"
示例 2：

输入：s = "cbacdcbc"
输出："acdb"


提示：

1 <= s.length <= 104
s 由小写英文字母组成


测试链接：https://leetcode.cn/problems/remove-duplicate-letters/description/
*/

/*
方法一，贪心：假设s="daccbdbaccdbba"
1. 先统计s字符串每个字符的词频。{"a":3, "b":4, "c":4, "d":3}
2. 遍历找到第一个使某个词频为0的地方，即最后一个d。在d前面的字符串中选择字典序最小的字符作为第一个字符。是第一个a
    a. 如果第一个字符的范围再往右，如果恰好选择了d后面的b，则肯定会错过d。所以不能再往右扩充了
3. 将a左边删掉，在右边按照1和2流程选择第二个字符。以此类推，找到所有字符
*/

class Solution
{
public:
    // 方法一
    // 如果最后剩K个字符，每次选一个字符需要遍历一遍数组N。所以总复杂度O(k*N)，因为k最多到N，所以最坏复杂度O(N^2)
    string removeDuplicateLetters(string s)
    {
        return remove(s);
    }
    string remove(string s)
    {
        int len = s.length();
        if (len <= 0)
        {
            return "";
        }
        map<char, int> cnt;
        for (int i = 0; i < len; i++)
        {
            if (cnt.find(s[i]) == cnt.end())
            {
                cnt[s[i]] = 0;
            }
            cnt[s[i]]++;
        }

        int min_idx = 0;
        for (int i = 0; i < len; i++)
        {
            cnt[s[i]]--;
            if (cnt[s[i]] == 0)
            {
                min_idx = s[min_idx] <= s[i] ? min_idx : i; // 最小的字典序字符，取第一个
                break;
            }
            else
            {
                // cout << "comp min_idx:" << min_idx << " i:" << i << endl;
                min_idx = s[min_idx] <= s[i] ? min_idx : i;
            }
        }
        string front;
        front = s[min_idx]; //选择一个。后面的选择递归
        // cout << "min_idx:" << min_idx << " front:" << front << endl;
        // cout << "min_idx:" << min_idx + 1 << " last str len:" << len - min_idx - 1 << endl;
        // cout << s.substr(min_idx + 1, len - min_idx - 1) << endl;
        string last_str = s.substr(min_idx + 1, len - min_idx - 1);
        last_str.erase(std::remove(last_str.begin(), last_str.end(), s[min_idx]), last_str.end());
        // return "";
        string last_remove = remove(last_str); // 挑选出min_idx之后， 前面的字符删掉，剩下的接着选剩余的字符串
        // cout << "front:" << front <<  " last_remove:" << last_remove << endl;
        return front + last_remove;
    }

    // 单调栈解法
    /*
        因为要保证字典序最小，那么肯定要让字典序更小的字符放在前面。让前面的字符尽可能的大，除非后面没有字符了
        1. 采用一个改进版的单调栈，第一个字符先入栈。
        2. 遍历到的字符b，如果比栈顶a小，并且在后面还有a，则可以使用后面的a，让当前的栈顶弹出，让b进去。
        3. 最后从栈底部到栈顶的所有元素弹出来就是要求的字符串
    */
    string removeDuplicateLetters1(string s)
    {
        int len = s.length();
        if (len <= 0)
        {
            return "";
        }
        map<char, int> cnt;
        for (int i = 0; i < len; i++)
        {
            if (cnt.find(s[i]) == cnt.end())
            {
                cnt[s[i]] = 0;
            }
            cnt[s[i]]++;
        }

        vector<char> stack(26, 0);
        int size = 0;
        vector<bool> entered(26, false);
        for (int i = 0; i < len; i++)
        {
            char cur = s[i];
            if (!entered[cur - 'a'])
            { // 没入过栈，则进入
                entered[cur - 'a'] = true;
                // 满足条件则弹出栈顶元素
                while (size > 0 && stack[size - 1] >= cur && cnt[stack[size - 1]] > 0)
                { // 这里之所以要判断大于0，是因为在来到当前字符之前词频就已经减过了
                    entered[stack[size - 1] - 'a'] = false;
                    size--;
                }
                stack[size++] = cur; // 入单调栈
            }
            cnt[cur]--;
        }
        string res = "";
        for (int i = 0; i < size; i++)
        {
            res += stack[i];
        }
        return res;
    }
};

int main()
{

    Solution sol;
    // vector<int> arr = {2,3,6,7,4,12,21,39};
    string s = "abacb";
    // string s = "abcabcbb";

    cout << sol.removeDuplicateLetters(s) << endl;
    cout << sol.removeDuplicateLetters1(s) << endl;
    return 0;
}