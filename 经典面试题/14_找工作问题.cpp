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

using namespace std;
void print(vector<int>& res, string suf = "res") {
    cout << suf;
    for (auto iter:res) {
        cout << " " << iter;
    }
    cout << endl;
}

/*
 * 给定一个Job类型的数组，表示所有的工作。给定一个int类型的数组arr，表示所有小伙伴的能力。
 * 返回int类型的数组，表示每一个小伙伴按照牛牛的标准选工作后所能获得的最大报酬。
*/

class Job {
public:
    int hard;
    int money;
    Job(int _hard, int _money) {
        hard = _hard;
        money = _money;
    }
    // 重载 < 运算符以定义比较逻辑  
    bool operator<(const Job& other) const {  
        return hard < other.hard;  
    } 
};

class Solution
{
public:
    //首先想法是，暴力枚举所有的数值对
    //然后对数值对排序，排序完成后找到第k小（下标为k-1）的数值对
    vector<int> find_job(vector<Job> &jobs, vector<int>& arr) {
        int lenj = jobs.size();
        int lena = arr.size();
        vector<int> res(lena, 0);
        if (lenj == 0 || lena  == 0) {
            return res;
        }

        // 使用比较函数对vector进行排序  
        std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {  
            if (a.hard != b.hard) {  
                return a.hard < b.hard;   //按照难度升序
            } else {  
                return a.money > b.money;  //按照报酬降序
            }  
        });

        //现在jobs数组中，按照难度升序排列。难度相同的报酬高的在前面
        //将所有难度想听报酬最高的放到有序表中
        multiset<Job> m_set;
        Job* pre = &jobs[0];
        m_set.insert(*pre); //上一组难度的组长入表

        for (int i=1;i<lenj;i++) {
            //这里舍弃掉了难度相同的其他成员，只保留第一个
            //也舍弃掉了难度相同，但报酬减小的工作
            if (pre->hard != jobs[i].hard && pre->money < jobs[i].money) {
                m_set.insert(jobs[i]);
                pre = &jobs[i];
            }
        }

        for (auto iter:m_set) {
            cout << iter.hard << " " << iter.money << endl;
        }

        for (int i=0;i<lena;i++) {
            auto it = m_set.lower_bound(Job(arr[i], 0));
            cout << "it:" << it->hard << endl;
            if (it != m_set.end()) {
                if (it->hard == arr[i]) { //看是否等于k
                    res[i] = it->money;
                } else if (it != m_set.begin()) { //必大于k  
                    --it; // 尝试找到前一个元素  
                    if (it->hard <= arr[i]) {
                        res[i] = it->money;
                    }  
                }
            } else { //没有找到难度大于等于arr[i]的工作。则挑选最后一份工作给他
                res[i] = m_set.rbegin()->money;
            }
        }

        return res;
    }
};

int main()
{
    Solution sol;
    vector<vector<int>> jobs_v = {
        {1,2},
        {1,1},
        {2,6},
        {1,5},
        {3,2},
        {3,7},
        {4,6}
    };
    vector<Job> jobs;
    for (int i=0;i<jobs_v.size();i++) {
        jobs.push_back(Job(jobs_v[i][0], jobs_v[i][1]));
    }
    vector<int> arr = {1,2,3,4};
    vector<int> res = sol.find_job(jobs, arr);
    for (auto iter:res) {
        cout << iter << " ";
    }
    cout << endl;



    
    return 0;
}