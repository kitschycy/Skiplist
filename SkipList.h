//
// Created by 杨晨雨 on 2022/3/14.
//

#ifndef TEST_CODE_SKIPLIST_H
#define TEST_CODE_SKIPLIST_H

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>

/**
 * @brief 跳表节点
 *
 * @tparam K
 * @tparam V
 */
template <typename K, typename V>
class Node {
public:
    Node(int level = 0) : key_(), value_(), node_level_(level) {
        if (level < 0)
            throw std::logic_error("level can not be a negative number");
        next_ = std::vector<Node<K, V> *>(node_level_ + 1, nullptr);
    };

    Node(const K &k, const V &v, int level) : key_(k), value_(v), node_level_(level) {
        if (level < 0)
            throw std::logic_error("level can not be a negative number");
        next_ = std::vector<Node<K, V> *>(node_level_ + 1, nullptr);
    }

    ~Node() {};

    const K &get_key() const { return key_; };

    const V &get_value() const { return value_; };

    void set_value(const V &value) { value_ = value; };

    std::ostream &dump(std::ostream &os) const {
        os << "[Node: key=" << key_ << ", value=" << value_ << ", next_count=" << next_.size()
           << "]";
        return os;
    }

    void displayNode() { dump(std::cout) << std::endl; };

public:
    int node_level_;
    std::vector<Node<K, V> *> next_; // next_[i] 指第 i 层 右边的 元素

private:
    K key_;
    V value_;
};

/**
 * @brief 跳表
 *
 * @tparam K
 * @tparam V
 */
template <typename K, typename V>
class SkipList {
public:
    SkipList(int);

    ~SkipList();

    // 增删改查
    int insert_element(K, V); // 增
    int delete_element(K);    // 删
    //    int update_element(K, V, bool); // 改
    int search_element(K);    // 查

    int get_random_level();

    int size() { return element_count_; };

    void display_list();

private:
    int max_level_;
    int skip_list_level_;
    int element_count_;
    Node<K, V> *header_;

    //    ofstream _file_writer;
    //    ifstream _file_reader;
};

template<typename K, typename V>
SkipList<K, V>::SkipList(int max_level)
        : max_level_(max_level),
          skip_list_level_(0),
          element_count_(0),
          header_(new Node<K, V>(max_level)) {};

template<typename K, typename V>
int SkipList<K, V>::get_random_level() {
    int k = 1;
    while (rand() % 2) {
        k++;
    }
    k = (k < max_level_) ? k : max_level_;
    return k;
}

template<typename K, typename V>
int SkipList<K, V>::insert_element(K key, V value) {
    Node<K, V> *cur = header_;
    //    cur->displayNode();
    // 申请 max_level_ 大小的空间
    std::vector<Node<K, V> *> update(max_level_ + 1, nullptr);

    for (int i = skip_list_level_; i >= 0; --i) {
        while (cur->next_[i] != nullptr && cur->next_[i]->get_key() < key) {
            cur = cur->next_[i]; //  cur 指向 cur 在第 i 层的 后一个元素
        }
        update[i] = cur; // update 数组存储当前层最符合要求的节点
    }

    //  此时到达第 0 层，cur->next_[0] 指向的是最底层 的下一个节点
    cur = cur->next_[0];

    // 1. 插入元素已经存在
    if (cur != nullptr && cur->get_key() == key) {
        std::cout << "key : " << key << ", exists" << std::endl;
        return -1;
    }

    if (cur == nullptr || cur != nullptr && cur->get_key() != key) {
        int random_level = get_random_level();

        //  如果高度大于当前跳表层高，需要更新 update 数组
        //  将原本新增范围内的 NULL 改为 header_
        if (random_level > skip_list_level_) {
            for (int i = skip_list_level_ + 1; i < random_level + 1; i++) {
                update[i] = header_;
            }
            skip_list_level_ = random_level;
        }

        //  创建新节点，进行插入
        Node<K, V> *inserted_node = new Node<K, V>(key, value, random_level);

        //  new_node->next = pre_node->next;
        //  pre_node->next = new_node;
        for (int i = 0; i <= random_level; i++) {
            inserted_node->next_[i] = update[i]->next_[i];
            update[i]->next_[i] = inserted_node;
        }
        std::cout << "Successfully inserted key : " << key << ", value : " << value << std::endl;
        element_count_++;
    }
    return 0;
}

template<typename K, typename V>
int SkipList<K, V>::delete_element(K key) {
    Node<K, V> *cur = this->header_;
    // cur->displayNode();
    std::vector<Node<K, V> *> update(max_level_ + 1);

    //  通过这个循环保存下来每层最后一个比目标元素小的元素
    for (int i = skip_list_level_; i >= 0; --i) {
        while (cur->next_[i] != nullptr && cur->next_[i]->get_key() < key) {
            cur = cur->next_[i];
        }
        update[i] = cur;
    }
    cur = cur->next_[0];

    if (cur != nullptr && cur->get_key() == key) {
        for (int i = 0; i <= skip_list_level_; ++i) {
            if (update[i]->next_[i] != cur) {
                // 说明该层开始已经不存在这个key了，提前结束循环
                break;
            }
            update[i]->next_[i] = cur->next_[i];
        }

        //  可能此时删除的元素恰好在顶层
        //  所以需要重新确定 skip_list_level_
        while (skip_list_level_ > 0 && header_->next_[skip_list_level_] == 0) {
            --skip_list_level_;
        }

        std::cout << "Successfully deleted key : " << key << std::endl;
        element_count_--;
        return 0;
    } else {
        std::cout << "the key \"" << key << "\" is not exist" << std::endl;
        //        mtx.unlock();
        return -1; // 返回值 -1 说明没有该键值
    }
}

template<typename K, typename V>
int SkipList<K, V>::search_element(K key) {
    Node<K, V> *cur = header_;
    //    std::vector<Node<K, V> *> update(skip_list_level_ + 1);
    for (int i = skip_list_level_; i >= 0; --i) {
        while (cur->next_[i] != nullptr && cur->next_[i]->get_key() < key) {
            cur = cur->next_[i];
        }
    }

    // 该操作后 current->get_key >= key 或者 null
    cur = cur->next_[0];
    if (cur && cur->get_key() == key) {
        cur->displayNode();
        return 0;
    } else {
        std::cout << "the key \"" << key << "\"is not exist" << std::endl;
        return -1;
    }
}

template<typename K, typename V>
void SkipList<K, V>::display_list() {
    std::cout << "******** Skip List *********" << std::endl;
    for (int i = 0; i <= skip_list_level_; i++) {
        Node<K, V> *node = this->header_->next_[i];
        std::cout << "Level " << i << " : ";
        while (node != nullptr) {
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->next_[i];
        }
        std::cout << std::endl;
    }
}

template<typename K, typename V>
SkipList<K, V>::~SkipList<K, V>() {
    if (header_ == nullptr)
        return;
    Node<K, V> *p = header_->next_[0];
    while (p != nullptr) {
        Node<K, V> *nextptr = p->next_[0];
        delete p;
        p = nextptr;
    }
    delete header_;
    header_ = nullptr;
}

#endif // TEST_CODE_SKIPLIST_H
