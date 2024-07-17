/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-07 11:58:37
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

using namespace std;

/*
给定一个长度为偶数的数组arr，长度记为2*n。前N个为左部分，后N个为右部分。arr就可以表示为{l1,l2,..ln,r1,r2,...rn}，
将数组调整成{r1,l1,r2,l2,...,rn,ln}的样子
要求使用有限几个变量，复杂度O(n)
*/
/*

1. 函数f根据i原来的位置，和一半长度n。返回应该在调整后来到的位置。int f(i,n);
假设[1 2 3 4 5 6 7 8 ]，此时n=4。
1. 当i>n时，来到的位置是(i-N)*2-1。将其调整到左半区，然后到奇数位置
2. 当i<=n时，来到的位置是i*2

2. 如果不使用额外空间，则需要使用下标循环怼方式：
[a b c d]
[1 2 3 4]
假设数组abcd的位置分别是1234，先计算a需要来到的位置是2，则空降2位置。将b怼出来，b将要来到的位置是2*2=4，则空降4位置。
将d怼出来，d将要来到的位置是(4-2)*2-1=3位置，空降3位置，将c怼出来；c将要来到(3-2)*2-1=1位置，将c填到1位置

3. 但并不是所有的数组都是一个循环。比如:
[a b c d e f]
[1 2 3 4 5 6]
a来到2位置，b来到d位置，d来到1位置形成一个循环。有个问题是怎么在有限变量下，找到所有的环？

4. 完美洗牌问题的结论：当数组长度是s=3^k-1（偶数,k>0）时，下标循环怼的环入口是1,3,9,....3^(k-1)

5. 如果s不正好等于3^k-1时，可以使用凑数的方式来搞。在此之前需要先了解一个算法原型：arr=[l1,l2,l3,r1,r2]，将数组arr调整成[r1,r2,l1,l2,l3]。
流程是将l1,l2,l3逆序，r1,r2逆序，然后让整体逆序：[l3,l2,l1,r2,r1]->[r1,r2,l1,l2,l3]

6. 假设s=14, 数组为arr=[l1,l2,l3,l4,l5,l6,l7,r1,r2,r3,r4,r5,r6,r7]。
 a. 取最接近14的3^k-1为8，则在右边取8/2=4个数和左边的l1...l4放在一起做下标循环怼。将[l5,l6,l7,r1,r2,r3,r4]做完整逆序。arr变成[l1,l2,l3,l4,r1,r2,r3,r4,l5,l6,l7,r5,r6,r7]
 b. 左边做下标循环怼，开始位置分别是l1,l3。右边长度s=6，再取3^k-1=2, 做逆序之后变成[l1,l2,l3,l4,r1,r2,r3,r4, l5,l6,r5,r6,l7,r7]
 c. 最后l6,l7刚好还剩2个是3^1-1，自己做下标循环怼

7. 复杂度估计：假设每次取来做下标循环怼的数是a=3^k-1, 则用来和a/2组合做下标循环怼的左边的数不会超过3a，因为如果超过的话k的值将可以取到+1的数。所以每次逆序的复杂度是O(a)， 下标循环怼的复杂度是O(a)。
   再下次的复杂度是O(b),O(c).....，而a+b+c+....=N。所以总复杂度是O(N)
*/

class Solution
{
public:


    //主函数
    //7,8,9,10
   void shuffle_main(vector<int>& arr) {
        shuffle(arr, 0, arr.size()-1);
   }
   //在l和r上做完美洗牌问题
   void shuffle(vector<int>& arr, int l, int r) {
        while (r-l+1>0) {//只要[l...r]上还剩余数字就一直处理
            int len = r-l+1;
            int base = 3;
            int k=1;
            //计算小于等于len，离len最近的满足3^k-1的数
            //即满足3^k-1<=len -> 3^k <= len+1
            while (base <= (len+1)/3) { //防止溢出
                base*=3;
                k++;
            }

            //base=3^k, 需要解决base-1个数
            int half = (base-1)/2;
            int mid = (l+r)/2; //总数组的中点位置. 上中点

            //需要旋转的左部分为[l+half...mid]，右部分为[mid+1, half+mid]
            rotate(arr, l+half, mid, half+mid);

            // //以l为开始，长度为base-1的数组上做下标循环怼
            cycles(arr, l, base-1, k);
            l += base-1;
        }
   }
   //[l..m]为左部分，[m+1...r]是右部分。左右两部分互换
   void rotate(vector<int> &arr, int l, int m, int r)
   {
        reverse(arr, l, m);
        reverse(arr, m + 1, r);
        reverse(arr, l, r);
   }
   //对l和r范围上做逆序
   void reverse(vector<int>& arr, int l, int r) {
       while (l<r) {
           int tmp = arr[r];
           arr[r--] = arr[l];
           arr[l++] = tmp;
       }
   }
    //输入老下标，输出新下标
   int modify_index(int i, int len) {
        cout << endl << "modify_index:" << i << " len:" << len << endl;
        if (i > (len/2)) {
            return (i-len/2)*2-1;
        } else {
            return i*2;
        }
   }
   //从start开始，往右len长度这一段，做下标连续推
   //出发位置依次是1,3,9
   void cycles(vector<int>& arr, int start, int len, int k) {
        cout <<  "start:" << start << " len:" << len << " k:" << k << endl;
       for (int i=0,trigger=1;i<k;i++,trigger*=3) { //枚举环开始的位置， k-1个开始点
           int s_idx = start+trigger-1;
           int pre_val = arr[s_idx];
           int cur_need_idx = modify_index(s_idx-start+1, len)+start-1;
           cout << "s_idx:" << s_idx << " cur_need_idx:" << cur_need_idx << endl;
           while (cur_need_idx != s_idx) {
               int tmp = arr[cur_need_idx];
               arr[cur_need_idx] = pre_val;
               pre_val = tmp;
               int old_need_idx = cur_need_idx;
               
               cur_need_idx = modify_index(cur_need_idx-start+1, len)+start-1;//计算时换算到从1开始下标
               cout << "old_need_idx:" << old_need_idx << " cur_need_idx:" << cur_need_idx << " len:" << len  << endl;
           }
           //最后将最后一个数放在开始的位置
           arr[s_idx] = pre_val;
       }
   }
};


int main()
{

    Solution sol;
    // string s = "aba";
    // vector<int> arr = {3,4,5,6,7,15,16,17,19,20};
    // vector<int> arr = {3,4};
    vector<int> arr = {1,2,3,4,5,6,7,8};
    sol.shuffle_main(arr);
    for (auto iter:arr) {
        cout << iter << " ";
    }
    cout << endl;
    return 0;
}