
n = 3
aim = 5
#从idx....n，还剩rest面值需要拼凑，返回拼凑组合的数量
# process(arr,0,aim)
def process(arr, idx, rest):
    arr_len = len(arr)
    if idx == arr_len:
        if rest == 0:
            return 1
        else:
            return 0
    ways = 0
    zhang = 0
    while zhang* arr[idx] <=rest:
        ways += process(arr, idx+1, rest - (zhang*arr[idx]))
        zhang+=1
    return ways
 
dp2 = [[0] * (aim+1) for _ in range(n+1)]  
def process2(arr, idx, rest):
    arr_len = len(arr)
    if idx == arr_len:
        if rest == 0:
            dp2[idx][rest] = 1
            return 1
        else:
            dp2[idx][rest] = 0
            return 0
    if dp2[idx][rest] != 0:
        return dp2[idx][rest]
    ways = 0
    zhang = 0
    while zhang* arr[idx] <=rest:
        ways += process(arr, idx+1, rest - (zhang*arr[idx]))
        zhang+=1
    dp2[idx][rest] = ways
    return ways

def process3(arr, aim):
    dp = [[0] * (aim+1) for _ in range(n+1)]
    dp[n][0] = 1
    ways = 0
    zhang = 0
    i = n-1
    while i>=0:
        for rest in range(0, aim+1):
            zhang = 0
            while zhang* arr[i] <=rest:
                dp[i][rest] += dp[i+1][rest-(zhang*arr[i])]
                zhang+=1
        i-=1
    return dp[0][aim]

arr = [ 1,2,5]
aim = 5
print(process(arr, 0, aim))
print(process2(arr, 0, aim))
print(process3(arr, aim))


# int main()
# {
#     Solution sol;
#     // vector<int> arr = {90,100,1,7};
#     vector<int> arr={5,10,50,100};
#     int aim=1000;
#     int ans0 = sol.coins_ways1(arr, aim);
#     int ans1 = sol.coins_ways2(arr, aim);
#     int ans2 = sol.coins_ways3(arr, aim);
#     int ans3 = sol.coins_ways4(arr, aim);

#     cout << "ans0:" << ans0 << " ans1:" << ans1 << " ans2:" << ans2 << " ans3:" << ans3 << endl;
#     cout << "测试结束" << endl;
# }