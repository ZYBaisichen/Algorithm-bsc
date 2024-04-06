/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-06 16:59:58
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
描述
给定一颗二叉树，已知所有节点的值都不一样， 返回其中最大的且符合搜索二叉树条件的最大拓扑结构的大小。
拓扑结构是指树上的一个联通块。
输入描述：
第一行输入两个整数 n 和 root，n 表示二叉树的总节点个数，root 表示二叉树的根节点。

以下 n 行每行三个整数 fa，lch，rch，表示 fa 的左儿子为 lch，右儿子为 rch。(如果 lch 为 0 则表示 fa 没有左儿子，rch同理)

ps:节点的编号就是节点的值。
输出描述：
输出一个整数表示满足条件的最大拓扑结构的大小。
示例1
输入：
3 2
2 1 3
1 0 0
3 0 0
输出：
3
备注：
1≤n≤200000
1≤fa,lch,rch,root≤n

https://www.nowcoder.com/practice/e13bceaca5b14860b83cbcc4912c5d4a
*/
class Node {
public:
    Node* left;
    Node* right;
    int val;
    Node(int v) {
        left = nullptr;
        right = nullptr;
        val = v;
    }
};
typedef unsigned long long ll;
class Solution
{
public:
    /*
    1. 拓扑贡献记录：每个节点上针对当前节点左树和右树的拓扑贡献节点数。
        5
      /    \
    4       8
   / \     /  \
  1   3   7    12
           \
            9
    比如上面的树，以当前5为头时，最大拓扑结构是541###87##12##。
    4的左树贡献1个节点，右树贡献0个。8的左树贡献1个，右树贡献献1个。5:[2,3], 1[0,0], 7[0,0], 12[0,0]

    2. 假设X左树上已经有了拓扑贡献度。假设左树上所有节点都对L节点负责，如果能将对L负责的拓扑贡献度，能很快的对X负责，则就可以写个递归实现了。
      X
    /   \
    L    R

    3. 遍历左树的右边界计算左树贡献度
    a. 比如13的左树，10的左右树各贡献20和50个节点。10比30小，10本身可以在30的左树上，而10的左树贡献的20个都比10小，所以也能给13贡献。此时13的左树就有了21个节点
    b. 12也可以贡献给13，所以12的左树也可以贡献给13
    c. 到达大于13的节点14，则回溯回去，沿途减去14的贡献节点数，即20+4+1=25, 12的右子树为13贡献25-25=0个节点。10的右子树为13贡献了50-25个节点
    同理遍历右树的左边界，就可以计算出来右树的贡献度。
          13
        /    \
      10
   20/  \50
        12
     24/  \25
           14
         4/   \20

    4. 每个节点都要过一遍左树右边界或者右树左边界。但每个节点的左树右边界都不重复，总共加起来是O(N)。同样右树左边界也是不重复的，总共加起来是O(N)

    这里只讲了思路，难度较大。先不实现了。
    */
    int get_max_sbt(Node* root) {

    }

    int f
    

    /*
    以cur为根节点，得到它的满足搜索二叉树条件的最大结构的节点数
    暴力：
    以root为头。深度优先遍历到每个节点cur，然后从root开始按照搜索二叉树的方式滑，如果能找到cur，则将cur放到拓扑结构中。
    每次以每个root为头（N），每次遍历到一个节点走一遍(N^2), 总复杂度O(N^3)
    */
    int f1(Node * cur) {
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

int main()
{

    Solution sol;
    // string s = "aba";
    int n=1000;
    int k=1000;
    // cout << sol.kInversePairs1(n,k) << endl;
    // cout << sol.kInversePairs(n,k) << endl;
    cout << sol.kInversePairs_lt(n,k)<< endl;
    return 0;
}