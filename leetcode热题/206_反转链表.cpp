/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-14 19:38:45
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
https://leetcode.cn/problems/reverse-linked-list/

给你单链表的头节点 head ，请你反转链表，并返回反转后的链表。
 

示例 1：


输入：head = [1,2,3,4,5]
输出：[5,4,3,2,1]
示例 2：


输入：head = [1,2]
输出：[2,1]
示例 3：

输入：head = []
输出：[]
 

提示：

链表中节点的数目范围是 [0, 5000]
-5000 <= Node.val <= 5000
 

进阶：链表可以选用迭代或递归方式完成反转。你能否用两种方法解决这道题？
*/

/*


*/
class Solution {
public:
    /*
    三指针法。
    pre,next,cur
    最优的算法。无论使用栈还是递归，都需要额外O(N)复杂度的空间。
    */
    ListNode* reverseList(ListNode* head) {
        if(!head || !head->next) {
            return head;
        }
        //三个节点
        if (!head->next->next) {
            head->next->next=head;
            head->next = nullptr;
            return head;
        }
        //至少三个节点
        ListNode* cur=head->next, *pre=head;
        ListNode* next=cur->next;
        pre->next = nullptr;
        while (cur) {
            cur->next = pre;
            pre = cur;
            cur = next;
            if (cur) {
                next = cur->next;
            }
        }
        return pre;
    }


    //为了使用递归来做，先看下栈是怎么做的。因为递归本质上就是使用了系统栈
    ListNode* reverseList(ListNode* head) {
        if(!head || !head->next) {
            return head;
        }
        stack<ListNode*> st;
        ListNode* cur=head;
        while (cur) {
            st.push(cur);
            cur = cur->next;
        }
        ListNode* tmp=nullptr, *tail=nullptr;
        head = nullptr;
        while (!st.empty()) {
            tmp = st.top();
            st.pop();
            tmp->next = nullptr;
            if (head == nullptr) {
                head = tmp;
                tail = tmp;
                // cout << "tail:" << tail->val << endl;
            } else {
                tail->next = tmp;
                tail = tmp;
            }
        }
        return head;
    }

   //下面使用递归来做。一直递归到最后，然后再往上返回
    ListNode* reverseList(ListNode* head) {
        if(!head || !head->next) {
            return head;
        }
        ListNode* new_head;
        reverse(head, &new_head);
        return new_head;
    }

    //反转从cur到之后的链表所有节点
    ListNode* reverse(ListNode* cur, ListNode** head) {
        // cout << "cur:" << cur->val << endl;
        if (!cur->next) {//一个节点时，反转之后还是它自己
            *head = cur;
            return cur;
        }

        auto* tail = reverse(cur->next, head);
        // cout << "tail:" << tail->val << " cur:" << cur->val << endl;
        tail->next = cur;
        cur->next = nullptr;
        return cur;
    }



};
int main()
{

    Solution sol;
    return 0;
}
