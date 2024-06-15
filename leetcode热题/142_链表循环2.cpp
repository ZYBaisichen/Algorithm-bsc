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
https://leetcode.cn/problems/linked-list-cycle-ii/
给定一个链表的头节点  head ，返回链表开始入环的第一个节点。 如果链表无环，则返回 null。

如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。 为了表示给定链表中的环，评测系统内部使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。如果 pos 是 -1，则在该链表中没有环。注意：pos 不作为参数进行传递，仅仅是为了标识链表的实际情况。

不允许修改 链表。

 

示例 1：



输入：head = [3,2,0,-4], pos = 1
输出：返回索引为 1 的链表节点
解释：链表中有一个环，其尾部连接到第二个节点。
示例 2：



输入：head = [1,2], pos = 0
输出：返回索引为 0 的链表节点
解释：链表中有一个环，其尾部连接到第一个节点。
示例 3：



输入：head = [1], pos = -1
输出：返回 null
解释：链表中没有环。
 

提示：

链表中节点的数目范围在范围 [0, 104] 内
-105 <= Node.val <= 105
pos 的值为 -1 或者链表中的一个有效索引
 

进阶：你是否可以使用 O(1) 空间解决此题？
*/

/*
捋一遍推导过程，参考K神的题解:https://leetcode.cn/problems/linked-list-cycle-ii/solutions/12616/linked-list-cycle-ii-kuai-man-zhi-zhen-shuang-zhi-
假设从头到环入口处的长度是a, 环的长度是b。
1. 快慢指针，fast每次走2步，slow每次走一步，则他们在环内必定相遇，因为每次fast比slow多走1步，所以在环内肯定相遇，且不会错过。
2. 当相遇是他们走的步数分别为f和s，则f=s+nb，即快指针比慢指针多走了n圈环；同时有f=2s，快指针走的路程是慢指针走的路程的2倍
3. 两个式子相减可以得到f=2nb, s=nb
4. 假设slow想走到入口，则需要从开头位置走a+nb步，而此时慢指针走了nb步，需要再走a步就可以到达入口处。
    将fast指针放到开头，然后每次走一步，当他们再次相遇的时候就是入口处节点。此时快指针一共走了2nb+a步，慢指针共走了nb+a步


*/

class Solution
{
public:
    ListNode *detectCycle(ListNode *head) {
        ListNode* fast =head , *slow=head;
        bool  flag = false;
        while (fast && fast->next && slow) {
            fast = fast->next->next;
            slow=slow->next;
            if (fast == slow) {
                flag = true;
                break;
            }
        } 
        // cout << "fast:" << fast->val << endl;
        if (flag) { //有环
            fast = head;
            while (fast != slow) {
                slow = slow->next;
                fast = fast->next;
            }
            return fast;
        } else {
            return nullptr;
        }
    }
};

int main()
{

    Solution sol;
    return 0;
}
