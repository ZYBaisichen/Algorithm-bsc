/*
 * @Author: baisichen
 * @Date: 2024-02-22 21:05:49
 * @LastEditTime: 2024-02-22 21:09:10
 * @LastEditors: baisichen
 * @Description: 
 */
#include <iostream>
#include <stack>
using namespace std;
int buttomOut(stack<int> &st);

void reverse(stack<int> &st)
{
    if (st.empty())
    {
        return;
    }
    int num = buttomOut(st);
    // cout << "num: " << num << endl;
    reverse(st);
    st.push(num); // 把系统栈当成额外的栈，实现了逆序
}

int buttomOut(stack<int> &st)
{
    int ans = st.top();
    st.pop();
    if (st.empty()) {
        return ans;
    } else {
        int last = buttomOut(st);
        // cout << "last: " << last << endl;
        st.push(ans);
        return last;
    }
}

int main()
{
    stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    st.push(5);
    reverse(st);
    while (!st.empty())
    {
        cout << st.top() << endl;
        st.pop();
    }
}