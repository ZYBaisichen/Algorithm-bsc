class Solution:

    def expand(self, s):
        res = "#"
        for i in range(0, len(s)):
            res += s[i]
            res += "#"
        # print(res)
        return res

    def longestPalindrome(self, s: str) -> str:
        if s == "":
            return s
        
        #扩充字符串
        ss = self.expand(s)
        
        #定义辅助元素
        ss_len = len(ss)
        p = [0]*ss_len
        r = -1
        c = -1

        #manacher算法
        for cur in range(0, ss_len):
            if (cur == 0):
                p[cur] = 1
                c = 0
                r = 0
            else:
                if cur > r:
                    count = 1
                    l_tmp = cur - 1
                    r_tmp = cur + 1
                    while r_tmp < ss_len and l_tmp >= 0 and ss[r_tmp] == ss[l_tmp]:
                        r_tmp += 1
                        l_tmp -= 1
                        count += 1
                    r = cur
                    c = cur
                    p[cur] = count
                else:
                    cur_tmp = c - (cur-c)
                    l = c - (r-c)
                    if cur_tmp - p[cur_tmp] > l:
                        p[cur] = p[cur_tmp]
                    elif cur_tmp - p[cur_tmp] < l:
                        p[cur] = r - cur + 1
                    else:
                        count = r - cur + 1
                        while r+1 < ss_len and cur-(r-cur)-1>=0 and ss[r+1] == ss[cur-(r-cur)-1]  :
                            r += 1
                            count += 1
                            c = cur
                        p[cur] = count
            # print(p[cur])
        
        #取结果
        res_idx = 0
        for i in range(0, ss_len):
            if p[i] > p[res_idx]:
                res_idx = i
        # print("res_idx:"+str(res_idx))

        res_l = res_idx - p[res_idx] + 1
        res_r = res_idx + p[res_idx] - 1 

        # print("ssr_es:" + ss[res_l:res_r+1])

        ss_tmp = ss[res_l:res_r+1]
        res = ""
        for i in range(0, len(ss_tmp)):
            if not ss_tmp[i] == "#":
                res += ss_tmp[i]
    
        return res