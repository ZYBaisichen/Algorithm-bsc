/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-06-19 11:49:08
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

https://leetcode.cn/problems/serialize-and-deserialize-binary-tree/
序列化是将一个数据结构或者对象转换为连续的比特位的操作，进而可以将转换后的数据存储在一个文件或者内存中，同时也可以通过网络传输到另一个计算机环境，采取相反方式重构得到原数据。

请设计一个算法来实现二叉树的序列化与反序列化。这里不限定你的序列 / 反序列化算法执行逻辑，你只需要保证一个二叉树可以被序列化为一个字符串并且将这个字符串反序列化为原始的树结构。

提示: 输入输出格式与 LeetCode 目前使用的方式一致，详情请参阅 LeetCode 序列化二叉树的格式。你并非必须采取这种方式，你也可以采用其他的方法解决这个问题。



示例 1：


输入：root = [1,2,3,null,null,4,5]
输出：[1,2,3,null,null,4,5]
示例 2：

输入：root = []
输出：[]
示例 3：

输入：root = [1]
输出：[1]
示例 4：

输入：root = [1,2]
输出：[1,2]

提示：

树中结点数在范围 [0, 104] 内
-1000 <= Node.val <= 1000
*/

/*

*/
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

class Codec
{
public:
    // Encodes a tree to a single string.
    string serialize(TreeNode *root)
    {
        string res = "";
        if (!root)
        {
            return res;
        }
        dfs(root, res);
        // cout << "res:" << res << endl;
        return res;
    }
    void dfs(TreeNode *root, string &res)
    {
        if (!root)
        {
            res += "#";
            return;
        }
        res += to_string(root->val);
        res += "*";
        dfs(root->left, res);
        dfs(root->right, res);
    }

    // Decodes your encoded data to tree.
    TreeNode *deserialize(string data)
    {
        if (data == "")
        {
            return nullptr;
        }
        int i = 0;
        auto *root = build_tree(data, i);
        return root;
    }

    TreeNode *build_tree(string &data, int &i)
    {
        if (i == data.size())
        {
            return nullptr;
        }
        if (data[i] == '#')
        {
            i++;
            return nullptr;
        }
        int val = 0;
        bool flag = false;
        if (data[i] == '-')
        {
            flag = true;
            i++;
        }
        while (data[i] >= '0' && data[i] <= '9')
        {
            // cout << "i:" << i << " data[i]:" << data[i] << endl;
            val = val * 10 + (data[i++] - '0');
        }
        if (data[i] == '*')
        {
            // cout << "**i:" << i << " data[i]:" << data[i] << endl;
            i++;
        }
        if (flag)
        {
            val = -val;
        }

        // cout << "i:" << i << " val:" << val <<endl;
        TreeNode *root = new TreeNode(val);
        root->left = build_tree(data, i);
        root->right = build_tree(data, i);
        return root;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
int main()
{

    return 0;
}
