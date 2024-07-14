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

using namespace std;

/*
// 生成长度为size的达标数组
	// 达标：对于任意的 i<k<j，满足   [i] + [j]  != [k] * 2

思路:
主要运用两个性质：奇数+偶数!=偶数数; 正整数*2 = 偶数

假设有个数组[a,b,c]已经达标了。那么有a+c!=2*b，那么一定有:
    1. 2a+2c!=2*(2*b)
    2. 2(a+c+1)!=2*(2*b+1) =>  2a+1 + 2c+1 != 2*(2*b+1)
所以[2a,2b,2c]和[2a+1, 2b+1, 2c+1]都是达标的。下面证明[2a,2b,2c, 2a+1, 2b+1, 2c+1]也是达标的。
a. 假设中间的k在左边偶数区内，则i也一定在偶数区内，j如果也在偶数区，必然满足[i]+[j]!=2*[k]。
    当j在右侧奇数区时，[j]为奇数,[i]为偶数，所以[j]+[i]一定为奇数，而2*[k]一定为偶数，所以也满足[i]+[j]!=2*[k]
b. 同理，假设k在右边的奇数区内，则j一定也在奇数区。此时，i如果也在奇数区，必然满足[i]+[j]!=2*[k]。
    的那个i在偶数区时，[i]为奇数,[j]为偶数，所以[j]+[i]一定为奇数，而2*[k]一定为偶数，所以也满足[i]+[j]!=2*[k]
综上[2a,2b,2c, 2a+1, 2b+1, 2c+1]也是达标的数组

所以如果想构建大小为size的数组，可以先构建一个size/2达标的数组，然后奇数和偶数组放到一起构建新的大小为size的数组也是达标的

这就可以用递归来做了

复杂度是T(N) = T(n/2) + O(N)
a= 1, b=2,d=1
所以根据master公式，log_b(a) < d， 总复杂度是O(N)
*/
class Solution
{
public:
    vector<int> make_no(int size)
    {
        //以1作为种子
        if (size == 1) {
            vector<int> res = {1};
            return res;
        }

        int half_size = (size+1)/2;
        //获取种子数组，生成当前的数组
        vector<int> base  = make_no(half_size);
        // cout << "cur size:" << size << endl;
        // for (auto iter:base) {
        //     cout << " " << iter;
        // }
        // cout << endl;
        vector<int> ans(size,0);
        //偶数
        int idx = 0;
        for (int i=0;i<half_size;i++) {
            ans[idx++] = base[i]*2;
        }
        //奇数
        for (int i=0;i<half_size&&idx<size;i++) {
            ans[idx++] = base[i]*2+1;
        }
        // for (auto iter:ans) {
        //     cout << " " << iter;
        // }
        // cout << endl;
        return ans;
    }

    bool is_valid(vector<int>& arr) {
        int n = arr.size();
        for (int i=0;i<n;i++) {
            for (int j=i+1;j<n;j++) {
                for (int k=i+1;k<j;k++) {
                    if ((arr[i]+arr[j]) == 2*arr[k]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};

int main()
{
    /*
    {1,1,0,0,1,0,1},
    {0,1,1,1,1,0,1},
    {0,1,0,0,1,1,0},
    {0,1,0,0,1,0,1},
    {0,1,1,1,1,1,1}
    */
    Solution sol;
    vector<int> res = sol.make_no(7);
    for (auto iter:res) {
        cout << " " << iter;
    }
    cout << endl;
    cout << "res: " << sol.is_valid(res) << endl;;

}