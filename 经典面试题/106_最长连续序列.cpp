/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-25 15:40:04
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
#include <functional>
#include <sstream>
#include <unordered_set>

using namespace std;

/*
https://leetcode.cn/problems/longest-consecutive-sequence/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china
给定一个未排序的整数数组 nums ，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。

请你设计并实现时间复杂度为 O(n) 的算法解决此问题。

 

示例 1：

输入：nums = [100,4,200,1,3,2]
输出：4
解释：最长数字连续序列是 [1, 2, 3, 4]。它的长度为 4。
示例 2：

输入：nums = [0,3,7,2,5,8,4,6,0,1]
输出：9
 

提示：

0 <= nums.length <= 105
-109 <= nums[i] <= 109
*/

/*
暴力思想：
可以排序，O(N*log(n))

两个map的方法：
一个记录头的head_map，一个记录尾的tail_map。
head_map[i]=j代表以i为开头时，序列长度是j.
tail_map[i]=j代表以i为结尾时，序列长度是j.
每次来一个数arr[i]时：
    1. 首先将head_map[i]=1和tail_map[i]=1插入
    2. 在tail_map中检查有没有以arr[i]-1结尾的序列。如果有，其长度是len=tail_map[arr[i]-1]，
        往前到达了start=arr[i]-1-len。
        从tail_map中删除以arr[i]-1结尾的数，然后tail_map[arr[i]]+=len
        从head_map中删除arr[i]为开头的数，head_map[start] += 1
    3. 在head_map中检查有没有i+1的key。如果有，长度是len=head_map[arr[i]+1]
        往后到达了end=arr[i]+1+len, 从head_map中删除arr[i]+1， tail_map中删除arr[i]
        head_map[arr[i]]+=len
        tail_map[end]+=1
最后从head_map中得到最大的value就是答案

*/

class Solution {
public:
    int longestConsecutive_origin(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        map<int,int> head_map,tail_map;
        for (int i=0;i<len;i++) {
            int num = nums[i];
            //理论上只有第一次到来时才处理。重复的数不会对答案有影响
            if (head_map.find(num) == head_map.end() && tail_map.find(num) == tail_map.end()) {
                head_map[num] = 1;
                tail_map[num] = 1;
                
                if (tail_map.find(num-1)!= tail_map.end()) {
                    int start = num-1 - tail_map[num-1] + 1;
                    tail_map[num] += tail_map[num-1]; //将前半段加上
                    head_map[start] += 1; //此时num贡献还是1
                    head_map.erase(num); //删掉以num开头的，此时以start开头
                    tail_map.erase(num-1); //删掉以num-1结尾的。此时以num结尾
                }

                if (head_map.find(num+1)!= head_map.end()) {
                    int end = num+1 + head_map[num+1] - 1;
                    int start = num-tail_map[num]+1; //计算此时从num出发可以往前到哪里
                    head_map[start] += head_map[num + 1]; //将后半段加上
                    tail_map[end] += tail_map[num];//前半段加上
                    
                    head_map.erase(num + 1);//删掉以num+1开头的， 此时以start开头
                    tail_map.erase(num); //肯定不会再以num结尾了， 此时以end结尾
                }
            }
            //每过一个数看下head_map和tail_map
            // cout << "i:" << i << " nums[i]:"<< nums[i] << " head_map:" << endl;
            // for (auto& iter:head_map) {
            //     cout << iter.first << "," << iter.second << endl;
            // }

            // cout << "i:" << i << " nums[i]:"<< nums[i] << " tail_map:" << endl;
            // for (auto& iter:tail_map) {
            //     cout << iter.first << "," << iter.second << endl;
            // }
        }
        int ans = 0;
        for (auto& iter:head_map) {
            ans = max(ans,iter.second);
        }
        return ans;
    }

    /*
    减少一个map的思路：
    map[nums[i]]=j代表nums[i]数字所在的序列长度是j
    每有一个新的nums[i]到来时：
        1. 因为nums[i]是第一次出现，所以nums[i]-1如果在map中存在，则一定是可以表示结尾。
            往前可以达到start=nums[i]-1-_map[nums[i]-1]+1。更新_map[start]+=1
            _map[nums[i]]=_map[start]
            此时start为开头，nums[i]为结尾
        2. 同理nums[i]+1如果在map中存在，一定是开头，长度是len=_map[nums[i]+1]。往后可以达到end=nums[i]+1+_map[nums[i]+1]-1
            更新_map[end]+=map[nums[i]]
            _map[start]+=len
            还是以start开头，此时到达了最后end处
    */
    int longestConsecutive(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        map<int,int> _map;
        int start = -1, end = -1;
        int ans = 0;
        for (int i=0;i<len;i++) {
            int num = nums[i];
            //不存在才做处理
            if (_map.find(num) == _map.end()) {
                _map[num] = 1;//num长度至少是1
                //如果前后都没有的话，开头和结尾都是num
                start = num;
                end = num;
                //往前合并。num-1存在肯定是结尾
                if (_map.find(num-1) != _map.end()) {
                    start = num-1-_map[num-1]+1;
                    //更新开头和结尾
                    _map[start] +=1;
                    _map[num] = _map[start];
                }
                //往后合并
                if (_map.find(num+1) != _map.end()) {
                    int len = _map[num+1];//往后的长度
                    end = num+1+_map[num+1]-1;
                    //仅更新开头和结尾
                    _map[end] += _map[num]; 
                    _map[start] += len;
                }
                ans = max(ans, max(_map[start], _map[end]));
            }
            // cout << "i:" << i << " nums[i]:" << nums[i] << endl;
            // for (auto& iter:_map) {
            //     cout << iter.first << "," << iter.second << " ";
            // }
            // cout << endl;
        }
        // int ans = 0;
        // for (auto& iter:_map) {
        //     ans = max(ans,iter.second);
        // }
        return ans;
    }

};

int main() {
    Solution sol;
    /*
    输入：words = ["abcd","dcba","lls","s","sssll"]
    输出：[[0,1],[1,0],[3,2],[2,4]]
    解释：可拼接成的回文串为 ["dcbaabcd","abcddcba","slls","llssssll"]
    */
//    vector<string> words = {"abcd","dcba","lls","s","sssll"};
    vector<int> nums = {100,1,3,0,4,2,17};
    cout << "ans:" << sol.longestConsecutive(nums) << endl;
    return 0;
}