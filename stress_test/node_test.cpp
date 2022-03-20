#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
//#include "../SkipList.h"
//#include "../SkipList_v1.h"
#include "../List.h"

#define NUM_THREADS 1
//#define TEST_COUNT 100
//#define TEST_COUNT 100000
#define TEST_COUNT 500000
//#define TEST_COUNT 1000000
//SkipList<int, std::string> skiplist(18);
List list;

//  创建插入线程
void *insertElement(void *threadid) {
    // 线程id
    long tid;
    tid = (long) threadid;
    std::cout << tid << std::endl;
    // 测试的轮数
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; i++) {
//        std::cout << count << std::endl;
        count++;
        list.insert(rand() % TEST_COUNT, "a");
    }
//    std::cout << tid << "will exit. " << std::endl;
    pthread_exit(NULL);
}

//void * getElement(void * threadid) {
//    long tid;
//    tid = (long)threadid;
//    std::cout << tid << std::endl;
//    pthread_exit(NULL);
//}

int main() {
    srand(time(NULL));  // 初始化随机数种子
    {
        pthread_t threads[NUM_THREADS];
        int rc;
        int i;

        // start 为当前时间
        auto start = std::chrono::high_resolution_clock::now();

        for (i = 0; i < NUM_THREADS; i++) {
            std::cout << "main() : creating thread, " << i << std::endl;
            rc = pthread_create(&threads[i], NULL, insertElement, (void *) i);

            if (rc) {
                std::cout << "Error:unable to create thread," << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for (i = 0; i < NUM_THREADS; i++) {
            if (pthread_join(threads[i], &ret) != 0) {
                perror("pthread_create() error");
                exit(3);
            }
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << TEST_COUNT << " data : " << "insert elapsed:" << elapsed.count() << std::endl;
    }
//    std::cout << "element of skiplist is " << list.size() << std::endl;
//    skiplist.display_list();
//    {
//        pthread_t threads[NUM_THREADS];
//        int rc;
//        int i;
//
//        // start 为当前时间
//        auto start = std::chrono::high_resolution_clock::now();
//
//        for (i = 0; i < NUM_THREADS; i++) {
//            std::cout << "main() : creating thread, " << i << std::endl;
//            rc = pthread_create(&threads[i], NULL, insertElement, (void *) i);
//
//            if (rc) {
//                std::cout << "Error:unable to create thread," << rc << std::endl;
//                exit(-1);
//            }
//        }
//
//        void *ret;
//        for (i = 0; i < NUM_THREADS; i++) {
//            if (pthread_join(threads[i], &ret) != 0) {
//                perror("pthread_create() error");
//                exit(3);
//            }
//        }
//        auto finish = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> elapsed = finish - start;
//        std::cout << "insert elapsed:" << elapsed.count() << std::endl;
//    }


    pthread_exit(NULL);
    return 0;

}



