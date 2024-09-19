#include <iostream>
#include <vector>
#include <unordered_map>
#include <vector>
using namespace std;


#define DISK_LEN 1000000
#define MEM_LEN 100000
#define TRUNCK_SIZE 10000

vector<int> disk(DISK_LEN, 0);

struct trunck {
    int idx;
    vector<int> cur_nums; //桶内数据
    trunck* next;
    trunck* pre;
    trunck() {
    	idx = -1;
    	next = nullptr;
    	pre = nullptr;
    	cur_nums.resize(TRUNCK_SIZE, -1);
    }
};

class LRU {
public:
	unordered_map<int, trunck*> _map; //桶标号, 桶地址
	trunck* head;
	trunck* tail;
	int capacity;
	int size;
	int eyery_trunck_size;
	LRU() {
		head = nullptr;
		tail = nullptr;
		eyery_trunck_size = TRUNCK_SIZE; //每个桶承载的数
        capacity = MEM_LEN / eyery_trunck_size; // 桶数量
        size = 0;
    }

	void move_to_tail(trunck* cur) {
		if (!head || cur == tail) {
			return;
		}
		if (cur == head) {
			head  = head->next;
			head->pre = nullptr;
		} else {
			tail->next = cur;
			cur->pre = tail;
			cur->next = nullptr;
			tail = cur;
		}
	}

	void delete_one_trunck() {
		if (!head) {
			return;
		}
		if (head == tail) {
			delete head;
			head = nullptr;
			tail = nullptr;
		}
		auto* tmp = head;
		head = head->next;
		delete tmp;
        size--;
	}
	void insert_new_trunck(trunck* cur) {
		if (!head) {
			head = cur;
			tail = cur;
		} else {
			tail->next = cur;
			cur->pre = tail;
			tail = cur;
		}
        size++;
    }

    int get(int idx) {
    	//获取桶编号
    	int trunck_idx = idx/eyery_trunck_size;
    	int offset = idx%eyery_trunck_size;
        // cout << "trunck_idx:" << trunck_idx << " offset:" << offset << endl;

            // 从map中拿，拿不到从磁盘中找
        if (_map.find(trunck_idx) != _map.end())
        {
            move_to_tail(_map[trunck_idx]); // 将桶挪到最后
            return _map[trunck_idx]->cur_nums[offset];
        } else {
            if(size+1 > capacity) {
                cout << "need detele ont trunck, trunck id:" << (head ? head->idx : -1) << endl;
    			delete_one_trunck(); //内存满了，则淘汰掉最久未使用的桶
    		}

    		auto* new_trunck = get_data_from_disk(idx); //载入
    		int val = new_trunck->cur_nums[offset];
    		return val;
        }
    }

    //从磁盘拿数据，加入到LRU内存结构中
    trunck* get_data_from_disk(int idx) {
    	int trunck_idx = idx/eyery_trunck_size; //将trunck_idx开头的桶载入到内存中
    	auto* new_trunck = new trunck();

        int cur_disk_idx = trunck_idx * eyery_trunck_size;
        for (int i=0;i<TRUNCK_SIZE;i++) {
            new_trunck->cur_nums[i] = disk[cur_disk_idx++];
        }
        new_trunck->idx = trunck_idx;

    	_map[trunck_idx] = new_trunck;
    	insert_new_trunck(new_trunck);
        return new_trunck;
    }
};
int main() {
    for (int i=0;i<DISK_LEN;i++) {
        disk[i] = i;
    }

    LRU lru;
    
    // int res = lru.get(10001);
    // cout << "res:" << res << endl;
    vector<int> res(100, -1);
    int test_idx = 1;
    for (int i=1;i<=10;i++) {
        int res = lru.get(test_idx);
        cout << "disk:" << disk[test_idx] << " lru_res:"<< res << endl;
        test_idx+=10000;
    }
    cout << "disk:" << disk[1] << " lru_1:" << lru.get(1) << endl;
    int ans = lru.get(test_idx);
    cout << "disk:" << disk[test_idx] << " lru_1:" << ans << endl;
    return 0;
}


