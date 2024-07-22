/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-05-26 15:34:39
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
#include <sstream>
#include <unordered_set>

using namespace std;

/*
给定一个querys数组，每行数据表示一次查询。要求查询querys[i][0]和querys[i][1]的最低公共祖先。
要求时间复杂度是O(N+M)

*/

/*
暴力思想：
生成一个father表，记录每个节点的父节点是谁。然后对于每两个节点：
    1. 一个往上找，并记录路径。
    2. 另一个也往上找，直到在另一个节点查找的路径包含就找到了
    3. 这种方法每次都需要往上遍历两遍，假设树退化成链表，每次需要找2*N次，总复杂度是M*2*N,O(M*N)



          a
     b         c
  d    e     f  g
h  i  g  k  l m

三种base：
1. 相同节点查询：[c,c]，直接返回c
2. 非空和空节点查询：直接返回非空节点
3. 两个空节点的公共父节点是空

普遍位置：比如query数组是[(b,e),(b,c),(e,c)]
   1. 生成问题表和填写表。
        问题表:{
            b:{e,c},//b和e、c各有一个问题
            e:{b,c},//同样的e和b、c各有一个问题
            c:{b,e},
        }
        填写表:{
            b:{0,1},//整体和问题表对应。b和e的问题填在答案的0位置，b和c位置填在答案的1位置
            e:{0,2},
            c:{1,2}
        }
    2. 遍历是二叉树的递归遍历，每个节点来到自己3次。如果先遇到b，但是它的问题是e和c，此时还没遇到e和c，则删除b:{e,c}。下次遇到e和c可以知道b已经遍历过了
        之所以即存{b,e}又存{e,b}，就是因为想在遍历过程中不错过问题
    3. 整体流程：假设问题表里有{e:{f,c},f:{e},c:{e}}
        a. 先序遍历整个二叉树
        b. 对于节点x，先看x节点在答案表中有没有，处理答案。然后往左遍历，让左边的节点合并成一个集合A返回，然后x与A合并到一起，并打上tag表示这个集合的父节点是x。
            再往右遍历合并成一个集合B返回，让A,X,B全都合并到一起，打上父节点是X的tag，返回给上一层。
        b. 第一次来到一个位置时，看问题表里有没有待解决的问题。当遍历到e时，查询问题表，发现f所在集合代表点(目前是f自己)还没有被打上父节点tag。
            先搁置删除e:f这条记录，等下次遇到f时再做处理。同理e:c答案表也删除。
        c. 集合A={b,d,e,h,i,g,k}返回到a时，将a与A合并变成A={a,b,d,e,h,i,g,k}并将其代表节点打上父节点为a的tag。
           注意这里结合A的代表节点不一定是a，因为a往A上合并，肯定是小挂大，所以代表节点还是原来A中的元素。
        d. 网右遍历到了c，发现还有c:e这个问题没有解决，看e上已经有父节点tag等于a，所以可以知道之前已经遇到过e了。所以c:e的共同父节点就是当前e的父节点tag(a),.
        e. 继续往下遍历到了f，发现还有f:e这个问题没有解决。同理它们的最近公共祖先就是e的父节点tag a。
*/

class node{
public:
    int val;
    node* left;
    node* right;
    node(int _val) {
        val = _val;
        left =nullptr;
        right = nullptr;
    }
};
class UnionFind{
public:
    UnionFind(vector<node*>& nums) {
        for(int i=0;i<nums.size();i++) {
            _nums.push_back(nums[i]);
            _parents[nums[i]] = nums[i];
            _sizes[nums[i]]=1;
        }
    }
    
    bool isSameSet(node* a, node* b){
        node* a_p = find_parent(a);
        node* b_p = find_parent(b);
        // if (a==2&&b==3) {
        //     cout << "isSameSet:" <<  a_p << "," << b_p << endl;
        // }
        return a_p == b_p;
    }

    void unionSet(node* a, node* b) {
        if (a == nullptr || b==nullptr) {
            return;
        }
        node* a_p = find_parent(a);
        node* b_p = find_parent(b);
        //不在一个集合
        if (a_p != b_p) {
            node* _big_p = _sizes[a_p] >= _sizes[b_p] ? a_p : b_p;
            node* _little_p = _sizes[a_p] >= _sizes[b_p] ? b_p : a_p;
            _parents[_little_p] = _big_p; //小挂大
            _sizes[_big_p] = _sizes[_big_p] + (_sizes[_little_p]==-1 ? 0 : _sizes[_little_p]); //大的更新size
            _sizes[_little_p] = -1;
        }
    }

    node* find_parent(node* cur) {
        node* res = cur;
        while(_parents[res] != res) {
            res = _parents[res];
        }
        //扁平化
        node* tmp = cur;
        while (_parents[tmp] != res) {
            tmp = _parents[tmp];
            _parents[tmp] = res;
        }
        return res;
    }

    //
    map<node*,node*> _parents; //记录每个节点的父节点
    map<node*,int> _sizes; //记录当前节点的所代表集合的节点数量
    vector<node*>  _nums;
};
class Solution {
public:
    //root为树节点
    //querys里面有多行，每行都有两个node*指针，表示两个节点
    //返回它们的公共节点
    vector<node*> query(node* root, vector<vector<node*>>querys) {
        int len  = querys.size();
        if (len == 0) {
            return vector<node*>();
        }
        //定义问题表和答案表
        map<node*,vector<node*>> query_map;
        map<node*, vector<int>> idx_map;

        //定义tag表，key为某个集合的代表点，value是key所在集合的所有节点的tag点。
        map<node*,node*> tag_map;

        vector<node*> path;
        //获取所有的节点，放在path中
        get_all_node(root, path);

        //初始化并查集
        //每个节点都是一个集合
        UnionFind sets(path);

        vector<node*> ans(len, nullptr);
        //构建问题表和位置idx表
        set_queries_ans_set_easy_anwser(querys, query_map, idx_map, ans);

        cout << "query_map:" <<endl;
        for (auto& iter:query_map) {
            cout << iter.first->val << ":";
            for (auto& iter2:iter.second) {
                cout << iter2->val << ",";
            }
            cout << endl;
        }

        cout << "idx_map:" << endl;
        for (auto& iter:idx_map) {
            cout << iter.first->val << ":";
            for (auto& iter2:iter.second) {
                cout << iter2 << ",";
            }
            cout << endl;
        }

        /*
        root:遍历
        ans:填答案
        querey_map,idx_map:方便知道有哪些问题
        tag_map:为集合代表点打父节点tag
        sets:并查集
        */
        set_answers(root, ans, query_map, idx_map, tag_map, sets);
        
        return ans;

    }

    void set_answers(node* root, vector<node*>& ans, map<node*,vector<node*>>& query_map, 
        map<node*,vector<int>>& idx_map, map<node*, node*>& tag_map, UnionFind& sets) {
        if (root == nullptr) {
            return;
        }
        //先看左边能不能算出来答案
        set_answers(root->left, ans, query_map, idx_map, tag_map, sets);
        // if (root->val == 5) {
        //     auto root_p = sets.find_parent(root);
        //     cout << "==222:" << root_p->val << endl;
        // }
        //跟左边集合合到一起
        sets.unionSet(root, root->left);
        // if (root->val == 5) {
        //     auto root_p = sets.find_parent(root);
        //     cout << "==222:" << root_p->val << endl;
        // }
        tag_map[sets.find_parent(root)] = root;
        // if (root->val==5) {
        //     auto root_p = sets.find_parent(root);
        //     // auto root_left_p = sets.find_parent(root->left);
        //     cout << "=====" <<endl;
        //     // cout << "asaaa:" << root->left->val << endl;
        //     // cout << "root_p:" << root_p->val << " root_left_p:" << root_left_p->val << endl;
        //     cout << "root_p:" << root_p->val << endl;
        //     cout << "tag_map[root_p]->val:" << tag_map[root_p]->val << endl;
        //     cout << "=====" <<endl;
        // }
        set_answers(root->right, ans, query_map, idx_map, tag_map, sets);
        sets.unionSet(root, root->right);
        tag_map[sets.find_parent(root)] = root;

        // if (root->val == 5) {
        //     auto pp = sets.find_parent(tree_map[3]);

        //     cout << "33333:" << pp->val << endl;
        //     cout << "33333:" << tag_map[pp]->val << endl;
        // }

        //后序遍历的时候处理root问题，其实前中后续都可以
        //处理root相关的问题
        vector<node*>& cur_query_list = query_map[root];
        vector<int>& cur_idx_list = idx_map[root];
        node* parent;
        int idx=0;
        for (int i=0;i<cur_query_list.size();i++) {
            node *cur = cur_query_list[i];
            parent = sets.find_parent(cur); // 找到答案表中的代表节点
            // 所在集合中的代表节点已经有tag了，则填答案。否则不做处理
            if (tag_map.find(parent) != tag_map.end()) { 
                idx = cur_idx_list[i];
                ans[idx] = tag_map[parent]; //答案是代表节点的tag
            }
        }
        //不管有没有答案都需要清理。
        query_map[root].clear();
        idx_map[root].clear();
    }

    void set_queries_ans_set_easy_anwser(
        vector<vector<node*>>& querys, 
        map<node*, vector<node*>>& query_map, 
        map<node*, vector<int>>& idx_map,
        vector<node*>& ans) {
        int len = querys.size();
        for (int i=0;i<len;i++) {
            node* first = querys[i][0];
            node* second = querys[i][1];
            //相等直接填答案
            //如果一个是空一个不是空，则直接返回
            if (first == second) {
                ans[i] = first;
                continue;
            }
            if (first == nullptr || second == nullptr) {
                if (first) {
                    ans[i] = first;
                }
                if (second) {
                    ans [i] = second;
                }
                continue;
            }
            
            if (query_map.find(first) == query_map.end()) {
                query_map[first] = vector<node*>();
                query_map[first].push_back(second);
            } else {
                query_map[first].push_back(second);
            }

            if (query_map.find(second) == query_map.end()) {
                query_map[second] = vector<node*>();
                query_map[second].push_back(first);
            } else {
                query_map[second].push_back(first);
            }

            //问题的答案填在i位置
            if (idx_map.find(first) == idx_map.end()) {
                idx_map[first] = vector<int>();
                idx_map[first].push_back(i);
            } else {
                idx_map[first].push_back(i);
            }
            if (idx_map.find(second) == idx_map.end()) {
                idx_map[second] = vector<int>();
                idx_map[second].push_back(i);
            } else {
                idx_map[second].push_back(i);
            }
        }
    }
    void get_all_node(node* root, vector<node*>& path) {
        if (root ==nullptr) {
            return;
        }
        path.push_back(root);
        get_all_node(root->left, path);
        get_all_node(root->right, path);
    }


    map<int, node*> tree_map;//记录id和node*的映射
    node* build_tree(string s, int* cur) {
        if (s[*cur] == '#') {
            (*cur)++;
            return nullptr;
        }

        int val = s[*cur] - '0';
        node* root = new node(val);
        tree_map[val] = root;
        (*cur)++;
        root->left = build_tree(s, cur);
        root->right = build_tree(s, cur);
        return root;
    }
};


void print_tree(node* root) {
    if (root == nullptr) {
        return;
    }
    // cout << root->val << " ";
    print_tree(root->left);
    // cout << root->val << " ";
    print_tree(root->right);
    cout << root->val << " ";
}

int main() {
    Solution sol;
    string s = "123##4##5##";
    int cur = 0;
    node* root = sol.build_tree(s, &cur);
    print_tree(root);
    cout << endl;

    map<int, node*>& tree_map = sol.tree_map;
    vector<vector<node *>> query = {
        {tree_map[3], tree_map[5]},
        {tree_map[3], tree_map[2]},
        {tree_map[4], tree_map[5]},
    };
    auto ans = sol.query(root, query);
    cout << "ans:" <<endl;
    for (auto a:ans) {
        cout << a->val << " ";
    }
    cout << endl;
    return 0;
}