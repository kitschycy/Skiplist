#!/bin/bash
g++ ./stress_test/node_test.cpp -o ./bin/listnode_stress  --std=c++11 -pthread
./bin/listnode_stress