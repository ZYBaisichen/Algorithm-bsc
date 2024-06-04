/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-03 19:57:43
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
https://leetcode.cn/problems/median-of-two-sorted-arrays/submissions/536919642/
本题和经典面试题72题特别类似，使用左神的这个版本实现一个。但常数时间较大


给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。请你找出并返回这两个正序数组的 中位数 。

算法的时间复杂度应该为 O(log (m+n)) 。

示例 1：

输入：nums1 = [1,3], nums2 = [2]
输出：2.00000
解释：合并数组 = [1,2,3] ，中位数 2
示例 2：

输入：nums1 = [1,2], nums2 = [3,4]
输出：2.50000
解释：合并数组 = [1,2,3,4] ，中位数 (2 + 3) / 2 = 2.5
 

 

提示：

nums1.length == m
nums2.length == n
0 <= m <= 1000
0 <= n <= 1000
1 <= m + n <= 2000
-106 <= nums1[i], nums2[i] <= 106
*/

/*
左神版本，求两个数组第K大的数：
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
            假设a1是第2小的，说明b3<=a1<=a2<=b4，对应到原数组，比b3大，比b4小，刚好是第4小的。
            假设b3是第2小的，说明a1<=a3<=a2<=b4，对应到原数组，比b1大, b3前面就有4个数了。

2. A和B长度都是奇数时：假设A=[a1,a2,a3,a4,a5], B=[b1,b2,b3,b4,b5]
    a. 去比较两个数组上中位数，如果a3==b3，则排完序后一定是第4和第5小的, 所以合并后上中位数是a3
    b. 如果a3>b3:
        i) 因为a3>b3>=a2>=a1，又因为a3>=a2>=a1，所以a3最多是第6个数，所以[a3,a4,a5]都不可能。
        ii) 因为b2最好的成绩就是将a1,a2,b1放在前面，所以b2不可能是上中位数。所以[b1,b2]不可能。如果b3>=a2则就是第5小
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

/*
左神的方法其实和力扣官方答案一样。但官方答案还介绍了另外一种集合论的方法： 摘自https://leetcode.cn/problems/median-of-two-sorted-arrays/solutions/258842/xun-zhao-liang-ge-you-xu-shu-zu-de-zhong-wei-s-114/

划分数组方法：
首先理解中位数的作用是什么。在统计中，中位数被用来：将一个集合划分为两个长度相等的子集，其中一个子集中的元素总是大于另一个子集中的元素

假设A是短的数组，长度是m。B是长的数组，长度是n。如果不是，则交换一下。
在任意位置i将A划分成两部分，left_a = [A[0],A[1],..A[i-1]] , right_a = [A[i],A[i+1]....A[m-1]]
由于A中有m个元素，所以有m+1种划分方法，i取值[0,m]。len(left_a) = i, len(right_a)=m-i。当i = 0时， left_a为空集，当i=m时right_a是空集

采用同样的方式将B分为left_b和right_b。

下面将left_a和left_b放入一个集合，right_a和right_b放入一个集合。再把两个新的集合分别命名为left_part和right_part。
left_part : A[0],A[1],...A[i-1], B[0],B[1],...B[j-1]
right_part : A[i],A[i+1],...A[m-1], B[j],B[j+1],...B[n-1]

当A和B的总长度是偶数时，如果可以确认以下两点：
    1. len(left_part) == len(right_part)
    2. max(left_part) <= min(right_part)
那么，{A,B}中的所有元素已经被划分为相同长度的两部分，且前一部分中的元素总是小于后一部分的元素。中位数就是迁一部分的最大值和最后一部分的最小值取平均：
    median = (max(left_part) + min(right_part))/2
当A和B总长度是奇数时，如果可以确认以下两点：
    1. len(left_part) = len(right_part)+1
    2. max(left_part) <= min(right_part)
那么， {A,B}中所有的元素都已经划分为两个部分，前一部分比后一部分多一个元素，且前一部分中的元素总是小于等于后一部分中的元素，中位数就是前一部分的最大值：
    median = max(left_part)
上述中，第一个条件对于总长度是偶数和奇数的情况有所不同，但可以将两种情况合并。第二个条件对于总长度是偶数和奇数的情况是一样的。

要保证这两个条件，只需要保证：
    1. i+j = m-i + n-j 或者 i+j = m-i + n-j + 1。 等号左侧为前一部分的元素个数，右侧为后一部分的元素个数
        两个式子相加，并将i和j全部移动到等号左侧，就可以得到， i+j = (m+n)/2 或者 i+j = (m+n+1)/2。 因为加1不会印象取整数部分，所以两个式子合并到一起是 i+j = (m+n+1)/2
    2. 0<=i<=m, 0<=j<=n. 如可以得到j=(m+n+1)/2 - i, 这里借用了前面规定的A是短数组，m<n。避免j计算出来负数。
    3. B[j-1]<=A[i]以及A[i-1]<=B[j]， 即前面一部分的最大值小于等于后面一部分的最小值。

为了简化分析，假设A[i-1],B[j-1],A[i],B[j]总是存在。对于i=0;i=m,j=0,j=n这样的临界条件 ，我们只需要规定A[-1]=B[-1]=INT_MIN，这样就不会对前一部分最大值的取值有影响。A[m]=B[n]=INT_MAX， 这样就不会对后一部分取最小值有影响


综上，我们有了以下条件：
    在[0,m]上枚举i，使得：
        B[j-1]<=A[i] 且A[i-1]<=B[j]， 其中j=(m+n+1)/2-i
接下来需要证明的是：
    上述的枚举其实是有极值的，会有一个最大的i，使得:
        A[i-1]<=B[j]， 其中j=(m+n+1)/2-i
证明：
    1. 当i从0到m递增式，A[i-1]递增， j递减，B[j]递减，所以一定存在一个拐点，最大的i满足A[i-1]<=B[j]，超过了这个i就会造成A[i-1]>B[j]
    2. 如果i是最大的，则i+1肯定不满足上述条件。即有A[i+1-1]>B[j]>B[j-1]， 化简是B[j-1]<A[i]。可以想象一下，越过了这个极值i，其实就达到了j的极值，一种正反面。

所以让i在[0,m]区间上进行二分搜索，找到最大的满足A[i-1]<=B[j]的i值，就得到了划分方法。此时，划分前一部分元素中的最大值，以及划分后一部分元素中的最小值，才可能作为这两个数组的中位数

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
            // s-1-(k-l-1) = s+l-k = l-1-(k-s-1)
            // cout << "hehh" << endl;/
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


    /*
    当是两个整数时，需要调用两边第k小的函数，每次复杂度是log(min{n,m})
    总复杂度是2*log(min{n,m})
    */
    double findMedianSortedArrays1(vector<int>& nums1, vector<int>& nums2) {
        int len1 = nums1.size();
        int len2 = nums2.size();
        if (len1 == 0 && len2 == 0) {
            return 0;
        }
        if (len1 == 0 && len2 != 0) {
            if (len2 % 2  == 0) {
                return (nums2[len2/2-1] + nums2[len2/2])/2.0;
            } else {
                return (double)nums2[len2/2];
            }
        }
        if (len1 != 0 && len2 == 0) {
            if (len1 % 2  == 0) {
                return (nums1[len1/2-1] + nums1[len1/2])/2.0;
            } else {
                return (double)nums1[len1/2];
            }
        }

        int all_len = len1+len2;
        if (all_len %2 == 0) {
            int left = get_kth_num(nums1, nums2, all_len/2);
            int right = get_kth_num(nums1, nums2, all_len/2+1);
            cout << "left:" << left << " right:" << right << " all_len:" << all_len << endl;

            return (left+right)/2.0;
        } else {
            return get_kth_num(nums1, nums2, all_len/2+1);
        }

    }

    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int>& long_nums = nums1.size() >= nums2.size() ? nums1 : nums2;
        vector<int>& short_nums = nums1.size() < nums2.size() ? nums1 : nums2;
        int m = short_nums.size();
        int n = long_nums.size();
        cout << "m:" << m << " n:" << n << endl;
        if (m == 0 && n == 0) {
            return 0;
        }
        if (m == 0 && n != 0) {
            if (n % 2  == 0) {
                return (long_nums[n/2-1] + long_nums[n/2])/2.0;
            } else {
                return (double)long_nums[n/2];
            }
        }
        if (m != 0 && n == 0) {
            if (m % 2  == 0) {
                return (short_nums[m/2-1] + short_nums[m/2])/2.0;
            } else {
                return (double)short_nums[m/2];
            }
        }

        int all_len = m+n;

        int l = 0, r = m; //在0...m上枚举

        //前一部分的最大值，和后一部分的最小值
        int median1 = 0, median2 = 0;

        while (l <= r) {
            //计算i和j的划分位置
            int i = (l+r)/2;
            int j = (all_len + 1) / 2 - i; 

            //计算A[i-1], A[i], B[j-1], B[j]
            int a_i_1 = i == 0 ? INT_MIN : short_nums[i-1];
            int a_i = i == m ? INT_MAX : short_nums[i];
            int b_j_1 = j == 0 ? INT_MIN : long_nums[j-1];
            int b_j = j == n ? INT_MAX : long_nums[j];
            // 满足：A[i-1]<=B[j]， 其中j=(m+n+1)/2-i 条件时，结算一次答案
            if (a_i_1 <= b_j) {
                median1 = max(a_i_1, b_j_1); //左边部分最大的
                median2 = min(a_i, b_j); //右边部分最小的
                l = i + 1; //往右再看还有没有答案
            } else { //如果不满足条件，说明i取的太大了，往左边找
                r = i - 1;
            }
        }
        if (all_len % 2 == 0) {
            return 0.5*(median1+median2);
        } else {
            return median1;
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



    // vector<int> A,B;
    // int test_time = 1000;
    // int len = 0;
    // int k= 0;
    // cout << "测试开始" << endl;

    // int ans0 = 0;
    // int ans1 = 0;
    // int ans2 = 0;
    // for (int i=0;i<test_time;i++) {
    //     len = (int)((std::rand() % 5) + 1);
    //     A.clear();
    //     B.clear();
    //     A.resize(len);
    //     B.resize(len);
    //     for (int j=0;j<len;j++) {
    //         A[j] = (int)((std::rand() % 10) + 1);
    //     }
    //     for (int j=0;j<len;j++) {
    //         B[j] = (int)((std::rand() % 10) + 1);
    //     }
    //     sort(A.begin(), A.end());
    //     sort(B.begin(), B.end());
    //     vector<int> arr1_A = A;
    //     vector<int> arr1_B = B;
    //     vector<int> arr2_A = A;
    //     vector<int> arr2_B = B;
    //     k = (int)((std::rand() % len)+1);
    //     ans0 = sol.get_kth_num(arr1_A, arr1_B, k);
    //     ans1 = sol.get_kth_num_all(arr2_A, arr2_B, k);
    //     if (ans0!=ans1) {
    //         cout << "=========" << k<< "===="  << endl;
    //         for (auto iter:A) {
    //             cout << iter << " ";
    //         }
    //         cout << endl;
    //         for (auto iter:B) {
    //             cout << iter << " ";
    //         }
    //         cout << endl;
    //         cout << " ans0:" << ans0 << " ans1:" << ans1 << endl;
    //     }
    // }
    // cout << "测试结束" << endl;

    // 获得两个数组中的上中位数
    // vector<int> A = {1,5,8,10};
    // vector<int> B = {2,3,11,20};
    // 1,2,3, 5,8, 10,11,20

    // vector<int> A = {1, 5, 8, 10, 11};
    // vector<int> B = {2, 3, 11, 13, 20};

    // cout << sol.get_up_median(A, 0, A.size() - 1, B, 0, B.size() - 1) << endl;
    vector<int> A = {2};
    vector<int> B = {};


    // cout << sol.get_kth_num(A, B, 2) << endl;
    // cout << sol.get_kth_num_all(A,B,2) << endl;
    cout << sol.findMedianSortedArrays(A, B) << endl;
    

    return 0;
}