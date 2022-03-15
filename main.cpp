#include "SkipList.h"
#include <string>

using std::string;
//using std::cout;
//using std::endl;

int main(){
    SkipList<std::string, std::string> myskiplist(6);
    cout << "-------insert test -------" << std::endl;
//    std::cout << "sizeof(myskiplist) = " <<

    myskiplist.insert_element("111", "oneoneone");
//    myskiplist.insert_element("222", "two");
//    myskiplist.insert_element("333", "threee");
//    myskiplist.insert_element("abc", "测试1");
//    myskiplist.insert_element("中文", "测试2");
////        myskiplist.insert_element();
//
//    std::cout << "skiplist size after insert_element(): " << myskiplist.size() << std::endl;
//
//    std::cout << "----- search test -----" << std::endl;
//    myskiplist.delete_element("noexsit");
//    myskiplist.delete_element("333");
//    std::cout << "skipList size after delete_elelment() : " << myskiplist.size();



    return 0;
}


//int main() {
//    Node<string, string> n1("kkk", "vvv", 10);
//    n1.displayNode();
//
//    SkipList<string, string> skl1(6);   // 六层跳表
////    skl1.header_
//
//    return 0;
//}