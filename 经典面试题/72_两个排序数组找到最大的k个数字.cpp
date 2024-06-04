/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-04 12:04:42
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

给定两个整数数组A和B。
A是长度为m，元素从小到大排好序了
B是长度为n，元素从小到大排好序了
希望从A和B数组中，找出最大的k个数字

*/

/*
朴素思想。
两个指针分别指向a和b的结尾，谁大谁左移动，将结果放入，复杂度O(K)


先考虑求第20小的数。
1. 先在A里二分在B里定位，假设A中<=a[mid]的数有甲个，然后在B中找到<=a[mid]的位置，算出来前面有乙个，这样总共定位出来了，tmp=甲+乙个小于等于A[mid]的数
假设tmp==20, 那a[mid]就是第20小的数。如果tmp>20，说明a[mid]选的太大了，再在左侧范围上继续这个过程，如果tmp<20，再在右侧范围上继续这个过程。
2. 假设A中找不到第20小的数，再从B数组中二分在A中定位一定会成功（因为第20小必然来自某个数组）。
3. 复杂度2*log(n)*log(m)，O(log(n)*log(m))


最优log(min(n,m))复杂度：
一、需要先了解一个基础算法：
A和B两个数组各有4个数（等长），求整体的上中位数。
1. A和B的元素都是偶数的情况。假设A=[a1,a2,a3,a4], B=[b1,b2,b3,b4]
    a. 取两个数组的上中位数，如果a2==b2则合到一起的上中位数一定是a2，总共8个数，排完序后一定是位于第3、4位置。
    b. 如果a2!=b2: a2>b2, a2<b2，只讨论a2>b2: 
        i)  因为a3>=a2>b2， 所以前半段数在a3前面，所以a3,a4不可能是上中位数
        ii) 因为b2<a2<=a3，同时b2<=b3<=b4，所以b1,b2一定在前面，不可能是上中数。
        iii) 下面只有b3、b4是可能成为上中位数，上面只有a1,a2有可能
        iv) 将[a1,a2], [b3,b4]看成新的数组，他们两个数组中的第2小就是整体数组中的第4小。递归求解。证明：
            假设a1是第2小的，说明a3<=a1<=a2<=b4，对应到原数组，比b3大，比b4小，刚好是第4小的。
            假设b3是第2小的，说明a1<=a3<=a2<=b4，对应到原数组，比b1大, b3前面就有4个数了。

2. A和B长度都是奇数时：假设A=[a1,a2,a3,a4,a5], B=[b1,b2,b3,b4,b5]
    a. 去两个数组上中位数，如果a3==b3，则排完序后一定是第4和第5小的, 所以合并后上中位数是a3
    b. 如果a3>b3:
        i) 因为a3>b3>=a2>=a1，又因为a3>=a2>=a1，所以a3最多是第6个数，所以[a3,a4,a5]都不可能。
        ii) 因为b2最好的成绩就是将a1,a2,b1放在前面，所以b2不可能是上中位数。所以[a1,a2]不可能。如果b3>=a2则就是第5小
        iii) 所以A中有2个元素有可能，B中有3个元素有可能。需要手动排除b3，如果b3>=a2则b3就是整体第5小。如果b3<a2则剔除b3。将[a1,a2]和[b4,b5]递归求上中位数

3. 每次拿中间数是O(1), 做各种判断是O(1)的, 递归每次将减少n/2的量，即T(n) = T(n/2)+o(1)，总复杂度是O(log(n))。

二、现在回到第k小的数
假设 A[a1,a2,....a10],B[b1,b2,b3....b17]，两个数组加起来共27个元素
k的取值范围是1<=k<=27
1. 1<=k<=10(短数组长度)时，假设要求第7小，则在A中拿7个，在B中拿7个，使用算法原型求上中位数就是第7小
2. 10(短数组长度)<k<=17(长数组长度)，假设要求第15小.
    a. A数组中所有的数都是有可能的(例如：b14<=a1<=b15, b13<=a1<=b14)
    b. B中b1,b2,b3,b4都不可能，因为即使b4比A中所有数都大，最多也是第14小。后面b16,b17也不可能(至少是16小)
    c. b中b5到b15是可能的，[b5...b15]有11个数，A[a1..a10]有10个数。所以单独判断b5>=a10，如果满足，则b5就是第15小。如果不满足则淘汰b5。
       将[a1..a10]和[b6...b16]组合求上中位数(第10小)，因为前面已经淘汰掉了[b1...b5]，所以刚好该上中位数就是第15小

3. 17(长数组长度)<k<=27(和)。假设求第23小
    a. A数组中，a6>=b17时，刚好a6是第23小。或者a7>=b16时，刚好a7是第23小。所以[a6,a7,a8,a9,a10]是有可能的。淘汰掉[a1..a5]共5个数
    b. B数组中，b13>=a10、b14>=a9..时都是有可能的, 及[b13...b17]。淘汰掉[b1..b12]共12个数
    c. 淘汰掉12+5=17个数，但[a6..a10]和[b13...b17]求出来的上中位数是第5小，是总数组的第22小，达不到第23小
    d. 通过c的分析，可以再手动判断a6和b13是否是第23小。如果不是，则继续淘汰掉a6和b13(共淘汰了19个数)。剩下的两个4长度的数组求上中位数, 求出来的第4小，刚好是大数组中的第23小

4. 在1,2,3每个范围上，基础操作是O(1)的。进行二分求上中位数的过程是A和B拿等长的数列，最大长度就是短数组长度，所以这部分复杂度是log(min(m,n))，总复杂度是log(min(m,n))
*/

class Solution
{
public:

    //两个相同数组获得上分位数
    //A[s1..e1]和B[s2..e2]等长且有序，求A[s1..e1]和B[s2..e2]上中位数
    int get_up_median(vector<int>& A, int s1, int e1, vector<int>& B, int s2, int e2) {
        int mid1=0,mid2=0;
        while (s1<e1) {
            int mid1 = (s1+e1)/2;
            int mid2 = (s2+e2)/2;
            if (A[mid1]==B[mid2]) {
                return A[mid1];
            }
            int odd = ((e1-s1+1)&1) == 1; //最后一位是1就是奇数

            if(odd) { //奇数长度
                if (A[mid1]>B[mid2]) {
                    if (B[mid2] >= A[mid1-1]) { //如果b3不满足条件，则去掉
                        return B[mid2];
                    }
                    e1 = mid1-1;
                    s2 = mid2+1;
                } else {
                    if (A[mid1] >= B[mid2-1]) { //如果a3不能比b2大，则不可能是第5小的了
                        return A[mid1];
                    }
                    s1 = mid1+1;
                    e2 = mid2-1;
                }
            } else { //偶数长度
                if (A[mid1]>B[mid2]) {
                    e1 = mid1;
                    s2 = mid2+1;
                } else {
                    s1 = mid1+1;
                    e2 = mid2;
                }
            }
        }
        //两个数组都只剩一个数了，合到一起的上中位数就是比较小的那个
        return min(A[s1], B[s2]);
    }

    //第k小的数字
    int get_kth_num(vector<int>& A, vector<int>& B, int k) {
        if (k<1 || k> A.size()+B.size()) {
            return -1;
        }

        vector<int>& longs = A.size()>=B.size()? A:B;
        vector<int>& shorts = A.size()<B.size()? A:B;

        int s = shorts.size();
        int l = longs.size();
        

        if (k<=s) { //k<=s的情况，短数组所有数都有可能
            return get_up_median(shorts, 0, k-1, longs, 0, k-1);
        }

        if (k>l) { //l<k<=l+s
            /*
            淘汰掉: shorts[1...k-l-1], longs[1..k-s-1]
            保留：shorts[k-l...s], longs[k-s...l]
            手动判断第k-l和k-s个数是否是第k小
            */
           if(shorts[k-l-1] >= longs[l-1]) { //(k-l)+l=k
               return shorts[k-l-1];
           }
           if (longs[k-s-1] >= shorts[s-1]) { //k-s+s = k
                return longs[k-s-1];
           }
            //同样元素的数组获取大小
            // s-1-(k-l-1+1) = s+l-k-1 = l-1-(k-s-1+1)
            return get_up_median(shorts, k-l-1+1, s-1, longs, k-s-1+1, l-1);
        } else { //s<k<=l

            /*
            淘汰掉: longs[1..k-s-1] longs[k+1...l]
            保留： shorts[1...s], longs[k-s...k]

            因为shorts长度是s-1+1=s, longs长度是k-(k-s)+1=s+1个数，所以单独判断longs的第k-s个数是否是第k小
            */
           if (longs[k-s-1]>=shorts[s-1]) { //s+(k-s)=k
               return longs[k-s-1];
           }
           return get_up_median(shorts, 0, s-1, longs, k-s-1+1, k-1);
        }
    }

    //暴力排序，求第k个数
    int get_kth_num_all(vector<int>& A, vector<int>& B, int k) {
        vector<int> tmp;
        for (auto iter:A) {
            tmp.push_back(iter);
        }
        for (auto iter:B) {
            tmp.push_back(iter);
        }
        sort(tmp.begin(), tmp.end());
        return tmp[k-1];
    }

};

int main()
{

    Solution sol;



    vector<int> A,B;
    int test_time = 1000;
    int len = 0;
    int k= 0;
    cout << "测试开始" << endl;

    int ans0 = 0;
    int ans1 = 0;
    int ans2 = 0;
    for (int i=0;i<test_time;i++) {
        len = (int)((std::rand() % 5) + 1);
        A.clear();
        B.clear();
        A.resize(len);
        B.resize(len);
        for (int j=0;j<len;j++) {
            A[j] = (int)((std::rand() % 10) + 1);
        }
        for (int j=0;j<len;j++) {
            B[j] = (int)((std::rand() % 10) + 1);
        }
        sort(A.begin(), A.end());
        sort(B.begin(), B.end());
        vector<int> arr1_A = A;
        vector<int> arr1_B = B;
        vector<int> arr2_A = A;
        vector<int> arr2_B = B;
        k = (int)((std::rand() % len)+1);
        ans0 = sol.get_kth_num(arr1_A, arr1_B, k);
        ans1 = sol.get_kth_num_all(arr2_A, arr2_B, k);
        if (ans0!=ans1) {
            cout << "=========" << k<< "===="  << endl;
            for (auto iter:A) {
                cout << iter << " ";
            }
            cout << endl;
            for (auto iter:B) {
                cout << iter << " ";
            }
            cout << endl;
            cout << " ans0:" << ans0 << " ans1:" << ans1 << endl;
        }
    }
    cout << "测试结束" << endl;

    // 获得两个数组中的上中位数
    // vector<int> A = {1,5,8,10};
    // vector<int> B = {2,3,11,20};

    // vector<int> A = {1, 5, 8, 10, 11};
    // vector<int> B = {2, 3, 11, 13, 20};

    // cout << sol.get_up_median(A, 0, A.size() - 1, B, 0, B.size() - 1) << endl;

    // cout << sol.get_kth_num(A, B, 3) << endl;
    // cout << sol.get_kth_num_all(A,B,3) << endl;
    

    return 0;
}