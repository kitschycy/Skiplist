//
// Created by 杨晨雨 on 2022/3/14.
//

#ifndef TEST_CODE_SKIPLIST_H
#define TEST_CODE_SKIPLIST_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
//#include <vector>
#include <mutex>

#define STORE_FILE "../store/dumpFile"

std::mutex mtx;
std::string delimiter = ":";

/**
 * @brief 跳表节点
 *
 * @tparam K
 * @tparam V
 */
template<typename K, typename V>
class Node {
public:
    Node(int level = 0) : key_(), value_(), node_level_(level) {
        if (level < 0)
            throw std::logic_error("level can not be a negative number");
        this->next_ = new Node<K, V> *[level + 1];
        memset(this->next_, 0, sizeof(Node<K, V> *) * (level + 1));
    };

    Node(const K &k, const V &v, int level) : key_(k), value_(v), node_level_(level) {
        if (level < 0)
            throw std::logic_error("level can not be a negative number");
        this->next_ = new Node<K, V> *[level + 1];
        memset(this->next_, 0, sizeof(Node<K, V> *) * (level + 1));
    }

    ~Node() {
        delete[]next_;
    };

    const K &get_key() const { return key_; };

    const V &get_value() const { return value_; };

    void set_value(const V &value) { value_ = value; };

//    friend std::ostream &operator<<(std::ostream &os, const Node<K, V> &node) {
//        os << "[Node: key=" << node.key_ << ", value=" << node.value_ << ", next_count=" << node.next_.size()
//           << "]";
//        return os;
//    }

public:
    int node_level_;
//    std::vector<Node<K, V> *> next_; // next_[i] 指第 i 层 右边的 元素
    Node<K, V> **next_;
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
template<typename K, typename V>
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

    void save_file();

    void load_file();

    void get_key_value_from_string(const std::string &str, std::string *key, std::string *value);

private:
    int max_level_;
    int skip_list_level_;
    int element_count_;
    Node<K, V> *header_;

    std::ofstream file_writer_;
    std::ifstream file_reader_;
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
    mtx.lock();
    Node<K, V> *cur = header_;
    //    cur->displayNode();
    // 申请 max_level_ 大小的空间
    Node<K, V> *update[max_level_ + 1];
    memset(update, 0, sizeof(Node<K, V> *) * (max_level_ + 1));

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
//        std::cout << "key : " << key << ", exists" << std::endl;
        mtx.unlock();
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
    mtx.unlock();
    return 0;
}

template<typename K, typename V>
int SkipList<K, V>::delete_element(K key) {
    mtx.lock();
    Node<K, V> *cur = this->header_;
    // cur->displayNode();
    Node<K, V> *update[max_level_ + 1];
    memset(update, 0, sizeof(Node<K, V> *) * (max_level_ + 1));

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
        delete cur;
        cur = nullptr;
        mtx.unlock();
        return 0;
    } else {
        std::cout << "the key is not exist" << std::endl;
        mtx.unlock();
        return -1; // 返回值 -1 说明没有该键值
    }
}

template<typename K, typename V>
int SkipList<K, V>::search_element(K key) {
    Node<K, V> *cur = header_;

    for (int i = skip_list_level_; i >= 0; --i) {
        while (cur->next_[i] != nullptr && cur->next_[i]->get_key() < key) {
            cur = cur->next_[i];
        }
    }

    // 该操作后 current->get_key >= key 或者 null
    cur = cur->next_[0];
    if (cur && cur->get_key() == key) {
        std::cout << "find the key \"" << key << "\" value " << cur->get_value() << std::endl;
        return 0;
    } else {
//        std::cout << "the key \"" << key << "\"is not exist" << std::endl;
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
    if (file_writer_.is_open()) {
        file_writer_.close();
    }
    if (file_reader_.is_open()) {
        file_reader_.close();
    }
    delete header_;
}

template<typename K, typename V>
void SkipList<K, V>::save_file() {
    std::cout << "******* save file *******" << std::endl;

    file_writer_.open(STORE_FILE);
    if (!file_writer_.is_open()) {
        std::cout << "cannot open file" << std::endl;
        return;
    }
    Node<K, V> *node = this->header_->next_[0];

    while (node != nullptr) {
        file_writer_ << node->get_key() << ":" << node->get_value() << "\n";
//        std::cout << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->next_[0];
    }

    file_writer_.flush();
    file_writer_.close();
    return;
}

template<typename K, typename V>
void SkipList<K, V>::load_file() {
    file_reader_.open(STORE_FILE);
    std::cout << "--------- load file ---------" << std::endl;
    std::string line;
    std::string *key = new std::string();
    std::string *value = new std::string();
    while (getline(file_reader_, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        insert_element(*key, *value);
        // log TBD
    }
    file_reader_.close();
}

template<typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string &str, std::string *key, std::string *value) {
    if (str.empty() || str.find(delimiter) == std::string::npos) {
        return;
    } else {
        *key = str.substr(0, str.find(delimiter));
        *value = str.substr(str.find(delimiter) + 1, str.length());
    }
}


#endif // TEST_CODE_SKIPLIST_H
