

#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>
using namespace std;

class single_instance_test1
{
public:
    single_instance_test1() {}
    static single_instance_test1 *singleton;
    static single_instance_test1 *instance() {
        if (singleton == nullptr) {
            static single_instance_test1 cur_singleton;
            singleton = &cur_singleton;
        }
        return singleton;
    }
    void print() {
        cout << "single_instance_test1" << endl;
    }

    single_instance_test1(single_instance_test1 &) = delete;
    single_instance_test1 &operator=(single_instance_test1 &) = delete;
};
single_instance_test1 *single_instance_test1::singleton = nullptr;

class single_instance_test2
{
public:
    single_instance_test2() {}
    static single_instance_test2 *singleton;
    static single_instance_test2 *instance() {
        if (singleton == nullptr) {
            mutex mt;
            mt.lock();
            if (singleton == nullptr) {
                singleton = new single_instance_test2();
            }
            mt.unlock();
        }
        return singleton;
    }

    void print() {
        cout << "single_instance_test2" << endl;
    }

    single_instance_test2(single_instance_test2 &) = delete;
    single_instance_test2 &operator=(single_instance_test2 &) = delete;
};
single_instance_test2* single_instance_test2::singleton = nullptr;

int main() {
    single_instance_test1* aa = single_instance_test1::instance();
    single_instance_test2* bb = single_instance_test2::instance();

    aa->print();
    bb->print();

    return 0;
}