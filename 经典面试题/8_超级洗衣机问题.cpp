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
* 有n个打包机器从左到右一字排开， 上方有一个自动装置会抓取一批放物品到每个打包机上
 * 放到每个机器上的这些物品数量有多有少， 由于物品数量不相同， 需要工人
 * 将每个机器上的物品进行移动从而到达物品数量相等才能打包。 每个物品重量太大、
 * 每次只能搬一个物品进行移动， 为了省力， 只在相邻的机器上移动。 请计算在搬动最
 * 小轮数的前提下， 使每个机器上的物品数量相等。 如果不能使每个机器上的物品相同，
 * 返回-1。
 * 例如：1 0 5。 第一轮变为1,1,4; 第二轮变为2,1,3;第三轮变为2,2,2
 *
 * 力扣原题：超级洗衣机问题：https://leetcode.cn/problems/super-washing-machines/description/
*/

class Solution
{
public:
    /*
    和网络流阻塞问题很像，任何一个位置是阻塞的痛点，则就是瓶颈
    假设包裹总和为sum，sum%n!=0则说明不能给n个机器均分，肯定不能均分，返回-1。
    当总包裹数能够均分时，对i=5位置进行分析，假设每个机器需要20个包裹。i左边的机器初始有100个包裹的话，实际需要扔出去100-(i-1)*20=20个包裹, 
    右边如果有80个包裹，实际需要120个，则还需要往右边扔40个包裹，则总的轮数是max{20, 40}。
    
    设左右两边实际有的包裹数分别为l_you, r_you。两边最终需要的包裹数分别为l_real, r_real
       ，则ll= |l_you-r_real|和rr=|l_you-r_real|分别是左右两边需要抛出或扔进去的包裹数量。
       下面进行分情况讨论：
       1. ll>0, rr>0: 总需要的轮数是max(ll,rr), 往中间i位置抛
       2. ll>0, rr<0: 总需要的轮数是max(ll,rr), 往rr抛
       3. ll<0, rr>0: 总需要的轮数是max(ll,rr), 往ll抛
       4. ll<0, rr<0: 两边都需要包裹的时候，就是需要从i往外抛，需要的轮数是|ll|+|rr|，痛点在arr[i]
    对于每个位置i分别求出来，需要的轮数，找到最痛的点O(n)。每轮可以用前缀数组或前缀和加速，为O(1)。总复杂度O(N)
    */
    int findMinMoves(vector<int>& machines) {
        int len = machines.size();
        int sum = 0;
        for (int i=0;i<len;i++) {
            sum+=machines[i];
        }
        if (sum%len!=0) {
            return -1;
        }

        int avg = sum/len;
        int left_sum = 0;
        int ans = -1;
        for (int i=0;i<len;i++) {
            int ll = left_sum - (i*avg); //左边需要扔出或收到的包裹数
            int rr = (sum - left_sum - matchines[i]) - (len-i-1)*avg; //右边需要扔出或收到的包裹数=右边实际的包裹数-最终需要的包裹数
            if (ll < 0 && rr < 0) {
                ans = max(ans, abs(ll)+abs(rr)); //痛点在matchines[i], 需要扔出去abs(ll)+abs[rr]个包裹
            } else {
                ans = max(ans, max(abs(ll), abs(rr)));
            }
            left_sum += machines[i];
        }
        return ans;
    }
};

int main()
{
    Solution sol;
}