/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-07 15:37:43
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

using namespace std;

/*
单调栈：

给出这样一座环形山，哪两座山的烽火可以相互看见呢？
{3,1,2,4,5}和{1,2,4,5,3}是相同结构环形山

必须满足如下两个标准：

1.如果两座山相邻，相邻必定能相互看见。

1既可以顺时针也可逆时针走到4。如果两座山峰不相邻，那么必然有两条路。

这两条路中，如果某一条路上出现了所有的值，比1和4的最小值 都不大，则1和4可以相互看见。
比如1到4两条路，都存在比1大的值，所以1和4之间的烽火互相看不见。

2.如果两座山不相邻，则两座山 之间有两条路，假设两座山的最小值是min
如果某一条路上出现了所有的值，都不大于min(<=min)

https://blog.csdn.net/Mr_zhang66/article/details/110392555

进阶：如果环形山中，含有相同高度的山，有多少对可见山峰
*/

/*
[3,1,2,4,5]，相邻的有5对，还有[3,4],[2,3]，共7对。

一、无重复值。O(1)。为了去重，规定只从小的数找大的数。
1. 如果数组长度len=1，只有0对
2. 2个数，有1对
3. 3个数，两两相邻，共3对
4. n个数，且n个数不重。一定能找到最大值max和次大值。任何位置x，顺时针一定找到找到第一个比x大的数y，同样在逆时针方向上可以找到第一个比x大的数z。
则一定可以找到[x,y]和[x,z]，所以x如果不是最大值和次大值，则一定可以找到两对。所以所有的x都可以找到2对，共2*(n-2)对。
最后一对是[次大值,次大值], 总对数是2*(n-1)+1=2*n-3

二、有重复值
1. 先遍历arr找到最大值max，假设为5
2. 从max触发逆时针出发，最后回到5. 准备一个栈，元素组织是[value, 次数]， 要求栈顶到栈底按照value从小到大
3. 当遇到比栈顶大的元素，栈顶元素弹出。当栈顶弹出时。假设栈顶是3个4，在下面是2个5，任何一个4在逆时针和顺时针方向上都能找到一个5(或者到不了5，将它弹出的数也是比它大的)，结算3*2=6对。3个4内部两两可见，结算c(3,2)=3对。共结算9对
4. 假设从max遍历之后，栈中还有数，就需要来到清算阶段。比如{5,4,3,2}，则栈中{{2,1},{3,1},{4,1},{5,1}}。
   a. 当栈大小>2时：栈顶元素为{2,k}，则顺时针和逆时针方向每个2都可以找到一对，然后2内部可以找到c(k,2)对
   b. 栈大小刚好等于2时：栈底max只有一个时，{{4,k},{5,1}}，顺时针或者逆时针方向遇到的是同一个数，则对外产生k*1对，内部产生c(k,2)对；
      {{4,k},{5,m}},栈底有多个，则每个4产生2对，内部产生c(k,2)对
   c. 栈大小为1时：只会内部产生可见对。{{max,k}}，产生c(k,2)对。
5. 之所以要从最大值开始遍历，是因为要为栈打底，保证转一圈有值

每个数最多进栈一次，出栈一次。复杂度是O(2*N)
*/

class Solution
{
public:
    
    struct Node {
        int val;
        int times;
        Node(int v) {
            val = v;
            times=1;
        }
    };
    void print_st(stack<Node>& st, string suf) {
        cout << suf << endl;
        stack<Node> st_tmp;
        while (!st.empty()) {
            cout << st.top().val << "," << st.top().times << " ";
            st_tmp.push(st.top());
            st.pop();
        }
        cout << endl;
        while (!st_tmp.empty()) {
            st.push(st_tmp.top());
            st_tmp.pop();
        }
    }
    int get_visible_num(vector<int>& arr) {
        int len = arr.size();
        if (len < 2) {
            return 0;
        }

        int max_idx = 0;
        for (int i = 1; i < len; i++) {
            max_idx = arr[max_idx] > arr[i] ? max_idx : i;
        }
        // cout << "max_idx=" << max_idx << endl;
        
        stack<Node> st; //栈顶到栈底，递增
        st.push(Node(arr[max_idx]));
        // cout <<  "start_top:"<< st.top().val << endl;
        // return 0;
        int idx = next_idx(max_idx, len);
        // cout << "遍历阶段开始======" << endl;
        // cout << "idx:" << idx << " " << endl;
        int ans = 0;
        //回到最大值位置停止
        for (;idx!=max_idx;) {
            /*
            当前的数要进入栈，如果栈顶比当前值小，则弹出结算
            因为栈底是最大值，所以第二个数入的时候肯定不会弹出下面的数，所以从第二个数弹出时外面一定可以产生两对
            */
           while (st.top().val < arr[idx]) {
                int k = st.top().times;
                st.pop();
                ans += k*2;
                ans += get_inter_sum(k);
           }
           
            //运行到这里代表st.top().val>=arr[idx]
           if (st.top().val == arr[idx]) {
               Node& cur_node = st.top();
               st.pop();
               cur_node.times++;
               st.push(cur_node);
           } else {
               st.push(Node(arr[idx]));
           }
           idx = next_idx(idx, len);
        //    cout << "=====" << endl;
        //    cout << "idx:" << idx << " " <<endl;
        }
        // print_st(st, "遍历结束栈状态");
        // cout << endl << "遍历阶段结束====== ans:" << ans << endl;

        //处理栈内剩余的部分
        while (!st.empty()) {
            int st_s = st.size();
            int k = -1;
            if (st_s > 2) { //清算栈大小>2的情况
                k = st.top().times;
                st.pop();
                ans += k*2;
                ans += get_inter_sum(k);
            } else if (st_s==2) { //清算栈中有2条记录的情况
                Node& top_node = st.top();
                k = top_node.times;
                st.pop();
                int last_times = st.top().times;
                if (last_times == 1) {
                    ans += k;
                } else {
                    ans += k*2;
                }
                ans += get_inter_sum(k);
            } else { //只剩一个数了
                k = st.top().times;
                st.pop();
                ans += get_inter_sum(k);
            }
        }

        return ans;
    }
    //或者c(k,2)，从k个里面选2个
    int get_inter_sum(int k) {
        return k*((k-1)/2);
    }

    //顺时针
    int next_idx(int i, int len) {
        if (i==len-1) {
            i=0;
        } else {
            i++;
        }
        return i;
    }
    //逆时针
    int last_idx(int i, int len) {
        if (i==0) {
            i=len-1;
        } else {
            i--;
        }
        return i;
    }

    //暴力
    int get_visibale_num_baoli(vector<int>& arr) {
        int len = arr.size();
        set<string> sets;
        int ans = 0;
        for (int i=0;i<len;i++) { //枚举开始
            //从每个位置开始，利用小找大原则，看能找到多少对

            int num = 0;
            for (int j=0;j<len;j++) {
                if (i!=j) { //不找自己
                    if (arr[i] == arr[j]) {
                        std::ostringstream oss;
                        oss << min(i,j) << "_" << max(i,j); //以下标为key，小在前面，大在后面
                        std::string key = oss.str();
                        if (sets.find(key) != sets.end() && is_visible(arr, i, j)) { // 可见
                            num++;
                            sets.insert(key);
                        }
                    } else if (is_visible(arr, i, j)){
                        num++;
                    }
                }
            }
            
            ans += num;
        }
        return ans;
    }

    bool is_visible(vector<int>& arr, int l_idx, int h_idx) {
        // cout << "l_idx " << l_idx << " h_idx " << h_idx << endl;
        if (arr[l_idx] > arr[h_idx]) {
            return false;
        }
        int len = arr.size();

        //顺时针
        bool next_walk = true;
        int cur = next_idx(l_idx, len);
        while (cur != h_idx) {
            // cout << "l_idx " << l_idx << " h_idx " << h_idx << " next_walk_cur:" << cur << endl;
            if (arr[cur] > arr[l_idx]) {
                next_walk = false;
            }
            cur = next_idx(cur, len);
        }

        //逆时针
        cur = last_idx(l_idx, len);
        bool last_walk = true;
        while (cur!=h_idx) {
            // cout << "l_idx " << l_idx << " h_idx " << h_idx << " last_walk_cur:" << cur << endl;
            if (arr[cur] > arr[l_idx]) { //遇到h_idx数之前，遇到了比l_idx大的，则l_idx和h_idx不可见
                last_walk = false;
            }
            cur = last_idx(cur, len);
        }

        return next_walk || last_walk;//有一个方向可见，就是可见的
    }

};

int main()
{

    Solution sol;

    int test_time = 10000;
    int max_len = 10;
    int max_num = 1000;
    for (int i = 1; i <= test_time; i++) {
        int len = (int) (rand() % max_len) + 1;
        vector<int> arr(len, 0);
        for (int j = 0; j < len; j++) {
            arr[j] = rand() % max_num + 1;
        }
        int ans1 = sol.get_visibale_num_baoli(arr);
        int ans2 = sol.get_visible_num(arr);
        if (ans1 != ans2)
        {
            cout << "ans1:" << ans1 << " ans2:" << ans2 << endl;
        }
    }

    // vector<int> arr = {3,1,2,4,5};
    // cout << sol.get_visibale_num_baoli(arr) << endl;
    // cout << sol.get_visible_num(arr) << endl;
    return 0;
}