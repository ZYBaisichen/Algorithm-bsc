//https://leetcode.cn/problems/palindrome-partitioning-ii/description/
//力扣第132题，范围dp和从左到右的dp结合
class Solution {
public:

    int minCut(string s) {
        int len = s.length();
        if (len == 0) {
            return 0;
        }
        if (len == 1) {
            return 1-1;
        }

        //预处理
        vector<vector<bool>> isP(len, vector<bool>(len, false));
        for (int i=0;i<len;i++) {
            isP[i][i] = true;
        } 

        for (int i=0;i<len-1;i++) {
            isP[i][i+1] = s[i] == s[i+1];
        }
        for (int i=len-3;i>=0;i--) {
            for (int col = i+2; col<len;col++) {
                isP[i][col] = s[i] == s[col] && isP[i+1][col-1];
            }
        }

        vector<int> dp(len+1, 2001);
        dp[len] = 0;

        for (int i=len-1;i>=0;i--) {
            for (int end=i;end<len;end++) {
                if (isP[i][end]) {
                    dp[i] = min(dp[i], dp[end+1]+1);
                }
            }
        }

        return dp[0]-1;
    }
};