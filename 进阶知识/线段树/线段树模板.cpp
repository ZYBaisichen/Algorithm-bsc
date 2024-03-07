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

using namespace std;
// 需要在范围上更新操作。父范围可以简单从左右孩子加工得到，而不用考虑左右孩子数据状况(最大值、最小值、累加和)。就可以使用线段树
class SegmentTree
{
public:
    int length;          // 数组的长度+1， 因为0下标不需要，为了更方便的做孩子下标的计算
    vector<int> arr;     // 原始数组的所有数据，从length+1开始
    vector<int> sum;     // 某个范围上的总和
    vector<int> lazy;    // 懒数组，如果要add的范围完全包住了当前节点，则可以直接放在lazy中，不往下再发了。也是线段树复杂度打到O(log(n))的核心
    vector<int> change;  // 记录需要修改的数
    vector<bool> has_update; // 记录是否有更新操作在当前节点。和lazy数组一样，当可以直接计算出当前节点的值时，不会往下发
    void print_sum(string name) {
        cout << name << ": ";
        for (int i = 0; i < sum.size(); i++) {
            cout << sum[i] << " ";
        }
        cout << endl;
    } 

    void print_update(string name) {
        cout << name << ": ";
        for (int i = 0; i < has_update.size(); i++) {
            cout << has_update[i] << "," << change[i] << " ";
        }
        cout << endl;
    } 

    SegmentTree(vector<int>& _arr)
    {
        length = _arr.size() + 1;
        arr.resize(length,0);
        for (int i = 0; i < _arr.size(); i++)
        {
            arr[i + 1] = _arr[i];
        }
        sum.resize(length << 2);    // 4倍长度
        lazy.resize(length << 2);   // 4倍长度
        change.resize(length << 2); // 4倍长度
        has_update.resize(length << 2, false);
    }

    /**
     * 对sum数组进行初始化， 初始化l到r的范围，求和结果放在rt上
     * @param l  左边界
     * @param r  右边界
     * @param rt sum的下标
     */
    void build(int l, int r, int rt)
    {
        // cout << "build: " << l << " " << r << " " << rt << endl;
        if (l == r)
        {
            sum[rt] = arr[l];
            return;
        }
        int mid = (l + r) >> 1;         // 中位数
        build(l, mid, rt << 1);         // 左子树，左树头结点是rt/2
        build(mid + 1, r, rt << 1 | 1); // 右子树， 右子树头结点是rt/2+1
        push_up(rt);                    // 从左右孩子汇总
    }

    void push_up(int rt)
    {
        sum[rt] = sum[rt << 1] + sum[rt << 1 | 1];
    }

    /**
     * 在L~R范围内，每个节点增加C
     *
     * @param L  目标的左边界
     * @param R  目标的右边界
     * @param C  新增的数量
     * @param l  当前递归的左边界
     * @param r  当前递归的右边界
     * @param rt sum数组的下标
     */
    // L和R是任务范围，永远不变的
    void add(int L, int R, int C, int l, int r, int rt)
    {
        // cout << "add: " << L << " " << R << " " << C << " " << l << " " << r << " " << rt << endl;
        if (L <= l && r <= R)
        { // 想要增加的范围超过了当前递归的返回， lazy住
            sum[rt] += C * (r - l + 1);
            lazy[rt] += C;
            return;
        }

        // 不能懒了，直接取线段树的中位数，递归调用
        int mid = (l + r) >> 1;
        push_down(rt, mid - l + 1, r - mid); // 传递给下游lazy信息，左右孩子数量传到函数中

        if (L <= mid)
        {
            add(L, R, C, l, mid, rt << 1);
        }

        if (R >= (mid + 1))
        {
            add(L, R, C, mid + 1, r, rt << 1 | 1);
        }
        push_up(rt);
    }

    // 向孩子传递lazy和更新信息
    void push_down(int rt, int l_num, int r_num)
    {
        if (has_update[rt])
        {
            cout << "push_down: rt:" << rt << " ln:" << l_num << "rn:" << r_num << " change[rt]:" << change[rt] << endl;
            has_update[rt << 1] = true;
            has_update[rt << 1 | 1] = true;
            change[rt << 1] = change[rt];
            change[rt << 1 | 1] = change[rt];
            sum[rt << 1] = change[rt] * l_num; // 左孩子的总和. 左孩子总共有l_num个节点
            sum[rt << 1 | 1] = change[rt] * r_num;
            lazy[rt << 1] = 0;
            lazy[rt << 1 | 1] = 0;
            cout << "push_down:" << sum[rt<<1] << ", " << sum[rt<<1|1] << " "<< sum[4] << ","<<sum[5]<<endl;
            lazy[rt] = 0;
            has_update[rt] = false;
        }
        // 懒数组，往下更新。也就是将当前数组的数据，下分发
        if (lazy[rt] != 0)
        { // 当前rt的lazy不为0的话，之前的操作序列最后一个操作一定是add的情况。比如(update、add、update、update、add)
            lazy[rt << 1] = lazy[rt];
            lazy[rt << 1 | 1] = lazy[rt];
            sum[rt << 1] += lazy[rt << 1] * l_num;
            sum[rt << 1 | 1] += lazy[rt << 1 | 1] * r_num;
            lazy[rt] = 0;
        }
    }

    // L到R范围上都更新成C
    // 基本初始时l和r都是从0到N，rt=0
    void update(int L, int R, int C, int l, int r, int rt)
    {
        if (L <= l && r <= R)
        {
            has_update[rt] = true;
            change[rt] = C;
            // cout << "update: " << L << " " << R << " " << C << " " << l << " " << r << " " << rt << endl;
            // cout << "cur_change:" << change[rt] << endl;
            sum[rt] = C * (r - l + 1);
            lazy[rt] = 0;
            return;
        }

        // 任务没有完全包住l..r
        int mid = (l + r) >> 1;
        push_down(rt, mid - l + 1, r - mid);
        if (L <= mid)
        {
            update(L, R, C, l, mid, rt<<1);
        }
        if (R >= (mid + 1))
        {
            update(L, R, C, mid + 1, r, rt<<1|1);
        }
        push_up(rt);
    }

    // 查询L到R范围上的sum总和
    long long query(int L, int R, int l, int r, int rt)
    {
        // cout << "query: " << L << " " << R << " " << l << " " << r << " " << rt << endl;
        if (L <= l && r <= R)
        {
            return sum[rt];
        }
        long long res = 0;
        int mid = (l + r) >> 1;
        // cout << "mid: ln:" << mid - l + 1 << "rn:" << r - mid << endl;
        push_down(rt, mid - l + 1, r - mid);

        if (L <= mid)
        {
            res += query(L, R, l, mid, rt << 1);
        }
        if (R >= (mid + 1))
        {
            res += query(L, R, mid + 1, r, rt << 1 | 1);
        }
        return res;
    }
};

class RightSegment
{
public:
    vector<int> arr;
    int length;
    RightSegment(vector<int> &_arr)
    {
        length = _arr.size() + 1;
        arr.resize(length, 0);
        for (int i = 0; i < _arr.size(); i++)
        {
            arr[i + 1] = _arr[i];
        }
    }

    void add(int L, int R, int C)
    {
        for (int i = L; i <= R; i++)
        {
            arr[i] += C;
        }
    }

    void update(int L, int R, int C)
    {
        for (int i = L; i <= R; i++)
        {
            arr[i] = C;
        }
    }

    long long query(int L, int R)
    {
        long long res = 0;
        for (int i = L; i <= R; i++)
        {
            res += arr[i];
        }
        return res;
    }
    void print_arr() {
        cout << "st.arr:";
        for (int i=0; i<arr.size();i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};
int main() {
    vector<int> arr;
    int test_time = 1000;
    int len = 0;
    int k= 0;

    int ans0 = 0;
    int ans1 = 0;
    int ans2 = 0;
    cout << "测试开始" << endl;
    for (int i=0;i<test_time;i++) {
        len = (int)((std::rand() % 5) + 1);
        arr.resize(len);
        for (int j=0;j<len;j++) {
            arr[j] = (int)((std::rand() % 10) + 1);
        }
        RightSegment rs(arr);
        SegmentTree st(arr);
        st.build(1, len, 1);
        // rs.print_arr();
        // st.print_sum("build");

        int R = (std::rand() % len) + 1;
        int L = (std::rand() % R) + 1;
        int C = std::rand() % 10;
        // cout << " L:" << L  << " R:" << R << " C:" << C << endl;

        int rr = std::rand() % 100;
        if (0<=rr<33) {
            st.add(L,R,C,1,len,1);
            rs.add(L,R,C);
        } else if (33 <= rr < 66)
        {
            st.update(L, R, C, 1, len, 1);
            rs.update(L, R, C);
        }
        if (66 <= rr < 100) {
            ans0 = st.query(L, R, 1, len, 1);
            ans1 = rs.query(L, R);
            if (ans0 != ans1) {
                cout << "ans0:" << ans0 << " ans1:" << ans1 << endl;
            }
        }
    }

    // vector<int> arr = {1,3,6,4};
    // int len =arr.size();

    // RightSegment rs(arr);
    // SegmentTree st(arr);
    // st.build(1, len, 1);
    // // int R = (std::rand() % len) + 1;
    // // int L = (std::rand() % R) + 1;
    // // int C = std::rand() % 1000;
    // int L=2, R=3, C=1;
    // for (int i=0;i<st.sum.size(); i++) {
    //     cout << st.sum[i] << " ";
    // }
    // cout << endl;

    // cout << "L:" << L << " R:" << R << " len:" << len << endl;
    // st.add(L, R, C, 1, len, 1);
    // rs.add(L,R,C);
    // rs.print_arr();
    // // st.print_sum("add");
    // st.update(L, R, C, 1, len, 1);
    // // st.print_sum("update");
    // // st.print_update("1_update");
    // rs.update(L,R,C);
    // rs.print_arr();
    // int res = st.query(L,R,1,len,1);
    // cout << "st:" <<  res << " rs:" << rs.query(L,R)<< endl;
    // st.print_update("1_query");
    cout << "测试结束" << endl;
}