
/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-31 17:28:20
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

void print_arr(string name, int* arr, int n) {
    cout << name;
    for (int i=0;i<n;i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

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
    // count occurrences
    // 统计每个数出现的频次
    int* c  = new int[K+1];
    for (int i=0;i<=K;i++) {
        c[i] = 0; // 初始化各个桶
    }
    // count occurrences
    //a[i]是对应下标的位置
    for (int i=0;i<n;i++) {
        c[r[a[i]]]++;
    }

    cout << "n:" << n << " K:" << K << endl;
    print_arr("radixPass after add c:", c, K+1);

    //exclusive prefix sums
    /*
    做累加和统计
    n=7
    比如a=[1,2,4,5,7,8,10,0,0,0]
    初始时传进来的是s+2即：r="ssissippi00"
    初始时统计s12类下标出现的个数是: c[1(0),0,0,0,0....3(i),0,0..,1(p),..,0,2(s),...]
    然后对c做累加和变成: c[0,1,1...,1(i),4,4,4,4,4(p),5,5,...5(s),7,7,7,...]
    c['i']就代表第一个i的排名，每次取出来一个i时，c['i']++， 再取时就是下一个i的排名了。有一些冗余传递。
    */
    for (int i=0,sum=0;i<=K;i++) {
        int t= c[i];
        c[i] = sum;
        sum+=t;
    }
    print_arr("radixPass after sum c:", c, K+1);


    for (int i=0;i<n;i++) {
        cout << "i:" << i << " c[r[a[i]]]:" << c[r[a[i]]] << " r[a[i]]:" << r[a[i]] << " a[i]:" << a[i] << endl;
        int idx = c[r[a[i]]]++;
        b[idx] = a[i]; //sort 排序
    }

    delete[] c;
}

/*
原文注释：
//find the suffix array SA of s[0..n-1] in {1..K}ˆn
// require s[n]=s[n+1]=s[n+2]=0, n>=2

构造数组s的后缀数组SA，s中元素的大小范围是{1...K}， 要去n大于2个数，并且，在s数组的后面跟两个0，用于求解s12时构造(s[n],s[n+1],s[n+2])，然后面跟着的字典序尽可能的小
*/
void suffixArray(int* s, int* SA, int n, int K) {
    cout << "new suffixArray n:" << n << " K:" << K << endl;
    print_arr("new suffixArray s:", s, n);
    int n0 = (n+2)/3; //假设有7个数，0,1,2,3,4,5,6 共(7+2)/3个s0类下标，是3元组的第一个，为了让最后一个3元祖的第一个数出来，需要加2补齐成3元组
    int n1 = (n+1)/3; //假设有8个数，0,1,2,3,4,5,6,7，共(8+1)/3=3个s1类下标，是3元祖的第二个，为了让最后一个3元祖的第二个数出来，需要加1补齐成3元组
    int n2 = n/3; //因为是三元组中的第3个数，所以直接除3
    int n02 = n0+n2; //因为n0>=n1，所以下面的s12的长度以n0+n2的长度为基准
    cout << "n0:" << n0 << " n1:" << n1 << " n2:" << n2 << endl;
    //s12的长度是n1+n2 = (2n+1)/3 = 2n/3 + 1/3
    //n0+n2 = (2n+3)/3 = 2n/3 + 1
    //n1+n2的向上取整就是n0+n2

    int * s12 = new int[n02+3];//后面需要补两个0
    s12[n02] = s12[n02+1] = s12[n02+2] = 0;

    //SA12[i]=j表示排第i名的后缀串，开始位置下标是j
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
    for (int i=0, j=0; i < n+(n0-n1); i++) {
        if (i % 3 != 0) {
            s12[j++] = i;
        }
    }
    print_arr("s12:", s12, n02+3);

    //lsd： 最低位优先级排序法。 从低位到高位排序
    // lsb radix sort the mod 1 and mod 2 triples
    //对sa12做基数排序
    radixPass(s12 , SA12, s+2, n02, K);
    print_arr("1st s12:", s12, n02+3);
    print_arr("1st SA12:", SA12, n02 + 3);
    radixPass(SA12, s12 , s+1, n02, K);
    print_arr("2st s12:", s12, n02 + 3);
    print_arr("2st SA12:", SA12, n02 + 3);
    radixPass(s12 , SA12, s , n02, K);
    print_arr("3st s12:", s12, n02 + 3);
    print_arr("4st SA12:", SA12, n02 + 3);
    //三次排序之后，就排出来了三元组字符串的排名，放在了SA12中。SA12[i]=j代表排名第i名的开头位置是在哪里。
    //第一次迭代mississippi的SA12初始排序是[10,7,1,4,8,2,5,0,0,0]
    //小写s12可以认为是中间数组

    //find lexicographic names of triples
    int name=0, c0=-1, c1=-1, c2=-1;
    for (int i=0; i < n02; i++){
        //每往后移动一次，都会看s12的三元组是否相等，如果不相等则让name的下标往后挪动，即排名往后变化
        if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) {
            name++;
            c0 = s[SA12[i]];
            c1 = s[SA12[i]+1];
            c2 = s[SA12[i]+2];
        }
        
        if (SA12[i] % 3 == 1) {
            s12[SA12[i]/3] = name; //左半边
        } else {
            s12[SA12[i]/3 + n0] = name; //右半边s1长度认为是n0
        }
    }

    //recurse if names are not yet unique
    if (name < n02) {
        // 递归调用, 此时的s12就是排名组成的新的

        print_arr("paiming digui:", s12, n02);
        suffixArray(s12, SA12, n02, name); 
        // store unique names in s12 using the suffix array
        // 通过递归生成的排名更新s12数组，初次迭代为：[3,3,2,1,5,5,4]
        // 第二次递归，不会再进来这个if了
        for (int i=0; i < n02; i++) {
            s12[SA12[i]] = i + 1;
        }
    } else { 
        // generate the suffix array of s12 directly
        //name到了结尾，直接生成SA12后缀数组
        for (int i=0; i < n02; i++) {
            SA12[s12[i]-1] = i;
        }
    }
    //首次到达这里是第二次递归，此时的s是[3,3,2,1,5,5,4],SA12是[7,2,1,5,4]
    //
    print_arr("SA12 end:", SA12, n02);

    // stably sort the mod 0 suffixes from SA12 by their first character
    // 根据SA12对s0排序
    for (int i=0, j=0; i < n02; i++) {
        if (SA12[i] < n0) {
            s0[j++] = 3*SA12[i];
        }
    }
    print_arr("s0 begin:", s0, n0);
    radixPass(s0, SA0, s, n0, K);
    print_arr("SA0 end:", SA0, n0);
    print_arr("SA12 end:", SA12, n02); //此时这里的SA12的每个元素是减一乘3了的，后面用的时候要乘3加1

    // merge sorted SA0 suffixes and sorted SA12 suffixes
    for (int p=0, t=n0-n1, k=0; k < n; k++) {
        #define GetI() (SA12[t] < n0 ? SA12[t]*3+1 : (SA12[t] - n0) * 3 + 2)
        int i = GetI(); // pos of current offset 12 suffix
        int j = SA0[p]; // pos of current offset 0 suffix
        if (SA12[t] < n0 ? // different compares for mod 1 and mod 2 suffixes
                leq(s[i], s12[SA12[t] + n0], s[j], s12[j/3]) : 
                leq(s[i],s[i+1],s12[SA12[t]-n0+1], s[j],s[j+1],s12[j/3+n0])) { // suffix from SA12 is smaller
            SA[k] = i; 
            t++;
            if (t == n02) {// done --- only SA0 suffixes left 
                for (k++; p < n0; p++, k++) {
                     SA[k] = SA0[p];
                }
            }
        } else { // suffix from SA0 is smaller
            SA[k] = j;
            p++;
            if (p == n0) { // done --- only SA12 suffixes left
                for (k++; t < n02; t++, k++) {
                    SA[k] = GetI();
                }
            }
        }
    }
    print_arr("SA end:", SA, n);
    delete[] s12;
    delete[] SA12;
    delete[] SA0;
    delete[] s0;
}


//封装成DC3类
class DC3 {

public:
    int * sa;
    int * rank;
    int K;
    DC3(int* s, int len, int max) {
        sa = new int[len];
        rank = new int[len];
        K = max;
        cout << "asdfasdf";
        suffixArray(s, sa, len, K);
        print_arr("sa:", sa, len);
        build_rank(rank, sa, len);
    }
    ~DC3() {
        delete[] sa;
        delete[] rank;
    }

    void build_rank(int* rank, int* sa, int len) {
        //rank[i]=j表示下标为0的后缀串排名第几
        for (int i=0;i<len;i++) {
            rank[sa[i]] = i; 
        }
        print_arr("rank:", rank, len);
    }
    void print_arr(string name, int* arr, int n) {
        cout << name;
        for (int i=0;i<n;i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

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
    void radixPass(int* a, int* b, int* r, int n, int K) {
        // count occurrences
        // 统计每个数出现的频次
        int* c  = new int[K+1];
        for (int i=0;i<=K;i++) {
            c[i] = 0; // 初始化各个桶
        }
        // count occurrences
        //a[i]是对应下标的位置
        for (int i=0;i<n;i++) {
            c[r[a[i]]]++;
        }

        cout << "n:" << n << " K:" << K << endl;
        print_arr("radixPass after add c:", c, K+1);

        //exclusive prefix sums
        /*
        做累加和统计
        n=7
        比如a=[1,2,4,5,7,8,10,0,0,0]
        初始时传进来的是s+2即：r="ssissippi00"
        初始时统计s12类下标出现的个数是: c[1(0),0,0,0,0....3(i),0,0..,1(p),..,0,2(s),...]
        然后对c做累加和变成: c[0,1,1...,1(i),4,4,4,4,4(p),5,5,...5(s),7,7,7,...]
        c['i']就代表第一个i的排名，每次取出来一个i时，c['i']++， 再取时就是下一个i的排名了。有一些冗余传递。
        */
        for (int i=0,sum=0;i<=K;i++) {
            int t= c[i];
            c[i] = sum;
            sum+=t;
        }
        print_arr("radixPass after sum c:", c, K+1);


        for (int i=0;i<n;i++) {
            cout << "i:" << i << " c[r[a[i]]]:" << c[r[a[i]]] << " r[a[i]]:" << r[a[i]] << " a[i]:" << a[i] << endl;
            int idx = c[r[a[i]]]++;
            b[idx] = a[i]; //sort 排序
        }

        delete[] c;
    }

    /*
    原文注释：
    //find the suffix array SA of s[0..n-1] in {1..K}ˆn
    // require s[n]=s[n+1]=s[n+2]=0, n>=2

    构造数组s的后缀数组SA，s中元素的大小范围是{1...K}， 要去n大于2个数，并且，在s数组的后面跟两个0，用于求解s12时构造(s[n],s[n+1],s[n+2])，然后面跟着的字典序尽可能的小
    */
    void suffixArray(int* s, int* SA, int n, int K) {
        cout << "new suffixArray n:" << n << " K:" << K << endl;
        print_arr("new suffixArray s:", s, n);
        int n0 = (n+2)/3; //假设有7个数，0,1,2,3,4,5,6 共(7+2)/3个s0类下标，是3元组的第一个，为了让最后一个3元祖的第一个数出来，需要加2补齐成3元组
        int n1 = (n+1)/3; //假设有8个数，0,1,2,3,4,5,6,7，共(8+1)/3=3个s1类下标，是3元祖的第二个，为了让最后一个3元祖的第二个数出来，需要加1补齐成3元组
        int n2 = n/3; //因为是三元组中的第3个数，所以直接除3
        int n02 = n0+n2; //因为n0>=n1，所以下面的s12的长度以n0+n2的长度为基准
        cout << "n0:" << n0 << " n1:" << n1 << " n2:" << n2 << endl;
        //s12的长度是n1+n2 = (2n+1)/3 = 2n/3 + 1/3
        //n0+n2 = (2n+3)/3 = 2n/3 + 1
        //n1+n2的向上取整就是n0+n2

        int * s12 = new int[n02+3];//后面需要补两个0
        s12[n02] = s12[n02+1] = s12[n02+2] = 0;

        //SA12[i]=j表示排第i名的后缀串，开始位置下标是j
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
        for (int i=0, j=0; i < n+(n0-n1); i++) {
            if (i % 3 != 0) {
                s12[j++] = i;
            }
        }
        print_arr("s12:", s12, n02+3);

        //lsd： 最低位优先级排序法。 从低位到高位排序
        // lsb radix sort the mod 1 and mod 2 triples
        //对sa12做基数排序
        radixPass(s12 , SA12, s+2, n02, K);
        print_arr("1st s12:", s12, n02+3);
        print_arr("1st SA12:", SA12, n02 + 3);
        radixPass(SA12, s12 , s+1, n02, K);
        print_arr("2st s12:", s12, n02 + 3);
        print_arr("2st SA12:", SA12, n02 + 3);
        radixPass(s12 , SA12, s , n02, K);
        print_arr("3st s12:", s12, n02 + 3);
        print_arr("4st SA12:", SA12, n02 + 3);
        //三次排序之后，就排出来了三元组字符串的排名，放在了SA12中。SA12[i]=j代表排名第i名的开头位置是在哪里。
        //第一次迭代mississippi的SA12初始排序是[10,7,1,4,8,2,5,0,0,0]
        //小写s12可以认为是中间数组

        //find lexicographic names of triples
        int name=0, c0=-1, c1=-1, c2=-1;
        for (int i=0; i < n02; i++){
            //每往后移动一次，都会看s12的三元组是否相等，如果不相等则让name的下标往后挪动，即排名往后变化
            if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) {
                name++;
                c0 = s[SA12[i]];
                c1 = s[SA12[i]+1];
                c2 = s[SA12[i]+2];
            }
            
            if (SA12[i] % 3 == 1) {
                s12[SA12[i]/3] = name; //左半边
            } else {
                s12[SA12[i]/3 + n0] = name; //右半边s1长度认为是n0
            }
        }

        //recurse if names are not yet unique
        if (name < n02) {
            // 递归调用, 此时的s12就是排名组成的新的

            print_arr("paiming digui:", s12, n02);
            suffixArray(s12, SA12, n02, name); 
            // store unique names in s12 using the suffix array
            // 通过递归生成的排名更新s12数组，初次迭代为：[3,3,2,1,5,5,4]
            // 第二次递归，不会再进来这个if了
            for (int i=0; i < n02; i++) {
                s12[SA12[i]] = i + 1;
            }
        } else { 
            // generate the suffix array of s12 directly
            //name到了结尾，直接生成SA12后缀数组
            for (int i=0; i < n02; i++) {
                SA12[s12[i]-1] = i;
            }
        }
        //首次到达这里是第二次递归，此时的s是[3,3,2,1,5,5,4],SA12是[7,2,1,5,4]
        //
        print_arr("SA12 end:", SA12, n02);

        // stably sort the mod 0 suffixes from SA12 by their first character
        // 根据SA12对s0排序
        for (int i=0, j=0; i < n02; i++) {
            if (SA12[i] < n0) {
                s0[j++] = 3*SA12[i];
            }
        }
        print_arr("s0 begin:", s0, n0);
        radixPass(s0, SA0, s, n0, K);
        print_arr("SA0 end:", SA0, n0);
        print_arr("SA12 end:", SA12, n02); //此时这里的SA12的每个元素是减一乘3了的，后面用的时候要乘3加1

        // merge sorted SA0 suffixes and sorted SA12 suffixes
        for (int p=0, t=n0-n1, k=0; k < n; k++) {
            #define GetI() (SA12[t] < n0 ? SA12[t]*3+1 : (SA12[t] - n0) * 3 + 2)
            int i = GetI(); // pos of current offset 12 suffix
            int j = SA0[p]; // pos of current offset 0 suffix
            if (SA12[t] < n0 ? // different compares for mod 1 and mod 2 suffixes
                    leq(s[i], s12[SA12[t] + n0], s[j], s12[j/3]) : 
                    leq(s[i],s[i+1],s12[SA12[t]-n0+1], s[j],s[j+1],s12[j/3+n0])) { // suffix from SA12 is smaller
                SA[k] = i; 
                t++;
                if (t == n02) {// done --- only SA0 suffixes left 
                    for (k++; p < n0; p++, k++) {
                        SA[k] = SA0[p];
                    }
                }
            } else { // suffix from SA0 is smaller
                SA[k] = j;
                p++;
                if (p == n0) { // done --- only SA12 suffixes left
                    for (k++; t < n02; t++, k++) {
                        SA[k] = GetI();
                    }
                }
            }
        }
        print_arr("SA end:", SA, n);
        delete[] s12;
        delete[] SA12;
        delete[] SA0;
        delete[] s0;
    }
};

int main() {
    string str = "mississippi";
    /*
    10 i: 0
    9  pi: 5
    8  ppi : 6
    7  ippi: 1
    6  sippi : 7
    5  ssippi : 9
    4  issippi : 2
    3  sissippi : 8
    2  ssissippi : 10
    1  ississippi : 3
    0  mississippi : 4
    SA: [10 7 4 1 0 9 8 6 3 5 2]
    */
    int len = str.length();
    int *s = new int[len];
    int *SA = new int[len];
    for (int i = 0; i < len; ++i) {
        s[i] = str[i];
    }
    int n = len;
    int K = 256;
    suffixArray(s, SA, n, K);
    for (int i=0;i<len; i++) {
        cout << SA[i] << " ";
    }
    cout << endl;
    delete[] s;
    delete[] SA;
    return 0;
}