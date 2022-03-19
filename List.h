//
// Created by 杨晨雨 on 2022/3/19.
//

#include <iostream>
#include <mutex>


#ifndef SKIPLIST1_LIST_H
#define SKIPLIST1_LIST_H

std::mutex mtx;

template<typename K, typename V>
class ListNode {
public:
    ListNode(K key, V value) : key_(key), value_(value), node_next_(nullptr) {};

    ~ListNode() {};

    const void get_node_key() const { return this->key_; };

    const void get_node_value() const { return this->value_; };

private:
    K key_;
    V value_;
    ListNode<K, V> *node_next_;
};

template<typename K, typename V>
class List {
public:
    List() : header_(nullptr) {};

    int insert_node(K k, V v);

    int search_node();

    int delete_node();

private:
    ListNode<K, V> *header_;
};

template<typename K, typename V>
int List<K, V>::insert_node(K key, V value) {
    mtx.lock();
    ListNode<K, V> *cur = header_;
    while (cur->node_next_ != nullptr && cur->node_next_->get_node_key() < key) {
        cur = cur->node_next_;
    }
    if (cur->get_node_key() != key) {
        ListNode<K, V> *newNode = ListNode<K, V>(key, value);
        newNode->next = cur->node_next;
        cur->node_next_ = newNode;
        mtx.unlock();
        return 0;
    } else {
        std::cout << "key " << key << "exsists"
        mtx.unlock();
        return 1;
    }
}


#endif //SKIPLIST1_LIST_H
