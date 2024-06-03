package class45;

// 一个非常经典的题
// 这道题课上没有讲
// 后缀数组的模版题
// 需要学会DC3算法生成后缀数组
// 需要学会课上讲的如何生成高度数组
// 时间复杂度O(N)，连官方题解都没有做到的时间复杂度，但这才是最优解
// 测试链接 : https://leetcode.cn/problems/longest-repeating-substring/
public class Code04_LongestRepeatingSubstring {

    //因为后缀串包含了所有子串，最长重复子串也一定是某个后缀的前缀。
    //而lcp[i]又表示排名第i的和排名第i-1的前缀串公共前缀长度，所以lcp数组中的最大值即是最长重复子串的长度
    //需要注意的是：排名越远，两个字符串的重复前缀长度约小。排名越靠近，前缀越相似。
    //所以最长重复子串，i只需要往前看一个i-1就可以。i往后看的，可以通过i+1往前看得到。即利用了LCP(i,i+1)=LCP(i+1, i)
	public static int longestRepeatingSubstring(String s) {
		if (s == null || s.length() == 0) {
			return 0;
		}
		char[] str = s.toCharArray();
		int n = str.length;
		int min = str[0];
		int max = str[0];
		for (int i = 1; i < n; i++) {
			min = Math.min(min, str[i]);
			max = Math.max(max, str[i]);
		}
		int[] all = new int[n];
		for (int i = 0; i < n; i++) {
			all[i] = str[i] - min + 1;
		}
		DC3 dc3 = new DC3(all, max - min + 1);
		int ans = 0;
		for (int i = 1; i < n; i++) {
			ans = Math.max(ans, dc3.height[i]);
		}
		return ans;
	}

	public static class DC3 {
		public int[] sa;
		public int[] rank;
		public int[] height;

		public DC3(int[] nums, int max) {
			sa = sa(nums, max);
			rank = rank();
			height = height(nums);
		}

		private int[] sa(int[] nums, int max) {
			int n = nums.length;
			int[] arr = new int[n + 3];
			for (int i = 0; i < n; i++) {
				arr[i] = nums[i];
			}
			return skew(arr, n, max);
		}

		private int[] skew(int[] nums, int n, int K) {
			//计算s0,s1,s2下标的个数. n为原数组长度
			int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2; 
			//长度为啥是n02+3?
			int[] s12 = new int[n02 + 3], sa12 = new int[n02 + 3];
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
			int[] s0 = new int[n0], sa0 = new int[n0];
			for (int i = 0, j = 0; i < n02; i++) {
				if (sa12[i] < n0) {
					s0[j++] = 3 * sa12[i];
				}
			}
			radixPass(nums, s0, sa0, 0, n0, K);
			int[] sa = new int[n];
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

		private void radixPass(int[] nums, int[] input, int[] output, int offset, int n, int k) {
			int[] cnt = new int[k + 1];
			for (int i = 0; i < n; ++i) {
				cnt[nums[input[i] + offset]]++;
			}
			for (int i = 0, sum = 0; i < cnt.length; ++i) {
				int t = cnt[i];
				cnt[i] = sum;
				sum += t;
			}
			for (int i = 0; i < n; ++i) {
				output[cnt[nums[input[i] + offset]]++] = input[i];
			}
		}

		private boolean leq(int a1, int a2, int b1, int b2) {
			return a1 < b1 || (a1 == b1 && a2 <= b2);
		}

		private boolean leq(int a1, int a2, int a3, int b1, int b2, int b3) {
			return a1 < b1 || (a1 == b1 && leq(a2, a3, b2, b3));
		}

		private int[] rank() {
			int n = sa.length;
			int[] ans = new int[n];
			for (int i = 0; i < n; i++) {
				ans[sa[i]] = i;
			}
			return ans;
		}

		private int[] height(int[] s) {
			int n = s.length;
			int[] ans = new int[n];
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
					while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
						++k;
					}
					ans[rank[i]] = k;
				}
			}
			return ans;
		}

	}

	// 为了测试, 不用提交
	public static String randomString(int n, int r) {
		char[] str = new char[n];
		for (int i = 0; i < n; i++) {
			str[i] = (char) ((int) (Math.random() * r) + 'a');
		}
		return String.valueOf(str);
	}

	// 为了测试, 不用提交
	public static void main(String[] args) {
		int n = 500000;
		int r = 3;
		long start = System.currentTimeMillis();
		longestRepeatingSubstring(randomString(n, r));
		long end = System.currentTimeMillis();
		System.out.println("字符长度为 " + n + ", 字符种类数为 " + r + " 时");
		System.out.println("求最长重复子串的运行时间 : " + (end - start) + " 毫秒");
	}

}
