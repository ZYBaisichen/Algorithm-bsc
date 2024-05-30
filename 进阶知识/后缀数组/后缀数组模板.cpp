
/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-28 21:21:47
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
论文地址：https://www.cs.cmu.edu/~guyb/paralg/papers/KarkkainenSanders03.pdf
dc3模板代码来自《Simple Linear Work Suffix Array Construction》论文

笔记地址位置：
@(数据结构与算法)
# 左神-进阶-后缀数组
*/


inline bool leq(int a1, int a2, int b1, int b2) { //字典序比较， 两个的情况
    return (a1<b1 || a1 == b1 && a2 <= b2);
}

//比较三个数字对的大小
//(a1,a2,a3)<=(b1,b2,b3)时，返回true
inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3){ // 3个的情况
    return (a1<b1 || a1==b1 && (leq(a2,a3, b2,b3)));
}

/*
原文注释:stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
从a到b的稳定基数排序，a中的每个元素大小是0...K范围的，获取到的后缀数组放在b中
*/
static void radixPass(int* a, int* b, int* r, int n, int K) {
    
}

/*
原文注释：
//find the suffix array SA of s[0..n-1] in {1..K}ˆn
// require s[n]=s[n+1]=s[n+2]=0, n>=2

构造数组s的后缀数组SA，s中元素的大小范围是{1...K}， 要去n大于2个数，并且，在s数组的后面跟两个0，用于求解s12时构造(s[n],s[n+1],s[n+2])，然后面跟着的字典序尽可能的小
*/
void suffixArray(int* s, int* SA, int n, int k) {
    int n0 = (n+2)/3; //假设有7个数，0,1,2,3,4,5,6 共(7+2)/3个s0类下标，是3元组的第一个，为了让最后一个3元祖的第一个数出来，需要加2补齐成3元组
    int n1 = (n+1)/3; //假设有8个数，0,1,2,3,4,5,6,7，共(8+1)/3=3个s1类下标，是3元祖的第二个，为了让最后一个3元祖的第二个数出来，需要加1补齐成3元组
    int n2 = n/3; //因为是三元组中的第3个数，所以直接除3
    int n02 = n0+n2; //因为n0>=n1，所以下面的s12的长度以n0+n2的长度为基准
    //s12的长度是n1+n2 = (2n+1)/3 = 2n/3 + 1/3
    //n0+n2 = (2n+3)/3 = 2n/3 + 1
    //n1+n2的向上取整就是n0+n2

    int * s12 = new int[n02+3];//后面需要补两个0
    s12[n02] = s12[n02+1] = s12[n02+2] = 0;

    int* SA12 = new int[n02+3];
    SA12[n02] = SA12[n02+1] = SA12[n02+2] = 0;
    int* s0 = new int[n0];
    int* SA0 = new int[n0];

    /*
    // generate positions of mod 1 and mod 2 suffixes
    // the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1 //没懂这为啥要加个(n0-n1=1/3)?
    如果n%3==1，此时最后一个下标是s0下标。比如n=7时，n0=3,n1=2,n2=2 n+(n0-n1)=7+(3-2)=8， s1和s2一共4个数
    生成s12的位置数组，s12[j]=i代表s12数组的j位置对应的是i开头的后缀串
    */
    for (int i=0;j=0;i<n+(n0-n1);i++) {
        if (i % 3 != 0) {
            s12[j++] = i;
        }
    }

    


}


int main() {
    Solution sol;
    string s = "aaaaa";
    cout << sol.longestRepeatingSubstring(s) << endl;

    return 0;
}