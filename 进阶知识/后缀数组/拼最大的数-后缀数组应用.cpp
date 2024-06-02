
/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-02 11:14:13
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

 https://leetcode.com/problems/create-maximum-number/
给你两个整数数组 nums1 和 nums2，它们的长度分别为 m 和 n。数组 nums1 和 nums2 分别代表两个数各位上的数字。同时你也会得到一个整数 k。

请你利用这两个数组中的数字中创建一个长度为 k <= m + n 的最大数，在这个必须保留来自同一数组的数字的相对顺序。

返回代表答案的长度为 k 的数组。

 

示例 1：

输入：nums1 = [3,4,6,5], nums2 = [9,1,2,5,8,3], k = 5
输出：[9,8,6,5,3]
示例 2：

输入：nums1 = [6,7], nums2 = [6,0,4], k = 5
输出：[6,7,6,0,4]
示例 3：

输入：nums1 = [3,9], nums2 = [8,9], k = 3
输出：[9,8,9]
 

提示：

m == nums1.length
n == nums2.length
1 <= m, n <= 500
0 <= nums1[i], nums2[i] <= 9
1 <= k <= m + n

*/

/*
方法一：单调数组方法：
在num1中拿i个数字， 在nums2中拿k-i个数字。
在nums中拿i个数字，使用一个单调栈，遍历一遍，在单调栈中从底到顶是递减的序列，最后保证栈中保留i个数字就可以
每次这样找到数组复杂度是O(m+n)

两个数组挑选出来后，做merge
nums1_i和nums2_k_i，双指针i和j分别指向两个数组的开始。然后每次到达一个新的位置时，从i和j开始向后比较，直到分出胜负，将字典序大的放在前面。向后移动。
最坏的情况是[9,9,9,9]和[9,9,9]，每次都需要遍历到数组结尾才能知道谁的字典序比较大，每次i和j到达新的位置，最坏遍历m+n次。复杂度O((N+M)^2)

枚举k次求结果，总复杂度O(k*(m+n+(n+m)^2))， 简化是O(k*(m+n)^2), k最大取到m+n，所以是O((m+n)^3)


方法二：dp+merge加速
首先第一个方法不再使用单调栈，而是先准备一个dp， dp[i][j]表示[i...]范围上，拿出j个数，最大数列的开头位置
   1. dp[i][len-i]，表示从i开始，往后取len-i个数，只有一种取法(因为i往后就只有len-i个数了), 就是i往后所有，开头位置是i
   2. dp[i][j]， st. j<len-i， 有两种情况，必须取i，依赖dp[i+1][j-1](i+1上取j-1个数)；必须不取i，依赖dp[i+1][j](i+1往后取j个数)
        取两个可能背后的arr对应元素，最大的；当相等时，取前面的，因为开头位置选的靠前可以有笔后面更多的可能性，方便后面取最大的。
每次在nums数组中取i个数组最大的数时，可以使用dp方便拿到。
比如要在arr中拿3个数，即从0开始往后范围，拿3个数：
    首先找到第一个数的位置dp[0][3]=2， 将arr[2]作为第一个数
    接下来需要在3...n范围上拿剩余的2个数，找到dp[3][2]=4， 则将arr[4]作为第二个数
    接下来在4...n范围上拿剩余的1个数，找到dp[4][1]=8, 则将arr[8]作为第三个数。
    可以在O(K)复杂度拿到，但dp数组是O(n*k)和O(m*k)的。

    同样使用方法一中的merge O((n+m)^2)

    总复杂度是O(m*k)+O(n*k)+O(k*(k+(n+m)^2)) 化简保留高阶是O(k*(n+m)^2)，k最大是m+n，复杂度还是O((m+n)^3)


方法三：后缀数组方法优化merge
第一步从nums中拿i个数的最大数列，还是使用方法二中的dp方法，复杂度O(k)，dp生成O(n*k)+O(m*k)
第二步，将两个数列拼接到一起，中间使用0隔断，得到后缀数组。比如:
    [3339] 下标为[0,1,2,3]
    [331] 下标为[0',1',2']
    排名一样，则右边的靠前，因为更短
    [33390331]的后缀数组的rank数组是：
    [45670321]
    两个数组继续merge，排名越大越在前面，所以整体排名是：[3339331] 对应到原数组是[0,1,2,3,0',1',2']

    生成后缀数组DC3是O(N), merge时每次可以在O(1)时间知道以i和j开头时的后缀数组排名，排名越大越排在前面，有O(n+m)复杂度。
    外层枚举k次，所以整体复杂度是O(n*k)+O(m*k)+O(k*(k + (n+m) + (m+n)*1)) = O(n*k+m*k+k^2+2*k*m+2*k*n)=O(3k*(m+n)+k^2)
    k最多取到M+N， 所以复杂度可以到达O((m+n)^2)

*/

/*
摘自《Simple Linear Work Suffix Array Construction》论文
*/


class DC3 {
public:
	vector<int> sa;

	vector<int> rank;

	//nums中不能有0这个值，如果有0，则每个数都加上1
	DC3(vector<int>& nums, int max) {
        int n = nums.size();
        sa.resize(n, 0);
        rank.resize(n, 0);
        get_sa(nums, max);
		get_rank();
	}

    ~DC3() {
        sa.clear();
        rank.clear();
    }

	void get_sa(vector<int>& nums, int max) {
		int n = nums.size();
		vector<int> arr(n+3, 0); //后面补3个数
		for (int i = 0; i < n; i++) {
			arr[i] = nums[i];
		}
		skew(arr, n, max, sa);
	}

	//这里的K是基数排序桶编号最大的值
	//下面的代码其实就是源自论文中的c代码
	void skew(vector<int>& nums, int n, int K, vector<int>& sa) {
		int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
		vector<int> s12(n02+3, 0);
		vector<int> sa12(n02+3, 0);
		for (int i = 0, j = 0; i < n + (n0 - n1); ++i) {
			if (0 != i % 3) {
				s12[j++] = i;
			}
		}
		radixPass(nums, s12, sa12, 2, n02, K);
		radixPass(nums, sa12, s12, 1, n02, K);
		radixPass(nums, s12, sa12, 0, n02, K);
		int name = 0, c0 = -1, c1 = -1, c2 = -1;
		for (int i = 0; i < n02; ++i) {
			if (c0 != nums[sa12[i]] || c1 != nums[sa12[i] + 1] || c2 != nums[sa12[i] + 2]) {
				name++;
				c0 = nums[sa12[i]];
				c1 = nums[sa12[i] + 1];
				c2 = nums[sa12[i] + 2];
			}
			if (1 == sa12[i] % 3) {
				s12[sa12[i] / 3] = name;
			} else {
				s12[sa12[i] / 3 + n0] = name;
			}
		}
		if (name < n02) {
			skew(s12, n02, name, sa12);
			for (int i = 0; i < n02; i++) {
				s12[sa12[i]] = i + 1;
			}
		} else {
			for (int i = 0; i < n02; i++) {
				sa12[s12[i] - 1] = i;
			}
		}
		vector<int> s0(n0, 0);
		vector<int> sa0(n0, 0);
		for (int i = 0, j = 0; i < n02; i++) {
			if (sa12[i] < n0) {
				s0[j++] = 3 * sa12[i];
			}
		}
		radixPass(nums, s0, sa0, 0, n0, K);
		for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
			int i = sa12[t] < n0 ? sa12[t] * 3 + 1 : (sa12[t] - n0) * 3 + 2;
			int j = sa0[p];
			if (sa12[t] < n0 ? leq(nums[i], s12[sa12[t] + n0], nums[j], s12[j / 3])
					: leq(nums[i], nums[i + 1], s12[sa12[t] - n0 + 1], nums[j], nums[j + 1], s12[j / 3 + n0])) {
				sa[k] = i;
				t++;
				if (t == n02) {
					for (k++; p < n0; p++, k++) {
						sa[k] = sa0[p];
					}
				}
			} else {
				sa[k] = j;
				p++;
				if (p == n0) {
					for (k++; t < n02; t++, k++) {
						sa[k] = sa12[t] < n0 ? sa12[t] * 3 + 1 : (sa12[t] - n0) * 3 + 2;
					}
				}
			}
		}
	}

	void radixPass(vector<int>& nums, vector<int>& input, 
			vector<int>& output, int offset, int n, int k) {
		vector<int> cnt(k+1, 0);
		for (int i = 0; i < n; ++i) {
			cnt[nums[input[i] + offset]]++;
		}
		for (int i = 0, sum = 0; i < cnt.size(); ++i) {
			int t = cnt[i];
			cnt[i] = sum;
			sum += t;
		}
		for (int i = 0; i < n; ++i) {
			output[cnt[nums[input[i] + offset]]++] = input[i];
		}
	}

	bool leq(int a1, int a2, int b1, int b2) {
		return a1 < b1 || (a1 == b1 && a2 <= b2);
	}

	bool leq(int a1, int a2, int a3, int b1, int b2, int b3) {
		return a1 < b1 || (a1 == b1 && leq(a2, a3, b2, b3));
	}

	void get_rank() {
		int n = sa.size();
		for (int i = 0; i < n; i++) {
			rank[sa[i]] = i;
		}
	}
};

class Solution {
public:

    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {

        int n = nums1.size();
        int m = nums2.size();
        if (n+m < k) {
            return vector<int>();
        }

        //dp[i][j]表示i...len-1范围上能组成的后缀串中，字典序最大的串的开头位置
        vector<vector<int>> dp1(n, vector<int>(max(n,k)+1, 0));//最多拿n个, 如果k比较大的话，多准备一些
        vector<vector<int>> dp2(m, vector<int>(max(m,k)+1, 0));
        // cout << "n:" << n << " m:" << m << " max(n,k):" << max(n,k) << " max(m,k):" << max(m,k) << endl;
        get_dp(nums1, dp1, k);
        get_dp(nums2, dp2, k);
        // print_arr(dp1, "dp1:");
        // print_arr(dp2, "dp2:");
        // return vector<int>();


        //枚举第一个数组中取的数字, 因为m可能比较小，所以可能取不到0，所以取0和k-m的最大值
        // 最多取k个数
        vector<int> res1,res2,res,res_tmp;

        for (int i=max(0, k-m);i<=min(n,k);i++) {
            //从nums1中取i个数，从nums2中取k-i个数
            res1.clear();
            res2.clear();
            get_nums(nums1, dp1, i, res1);
            get_nums(nums2, dp2, k-i, res2);
            // cout << "i:" << endl;
            // print_arr(res1, "res1:");
            // print_arr(res2, "res2:");

            //merge两个数组得到一个新的结果
            merge_with_suffix_array(res1, res2, res_tmp);
            // print_arr(res_tmp, "res_tmp:");
            //新和成的要比res大的话就更新res
            if (more_than(res_tmp, res)) {
                res = res_tmp;
            }
        }
        return res;
    }

    bool more_than(vector<int>& new_res, vector<int> &res) {
        int len1 = new_res.size();
        int len2 = res.size();
        // print_arr(res, "cur_res:");
        // print_arr(new_res, "new_res:");
        if (len1 != len2) {
            return len1 > len2;
        }

        for (int i=0;i<len1;i++) {
            if (new_res[i] != res[i]) {
                if (new_res[i] > res[i]) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        return false;

    }

    //将res1和res2的数字合并，组成字典序最大的串放在res_tmp
    void merge_with_suffix_array(vector<int> &res1, vector<int> &res2, vector<int> & res_tmp) {
        int len1 = res1.size();
        int len2 = res2.size();
        res_tmp.resize(len1+len2, 0);

        //res1和res2组成到一起，各个值加2，中间用1间隔开，求后缀数组
        int all_len = len1+len2+1;
        vector<int> nums(all_len, 0);
        for (int i=0;i<len1;i++) {
            nums[i] = res1[i]+2;
        }
        nums[len1] = 1;
        for (int i=0;i<len2;i++) {
            nums[i+len1+1] = res2[i]+2;
        }

        // cout << "all_len:" << all_len << " nums:" ;
        // for (int i=0;i<all_len;i++) {
        //     cout << nums[i] << " ";
        // }
        // cout <<endl;

        //nums最大为9，+2是11，内部用11个桶做基数排序
        DC3 dc3(nums, 11);
        //rank[i]=j，以i为开头的后缀串排名第j位
        vector<int>& rank = dc3.rank;

        // print_arr(rank, "rank:");

        int idx=0;
        int i1 = 0;
        int i2 = 0;
        while (i1<len1 && i2<len2) {
            //排名只可能是大于或小于
            //排名越大越靠前
            if (rank[i1] > rank[i2+len1+1]) { 
                res_tmp[idx++] = res1[i1++];
            } else {
                res_tmp[idx++] = res2[i2++];
            }
        }

        if (i1<len1) {
            while (i1 < len1) {
                res_tmp[idx++] = res1[i1++];
            }
        }
        
        if (i2<len2) {
            while (i2 < len2) {
                res_tmp[idx++] = res2[i2++];
            }
        }

    }

    void print_arr(vector<int>& nums, string suffix) {
        cout << suffix;
        for (auto it : nums) {
            cout << it << " ";
        }
        cout << endl;
    } 

    void print_arr(vector<vector<int>>& nums, string suffix) {
        cout << suffix << endl;
        for (auto& it : nums) {
            for (auto it2: it) {
                cout << it2 << " ";
            }
            cout << endl;
        }
        cout << endl;
    } 

    void get_dp(vector<int>& nums, vector<vector<int>>& dp, int k) {
        int len = nums.size();
        //最多拿k和len的最小值个
        int max_num = min(k, len);
        // cout << "get_dp len:" << len << " k:" << k << endl;
        // print_arr(nums, " get_dp nums:");

        //斜对角线
        //从i...n上拿len-i个，首个元素肯定是i
        for (int i=len-1;i>=0;i--) {
            int j = len - i;
            // cout << "j:" << j << " max_num:" << max_num << endl; 
            if (j<=max_num) {
                dp[i][j] = i;
            }
        }

        // print_arr(dp, "get_dp first dp:");

        for (int j=1;j<k;j++) {
            for (int i=len-j-1;i>=0;i--) {
                if (len-i < j) { //i往后的数字不够j个，不填
                    continue;
                }
                //选i时开头必然是i
                //不选i时，由dp[i+1][j]决定
                //大于或等于时都选择以i开头
                if (nums[i] >= nums[dp[i+1][j]]) {
                    dp[i][j] = i;
                } else {
                    dp[i][j] = dp[i+1][j];
                }
            }
        }
    } 

    //从nums中取k个数，字典序最大的后缀串放在res中
    void get_nums(vector<int>& nums, vector<vector<int>>& dp, int k, vector<int>& res) {
        int len = nums.size();
        int idx = 0;
        res.resize(k);
        // cout << "get_nums: k:" << k << " len:" << len << endl;
        //0..len范围取k个数的第一个数的位置
        int cur = dp[0][k];
        // cout << "get_nums cur: " << cur << endl;
        while (idx<k) {
            // cout << "get_nums cur: " << cur << " idx:" << idx << " k-idx:" << k-idx << endl;
            res[idx++] = nums[cur];
            if (cur+1 >= len) { //到结尾了不需要
                break;
            }
            cur = dp[cur+1][k-idx];
        }
        // print_arr(res, "get_nums:");
    }
};


int main() {
    
    // vector<int> nums1 = {3,4,6,5};
    // vector<int> nums2 = {9,1,2,5,8,3};
    vector<int> nums1 = {8,6,9};
    vector<int> nums2 = {1, 7, 5};
    int k = 3;
    Solution sol;
    vector<int> res = sol.maxNumber(nums1, nums2, k);
    cout << "res:";
    for (int i=0;i<res.size();i++) {
        cout << res[i] << " ";
    }
    cout << endl;

    return 0;
}