/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-18 11:07:35
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
#include <unordered_map>

using namespace std;

/*

https://leetcode.cn/problems/palindrome-linked-list/description/

给你一个单链表的头节点 head ，请你判断该链表是否为
回文链表
。如果是，返回 true ；否则，返回 false 。

示例 1：


输入：head = [1,2,2,1]
输出：true
示例 2：


输入：head = [1,2]
输出：false
 

提示：

链表中节点数目在范围[1, 105] 内
0 <= Node.val <= 9
 

进阶：你能否用 O(n) 时间复杂度和 O(1) 空间复杂度解决此题？
*/

/*
方法1：使用栈来做，压栈序列和出栈序列相同，则为回文串。O(N)空间复杂度

方法二：
指针，快慢指针，慢指针走一步，快指针走两步；当快指针走到头的时候，慢指针刚好来到上中位数。
然后两个指针，从slow往后的链表都翻转过来。
翻转后产生了两个指针，从两个指针的头开始遍历，看是否相等。

*/
class Solution {
public:
    bool isPalindrome_st(ListNode* head) {
        if (!head  || !head->next) {
            return true;
        }
        stack<ListNode*> st;
        auto* cur = head;
        while (cur) {
            st.push(cur);
            cur=cur->next;
        }
        cur = head;
        while (!st.empty() && cur) {
            if (cur->val != st.top()->val) {
                return false;
            }
            cur =cur->next;
            st.pop();
        }
        return true;
    }

   bool isPalindrome(ListNode* head) {
        if (!head  || !head->next) {
            return true;
        }
        ListNode* slow=head, *fast=head;
        while (fast && fast->next && fast->next->next && slow) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // cout << "slow:" << slow->val << endl;
        //slow在上分位位置, 无论是偶数还是奇数都是，从slow->next开始反转。
        ListNode* pre=nullptr, *cur=slow->next, *reverse_head = nullptr;
        ListNode* next=cur->next;
        slow->next = nullptr;
        while (cur) {
            // cout << "cur:" << cur->val << endl;
            if (!cur->next) { //来到了最后一个节点
                reverse_head = cur;
            }
            cur->next = pre;
            pre = cur;
            cur = next;
            if (cur) {
                next = cur->next;
            }
        }

        cur = reverse_head;
        ListNode* cur2 = head;
        while (cur && cur2) {
            if (cur->val != cur2->val) {
                return false;
            }
            cur = cur->next;
            cur2 = cur2->next;
        }
        
        return true;
    }

};
int main()
{

    Solution sol;
    vector<int> arr = {3,2,1,5,6,4}; 
    int k = 2;
    cout << sol.findKthLargest(arr, k) << endl;;
    return 0;
}
