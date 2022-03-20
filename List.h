//
// Created by 杨晨雨 on 2022/3/19.
//
#ifndef SKIPLIST1_LIST_H
#define SKIPLIST1_LIST_H

#include <iostream>

using std::string;

class Node {
public:
    int key_;
    std::string value_;
    Node *next_;

    Node() : key_(0), value_(""), next_(nullptr) {};

    Node(int k, string v) : key_(k), value_(v), next_(nullptr) {};
};

class List {
private:
    Node *head_;
    int count_;
public:
    List() : count_(0) { head_ = new Node(); };

    ~List() { delete head_; };

    void insert(int, string);
};

void List::insert(int key, string value) {
    Node *cur = head_;
    while (cur->next_ != nullptr && cur->next_->key_ < key) {
        cur = cur->next_;
    }
    if (cur->next_ != nullptr && cur->next_->key_ == key) {
        return;
    } else {
        Node *tmp = new Node(key, value);
        tmp->next_ = cur->next_;
        cur->next_ = tmp;
        std::cout << "insert key :" << key << std::endl;
    }
}




#endif //SKIPLIST1_LIST_H
