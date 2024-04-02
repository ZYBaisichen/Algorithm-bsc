/*
 * @Author: baisichen
 * @Date: 2024-02-26 15:02:38
 * @LastEditTime: 2024-04-01 20:32:20
 * @LastEditors: baisichen
 * @Description:
 */
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
#include <cmath>

using namespace std;

/*

一个消息接收并打印的结构设计
已知一个消息会不断地吐出整数1-N，但不一定按照顺序吐出。如果上次打印的数为1，那么当i+1出现时，请打印i+1及其之后接收过的并且连续的所有数，知道1—N全部接收并打印完，请设计这种接收并打印的结构，初始时默认i==0
*/

/*
很类似视频流UDP，在编号为1到来前不会先播放后面的包。

1. 将每个到来的节点封装成一个节点，节点中具有next指针，用于构造单链表
2. 准备两个map，head_map和tail_map，记录有多少头节点和尾结点。比如到达顺序为2, 4, 7,6,5
    a. 每个元素到来时都先创建只有自己的链表。head_map{2:{2,next}} tail_map{2:{2,next}}
    b. 4到来时，先创建只有自己的链表。head_map{2:{2,next}, 4:{4,next}} tail_map{2:{2,next}, 4:{4,next}}。
        然后检查有没有以3结尾的链表，有没有以5开头的链表，没有则继续
    c. 7同4。当到达6时head_map{2:{2,next}, 4:{4,next}, 7:{7,next}, 6:{6,next}} tail_map{2:{2,next}, 4:{4,next}, 7:{7,next}, 6:{6,next}}。
        检查到了有7开头的链表，则将6的next指向7. 在尾表中删除6，在头表中删除7。最后变成head_map{2:{2,next}, 4:{4,next}, 6:{6,next=7}} tail_map{2:{2,next}, 4:{4,next}, 7:{7,next}}。
    d. 当5到达时。先建立只有自己的链表。然后检查到了有4结尾的链表，将4的next指向5，删除4的结尾，删除开头为5的节点。
        发现有6开头的链表，将5的next指向6，删除结尾为5的节点，删除开头为6的节点。最后变成head_map{2:{2,next}, 4:{4,next=5}} tail_map{2:{2,next}, 7:{7,next}}。




*/
class Node
{
public:
    string val;
    int num;
    Node *next;
    Node(int num, string _val)
    {
        val = _val;
        next = NULL;
        num = 0;
    }
};
class MessageBox
{
public:
    map<int, Node *> head_map;
    map<int, Node *> tail_map;
    int wait_num;
    MessageBox()
    {
        wait_num = 1; // 从1开始
    }
    void print() {
        Node* cur = head_map[wait_num], *pre=NULL;
        head_map.erase(wait_num); //移除在head表里的节点
        while (cur) {
            if (pre) {
                delete pre;
            }
            pre = cur;
            cout << cur->val << " ";
            cur = cur->next;
            wait_num++;
        }
        if (pre) {
            delete pre;
        }
        tail_map.erase(wait_num-1); //移除在tail表里的节点


    }
    void receive(int num, string val)
    {   
        //创建只有自己的链表
        Node *new_node = new Node(num, val);
        head_map[num] = new_node;
        tail_map[num] = new_node;

        // 检查是否有以num+1开头的链表
        if (head_map.find(num+1) != head_map.end()) {
            Node* tmp = head_map[num+1];
            new_node->next = tmp;
            head_map.erase(num+1); //移除num+1作为头
            tail_map.erase(num); //移除当前节点作为尾
        } 
        // 检查是否有以num-1结尾的链表
        if (tail_map.find(num-1) != tail_map.end()) {
            Node* tmp = tail_map[num-1];
            tmp->next = new_node;
            tail_map.erase(num-1); //移除num-1作为尾
            head_map.erase(num); //移除当前节点作为头
        }

        if (wait_num == num) {
            print();
        }


    }
};

int main()
{

    // MessageBox only receive 1~N
    MessageBox box;
    // 1....
    cout << "这是2来到的时候" << endl;
    box.receive(2,"B"); // - 2"
    cout << "这是1来到的时候" << endl;
    box.receive(1,"A"); // 1 2 -> print, trigger is 1
    cout << "这是4来到的时候" << endl;
    box.receive(4,"D"); // - 4
    box.receive(5,"E"); // - 4 5
    box.receive(7,"G"); // - 4 5 - 7
    box.receive(8,"H"); // - 4 5 - 7 8
    box.receive(6,"F"); // - 4 5 6 7 8
    cout << "这是3来到的时候" << endl;
    box.receive(3,"C"); // 3 4 5 6 7 8 -> print, trigger is 3
    cout << "这是9来到的时候" << endl;
    box.receive(9,"I"); // 9 -> print, trigger is 9
    box.receive(10,"J"); // 10 -> print, trigger is 10
    box.receive(12,"L"); // - 12
    box.receive(13,"M"); // - 12 13
    box.receive(11,"K"); // 11 12 13 -> print, trigger is 11

    return 0;
}