#!/bin/bash

if [ ! -d ./build ];then
    echo "mkdir build directory"
    mkdir build
fi

cd build && cmake .. && make  
