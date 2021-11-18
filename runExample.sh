#! /bin/bash

g++ -std=c++11 ./src/*.cpp; if [ $? -eq 0 ]; then ./a.out; fi

