/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
<<<<<<< HEAD
 * @LastEditTime: 2024-06-03 13:43:10
=======
 * @LastEditTime: 2024-06-03 14:21:38
>>>>>>> 后缀数组2
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
1062题dp就可以过：https://leetcode.cn/problems/longest-repeating-substring/description/
1044题更严格一些: https://leetcode.cn/problems/longest-duplicate-substring/
给定字符串 S，找出最长重复子串的长度。如果不存在重复子串就返回 0。

 

示例 1：

输入："abcd"
输出：0
解释：没有重复子串。
示例 2：

输入："abbaba"
输出：2
解释：最长的重复子串为 "ab" 和 "ba"，每个出现 2 次。
示例 3：

输入："aabcaabdaab"
输出：3
解释：最长的重复子串为 "aab"，出现 3 次。
示例 4：

输入："aaaaa"
输出：4
解释：最长的重复子串为 "aaaa"，出现 2 次。
 

提示：

字符串 S 仅包含从 'a' 到 'z' 的小写英文字母。
1 <= S.length <= 1500

*/

/*
dp方法：O(N^2)
一个样本做行，一个样本做列。两个相同的字符串
dp[i][j]表示必须以s1[i]结尾，且以s2[j]结尾情况下的最长公共子串长度是多长。


当s1[i]==s2[j]的时候，依赖dp[i-1][j-1]，即往左还能扩到dp[i-1][j-1]个字符
当s1[i]!=s2[j]的时候，dp[i][j]=0

*/



//C++优化版本
class DC3 {
public:
	vector<int> sa;

	vector<int> rank;
    vector<int> height;

	//nums中不能有0这个值，如果有0，则每个数都加上1
	DC3(vector<int>& nums, int max) {
        int n = nums.size();
        sa.resize(n, 0);
        height.resize(n, 0);
        rank.resize(n, 0);
        get_sa(nums, max);
		get_rank();
        get_height(nums);
	}

    ~DC3() {
        sa.clear();
        height.clear();
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

    void get_height(vector<int>& nums) {
        int n = nums.size();
        for (int i = 0, k = 0; i < n; ++i) {
            if (rank[i] != 0) {//lcp[0] = 0，排名第0的字符串前面数字了，所以是0，不处理
                if (k > 0) {
                    --k;
                }
                int j = sa[rank[i] - 1]; //排在开头后缀串的前一名所在开头位置
                //以i和j开头，从i+k和j+k开始，往后遍历，有多少相等的字符出
                //这里的贪心不懂，rank中前一个后缀比较得来的结果，为什么可以不回退？
                /*
                一个直观的解释（公式太复杂，暂时看不懂）: 以str="banana"为例子
                sa数组：[5,3,1,0,4,2]
                rank数组: [3,2,5,1,4,0]
                从头开始往后遍历rank数组，也就是从头到尾遍历str
                    0. 遍历到i=0位置，k=0, 需要比较的后缀串是banana和anana，从k=0开始比较，得到lcp[rank[0]] = lcp[3] = 0
                    1. 首先遍历到i=1, k=0, 从i+0和j+0开始检查。(j是排名在i前面一个名词次的后缀串开头)
                        两个后缀串分别是anana和ana
                        往后一直比较，k会一直累加到3
                        得到lcp[rank[1]] = lcp[2] = 3
                    2. 遍历到了i=2时，需要比较的两个字符串是nana和na
                        可以注意到前面anana和ana重复前缀是3，将产生a,an,ana三个子串在i-1=1开头的后缀串中是重复的。
                        往后比那里一个i位置时，去掉a后, 后面的字符串至少有n和na是重复的。所以i开头的后缀串和它的前一名比较，重复前缀长度至少是2(na)
                        所以k的位置从上个值减一开始比较，得到lcp[rank[2]] = lcp[5] = 2, 此时k=2
                    3. 当i=3时，ana和a，根据前面nana求出来的lcp[2]=2, 去掉一个n，从k=2-1=1开始比较，得到lcp[rank[3]]=lcp[1]=1, 此时k=1
                    4. 当i=4时，na和banana， 从k=k-1=0开始比较，得到lcp[rank[4]] = lcp[4] = 0, 此时k=0;
                    5. 当i=5时，a排名第0名，所以直接设置：lcp[rank[5]]=lcp[0]=0
                    6. lcp数组汇总为[0,1,3,0,0,2]
                注：虽然先遍历到了0，但整体其实是都能填上的，复杂度可以认为是从字符串的头开始遍历。O(N)复杂度
                */
                while (i + k < n && j + k < n && nums[i + k] == nums[j + k]) {
                    ++k;
                }
                height[rank[i]] = k;
            }
        }
    }
};

class Solution {
public:
    void print_arr(vector<int>& nums, string suff) {
        cout << suff;
        for (auto it: nums) {
            cout << it << " ";
        }
        cout << endl;
    }
    int longestRepeatingSubstring(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        vector<int> nums(len, 0);
        for (int i=0;i<len;i++) {
            nums[i] = s[i] - 'a' + 1;
        }
        DC3 dc3(nums, 256);
        print_arr(nums, "nums:");
        print_arr(dc3.sa, "sa:");
        print_arr(dc3.rank, "rank:");
        print_arr(dc3.height, "height:");
        int ans = 0;
        for (int i=0;i<len;i++) {
            ans = max(ans, dc3.height[dc3.rank[i]]);
        }
        return ans;
    }

    //1044
    string longestDupSubstring(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        vector<int> nums(len, 0);
        for (int i=0;i<len;i++) {
            nums[i] = s[i] - 'a' + 1;
        }
        DC3 dc3(nums, 256);
        // print_arr(nums, "nums:");
        // print_arr(dc3.sa, "sa:");
        // print_arr(dc3.rank, "rank:");
        // print_arr(dc3.height, "height:");
        int max_len = 0;
        string ans = "";
        
        for (int i=0;i<len;i++) {
            if (dc3.height[dc3.rank[i]] > max_len) {
                max_len = dc3.height[dc3.rank[i]];
                ans = s.substr(i, max_len);
            }
        }
        return ans;
    }
};

int main() {
    Solution sol;
    string s = "banana";
    /*
    字符串：aaaaa

        a 4
       aa 3
      aaa 2
     aaaa 1
    aaaaa 0
    sa   [4,3,2,1,0]
    rank [4,3,2,1,0]
    height [0,1,2,3,4]
    */
   cout << sol.longestRepeatingSubstring(s) << endl;
    cout << sol.longestDupSubstring(s) << endl;

    return 0;
}