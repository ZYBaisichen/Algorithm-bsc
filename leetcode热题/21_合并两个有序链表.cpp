/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-04 17:07:59
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

using namespace std;

/*
https://leetcode.cn/problems/merge-two-sorted-lists/description/
将两个升序链表合并为一个新的 升序 链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。 

 

示例 1：


输入：l1 = [1,2,4], l2 = [1,3,4]
输出：[1,1,2,3,4,4]
示例 2：

输入：l1 = [], l2 = []
输出：[]
示例 3：

输入：l1 = [], l2 = [0]
输出：[0]
 

提示：

两个链表的节点数目范围是 [0, 50]
-100 <= Node.val <= 100
l1 和 l2 均按 非递减顺序 排列

*/

/*
最简单的就是双指针，每到一个元素，小的先加到新的链表上，然后后移动

如果想原地的话，也可以用双指针。
*/
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
struct ListNode {
     int val;
     ListNode *next;
     ListNode() : val(0), next(nullptr) {}
     ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (!list1) {
            return list2;
        }
        if (!list2) {
            return list1;
        }
        ListNode* head = nullptr;
        ListNode *tail = nullptr;
        ListNode *l1 = list1, *l2 = list2;
        if (list1->val < list2->val) {
            head = list1;
            l1 = list1->next;
        } else {
            head = list2;
            l2 = list2->next;
        }
        tail = head;
        // cout << "head:" << head->val << endl;

        while (l1 && l2) {
            if (l1->val < l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        while (l1) {
            tail->next = l1;
            tail = l1;
            l1 = l1->next;
        }
        while (l2) {
            tail->next = l2;
            tail = l2;
            l2 = l2->next;
        }
        return head;
    }

    ListNode* build_list(vector<int>& nums) {
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        for (auto it:nums) {
            if (head == nullptr) {
                head = new ListNode(it);
                tail = head;
            } else {
                ListNode* node = new ListNode(it);
                tail->next = node;
                tail = node;
            }
        }
        return head;
    }
};

void print_head(ListNode* head) {
    while (head) {
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}
int main()
{

    Solution sol;

    vector<int> l1 ={ 1, 2, 4 };
    vector<int> l2 = { 1, 3, 4 };
    ListNode* list1 = sol.build_list(l1);
    print_head(list1);
    ListNode* list2 = sol.build_list(l2);
    print_head(list2);

    ListNode* head = sol.mergeTwoLists(list1, list2);
    print_head(head);

    return 0;
}