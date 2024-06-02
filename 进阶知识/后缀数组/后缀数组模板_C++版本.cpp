/*
 * @Author: baisichen
 * @Date: 2023-12-17 23:03:16
 * @LastEditTime: 2024-06-01 23:51:12
 * @LastEditors: baisichen
 * @Description: 
 */
// һ������Ǳ�������ݽṹ����ʲô�㷨���ø�
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <map>
#include <string>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <set>
#include <functional>
#include <climits>
using namespace std;


//C++版本，翻译自左神的java版本
class DC3_origin {
public:
	vector<int> sa;

	vector<int> rank;

	//nums中不能有0这个值，如果有0，则每个数都加上1
	DC3(vector<int>& nums, int max) {
		sa = get_sa(nums, max);
		rank = get_rank();
	}

	vector<int> get_sa(vector<int>& nums, int max) {
		int n = nums.size();
		vector<int> arr(n+3, 0); //后面补3个数
		for (int i = 0; i < n; i++) {
			arr[i] = nums[i];
		}
		return skew(arr, n, max);
	}

	//这里的K是基数排序桶编号最大的值
	//下面的代码其实就是源自论文中的c代码
	vector<int> skew(vector<int>& nums, int n, int K) {
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
			sa12 = skew(s12, n02, name);
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
		vector<int> sa(n, 0);
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
		return sa;
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

	vector<int> get_rank() {
		int n = sa.size();
		vector<int> ans(n, 0);
		for (int i = 0; i < n; i++) {
			ans[sa[i]] = i;
		}
		return ans;
	}
};


//C++优化版本
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




int main()
{
	int n = 6;
	// vector<int> arr = {1,11,4,7,10,5};
	vector<int> arr = {1,11,4,7,10,5};
	DC3 dc3(arr, 11);

	for (auto it:dc3.rank) {
		cout << it << " ";
	}
	cout << endl;

	return 0;
}