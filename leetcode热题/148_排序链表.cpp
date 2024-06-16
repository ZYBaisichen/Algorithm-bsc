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
https://leetcode.cn/problems/sort-list/
给你链表的头结点 head ，请将其按 升序 排列并返回 排序后的链表 。

 

示例 1：


输入：head = [4,2,1,3]
输出：[1,2,3,4]
示例 2：


输入：head = [-1,5,3,4,0]
输出：[-1,0,3,4,5]
示例 3：

输入：head = []
输出：[]
 

提示：

链表中节点的数目在范围 [0, 5 * 104] 内
-105 <= Node.val <= 105
 

进阶：你可以在 O(n log n) 时间复杂度和常数级空间复杂度下，对链表进行排序吗？
*/

/*
暴力：
 O(N^2)复杂度，新建一个链表返回


*/
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


class Solution
{
public:
    void print_list(ListNode* head, string suff) {
        cout << suff;
        auto* cur= head;
        while(cur) {
            cout << cur->val << " ";
            cur=cur->next;
        }
        cout << endl;
    }
    ListNode* sortList_origin(ListNode* head) {
        ListNode* head_new = nullptr;
        ListNode* cur = head;
        while (cur) {
            ListNode* node = new ListNode(cur->val);
            if (!head_new) {
                head_new = node;
            } else {
                if (head_new->val > cur->val) {
                    node->next = head_new;
                    head_new = node;
                } else {
                    auto* tmp = head_new, *pre=head_new;
                    while (tmp && tmp->val < cur->val) {
                        pre=tmp;
                        tmp = tmp->next;
                    }
                    
                    node->next = pre->next;
                    pre->next = node;
                }
            }
            // cout << "cur:" << cur << endl;
            // print_list(head_new, "head_new:");
            cur = cur->next;
            
        }
        return head_new;
    }

    //归并排序
    //找到每次找到中间节点，让两个节点合并
    ListNode* sortList(ListNode* head) {
        if (!head) {
            return head;
        }
        auto* tail  = head;
        while (tail->next) {
            tail = tail->next;
        }
        return sort_core(head, tail);
    }
    //把head和tail之间的链表排序后返回头
    ListNode* sort_core(ListNode* head,  ListNode* tail) {
        if (!head || !tail) {
            return nullptr;
        }
        // cout << "head:" << head->val << " tail:" << tail->val << endl;
        if (head == tail) {
            return head;
        }

        auto* fast=head, *slow=head;
        //fast走到头，slow走到了中间位置。上分位数
        while (fast && fast->next && fast->next->next && slow) {
            fast = fast->next->next;
            slow = slow->next;
        }

        auto* right_begin = slow->next;
        slow->next = nullptr;
        // cout << "slow:" << slow->val << " right_beign:"  << (right_begin?right_begin->val:-1) << endl;

        auto* left_head = sort_core(head, slow);
        auto* right_head = sort_core(right_begin, tail);

        auto* all_head = merge(left_head, right_head);
        return all_head;

    }

    ListNode* merge(ListNode* left, ListNode* right) {
        auto* cur_l = left, *cur_r = right;
        ListNode* head=nullptr,*tail=nullptr;
        while (cur_l && cur_r) {
            if (cur_l->val < cur_r->val) {
                if (!head) {
                    head = cur_l;
                    tail = head;
                } else {
                    tail->next=cur_l;
                    tail = cur_l;
                }
                cur_l = cur_l->next;
                tail->next = nullptr;
            } else {
                if (!head) {
                    head = cur_r;
                    tail = head;
                } else {
                    tail->next=cur_r;
                    tail = cur_r;
                }
                cur_r = cur_r->next;
                tail->next = nullptr;
            }
        }
        while (cur_l) {
            tail->next = cur_l;
            tail = cur_l;
            cur_l = cur_l->next;
            tail->next = nullptr;
        }
        while (cur_r) {
            tail->next = cur_r;
            tail = cur_r;
            cur_r = cur_r->next;
            tail->next = nullptr;
        }
        return head;
    }
};

int main()
{

    Solution sol;
    return 0;
}
