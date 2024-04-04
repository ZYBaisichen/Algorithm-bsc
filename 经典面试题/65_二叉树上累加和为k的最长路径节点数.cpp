/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-03 15:21:31
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

using namespace std;

/*
给定一颗二叉树的头结点head，和一个数k
路径的定义：
可以从任何一个点开始，但是只能往下走，往下可以走到任何节点停止，返回路径累加和为k的所有路径中，最长的路径最多有几个节点。

*/

/*

使用一个map记录每层的前缀和。key是前缀和，vakye是该前缀和最早出现的位置。和数组累加和为k算法原型一样

*/
class Node{
public:
    int val;
    Node* left;
    Node* right;
    Node(int v) {
        val=v;
    }
};
class Solution
{
public:
    int ans = 0;//收集累加和为k的最长路径有多少节点
    int longest(Node* head, int k) {
        ans = 0;
        //key是前缀和，value是该前缀和最早出现在哪一层
        //只维持从头结点出发到当前节点的前缀和。就是只维持头结点到当前节点的路径
        map<int, int> sum_map;
        sum_map[0] = -1;
        int pre_sum = 0;
        process(head, 0, pre_sum, k, sum_map);
        return ans;
    }
    

    /*
    当前在cur节点，在level层
    presum是之前路径形成了多少前缀和
    */
    void process(Node* cur, int level, int pre_sum, int k, map<int,int>& sum_map) {
        if (!cur) {
            return;
        }

        int cur_sum = pre_sum + cur->val;
        if (sum_map.find(cur_sum-k) != sum_map.end()) { //当前累加了cur_sum，要构成k，需要找cur_sum-k最早出现的位置
            cout << "cur_sum:"<< cur_sum << " k:"<<k<<" level:"<<level << " sum_map[cur_sum-k]:" << sum_map[cur_sum-k] <<endl; 
            ans = max(ans, level - sum_map[cur_sum-k]); //不能加上前面的层
        }
        if (sum_map.find(cur_sum) == sum_map.end()) { //必须不存在才更新，因为要保留最早的前缀和
            sum_map[cur_sum] = level;
        }
        process(cur->left, level+1, cur_sum, k, sum_map);
        process(cur->right, level+1, cur_sum, k, sum_map);
        if (sum_map[cur_sum] == level) {
            sum_map.erase(cur_sum); //回溯
        }
    }

};

//前序遍历建树
static int idx = 0;
Node* build_tree(string str) {
    if (idx==str.length()){
        return nullptr;
    }
    if (str[idx] == '#') {
        idx++;
        return nullptr;
    }
    cout << "i: "<< idx << " str[i]:"<< str[idx] << endl;

    Node *node = new Node(str[idx] - '0');
    idx++;
    node->left = build_tree(str);
    node->right = build_tree(str);
    return node;    
}

//中序输出
void in_order(Node* head) {
    if (!head) {
        cout << "#";
        return;
    }
    in_order(head->left);
    cout<<head->val;
    in_order(head->right);
}


int main()
{

    Solution sol;

    // string s="12##3##";
    string s = "16##273###8##";
    idx = 0;
    Node* head = build_tree(s);
    in_order(head);
    cout << endl;
    int k=13;
    cout << sol.longest(head, k) << endl;

    return 0;
}