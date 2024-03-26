/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-03-26 11:33:17
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

using namespace std;

/*
最大子数组异或和
给定一个数组，求最大子数组异或和
比如[11,1,15,10,13,4]，
*/
/*
暴力方法是，枚举所有子数组，

*/

class num_trie{
public:
    struct Node{
        map<int, Node*> next;
        Node() {
            next[0] = nullptr;
            next[1] = nullptr;
        }
    };    
    Node* head;
    num_trie() {
        head = new Node();
    }
    void add(int num) {
        Node* cur = head;
        for (int move=31;move>=0;move--) {
            int path = (num >> move) & 1; //从左到右拿出来每一位
            if (cur->next[path]==nullptr) {
                cur->next[path] = new Node();
            }
            cur = cur->next[path];
        }
    }


    void bfs_print() {
        queue<Node*> que, que_after;
        que.push(head);

        while (!que.empty()){

            while (!que.empty()) {
                Node* cur = que.front();
                que.pop();
                for (auto& it: cur->next) {
                    if (it.second!=nullptr) {
                        cout << it.first << " ";
                        que_after.push(it.second);
                    }
                }
            }
            cout << endl;

            while (!que_after.empty()) {
                que.push(que_after.front());
                que_after.pop();
            }
        }
        cout << "====" << endl;
    }

    //之前收集了很多数字(前缀和)
    //当前的前缀和，和前面出现过的前缀和，看怎么样能异或到最大
    //比如当前算到了[0..i]的前缀和。如果[0...j]的前缀异或和 与其相异或出来了最大值，则数组为[j...i]
    int max_xor(int cur_xor_sum) {
        // cout << "max_xor start" << endl;
        Node* cur=head;
        int ans = 0;
        for (int move=31;move>=0 && cur!=nullptr;move--) {
            //去除当前sum的高位状态
            int path = (cur_xor_sum >> move) & 1;
            // cout << "cur_path:" << path << " move:" << move << endl;
            //最高位如果是1的话为负数，期望需要1，异或成0变成正数; 如果高位为0，期望遇到0。最高位期望遇到相同的数字
            //其他位置期望遇到和当前位置相反的数字
            int best = move==31?path:(path^1);
            // cout << "best:" << best << endl;
            //实际在前缀树上的数字
            if (cur->next[best] != nullptr || cur->next[best^1] != nullptr) {
                best = cur->next[best] == nullptr ? (best^1): best;
                // cout << "best:" << best << endl;

                ans |= (path^best) << move;
            }
            cur = cur->next[best];
        }
        // cout << "max_xor end" << endl;
        return ans;
    }
};

class Solution
{
public:
    //暴力枚举1， O(N^3)
    int max_xor1(vector<int> &nums) {
        int len  = nums.size();
        int res  = 0;
        for (int i=0;i<len;i++) { //枚举开头
            for (int j=i;j<len;j++) { //枚举结尾
                int _xor = 0;
                for (int kk=i;kk<=j;kk++) {
                    _xor = _xor ^ nums[kk];
                }
                res = max(res, _xor);
            }
        }
        return res;
    }


    //暴力枚举2 O(N^2)
    int max_xor2(vector<int> &nums) {
        int len  = nums.size();
        vector<int> xor_nums(len, 0);
        xor_nums[0] = nums[0];
        for (int i=1;i<len;i++) { //前缀异或和
            xor_nums[i] = xor_nums[i-1] ^ nums[i];
        }

        int res = 0;
        for (int j=0;j<len;j++) { //枚举结尾
            for (int i=0;i<=j;i++) { //枚举[0..j], [1...j]
                int tmp = 0;
                //O(1)级别计算出[i..j]数组的异或和
                if (i==0) {
                    tmp = xor_nums[j];
                } else {
                    tmp = xor_nums[j] ^ xor_nums[i - 1];
                }
                res = max(res, tmp);
            }
        }

        return res;
    }

    void print_binary(int nums) {
        for (int i=31;i>=0;i--) {
            cout << ((nums >> i) & 1);
        }
        cout << endl;
    }

    //前缀树O(N)
    int max_xor3(vector<int> &nums) {
        int len = nums.size();
        if (len==0) {
            return 0;
        }
        num_trie nt;
        //[0...i]整体异或和
        int _xor = 0;

        int res = 0;
        nt.add(0); //添加0，是为了得到[0...i]的子数组和
        for (int i=0;i<len;i++) {
            _xor = _xor ^ nums[i];
            // cout << "cur_sum:";
            // print_binary(_xor);
            // int tmp = nt.max_xor(_xor);
            // cout << "tmp:" << tmp << ", binary:";
            // print_binary(tmp);
            nt.add(_xor);
            // nt.bfs_print();
            // if (i==1) {
            // break;
            // }
            // cout << "cur_res:";
            // print_binary(tmp);
            res = max(res, nt.max_xor(_xor));
        }
        return res;
    }
};

void generate_random_array(int max_size, int max_value, vector<int>& arr) {
    int size = (rand() % max_size)+1;
    for (int i = 0; i < size; i++) {
        arr.push_back(rand() % max_value);
    }
}

void print(vector<int>& arr) {
    for (auto iter:arr) {
        cout << iter  << ",";
    }
    cout << endl;
}
int main()
{
    
    int testTime = 1000;
    int maxSize = 30;
    int maxValue = 50;
    bool succeed = true;
    Solution s;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++) {
        vector<int> arr;
        generate_random_array(maxSize, maxValue, arr);
        int ans1 = s.max_xor1(arr);
        int ans2 = s.max_xor2(arr);
        int ans3 = s.max_xor3(arr);

        if (ans1 != ans2)
        {
            succeed = false;
            cout << "max_xor1: " << ans1 << " xor2:" << ans2 <<endl;
            break;
        }
        if (ans2!=ans3) {
            succeed = false;
            cout << "max_xor2: " << ans2 << " xor3:" << ans3 << endl;
            print(arr);
            break;
        }
    }
    cout << "测试结束" << endl;

    /*
    1011
    0001
    1111
    0101
    1111
    */
    // vector<int> nums = {11, 1, 15, 10, 13, 40};
    // Solution s;

    // vector<int> nums = {47,21,31,25,9,27,17,6};
    // vector<int> nums = {47, 21};
    // Solution s;
    // cout << s.max_xor1(nums) << endl;
    // cout << s.max_xor2(nums) << endl;
    // cout << s.max_xor3(nums) << endl;

    return 0;
}