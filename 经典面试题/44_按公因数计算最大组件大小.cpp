/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-30 13:30:42
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
给定一个由不同正整数的组成的非空数组 nums ，考虑下面的图：

有 nums.length 个节点，按从 nums[0] 到 nums[nums.length - 1] 标记；
只有当 nums[i] 和 nums[j] 共用一个大于 1 的公因数时，nums[i] 和 nums[j]之间才有一条边。
返回 图中最大连通组件的大小 。



示例 1：
输入：nums = [4,6,15,35]
输出：4
示例 2：



输入：nums = [20,50,9,63]
输出：2
示例 3：



输入：nums = [2,3,6,7,4,12,21,39]
输出：8


提示：
1 <= nums.length <= 2 * 104
1 <= nums[i] <= 105
nums 中所有值都 不同

测试链接：https://leetcode.com/problems/largest-component-size-by-common-factor/
*/

// 记录因子所属数字的下标
class UnionFind
{
public:
    UnionFind(int len)
    {
        for (int i = 0; i < len; i++)
        {
            _parents[i] = i;
            _sizes[i] = 1;
        }
    }

    bool isSameSet(int a, int b)
    {
        int a_p = find_parent(a);
        int b_p = find_parent(b);
        // if (a==2&&b==3) {
        //     cout << "isSameSet:" <<  a_p << "," << b_p << endl;
        // }
        return a_p == b_p;
    }

    void unionSet(int a, int b)
    {
        int a_p = find_parent(a);
        int b_p = find_parent(b);
        // 不在一个集合
        if (a_p != b_p)
        {
            int _big_p = _sizes[a_p] >= _sizes[b_p] ? a_p : b_p;
            int _little_p = _sizes[a_p] >= _sizes[b_p] ? b_p : a_p;
            _parents[_little_p] = _big_p;                                                        // 小挂大
            _sizes[_big_p] = _sizes[_big_p] + (_sizes[_little_p] == -1 ? 0 : _sizes[_little_p]); // 大的更新size
            _sizes[_little_p] = -1;
            // if (a==2&&b==3) {
            //     cout << "unionSet:" << _big_p << "," << _little_p << endl;
            //     cout << "_parents:" << _parents[_little_p] << "," << _parents[_big_p] << endl;
            //     cout << "_sizes:" << _sizes[_little_p] << "," << _sizes[_big_p] << endl;
            // }
        }
    }

    int find_parent(int cur)
    {
        int res = cur;
        while (_parents[res] != res)
        {
            res = _parents[res];
        }
        // if (cur==3) {
        //         cout << "find_parent:" << res << endl;
        //     }
        // 扁平化
        int tmp = cur;
        while (_parents[tmp] != res)
        {
            int old_parent = _parents[tmp];
            _parents[tmp] = res;
            tmp = old_parent;
        }
        return res;
    }

    int max_size() {
        int ans = 0;
        for (auto& iter : _sizes) {
            ans = max(ans, iter.second);
        }
        return ans;
    }

    //
    map<int, int> _parents; // 记录每个节点的父节点
    map<int, int> _sizes;   // 记录当前节点的所代表集合的节点数量

    // map<V, Node<V>> nodes;
};

class Solution
{
public:
    int largestComponentSize1(vector<int> &nums)
    {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        UnionFind uf(len);
        for (int i = 0; i < len; i++) {
            for (int j = i + 1; j < len; j++) {
                if (gcd(nums[i], nums[j]) != 1) { //最大公约数不是1的话将两个数合并到一个集合
                    uf.unionSet(i, j);
                }
            }
        }
        return uf.max_size();
    }
    
    // a和b不为0，a>b
    int gcd(int a, int b)
    {
        return b == 0 ? a : gcd(b, a % b);
    }

    /*
    质数因子：
    1. 选择一些因子代表nums[i]，可以使用质数因子来代表。假设每个数的大小是v规模，质数因子本身判断是srqt(v)的复杂度，然后遍历每个数字遍历因子也是sqrt(v)，所以总复杂度是o(v)的
    2. 遍历每个数得到质数因子，然后将之前出现过该因子所代表的的数合并到一起，最后得到最大集合
    3. 总复杂度是O(n*v)的

    普遍因子：
    1. 选择普通因子来代表nums[i]，遍历到sqrt(nums[i])，每次出来两个因子a和b, a*b=nums[i]。并让每个因子的数代表nums[i]
    2. 同样是每出来一个因子，看能不能和别的数合并
    3. 总复杂度是O(n*sqrt(v))
    */
    int largestComponentSize(vector<int> &nums) {
        int len = nums.size();
        if (len == 0)
        {
            return 0;
        }
        UnionFind uf(len);
        map<int, int> tmp_map;
        for (int i = 0; i < len; i++)
        {
            cout << "nums[i]:" << nums[i] << " sqrt:" << sqrt(nums[i]) << endl;
            for (int first=1;first<=sqrt(nums[i]);first++) {
                if (nums[i] % first != 0)
                {
                    continue;
                }
                //一次找到一对
                if (tmp_map.find(first) != tmp_map.end() && first!=1) {
                    int pre = tmp_map[first];
                    uf.unionSet(i, pre); //让当前数和pre集合合并
                }
                //
                int second = nums[i]/first;
                if (tmp_map.find(second) != tmp_map.end() && second!=1) {
                    int pre = tmp_map[second];
                    uf.unionSet(i, pre); // 让当前数和pre集合合并
                }
                cout << "first:" << first << " second:" << second << " i:" << i << endl;
                if (first != 1) {
                    tmp_map[first] = i;
                }
                if (second!=1) {
                    tmp_map[second] = i;
                }
            }
        }
        return uf.max_size();
    }
};

int main()
{

    Solution sol;
    // vector<int> arr = {2,3,6,7,4,12,21,39};
    vector<int> arr = {20, 50, 9, 63};
    // string s = "abcabcbb";

    cout << sol.largestComponentSize1(arr) << endl;
    cout << sol.largestComponentSize(arr) << endl;
    return 0;
}