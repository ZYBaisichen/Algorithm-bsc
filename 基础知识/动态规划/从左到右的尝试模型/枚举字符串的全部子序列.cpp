#include <iostream>
#include <stack>
using namespace std;
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param s string字符串 
     * @return string字符串vector
     */
    vector<string> generatePermutation(string s) {
        // write code here
        int len = s.length();
        vector<string> res;
        if (len==0) {
            res.push_back("");
            return res;
        }
        string path;
        set<string> tmp_res;
        generatePermutationCore(s,tmp_res,path,0);
        for (auto& itr: tmp_res) {
            res.push_back(itr);
        }
        return res;

    }
    void generatePermutationCore(string s, set<string>& res, string& path, int cur) {
        int len = s.length();
        if (cur == len) {
            res.insert(path);
            return;
        }

        generatePermutationCore(s,res,path,cur+1); //枚举不要字符的情况
        
        path += s[cur]; //枚举要字符的情况
        // cout << "before_sub path:" << path<<endl;
        generatePermutationCore(s,res,path,cur+1);
        path = path.substr(0, path.length()-1);
        // cout << "path:" << path<<endl;
    }
};