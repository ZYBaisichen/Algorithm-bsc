
class Solution
{
public:
    class project {
        public:
            int _capital;
            int _profit;
            project(int capital, int profit) : _capital(capital), _profit(profit){}
    };
    int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
        int pro_len = profits.size();
        if (pro_len==0) {
            return 0;
        }
        auto cmp = [](project* a, project* b) { return a->_capital > b->_capital; }; //按照资源成本组织小顶堆
        auto cmp_big = [](project* a, project* b) { return a->_profit < b->_profit; }; //按照利润组织大顶堆
        priority_queue<project*, vector<project*>, decltype(cmp)> min_heap(cmp);
        priority_queue<project*, vector<project*>, decltype(cmp_big)> max_heap(cmp_big);
        for (int i=0; i<pro_len; i++) {
            min_heap.push(new project(capital[i], profits[i]));
        }

        int count = 1;
        int curW = w;
        while (count <= k) {
            //所有能hold住的项目都入大顶堆
            while(!min_heap.empty() && min_heap.top()->_capital<=curW) {
                // cout << " " << min_heap.top()->_profit << endl;
                max_heap.push(min_heap.top());
                min_heap.pop();
            }

            if (max_heap.empty()) {
                break;
            }
            //取一个利润最大的项目做
            curW += max_heap.top()->_profit;
            // cout << "curW:" << curW << endl;
            max_heap.pop();
            count++;
        }
        return curW;
    }
};