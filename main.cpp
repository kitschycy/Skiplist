#include "SkipList.h"
//#include "SkipListv1.h"
#include <string>

using std::string;

//void file_print() {
//    std::ifstream ifs;
//    ifs.open(STORE_FILE);
//    string test;
//    while (getline(ifs, test)) {
//        std::cout << test << std::endl;
//    }
//}

int main() {
    SkipList<std::string, std::string> myskiplist(6);

    std::cout << "-------insert test -------" << std::endl;
    std::cout << "skiplist item_size before insert_element(): " << myskiplist.size() << std::endl;

    myskiplist.insert_element("111", "oneoneone");
    myskiplist.insert_element("222", "two");
    myskiplist.insert_element("333", "threee");
    myskiplist.insert_element("abc", "测试1");
    myskiplist.insert_element("中文", "测试2");

    std::cout << "skiplist item_size after insert_element(): " << myskiplist.size() << std::endl;

    std::cout << "------- search test -------" << std::endl;
    myskiplist.search_element("111");
    myskiplist.search_element("noexsit");
    myskiplist.search_element("中文");

    std::cout << "------- delete test -------" << std::endl;
    myskiplist.delete_element("noexsit");
    myskiplist.delete_element("333");
    std::cout << "skipList size after delete_elelment() : " << myskiplist.size() << std::endl;

    std::cout << "------- display test -------" << std::endl;
    myskiplist.display_list();

    std::cout << "------- data_save test -------" << std::endl;
    myskiplist.save_file();

//    file_print();
    return 0;
}