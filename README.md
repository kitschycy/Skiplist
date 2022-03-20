# SkipList

C++实现的跳表存储引擎，支持多种数据类型，后继节点数组使用STL vector实现

**benchline List.h 普通有序链表**
有序链表

100 data : insert elapsed:0.000336863

100000 data : insert elapsed:42.52

500000 data : insert elapsed:3261.85

**SkipList with vector**

10w data : insert elapsed:0.611736

50w data : insert elapsed:3.72877

100w data : insert elapsed:7.90948

**SkipList without vector**

10w data : insert elapsed: 0.584645

50w data : insert elapsed: 2.61981

100w data : insert elapsed: 5.82261

## TBD
