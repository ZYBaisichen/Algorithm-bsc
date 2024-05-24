/*** 
 * @Author: baisichen
 * @Date: 2023-10-21 23:02:34
 * @LastEditTime: 2023-10-22 22:37:02
 * @LastEditors: baisichen
 * @Description: 
 * 测试链接：https://www.nowcoder.com/practice/e7ed657974934a30b2010046536a5372
 */
#include <iostream>
#include <vector>
#include "map"
using namespace std;


//标准C++工程级写法，但会卡常数时间；如果想达到acm级别的话需要改map为数组，尽量减少容器使用
class UnionFind{
public:
    UnionFind(vector<int>& nums) {
        for(int i=0;i<nums.size();i++) {
            _nums.push_back(nums[i]);
            _parents[i] = i;
            _sizes[i]=1;
        }
    }
    
    bool isSameSet(int a, int b){
        int a_p = find_parent(a);
        int b_p = find_parent(b);
        // if (a==2&&b==3) {
        //     cout << "isSameSet:" <<  a_p << "," << b_p << endl;
        // }
        return a_p == b_p;
    }

    void unionSet(int a, int b) {
        int a_p = find_parent(a);
        int b_p = find_parent(b);
        //不在一个集合
        if (a_p != b_p) {
            int _big_p = _sizes[a_p] >= _sizes[b_p] ? a_p : b_p;
            int _little_p = _sizes[a_p] >= _sizes[b_p] ? b_p : a_p;
            _parents[_little_p] = _big_p; //小挂大
            _sizes[_big_p] = _sizes[_big_p] + (_sizes[_little_p]==-1 ? 0 : _sizes[_little_p]); //大的更新size
            _sizes[_little_p] = -1;
            // if (a==2&&b==3) {
            //     cout << "unionSet:" << _big_p << "," << _little_p << endl;
            //     cout << "_parents:" << _parents[_little_p] << "," << _parents[_big_p] << endl;
            //     cout << "_sizes:" << _sizes[_little_p] << "," << _sizes[_big_p] << endl;
            // }
        }
    }

    int find_parent(int cur) {
        int res = cur;
        while(_parents[res] != res) {
            res = _parents[res];
        }
        // if (cur==3) {
        //         cout << "find_parent:" << res << endl;
        //     }
        //扁平化
        int tmp = cur;
        while (_parents[tmp] != res) {
            tmp = _parents[tmp];
            _parents[tmp] = res;
        }
        return res;
    }

    //
    map<int,int> _parents; //记录每个节点的父节点
    map<int,int> _sizes; //记录当前节点的所代表集合的节点数量
    vector<int>  _nums;

    // map<V, Node<V>> nodes;

};

//数组版本
class UnionFind_arr{
public:
    int* _parents; //记录每个节点的父节点
    int* _sizes; //记录当前节点的所代表集合的节点数量
    vector<int>  _nums;

    UnionFind_arr(vector<int>& nums) {
        init_space(nums);
        for(int i=0;i<nums.size();i++) {
            _nums.push_back(nums[i]);
            _parents[i] = i;
            _sizes[i] = 0;
        }
    }
    void init_space(vector<int>& nums) {
        int len = nums.size();
        _parents = new int[len];
        _sizes = new int[len];
    }

    ~UnionFind_arr() {
        if (_parents) {
            delete _parents;
        }
        if (_sizes) {
            delete _sizes;
        }
    }
    
    bool isSameSet(int a, int b){
        int a_p = find_parent(a);
        int b_p = find_parent(b);
        // if (a==2&&b==3) {
        //     cout << "isSameSet:" <<  a_p << "," << b_p << endl;
        // }
        return a_p == b_p;
    }

    void unionSet(int a, int b) {
        int a_p = find_parent(a);
        int b_p = find_parent(b);
        //不在一个集合
        if (a_p != b_p) {
            int _big_p = _sizes[a_p] >= _sizes[b_p] ? a_p : b_p;
            int _little_p = _sizes[a_p] >= _sizes[b_p] ? b_p : a_p;
            _parents[_little_p] = _big_p; //小挂大
            _sizes[_big_p] = _sizes[_big_p] + (_sizes[_little_p]==-1 ? 0 : _sizes[_little_p]); //大的更新size
            _sizes[_little_p] = -1;
            // if (a==2&&b==3) {
            //     cout << "unionSet:" << _big_p << "," << _little_p << endl;
            //     cout << "_parents:" << _parents[_little_p] << "," << _parents[_big_p] << endl;
            //     cout << "_sizes:" << _sizes[_little_p] << "," << _sizes[_big_p] << endl;
            // }
        }
    }

    int find_parent(int cur) {
        int res = cur;
        while(_parents[res] != res) {
            res = _parents[res];
        }
        // if (cur==3) {
        //         cout << "find_parent:" << res << endl;
        //     }
        //扁平化
        int tmp = cur;
        while (_parents[tmp] != res) {
            tmp = _parents[tmp];
            _parents[tmp] = res;
        }
        return res;
    }
    // map<V, Node<V>> nodes;

};
int main() {
    int N, M;
    cin >> N >> M;
    vector<int> nums;
    for (int i=0;i<N;i++) {
        nums.push_back(i+1);
    }
    UnionFind_arr union_find_obj(nums);
    int opt,x,y;
    for (int i=0;i<M;i++) {
        cin >> opt>>x>>y;
        if (opt==1) {
            cout << (union_find_obj.isSameSet(x, y) ?  "Yes" : "No") << " ";
        } else if (opt==2) {
            union_find_obj.unionSet(x,y);
        }
    }

    return 0;
}
// 64 位输出请用 printf("%lld")