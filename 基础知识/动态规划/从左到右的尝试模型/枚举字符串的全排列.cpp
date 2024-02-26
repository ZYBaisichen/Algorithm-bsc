/*
 * @Author: baisichen
 * @Date: 2024-02-22 21:05:49
 * @LastEditTime: 2024-02-23 11:59:59
 * @LastEditors: baisichen
 * @Description: 
 */
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <set>
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
        generateAllPermutationCore(s,0,res);
        for (auto& itr: res) {
            cout << itr << endl;
        }
        return res;

    }

    //在原地递归
    //str[0...i]已经做好决定
    //i....的字符都有机会在i位置
    //i到终止位置，s当前的样子就是一种结果
    void generateAllPermutationCore(string s, int i, vector<string>& res) {
        int len = s.length();
        if (i == len) {
            res.push_back(s);
            return;
        }
        
        for (int j=i;j<len;j++) {
            swap(s, i,j);
            generateAllPermutationCore(s,i+1,res); //枚举下一个位置的字符
            swap(s, j,i);
        }
    }
    void swap(string& s,int i, int j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }


    //全排列去重版本
    void generateAllPermutationDedupCore(string s, int i, vector<string>& res) {
        int len = s.length();
        if (i == len) {
            res.push_back(s);
            return;
        }
        
        //分支限界，减枝
        int visited[27] = {0};
        for (int j=i;j<len;j++) {
            if (visited[s[j]-'a'] == 0) { 
                visited[s[j]-'a'] = 1;
                swap(s, i,j);
                generateAllPermutationCore(s,i+1,res); //枚举下一个位置的字符
                swap(s, i,j);
            }
        }
    }
};
int main()
{
    Solution sol;
    sol.generatePermutation("abc");
}