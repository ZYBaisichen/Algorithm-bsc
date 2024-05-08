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
一棵搜索二叉树，val值不重复，假设有且仅有两个节点发生了互换，导致了整棵树的错误。请返回这两个节点的值。

进阶：想恢复成搜索二叉树的话，当然可以通过交换这两个节点的值实现，但怎么样才能在结构上调整将这两个节点交换

左神代码：coding-for-great-offer/src/class14/Code05_RecoverBinarySearchTree.java
leetcode： https://leetcode.cn/problems/recover-binary-search-tree/description/

*/

/*
一、 中序遍历序列中，第一次降序的第一个节点，和最后一次降序的后一个节点，就是两个错误的节点
二、如果节点上存在很多其他数据，直接复制就可能成本较大。比如两个节点是两个服务器
//函数f将头为h，错误节点是e1和e2，交换调整后，返回新的头
Node* f(h,e1,e2)

假设e1相关的节点：父节点(p1), 左子节点(l1), 右子节点(r1)
假设e2相关的节点：父节点(p2), 左子节点(l2), 右子节点(r2)

可能性:
1. e1和e2可能有一个是头
2. e1,e2是其父亲的左、右孩子
3. e1,e2是否挨着，谁是谁的父亲？

*/
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution
{
public:
    /*
    值交换版
    中序遍历，第一次降序的第一个节点，和最后一次降序的后一个节点。
    */
   void recoverTree(TreeNode* root) {
        if (!root) {
            return;
        }
        TreeNode* first=nullptr, *second=nullptr;
        two_errors(root, &first, &second);
        if (first != nullptr && second != nullptr) {
            int tmp = first->val;
            first->val = second->val;
            second->val = tmp;
        }
   }
   void two_errors(TreeNode* root, TreeNode** first, TreeNode** second) {
        if (!root) {
            return;
        }

        TreeNode* cur = root;
        TreeNode* pre = nullptr;
        TreeNode* most_right = nullptr;
        bool has_find_reverse = false;
        //morris遍历
        /*
        当前节点cur，一开始cur来到整棵树的头(cur走到null结束)
        1. cur无左树，cur=cur.right
        2. cur有左树，找到左树的最右节点，mostRight
            a. mostRight的右指针指向null时，mostRight.right=cur, cur=cur.left
            b. mostRight的右指针指向cur时，将mostRight.right=null, cur=cur.right
        */
        while (cur != nullptr) {
            if (cur->left) {
                most_right = cur->left;
                while (most_right->right && most_right->right != cur) {
                    most_right = most_right->right;
                }
                if (most_right->right == nullptr) {
                    most_right->right = cur;
                    cur = cur->left;
                } else {
                    most_right->right = nullptr;
                    // cout << " cur:" << cur->val << " ";
                    // if (pre) {
                    //     cout << " pre:" << pre->val << " ";
                    // }
                    // cout << endl;
                    if (pre && pre->val >= cur->val) {
                        *second = cur; //有可能只有两个节点逆序的情况，所以每次第二个节点都需要赋值
                        if (*first == nullptr) {
                            *first = pre;
                        }
                    }
                    pre = cur;
                    cur = cur->right;
                    
                }
            } else {
                // cout << " cur:" << cur->val << " ";
                // if (pre) {
                //     cout << " pre:" << pre->val << " ";
                // }
                // cout << endl;
                if (pre && pre->val > cur->val) {
                    *second = cur;
                    if (*first == nullptr) {
                        *first = pre;
                    }
                }
                pre = cur;
                cur = cur->right;

            }
        }
   }
};


/*
交换两个节点的方法，工程性太高了，就先不写了
*/
/*
// 以下的方法，提交leetcode是通过不了的，但那是因为leetcode的验证方式有问题
	// 但其实！以下的方法，才是正路！在结构上彻底交换两个节点，而不是值交换
	public static TreeNode recoverTree2(TreeNode head) {
		TreeNode[] errs = getTwoErrNodes(head);
		TreeNode[] parents = getTwoErrParents(head, errs[0], errs[1]);
		TreeNode e1 = errs[0];
		TreeNode e1P = parents[0];
		TreeNode e1L = e1.left;
		TreeNode e1R = e1.right;
		TreeNode e2 = errs[1];
		TreeNode e2P = parents[1];
		TreeNode e2L = e2.left;
		TreeNode e2R = e2.right;
		if (e1 == head) {
			if (e1 == e2P) {
				e1.left = e2L;
				e1.right = e2R;
				e2.right = e1;
				e2.left = e1L;
			} else if (e2P.left == e2) {
				e2P.left = e1;
				e2.left = e1L;
				e2.right = e1R;
				e1.left = e2L;
				e1.right = e2R;
			} else {
				e2P.right = e1;
				e2.left = e1L;
				e2.right = e1R;
				e1.left = e2L;
				e1.right = e2R;
			}
			head = e2;
		} else if (e2 == head) {
			if (e2 == e1P) {
				e2.left = e1L;
				e2.right = e1R;
				e1.left = e2;
				e1.right = e2R;
			} else if (e1P.left == e1) {
				e1P.left = e2;
				e1.left = e2L;
				e1.right = e2R;
				e2.left = e1L;
				e2.right = e1R;
			} else {
				e1P.right = e2;
				e1.left = e2L;
				e1.right = e2R;
				e2.left = e1L;
				e2.right = e1R;
			}
			head = e1;
		} else {
			if (e1 == e2P) {
				if (e1P.left == e1) {
					e1P.left = e2;
					e1.left = e2L;
					e1.right = e2R;
					e2.left = e1L;
					e2.right = e1;
				} else {
					e1P.right = e2;
					e1.left = e2L;
					e1.right = e2R;
					e2.left = e1L;
					e2.right = e1;
				}
			} else if (e2 == e1P) {
				if (e2P.left == e2) {
					e2P.left = e1;
					e2.left = e1L;
					e2.right = e1R;
					e1.left = e2;
					e1.right = e2R;
				} else {
					e2P.right = e1;
					e2.left = e1L;
					e2.right = e1R;
					e1.left = e2;
					e1.right = e2R;
				}
			} else {
				if (e1P.left == e1) {
					if (e2P.left == e2) {
						e1.left = e2L;
						e1.right = e2R;
						e2.left = e1L;
						e2.right = e1R;
						e1P.left = e2;
						e2P.left = e1;
					} else {
						e1.left = e2L;
						e1.right = e2R;
						e2.left = e1L;
						e2.right = e1R;
						e1P.left = e2;
						e2P.right = e1;
					}
				} else {
					if (e2P.left == e2) {
						e1.left = e2L;
						e1.right = e2R;
						e2.left = e1L;
						e2.right = e1R;
						e1P.right = e2;
						e2P.left = e1;
					} else {
						e1.left = e2L;
						e1.right = e2R;
						e2.left = e1L;
						e2.right = e1R;
						e1P.right = e2;
						e2P.right = e1;
					}
				}
			}
		}
		return head;
	}

	public static TreeNode[] getTwoErrNodes(TreeNode head) {
		TreeNode[] errs = new TreeNode[2];
		if (head == null) {
			return errs;
		}
		Stack<TreeNode> stack = new Stack<TreeNode>();
		TreeNode pre = null;
		while (!stack.isEmpty() || head != null) {
			if (head != null) {
				stack.push(head);
				head = head.left;
			} else {
				head = stack.pop();
				if (pre != null && pre.val > head.val) {
					errs[0] = errs[0] == null ? pre : errs[0];
					errs[1] = head;
				}
				pre = head;
				head = head.right;
			}
		}
		return errs;
	}

	public static TreeNode[] getTwoErrParents(TreeNode head, TreeNode e1, TreeNode e2) {
		TreeNode[] parents = new TreeNode[2];
		if (head == null) {
			return parents;
		}
		Stack<TreeNode> stack = new Stack<TreeNode>();
		while (!stack.isEmpty() || head != null) {
			if (head != null) {
				stack.push(head);
				head = head.left;
			} else {
				head = stack.pop();
				if (head.left == e1 || head.right == e1) {
					parents[0] = head;
				}
				if (head.left == e2 || head.right == e2) {
					parents[1] = head;
				}
				head = head.right;
			}
		}
		return parents;
	}

	// for test -- print tree
	public static void printTree(TreeNode head) {
		System.out.println("Binary Tree:");
		printInOrder(head, 0, "H", 17);
		System.out.println();
	}
*/

int main()
{

    Solution sol;

    // int test_time = 10000;
    // int max_len = 10;
    // int max_num = 1000;
    // for (int i = 1; i <= test_time; i++) {
    //     int len = (int) (rand() % max_len) + 1;
    //     vector<int> arr(len, 0);
    //     for (int j = 0; j < len; j++) {
    //         arr[j] = rand() % max_num + 1;
    //     }
    //     int ans1 = sol.get_visibale_num_baoli(arr);
    //     int ans2 = sol.get_visible_num(arr);
    //     if (ans1 != ans2)
    //     {
    //         cout << "ans1:" << ans1 << " ans2:" << ans2 << endl;
    //     }
    // }

    // // vector<int> arr = {3,1,2,4,5};
    // // cout << sol.get_visibale_num_baoli(arr) << endl;
    // // cout << sol.get_visible_num(arr) << endl;
    return 0;
}