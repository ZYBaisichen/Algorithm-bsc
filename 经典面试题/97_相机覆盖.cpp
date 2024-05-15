/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-15 13:30:22
 * @LastEditors: baisichen
 * @Description: 指针指向有点问题，leetcode没有通过，但思想已经通了
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
#include <sstream>

using namespace std;

/*
https://leetcode.cn/problems/binary-tree-cameras/description/?utm_source=LCUS&utm_medium=ip_redirect&utm_campaign=transfer2china

给定一个二叉树，我们在树的节点上安装摄像头。

节点上的每个摄影头都可以监视其父对象、自身及其直接子对象。

计算监控树的所有节点所需的最小摄像头数量。

 

示例 1：
输入：[0,0,null,0,0]
输出：1
解释：如图所示，一台摄像头足以监控所有节点。

示例 2：
输入：[0,0,null,0,null,0,null,null,0]
输出：2
解释：需要至少两个摄像头来监视树的所有节点。 上图显示了摄像头放置的有效位置之一。

提示：
给定树的节点数的范围是 [1, 1000]。
每个节点的值都是 0。

*/

/*
二叉树递归套路，三种可能：
1. x节点上放一个相机；这种情况下，下面所有节点都被覆盖，最少需要多少相机。 a
2. x节点上不放相机，但被覆盖了，下面所有节点都被覆盖，总共最少需要多少相机. b
3. x不能被覆盖，下面所有节点都被覆盖，总共需要多少相机. c

*/
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


typedef unsigned long long ll;
class Solution {
public:
    struct info {
        ll uncovered;//x没有被覆盖，x为头的树最少需要多少相机
        ll covered_no_camera; //x被覆盖，但x上没有相机，x为头的树至少需要多少相机
        ll covered_has_camera; //x被覆盖，且x上有相机，x为头的树至少需要多少相机
        info(ll a, ll b, ll c) {
            uncovered = a;
            covered_no_camera = b;
            covered_has_camera = c;
        }
    };
    int minCameraCover_erchashu(TreeNode* root) {
        if(!root) {
            return 0;
        }   
        info ans = process(root);
        return min(ans.uncovered+1,  min(ans.covered_has_camera, ans.covered_no_camera));
    }
    info process(TreeNode* x) {
        //为空时，认为是被覆盖了，但需要0个相机
        if (x==nullptr) {
            return info(INT_MAX, 0, INT_MAX);
        }

        info left = process(x->left);
        info right = process(x->right);

        /*
        x没有被覆盖，则以来左孩子被覆盖并且没相机，加上有孩子被覆盖并且没相机
        */
        int uncovered = left.covered_no_camera + right.covered_no_camera;

        //被覆盖但没相机，说明左右孩子至少有一个有相机
        int covered_no_camera = min(
            left.covered_has_camera + right.covered_has_camera,
            min(left.covered_no_camera+right.covered_has_camera, 
                left.covered_has_camera+right.covered_no_camera)
        );

        //x上放相机，左右两个孩子可以任意放相机, 取最小的情况
        int covered_has_camera = 
            min(left.uncovered, min(left.covered_no_camera, left.covered_has_camera))
            + min(right.uncovered, min(right.covered_has_camera, right.covered_no_camera))
            + 1;
        
        // cout << "x_value:" << x->val << " uncovered:" << uncovered << " covered_no_camera:" << covered_no_camera << " covered_has_camera:" << covered_has_camera << endl; 
        
        return info(uncovered, covered_no_camera, covered_has_camera);
    }


    /*
    贪心
    基于一种假设，明确知道父亲需要什么答案
    */
    int minCameraCover(TreeNode* root) {
        if(!root) {
            return 0;
        }   
        data ans = process2(root);
        if (ans.status == UNCOVERED) {
            return ans.cameras+1;
        } else {
            return ans.cameras;
        }
    }
    enum Status{
        UNCOVERED,
        COVER_NO_CAMERA,
        COVER_HAS_CAMETA
    };
    struct data {
        Status status;
        int cameras;
        data(Status s, int c) {
            status = s;
            cameras = c;
        }
    };
    data process2(TreeNode* x) {
        if (x == nullptr) {
            //当x为空时，自己的父亲只需要知道我被覆盖了，需要0个相机
            return data(COVER_NO_CAMERA, 0);
        }

        data left = process2(x.left);
        data right = process3(x.right);
        int cameras = left.cameras + right.cameras;

        //左边或右边哪怕有一个没被覆盖。就需要再x节点上补一个相机
        //
        if (left.status == UNCOVERED || right.status == UNCOVERED) {
            return data(COVER_HAS_CAMETA, cameras+1);
        }

        //下面是左右两个孩子不存在没有被覆盖的情况

        //左右两个孩子有一个有相机
        //这个时候x节点被覆盖了，x节点如果选择放置相机，则只可能影响父节点；
        //而如果将选择权交给父亲，就可能影响兄弟节点，和爷爷节点。所以这里贪心，x节点不放相机了，将选择权交给父亲
        if (left.status == COVER_HAS_CAMETA || right.status == COVER_HAS_CAMETA) {
            return data(COVER_NO_CAMERA, cameras);
        }

        //最后一种情况，两个节点都被覆盖了，但都没有相机
        //同样x加了相机，只会影响自己和父亲节点。而父亲节点加的话可能会影响多个节点，所以x节点不放相机
        return data(UNCOVERED, cameras);
    }
    
};

int main() {
    Solution sol;
    vector<int> arr = {1,3,5,5,3,2,2,1,4,5,6,10,5,9};
    cout << sol.candy(arr) << endl;
    cout << sol.candy_two_arr(arr) << endl;
    cout << sol.candy2(arr) << endl;
    cout << sol.candy2_two_arr(arr) << endl;
    return 0;
}