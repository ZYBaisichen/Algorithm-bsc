/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-04 19:59:37
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
https://leetcode.cn/problems/merge-k-sorted-lists/description/

给你一个链表数组，每个链表都已经按升序排列。

请你将所有链表合并到一个升序链表中，返回合并后的链表。

 

示例 1：

输入：lists = [[1,4,5],[1,3,4],[2,6]]
输出：[1,1,2,3,4,4,5,6]
解释：链表数组如下：
[
  1->4->5,
  1->3->4,
  2->6
]
将它们合并到一个有序链表中得到。
1->1->2->3->4->4->5->6
示例 2：

输入：lists = []
输出：[]
示例 3：

输入：lists = [[]]
输出：[]
 

提示：

k == lists.length
0 <= k <= 10^4
0 <= lists[i].length <= 500
-10^4 <= lists[i][j] <= 10^4
lists[i] 按 升序 排列
lists[i].length 的总和不超过 10^4
*/

/*
1. 堆。O(n*log(n))
使用一个大小为k的小顶堆，堆的节点要包含它是从哪个节点来的。
初始时每个链表各入一个节点，然后每次从堆中弹出一个，连到新的链表上。弹出节点所在链表的下一个节点入堆。
2. 归并合并：还看到了有人提出了个有意思的思路，使用归并排序的思想，两两合并，合并出来的链表规模变成了k/2。
然后再两两合并，规模变成k/4，每次合并的代价最多到N，总复杂度是O(N*log(k))

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
    struct cmp {
        bool operator()(const ListNode* a, const ListNode* b) const{ 
            return a->val > b->val; //新来b，val小时放在堆顶
        }
    };
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        int k = lists.size();
        if (k == 0) return nullptr;
        priority_queue<ListNode*, vector<ListNode*>, cmp> pq;
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        //初始化将lists的第一个入堆
        for (int i=0;i<k;i++) {
            if (lists[i]) {
                pq.push(lists[i]);
            }
        }

        while (!pq.empty()) {
            auto tmp_node = pq.top();
            pq.pop();
            if (head == nullptr) {
                head = tmp_node;
                tail = head;
            } else {
                tail->next = tmp_node;
                tail = tail->next;
            }
            if (tmp_node->next) {
                pq.push(tmp_node->next);
            }
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

// 输入：lists = [[1,4,5],[1,3,4],[2,6]]
// 输出：[1,1,2,3,4,4,5,6]
    vector<int> l1 ={ 1, 4, 5 };
    vector<int> l2 = { 1, 3, 4 };
    vector<int> l3 = {2, 6};
    ListNode* list1 = sol.build_list(l1);
    print_head(list1);
    ListNode* list2 = sol.build_list(l2);
    print_head(list2);
    ListNode *list3 = sol.build_list(l3);
    print_head(list3);
    vector<ListNode *> lists = {list1, list2, list3};

    ListNode* head = sol.mergeKLists(lists);
    print_head(head);

    return 0;
}