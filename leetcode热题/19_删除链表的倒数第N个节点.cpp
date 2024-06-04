/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-04 15:42:23
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
https://leetcode.cn/problems/remove-nth-node-from-end-of-list/description/

给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。

 

示例 1：


输入：head = [1,2,3,4,5], n = 2
输出：[1,2,3,5]
示例 2：

输入：head = [1], n = 1
输出：[]
示例 3：

输入：head = [1,2], n = 1
输出：[1]
 

提示：

链表中结点的数目为 sz
1 <= sz <= 30
0 <= Node.val <= 100
1 <= n <= sz
 

进阶：你能尝试使用一趟扫描实现吗？

*/

/*
使用一个快指针走n步，然后慢指针一起往后走，快指针走到最后，慢指针刚好停在倒数第n个节点上

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
class Solution
{
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if (head == nullptr) {
            return nullptr;
        }

        ListNode* fast = head;
        int i =1;
        while (i<n && fast) {
            fast = fast->next;
            i++;
        }
        // cout << "fast:" << fast->val << endl;
        //不足n个
        if (i < n-1) {
            return nullptr;
        }
        ListNode* slow = head, *pre = head;
        //让fast来到最后一个节点
        while (fast && fast->next) {
            fast = fast->next;
            pre = slow;
            slow = slow->next;
        }

        if (slow == head) {
            head = head->next;
            delete slow;
        } else {
            pre->next = slow->next;
            slow->next = nullptr;
            delete slow;
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
    string s = "2";
    vector<int> nums = {2,3,4,5};
    ListNode* head = sol.build_list(nums);
    print_head(head);
    head = sol.removeNthFromEnd(head, 2);
    print_head(head);

    return 0;
}